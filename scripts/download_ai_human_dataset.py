import kagglehub
import pathlib
import shutil

FILENAME = "AI_Human.csv"
THIS_DIR = pathlib.Path(__file__).parent.resolve()
TRAINING_DIR = THIS_DIR.parent / "data" / "training"
DEST_FILE = TRAINING_DIR / FILENAME
TRAINING_DIR.mkdir(parents=True, exist_ok=True)

# Download latest version
download_dir = kagglehub.dataset_download("shanegerami/ai-vs-human-text")
download_file = pathlib.Path(download_dir) / FILENAME
print("Path to dataset files:", download_file)

print(f"Copying {download_file} to {DEST_FILE}...")
shutil.copy(str(download_file), str(DEST_FILE))
