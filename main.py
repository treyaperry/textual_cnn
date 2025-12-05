import pandas as pd
import math
from typing import List, Tuple

import numpy as np
from sklearn.model_selection import train_test_split
import torch
import torch.nn as nn
import torch.nn.functional as F
from torch.utils.data import Dataset, DataLoader

# ---------------------------
# Character / text utilities
# ---------------------------

# Our definition of simple char vocab:
# 0 = PAD
# 1..95 = printable ASCII (space char to '~')
VOCAB_SIZE = 96  # 0 + 95 printable characters

# This will serve as our "unitless grayscale" to mimic an image
def char_to_id(c: str) -> int:
    code = ord(c)
    if 32 <= code < 127:  # printable ASCII range
        return code - 31  # 32 -> 1, ..., 126 -> 95
    else:
        return 0  # PAD


# Convert text into a 2D array of char IDs with fixed width and variable height,
# truncate at width * max_rows characters.
# Returns shape: (height, width) where 1 <= height <= max_rows
def text_to_grid_ids(
    text: str,
    width: int = 128,
    max_rows: int = 64
) -> np.ndarray:

    # normalize / clean for future work...
    max_len = width * max_rows
    text = text[:max_len]

    ids = [char_to_id(c) for c in text]
    if len(ids) == 0:
        # avoid empty - pad row
        ids = [0]

    # compute required rows
    H = math.ceil(len(ids) / width)
    H = min(H, max_rows)

    # truncate to H * width
    ids = ids[:H * width]

    # pad to fill last row if needed
    if len(ids) < H * width:
        ids += [0] * (H * width - len(ids))

    grid = np.array(ids, dtype=np.int64).reshape(H, width)
    return grid  # (H, width)

# -----------------------------------------------------------------------------

# -------------------------
# Dataset and collate 
# -------------------------

class TextGridDataset(Dataset):
    def __init__(
        self,
        texts: List[str],
        labels: List[int],
        width: int = 128,
        max_rows: int = 64,
    ):
        assert len(texts) == len(labels)
        self.texts = texts
        self.labels = labels
        self.width = width
        self.max_rows = max_rows

    def __len__(self):
        return len(self.texts)

    def __getitem__(self, idx):
        text = self.texts[idx]
        label = self.labels[idx]
        grid = text_to_grid_ids(text, width=self.width, max_rows=self.max_rows)

        # grid: (H, W), label: 0 or 1
        return torch.from_numpy(grid), torch.tensor(label, dtype=torch.float32)


def collate_variable_height(
    batch: List[Tuple[torch.Tensor, torch.Tensor]]
):
    """
    Batch is a list of (grid, label).
    Grids have shape (H_i, W); we pad along H to match the max height in the batch.
    Returns:
        grids_padded: (B, H_max, W) long
        labels: (B,) float
    """
    grids, labels = zip(*batch)
    widths = [g.shape[1] for g in grids]
    assert len(set(widths)) == 1, "All widths in a batch must be the same"
    W = widths[0]
    heights = [g.shape[0] for g in grids]
    H_max = max(heights)

    padded = []
    for g in grids:
        H = g.shape[0]
        if H < H_max:
            pad_rows = H_max - H
            # pad rows of zeros (PAD ID)
            pad = torch.zeros((pad_rows, W), dtype=g.dtype)
            g_padded = torch.cat([g, pad], dim=0)
        else:
            g_padded = g
        padded.append(g_padded)

    grids_padded = torch.stack(padded, dim=0)  # (B, H_max, W)
    labels = torch.stack(labels, dim=0)  # (B,)
    return grids_padded, labels


# -----------------------------------------------------------------------------

# ------------------------------------
# CNN model based on Transformed Text
# ------------------------------------

