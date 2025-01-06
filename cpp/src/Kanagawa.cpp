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

size_t SEED_LENGTH = 4;  // 16 bases
size_t DATA_LENGTH = 16; // 64 bases
size_t ECC_LENGTH = 5;   // 20 bases
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

int main()
{
    ifstream dnaDataStream("50-SF.txt");
    if (!dnaDataStream.is_open())
    {
        cerr << "Error opening file" << endl;
        return 1;
    }

    string line;
    vector<vector<uint8_t>> binary_vectors;

    while (getline(dnaDataStream, line))
    {
        vector<uint8_t> droplet = DNAtoBinary(line);
        // handle every droplet
        RS::ReedSolomon rs(droplet.size(), ECC_LENGTH);
		rs.Decode(droplet.data(), droplet.data(), NULL, 0);
        binary_vectors.push_back(droplet);
    }

    dnaDataStream.close();

    return 0;
}