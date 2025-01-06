#include "ReedSolomon.hpp"
#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <openssl/sha.h>
#include <set>
#include <vector>

const int SEED_SIZE = 4;
const int DATA_SIZE = 16;
const int RSCODE_SIZE = 5;
const int CHUNK_NUM = 1494;

std::vector<uint8_t> read_image(const std::string &file_path)
{
    cv::Mat img = cv::imread(file_path, cv::IMREAD_UNCHANGED);
    if (img.empty())
    {
        throw std::runtime_error("Failed to read image");
    }
    std::vector<uint8_t> data(img.begin<uint8_t>(), img.end<uint8_t>());
    return data;
}

std::vector<std::vector<uint8_t>> divide_into_chunks(const std::vector<uint8_t> &data, size_t chunk_size)
{
    std::vector<std::vector<uint8_t>> chunks;
    for (size_t i = 0; i < data.size(); i += chunk_size)
    {
        chunks.push_back(std::vector<uint8_t>(data.begin() + i, data.begin() + i + chunk_size));
    }
    return chunks;
}

std::vector<size_t> hash_to_indices(uint32_t seed, size_t num_chunks)
{
    std::vector<size_t> indices;
    uint8_t hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const uint8_t *>(&seed), sizeof(seed), hash);
    for (size_t i = 0; i < num_chunks; ++i)
    {
        size_t index = *reinterpret_cast<size_t *>(hash + (i % SHA256_DIGEST_LENGTH)) % num_chunks;
        indices.push_back(index);
    }
    return indices;
}

std::vector<uint8_t> generate_droplet(const std::vector<std::vector<uint8_t>> &chunks, uint32_t seed)
{
    std::vector<size_t> chunk_indices = hash_to_indices(seed, chunks.size());
    std::vector<uint8_t> data(DATA_SIZE, 0);
    for (size_t idx : chunk_indices)
    {
        for (size_t i = 0; i < DATA_SIZE; ++i)
        {
            data[i] ^= chunks[idx][i];
        }
    }
    return data;
}

std::vector<uint8_t> encode_droplet(const std::vector<uint8_t> &droplet, uint32_t seed, RS::ReedSolomon &rs)
{
    std::vector<uint8_t> message(SEED_SIZE + DATA_SIZE);
    std::memcpy(message.data(), &seed, SEED_SIZE);
    std::memcpy(message.data() + SEED_SIZE, droplet.data(), DATA_SIZE);
    std::vector<uint8_t> encoded_message;
    encoded_message.resize(SEED_SIZE + DATA_SIZE + RSCODE_SIZE);
    rs.Encode(message.data(), encoded_message.data());
    return encoded_message;
}

/**
 * Convert binary data to DNA sequence.
 * MSB seed LSB: 4 byte
 * MSB data LSB: 16 byte
 * MSB ecc LSB: 5 byte
 */
std::string binary_to_dna(const std::vector<uint8_t> &binary_data)
{
    const char base_mapping[] = {'A', 'C', 'G', 'T'};
    std::string dna_string;
    for (uint8_t byte : binary_data)
    {
        for (int i = 6; i >= 0; i -= 2)
        {
            dna_string += base_mapping[(byte >> i) & 0x03];
        }
    }
    return dna_string;
}

void write_encoded_file(const std::vector<std::string> &droplets, const std::string &file_path)
{
    std::ofstream file(file_path);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file for writing");
    }
    for (const std::string &droplet : droplets)
    {
        file << droplet << '\n';
    }
    file.close();
}

int main()
{
    try
    {
        // Read the input image file
        std::vector<uint8_t> data = read_image("50-SF.jpg");

        // Divide the data into chunks
        std::vector<std::vector<uint8_t>> chunks = divide_into_chunks(data, DATA_SIZE);

        // Initialize Reed-Solomon codec
        RS::ReedSolomon rs(DATA_SIZE, RSCODE_SIZE);

        // Generate and encode droplets
        std::vector<std::string> droplets;
        for (uint32_t seed = 0; seed < chunks.size(); ++seed)
        {
            std::vector<uint8_t> droplet = generate_droplet(chunks, seed);
            std::vector<uint8_t> encoded_droplet = encode_droplet(droplet, seed, rs);
            std::string dna_sequence = binary_to_dna(encoded_droplet);
            droplets.push_back(dna_sequence);
        }

        // Write the encoded droplets to a file
        write_encoded_file(droplets, "50-SF.txt");

        std::cout << "Encoding completed successfully!" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}