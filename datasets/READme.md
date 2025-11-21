# Spoken Arabic Digits for Edge AI (with Noise)
## use in kaggle : https://www.kaggle.com/datasets/saidddd65215/0to10-number-in-arabic-for-edge-ai/

## 📌 Overview
This dataset contains spoken **Arabic digits (0-9)** and **background noise samples** in `.wav` format. It is specifically curated for training lightweight speech recognition models intended for **edge devices** (like microcontrollers or mobile phones).

The data is a clean merge of two Hugging Face datasets, standardized into a single folder with a simple, parsable naming convention.

## 📂 Data Structure & Labels
All audio files are located in the `data/` directory. The label for each audio file is the first part of its name.

### Filename Convention
Files are named in the format: `[Label]_[Description]_[UniqueID].wav`

**Examples:**
*   `0_zero_0001.wav` → Label is `0`
*   `9_nine_0150.wav` → Label is `9`
*   `-1_noise_0012.wav` → Label is `-1` (Noise)

## 🎯 Project Goal & Example Notebook
This dataset was prepared to train a simple and efficient keyword-spotting model for number recognition on low-power devices.

## 📜 Sources & License
This combined dataset is distributed under the **CC-BY 4.0** license. You must give appropriate credit to the original authors.

The data was sourced and merged from:
1.  **[mohnasgbr/spoken-arabic-digits](https://huggingface.co/datasets/mohnasgbr/spoken-arabic-digits/)** (License: Apache 2.0)
2.  **[gfbati/Ten2Zero](https://huggingface.co/datasets/gfbati/Ten2Zero)** (License: CC-BY 4.0)



similar datasets in english : https://www.kaggle.com/datasets/alanchn31/free-spoken-digits 
https://github.com/Jakobovski/free-spoken-digit-dataset/tree/master
