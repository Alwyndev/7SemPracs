# Indian Language Recognizer

This project aims to build a hierarchical model for language identification and character recognition for Indian languages: Hindi, English, Malayalam, Tamil, and Punjabi.

## Project Structure

```
IndianLanguageRecognizer/
├── datasets/
│   ├── character_images/   # Character image datasets
│   ├── text_samples/       # Text samples for language identification
├── models/                 # Trained models
├── scripts/                # Scripts for preprocessing, training, and evaluation
├── results/                # Results and logs
└── README.md               # Project documentation
```

## Steps to Run the Project
1. Download datasets and place them in the `datasets/` folder.
2. Preprocess the datasets using scripts in the `scripts/` folder.
3. Train the models using the training scripts.
4. Evaluate the models and save results in the `results/` folder.

## Datasets
- **Character Image Datasets**:
  - English: EMNIST
  - Hindi: Kaggle Devanagari Handwritten Character Dataset
  - Malayalam: Kaggle Malayalam Handwritten Character Dataset
  - Tamil: Kaggle Tamil Handwritten Character Recognition
  - Punjabi: Kaggle Augmented Gurmukhi Handwritten Dataset

- **Text Samples for Language Identification**:
  - Kaggle Indian Language Identification
  - IndicLID (GitHub, AI4Bharat)

## Online Dataset Links

- **Character Image Datasets**:
  - [EMNIST (English)](https://www.nist.gov/itl/products-and-services/emnist-dataset)
  - [Kaggle Devanagari Handwritten Character Dataset (Hindi)](https://www.kaggle.com/datasets/medahmedkrichen/devanagari-handwritten-character-datase)
  - [Kaggle Malayalam Handwritten Character Dataset](https://www.kaggle.com/datasets/ajayjames/malayalam-handwritten-character-dataset)
  - [Kaggle Tamil Handwritten Character Recognition](https://www.kaggle.com/datasets/gauravduttakiit/tamil-handwritten-character-recognition)
  - [Kaggle Augmented Gurmukhi Handwritten Dataset (Punjabi)](https://www.kaggle.com/datasets/moodywriter/augmented-gurmukhi-handwritten)

- **Text Samples for Language Identification**:
  - [Kaggle Indian Language Identification](https://www.kaggle.com/datasets/processvenue/indian-language-identification)
  - [IndicLID (GitHub, AI4Bharat)](https://github.com/AI4Bharat/IndicLID)

## Requirements
- Python 3.8+
- TensorFlow, Keras, NumPy, Pandas, Matplotlib, Scikit-learn

## Future Work
- Add support for more Indian languages.
- Optimize models for better accuracy and performance.
