import os
import cv2
import numpy as np

def preprocess_images(input_dir, output_dir, img_size=(28, 28)):
    """
    Preprocess character image datasets by resizing and normalizing.

    Args:
        input_dir (str): Path to the raw image dataset.
        output_dir (str): Path to save preprocessed images.
        img_size (tuple): Target size for resizing images.
    """
    os.makedirs(output_dir, exist_ok=True)
    for filename in os.listdir(input_dir):
        if filename.endswith(('.png', '.jpg', '.jpeg')):
            img_path = os.path.join(input_dir, filename)
            img = cv2.imread(img_path, cv2.IMREAD_GRAYSCALE)
            if img is not None:
                img = cv2.resize(img, img_size)
                img = img / 255.0  # Normalize pixel values to [0, 1]
                save_path = os.path.join(output_dir, filename.split('.')[0] + '.npy')
                np.save(save_path, img)

if __name__ == "__main__":
    # Example usage
    input_directory = "../datasets/character_images/raw"
    output_directory = "../datasets/character_images/preprocessed"
    preprocess_images(input_directory, output_directory)