class TextualCNN(nn.Module):
    """
    CNN for AI-vs-human text detection using a 2D 'grid' transformation of character IDs.

    Expected input to forward():
        grid_ids: LongTensor of shape (B, H, W)
            - B: batch size
            - H: number of rows (can vary between batches)
            - W: fixed width (chars per row)

    Output:
        logits: FloatTensor of shape (B, 1)
            - Use torch.sigmoid(logits) to get probability of AI generation
    """
    def __init__(
        self,
        vocab_size: int,
        emb_dim: int = 32,
        conv_channels: tuple = (64, 128, 256),
        dropout_p: float = 0.5,
    ):
        super().__init__()

        # Embedding for character IDs: 0 ... vocab_size-1
        # padding_idx = 0 so PAD rows/cols map to appx zero vectors
        self.emb = nn.Embedding(vocab_size, emb_dim, padding_idx = 0)

        c1, c2, c3 = conv_channels

        # Convolutional backbone
        self.conv1 = nn.Conv2d(emb_dim, c1, kernel_size = 3, padding = 1)
        self.bn1   = nn.BatchNorm2d(c1)

        self.conv2 = nn.Conv2d(c1, c2, kernel_size = 3, padding = 1)
        self.bn2   = nn.BatchNorm2d(c2)

        self.conv3 = nn.Conv2d(c2, c3, kernel_size = 3, padding = 1)
        self.bn3   = nn.BatchNorm2d(c3)

        self.pool = nn.MaxPool2d(kernel_size=2, stride = 2)
        self.dropout = nn.Dropout(dropout_p)

        # After global average pooling, we just have c3 features
        self.fc1 = nn.Linear(c3, 128)
        self.fc_out = nn.Linear(128, 1)  # binary logit

    def forward(self, grid_ids: torch.Tensor) -> torch.Tensor:
        """
        grid_ids: (B, H, W) of data type torch.long
        returns: logits of dimension (B, 1)
        """
        # (B, H, W) -> (B, H, W, emb_dim)
        x = self.emb(grid_ids)

        # (B, H, W, C) -> (B, C, H, W) for Conv2d
        x = x.permute(0, 3, 1, 2)

        # Conv block 1 + pool
        x = self.conv1(x)
        x = self.bn1(x)
        x = F.relu(x)
        x = self.pool(x)   # (B, c1, H/2, W/2)

        # Conv block 2 + pool
        x = self.conv2(x)
        x = self.bn2(x)
        x = F.relu(x)
        x = self.pool(x)   # (B, c2, H/4, W/4)

        # Conv block 3 (NO pool here)
        x = self.conv3(x)
        x = self.bn3(x)
        x = F.relu(x)      # (B, c3, H/4, W/4)

        # Global average pooling over spatial dimensions -> (B, c3)
        x = x.mean(dim=[2, 3])

        # Fully-connected head
        x = self.dropout(F.relu(self.fc1(x)))  # (B, 128)
        logits = self.fc_out(x)                # (B, 1)

        return logits
    
# -----------------------------------------------------------------------------
    
# -------------------------
# Training loop
# -------------------------

