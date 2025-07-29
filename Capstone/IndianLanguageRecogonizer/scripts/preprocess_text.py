import os
import pandas as pd
import re

def preprocess_text(input_file, output_file):
    """
    Preprocess text samples for language identification by cleaning and tokenizing.

    Args:
        input_file (str): Path to the raw text dataset (CSV format).
        output_file (str): Path to save the preprocessed text dataset.
    """
    # Load dataset
    data = pd.read_csv(input_file)

    # Clean text
    def clean_text(text):
        text = re.sub(r'[^\w\s]', '', text)  # Remove punctuation
        text = text.lower()  # Convert to lowercase
        return text

    data['text'] = data['text'].apply(clean_text)

    # Save preprocessed data
    data.to_csv(output_file, index=False)

if __name__ == "__main__":
    # Example usage
    input_file = "../datasets/text_samples/raw_text.csv"
    output_file = "../datasets/text_samples/preprocessed_text.csv"
    preprocess_text(input_file, output_file)
