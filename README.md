# Kanagawa_DNA_Storage

An encoding/decoding toolkit for DNA digital data storage. This project is developed for EE411 Information Theory and Coding at SUSTech.

## Introduction

Kanagawa_DNA_Storage is a toolkit designed to encode digital data into DNA sequences and decode DNA sequences back into digital data. The project leverages Reed-Solomon error correction and robust soliton distribution for efficient and reliable data storage and retrieval.

## Requirements

- C++17 compatible compiler
  - CMake 3.10 or higher
  - OpenCV
  - OpenSSL
- Python 2.7
  - NumPy
  - tqdm
  - reedsolo

## Installation

First, clone the repository:

```sh
git clone git@github.com:SUSTech-ISACG-Association/Kanagawa_DNA_Storage.git
```

### C++ Components

```sh
# Create a build directory
mkdir build
cd build
# Make sure you have OpenCV and OpenSSL installed
cmake ..
make
```

### Python Components

Install the required Python packages:

```sh
pip install -r requirements.txt
```

## Usage

### Decoding

The decoding step is divided into two parts: decoding the DNA sequence into an intermediate binary file (SeedData.txt) and decoding the binary file into the original data.

1. Compile the decoder
2. Run the decoder with the DNA sequence as input

	```sh
	./decoder
	```
3. Run the Python script to decode the SeedData.txt file into the original data

	```sh
	python finalize.py
	```

Note that by default, the DNA sequence is read from the file `50-SF.txt`, the intermediate binary file is written to `SeedData.txt`, and the decoded data is written to `Kanagawa.jpg`. You can change these settings in the `decoder.cpp` and `finalize.py` files.