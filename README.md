# textual_cnn
A convolutional neural network which converts text to an "image" to check for AI generation

# Training Data
In order to train the data, you will need to:
- download the csv of the text used at this url - https://www.kaggle.com/datasets/shanegerami/ai-vs-human-text?resource=download
- place the csv in the location ./data/textual_cnn/training/ 

Latest output w/ 10k sampes and 10 epochs:
RangeIndex: 487235 entries, 0 to 487234
Data columns (total 2 columns):
 #   Column     Non-Null Count   Dtype  
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
