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

### C++ Components

1. Clone the repository:

```sh
git clone https://github.com/yourusername/Kanagawa_DNA_Storage.git
cd Kanagawa_DNA_Storage
```

2. Create a build directory and navigate to it:

   ```sh
   mkdir build
   cd build
   ```

3. Run CMake to configure the project:

   ```sh
   cmake ..
   ```

4. Build the project:

   ```sh
   make
   ```

### Python Components

1. Install the required Python packages:

   ```sh
   pip install numpy tqdm reedsolo
   ```

## Usage

### Encoding

To encode an image into DNA sequences, run the encoder executable:

```sh
./encoder
```

