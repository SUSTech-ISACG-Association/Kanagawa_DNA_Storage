#include "Random.hpp"
#include <cmath>
#include <cstdint>
#include <ctime>
#include <stdexcept>
#include <set>

void Random::init_genrand(unsigned long s)
{
    state[0] = s & 0xffffffffUL;
    for (int mti = 1; mti < N; mti++)
    {
        state[mti] = (1812433253UL * (state[mti - 1] ^ (state[mti - 1] >> 30)) + mti);
        state[mti] &= 0xffffffffUL;
    }
    index = N;
}

void Random::init_by_array(const std::vector<unsigned long> &init_key)
{
    int key_length = init_key.size();
    init_genrand(19650218UL);
    int i = 1, j = 0;
    int k = (N > key_length) ? N : key_length;
    for (; k > 0; k--)
    {
        state[i] = (state[i] ^ ((state[i - 1] ^ (state[i - 1] >> 30)) * 1664525UL)) + init_key[j] + j;
        state[i] &= 0xffffffffUL;
        i++;
        j++;
        if (i >= N)
        {
            state[0] = state[N - 1];
            i = 1;
        }
        if (j >= key_length)
            j = 0;
    }
    for (k = N - 1; k > 0; k--)
    {
        state[i] = (state[i] ^ ((state[i - 1] ^ (state[i - 1] >> 30)) * 1566083941UL)) - i;
        state[i] &= 0xffffffffUL;
        i++;
        if (i >= N)
        {
            state[0] = state[N - 1];
            i = 1;
        }
    }
    state[0] = 0x80000000UL;
}

unsigned long Random::genrand_int32()
{
    unsigned long y;
    static unsigned long mag01[2] = {0x0UL, MATRIX_A};

    if (index >= N)
    {
        int kk;
        for (kk = 0; kk < N - M; kk++)
        {
            y = (state[kk] & UPPER_MASK) | (state[kk + 1] & LOWER_MASK);
            state[kk] = state[kk + M] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        for (; kk < N - 1; kk++)
        {
            y = (state[kk] & UPPER_MASK) | (state[kk + 1] & LOWER_MASK);
            state[kk] = state[kk + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        y = (state[N - 1] & UPPER_MASK) | (state[0] & LOWER_MASK);
        state[N - 1] = state[M - 1] ^ (y >> 1) ^ mag01[y & 0x1UL];

        index = 0;
    }

    y = state[index++];
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return y;
}
Random::Random() : state(N), index(N) {}

void Random::seed(unsigned long s) { init_by_array({s}); }

void Random::seed(const std::vector<unsigned long> &init_key) { init_by_array(init_key); }

double Random::random()
{
    unsigned long a = genrand_int32() >> 5;
    unsigned long b = genrand_int32() >> 6;
    return (a * 67108864.0 + b) * (1.0 / 9007199254740992.0);
}

void Random::seed_with_time() { seed(static_cast<unsigned long>(std::time(nullptr))); }

std::vector<int> Random::sample(const std::vector<int> &population, int k)
{
    int n = population.size();
    if (k > n)
    {
        throw std::invalid_argument("Sample size larger than population.");
    }

    std::vector<int> result(k);
    if (n <= 21 || k > 5)
    { // Simple logic to use pool method for smaller populations
        std::vector<int> pool = population;
        for (int i = 0; i < k; ++i)
        {
            int j = static_cast<int>(this->random() * (n - i));
            result[i] = pool[j];
            pool[j] = pool[n - i - 1];
        }
    }
    else
    { // Use the set-based method for larger populations
        std::set<int> selected;
        int j;
        for (int i = 0; i < k; ++i)
        {
            do
            {
                j = static_cast<int>(this->random() * n);
            } while (selected.find(j) != selected.end());
            selected.insert(j);
            result[i] = population[j];
        }
    }

    return result;
}