def main():

    # A collection of AI generated and human generated text from Kaggle
    df = pd.read_csv("data/textual_cnn/training/AI_Human.csv")

    # Print data info
    print(df.info())

    count = 0
    generated_count = 0
    text_count = 0

    texts = []
    generated_labels = []

    for index, row in df.iterrows():

        text = df.iloc[index, 0]
        generated = df.iloc[index, 1]

        if generated != 0.0: 
            generated_count = generated_count + 1

        text_count = text_count + len(text)
        count = index

        texts.append(text)
        generated_labels.append(generated)

        # Limit the number of training/validation samples, can be up to 500,000
        if index == 10000: break


    print(f"\nNumber of generated samples: {generated_count} for {count} samples")
    print(f"Average length: {text_count/count} for {count} samples")

    width = 128
    max_rows = 64

    # 80 / 20 split for training and validation seems to be best tuning
    train_texts, val_texts, train_labels, val_labels = train_test_split(
        texts,
        generated_labels,
        test_size = 0.2,                # 20% validation
        stratify = generated_labels,    # keep class balance
        random_state = 37,
    )

    # Transform the text into essentially a grid of unitless magnitudes, like grayscale
    train_ds = TextGridDataset(train_texts, train_labels, width=width, max_rows=max_rows)
    val_ds = TextGridDataset(val_texts, val_labels, width=width, max_rows=max_rows)

    train_loader = DataLoader(
        train_ds,
        batch_size=2,
        shuffle=True,
        collate_fn=collate_variable_height
    )

    val_loader = DataLoader(
        val_ds,
        batch_size=2,
        shuffle=False,
        collate_fn=collate_variable_height
    )

    # ---- Instantiate Model / optimizer / loss ----
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    model = TextualCNN(vocab_size=VOCAB_SIZE, emb_dim=32).to(device)

    criterion = nn.BCEWithLogitsLoss()
    optimizer = torch.optim.Adam(model.parameters(), lr=1e-3)

    # ---- Training loop ----
    # Still haven't figured out the sweet spot here...
    num_epochs = 10

    for epoch in range(num_epochs):

        model.train()
        total_train_loss = 0.0

        for grids, labels_batch in train_loader:
            # (B, H, W)
            grids = grids.to(device)
            labels_batch = labels_batch.to(device)

            optimizer.zero_grad()
            logits = model(grids).squeeze(1)
            loss = criterion(logits, labels_batch)
            loss.backward()
            optimizer.step()

            total_train_loss += loss.item() * grids.size(0)

        avg_train_loss = total_train_loss / len(train_ds)

        # --- Validate ---
        model.eval()
        total_val_loss = 0.0
        all_probs = []
        all_true = []

        with torch.no_grad():
            for grids, labels_batch in val_loader:
                grids = grids.to(device)
                labels_batch = labels_batch.to(device)

                logits = model(grids).squeeze(1)
                loss = criterion(logits, labels_batch)
                total_val_loss += loss.item() * grids.size(0)

                probs = torch.sigmoid(logits)
                all_probs.append(probs.cpu().numpy())
                all_true.append(labels_batch.cpu().numpy())

        avg_val_loss = total_val_loss / len(val_ds)
        all_probs = np.concatenate(all_probs)
        all_true = np.concatenate(all_true)

        print(
            f"Epoch {epoch + 1}/{num_epochs} "
            f"- train_loss={avg_train_loss:.4f} "
            f"- val_loss={avg_val_loss:.4f}"
        )

    # ------------   Inference helper   -------------
    def predict_ai_percent(text: str) -> float:
        model.eval()
        with torch.no_grad():
            grid = text_to_grid_ids(text, width=width, max_rows=max_rows)
            grid_tensor = torch.from_numpy(grid).unsqueeze(0).to(device)  # (1, H, W)
            logit = model(grid_tensor).item()
            prob = 1 / (1 + math.exp(-logit))  # sigmoid
        return prob * 100.0

    # quick sanity test
    # example_text = "This is a sample text to check the detector."
    # print(f"AI probability: {predict_ai_percent(example_text):.2f}%")
    
    # An unprompted AI essay from ChatGPT OR human authored text around 2000-3000 characters
    # There are multiple test cases in data/ to swap in here
    file_name = "test_specimen_easy"
    test_file = "data/textual_cnn/testing/" + str(file_name) + ".txt"

    print(f"Using file: '{file_name}':")

    try:
        with open(test_file, "r", encoding="utf-8") as file:
            test_text = file.read()
        # print(test_text)
    except FileNotFoundError:
        print(f"Error: The file '{test_file}' was not found.")
    except Exception as e:
        print(f"An error occurred: {e}")

    print(f"\nWill begin the process of ID-ing given text as AI generated or not...")
    percent_ai = predict_ai_percent(test_text)
    print(f"Predicted AI probability of '{file_name}': {percent_ai:.2f}%")

    file_name = "test_specimen_easy_2"
    test_file = "data/textual_cnn/testing/" + str(file_name) + ".txt"

    print(f"Using file: '{file_name}':")

    try:
        with open(test_file, "r", encoding="utf-8") as file:
            test_text = file.read()
        # print(test_text)
    except FileNotFoundError:
        print(f"Error: The file '{test_file}' was not found.")
    except Exception as e:
        print(f"An error occurred: {e}")
    percent_ai = predict_ai_percent(test_text)
    print(f"Predicted AI probability of '{file_name}': {percent_ai:.2f}%")

    file_name = "test_specimen_easy_3"
    test_file = "data/textual_cnn/testing/" + str(file_name) + ".txt"

    print(f"Using file: '{file_name}':")

    try:
        with open(test_file, "r", encoding="utf-8") as file:
            test_text = file.read()
        # print(test_text)
    except FileNotFoundError:
        print(f"Error: The file '{test_file}' was not found.")
    except Exception as e:
        print(f"An error occurred: {e}")
    percent_ai = predict_ai_percent(test_text)
    print(f"Predicted AI probability of '{file_name}': {percent_ai:.2f}%")

    file_name = "test_specimen_moderate"
    test_file = "data/textual_cnn/testing/" + str(file_name) + ".txt"

    print(f"Using file: '{file_name}':")

    try:
        with open(test_file, "r", encoding="utf-8") as file:
            test_text = file.read()
        # print(test_text)
    except FileNotFoundError:
        print(f"Error: The file '{test_file}' was not found.")
    except Exception as e:
        print(f"An error occurred: {e}")
    percent_ai = predict_ai_percent(test_text)
    print(f"Predicted AI probability of '{file_name}': {percent_ai:.2f}%")

    file_name = "test_specimen_hard"
    test_file = "data/textual_cnn/testing/" + str(file_name) + ".txt"

    print(f"Using file: '{file_name}':")

    try:
        with open(test_file, "r", encoding="utf-8") as file:
            test_text = file.read()
        # print(test_text)
    except FileNotFoundError:
        print(f"Error: The file '{test_file}' was not found.")
    except Exception as e:
        print(f"An error occurred: {e}")
    percent_ai = predict_ai_percent(test_text)
    print(f"Predicted AI probability of '{file_name}': {percent_ai:.2f}%")

    file_name = "test_specimen_human"
    test_file = "data/textual_cnn/testing/" + str(file_name) + ".txt"

    print(f"Using file: '{file_name}':")

    try:
        with open(test_file, "r", encoding="utf-8") as file:
            test_text = file.read()
        # print(test_text)
    except FileNotFoundError:
        print(f"Error: The file '{test_file}' was not found.")
    except Exception as e:
        print(f"An error occurred: {e}")
    percent_ai = predict_ai_percent(test_text)
    print(f"Predicted AI probability of '{file_name}': {percent_ai:.2f}%")

if __name__ == "__main__":
    main()


