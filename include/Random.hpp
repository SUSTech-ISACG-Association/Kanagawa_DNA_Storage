#pragma once
#include <vector>

class Random {
private:
    static const int N = 624;
    static const int M = 397;
    static const unsigned long MATRIX_A = 0x9908b0dfUL;
    static const unsigned long UPPER_MASK = 0x80000000UL;
    static const unsigned long LOWER_MASK = 0x7fffffffUL;

    std::vector<unsigned long> state;
    int index;

    void init_genrand(unsigned long s);
    void init_by_array(const std::vector<unsigned long>& init_key);
    unsigned long genrand_int32();

public:
    Random();

    void seed(unsigned long s);
    void seed(const std::vector<unsigned long>& init_key);
    void seed_with_time();

    double random();
    std::vector<int> sample(const std::vector<int>& population, int k);
};