#include "ReedSolomon.hpp"
#include <bits/stdc++.h>
using namespace std;

enum DNA
{
    A = 0b00, // 00
    C = 0b01, // 01
    G = 0b10, // 10
    T = 0b11  // 11
};

size_t SEED_LENGTH = 4;                        // 16 bases
size_t DATA_LENGTH = 16;                       // 64 bases
size_t ECC_LENGTH = 5;                         // 20 bases
size_t MSG_LENGTH = SEED_LENGTH + DATA_LENGTH; // 80 bases
size_t CHUNK_NUM = 1494;

/**
 * @brief Convert DNA base to binary, [A, C, G, T] -> [00, 01, 10, 11]
 */
uint8_t B2B(char dna)
{
    switch (dna)
    {
    case 'A':
        return DNA::A;
    case 'C':
        return DNA::C;
    case 'G':
        return DNA::G;
    case 'T':
        return DNA::T;
    default:
        return 0xff;
    }
}

/**
 * @brief Convert DNA string to binary, 2 bits per base, packed into bytes, last group padded with A(00)
 *
 * example: [AAATAAA] -> [AAAT, AAAA]
 *
 */
vector<uint8_t> DNAtoBinary(const string &dna)
{
    vector<uint8_t> binary;
    for (size_t i = 0; i < (dna.size() >> 2); i++)
    {
        uint8_t byte = 0;
        for (size_t j = 0; j < 4; j++)
        {
            byte = (byte << 2) | B2B(dna[(i << 2) + j]);
        }
        binary.push_back(byte);
    }
    if (dna.size() & 3)
    {
        uint8_t byte = 0;
        for (size_t j = 0; j < (dna.size() & 3); j++)
        {
            byte = (byte << 2) | B2B(dna[(dna.size() & 3) + j]);
        }
        binary.push_back(byte);
    }
    return binary;
}

void message_pass(size_t droplet_index, vector<pair<uint32_t, vector<uint8_t>>> &droplets, vector<vector<uint8_t>> &chunks, set<size_t> &done_segments, unordered_map<size_t, set<size_t>> &chunk_to_droplets)
{
    auto &droplet = droplets[droplet_index];
    srand(droplet.first);
    set<size_t> num_chunks;
    for (size_t i = 0; i < CHUNK_NUM; i++)
    {
        if (rand() % 2)
        {
            num_chunks.insert(i);
        }
    }

    for (size_t chunk_num : num_chunks)
    {
        if (done_segments.count(chunk_num))
        {
            for (size_t i = 0; i < DATA_LENGTH; i++)
            {
                droplet.second[i] ^= chunks[chunk_num][i];
            }
            num_chunks.erase(chunk_num);
            chunk_to_droplets[chunk_num].erase(droplet_index);
        }
    }

    if (num_chunks.size() == 1)
    {
        size_t lone_chunk = *num_chunks.begin();
        chunks[lone_chunk] = droplet.second;
        done_segments.insert(lone_chunk);
        chunk_to_droplets[lone_chunk].erase(droplet_index);

        for (auto other_droplet_index : chunk_to_droplets[lone_chunk])
        {
            message_pass(other_droplet_index, droplets, chunks, done_segments, chunk_to_droplets);
        }
    }
}

int main()
{
    ifstream dnaDataStream("50-SF.txt");
    if (!dnaDataStream.is_open())
    {
        cerr << "Error opening file" << endl;
        return 1;
    }

    string line;
    vector<vector<uint8_t>> chunks(CHUNK_NUM, vector<uint8_t>(DATA_LENGTH, 0));
    set<size_t> done_segments;
    unordered_map<size_t, set<size_t>> chunk_to_droplets;
    vector<pair<uint32_t, vector<uint8_t>>> droplets;

    while (getline(dnaDataStream, line))
    {
        vector<uint8_t> droplet = DNAtoBinary(line);
        if (droplet.size() != 25)
        {
            continue;
        }
        RS::ReedSolomon rs(MSG_LENGTH, ECC_LENGTH);
        vector<uint8_t> output(MSG_LENGTH);
        bool faulty = rs.Decode(droplet.data(), output.data());
        if (faulty)
        {
            continue;
        }

        uint32_t seed = output[0] | (output[1] << 8) | (output[2] << 16) | (output[3] << 24);
        vector<uint8_t> data(output.begin() + SEED_LENGTH, output.end());
        droplets.push_back({seed, data});

        srand(seed);
        set<size_t> num_chunks;
        for (size_t i = 0; i < CHUNK_NUM; i++)
        {
            if (rand() % 2)
            {
                num_chunks.insert(i);
            }
        }
        for (size_t chunk_num : num_chunks)
        {
            chunk_to_droplets[chunk_num].insert(droplets.size() - 1);
        }
    }

    for (size_t i = 0; i < droplets.size(); i++)
    {
        message_pass(i, droplets, chunks, done_segments, chunk_to_droplets);
    }

    dnaDataStream.close();

    ofstream decodedFile("50-SF_decoded.txt", ios::binary);
    for (const auto &chunk : chunks)
    {
        decodedFile.write(reinterpret_cast<const char *>(chunk.data()), chunk.size());
    }
    decodedFile.close();

    return 0;
}