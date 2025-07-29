import tensorflow as tf
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Embedding, LSTM, Dense
from tensorflow.keras.preprocessing.text import Tokenizer
from tensorflow.keras.preprocessing.sequence import pad_sequences
import pandas as pd

# Load dataset directly from Kaggle
url = "https://www.kaggleusercontent.com/datasets/processvenue/indian-language-identification/download"
data = pd.read_csv(url)

# Preprocess text data
def preprocess_text(text):
    return text.lower()

data['text'] = data['text'].apply(preprocess_text)

# Tokenize and pad sequences
tokenizer = Tokenizer(num_words=5000)
tokenizer.fit_on_texts(data['text'])
sequences = tokenizer.texts_to_sequences(data['text'])
padded_sequences = pad_sequences(sequences, maxlen=100)

# Prepare labels
labels = pd.get_dummies(data['language']).values

# Build the model
model = Sequential([
    Embedding(input_dim=5000, output_dim=128, input_length=100),
    LSTM(128, return_sequences=False),
    Dense(labels.shape[1], activation='softmax')  # Number of languages
])

model.compile(optimizer='adam', loss='categorical_crossentropy', metrics=['accuracy'])

# Train the model
model.fit(padded_sequences, labels, epochs=5, batch_size=32, validation_split=0.2)

# Save the model
model.save('../models/language_identifier.h5')
