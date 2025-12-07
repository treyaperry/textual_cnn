# textual_cnn
A convolutional neural network which converts text to an "image" to check for AI generation

## Overview of Textual CNN
- A goal to detect whether a given piece of text is AI-generated or human-written by learning characteristic patterns in writing style, diction, and character-level composition
- Transform text into a form suitable for computer vision models, enabling a convolutional neural network to interpret textual patterns as spatial “textures” to make a sort of image
- Take a risk by claiming meaning and semantics of the text are unimportant - or even that the meaning is really just a higher level construct of this “image” of transformed text
- Produce a single probability score indicating how likely the text is AI-generated, supporting classification, detection, and downstream decision-making

## Details of Model
1) Convert text into a 2D character grid: Each character becomes an integer ID, laid out row-by-row into a fixed-width grid (e.g., 128 chars per row), producing a matrix (H, W) where height varies with text length
2) Batch with variable heights via dynamic padding: During batching, each grid is padded with extra rows so that all samples in a batch share the same height, yielding tensors shaped (B, H_max, W).
3) Embed characters to create a multi-channel “image”: An embedding layer transforms each character ID into a vector, turning the grid into a tensor (B, C, H, W) suitable for convolution
4) Use a CNN that pools only across width: Convolutional layers extract spatial/textural patterns across characters so max-pooling is applied only along the width to avoid collapsing height, allowing for for very short texts
5) Global average pooling produces a single document representation: After the convolutional lifting, global average pooling reduces (B, C, H, W) to (B, C), which feeds into fully connected layers to output a single logit to sigmoid to AI-generated probability for the entire text

## Training Data
In order to train the data, you will need to:
- download the csv of the text used at this url - https://www.kaggle.com/datasets/shanegerami/ai-vs-human-text?resource=download
- place the csv in the location ./data/textual_cnn/training/ 

## Testing
For ease of testing and simplicity, the trained model is currently tested against unprompted ChatGPT essays around 3000 characters:
- 3 “easy” to distinguish AI-generated essays
- 2 “moderate” - 1 “easy-moderate” and 1 “moderate-hard”
- 1 “very difficult”
- 1 human written test (~3000 char excerpt from _Ulysses_)

_These are currently located in ./data/textual_cnn/testing/_

Latest output w/ 10k sampes and 10 epochs:

```
RangeIndex: 487235 entries, 0 to 487234
Data columns (total 2 columns):
     Column     Non-Null Count   Dtype  
---  ------     --------------   -----  
 0   text       487235 non-null  object 
 1   generated  487235 non-null  float64
dtypes: float64(1), object(1)
memory usage: 7.4+ MB
None

Number of generated samples: 1036 for 10000 samples
Average length: 2458.008 for 10000 samples
Epoch 1/10 - train_loss=0.1368 - val_loss=0.1227
Epoch 2/10 - train_loss=0.0888 - val_loss=0.0252
Epoch 3/10 - train_loss=0.0866 - val_loss=0.0129
Epoch 4/10 - train_loss=0.0636 - val_loss=0.0773
Epoch 5/10 - train_loss=0.0630 - val_loss=0.0225
Epoch 6/10 - train_loss=0.0455 - val_loss=0.1431
Epoch 7/10 - train_loss=0.0421 - val_loss=0.0180
Epoch 8/10 - train_loss=0.0237 - val_loss=0.0066
Epoch 9/10 - train_loss=0.0292 - val_loss=0.0207
Epoch 10/10 - train_loss=0.0248 - val_loss=0.0058
Using file: 'test_specimen_easy':

Will begin the process of ID-ing given text as AI generated or not...
Predicted AI probability of 'test_specimen_easy': 99.97%
Using file: 'test_specimen_easy_2':
Predicted AI probability of 'test_specimen_easy_2': 99.99%
Using file: 'test_specimen_easy_3':
Predicted AI probability of 'test_specimen_easy_3': 67.10%
Using file: 'test_specimen_moderate':
Predicted AI probability of 'test_specimen_moderate': 96.47%
Using file: 'test_specimen_hard':
Predicted AI probability of 'test_specimen_hard': 0.80%
Using file: 'test_specimen_human':
Predicted AI probability of 'test_specimen_human': 0.00%
```
