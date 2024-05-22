#include "chunk.h"
#include "naive.h"
#include "object_mock.h"

#include <cassert>
#include <chrono>
#include <iostream>
#include <random>

using Timer = std::chrono::system_clock;
using TimePoint = std::chrono::time_point<Timer>;
using DurationMs = std::chrono::duration<double, std::milli>;
using Ms = std::chrono::milliseconds;

constexpr size_t width = 2000;
constexpr size_t height = 2000;
constexpr size_t defaultSeed = 0xDEADDEAD;

std::mt19937_64 mt;
static size_t seed = 0;

void RandomStep(size_t& x, size_t& y) {
    size_t r = rand() & 3;
    if (r == 0 && x > 0) {
        x--;
    } else if (r == 1 && x < width - 1) {
        x++;
    } else if (r == 2 && y > 0) {
        y--;
    } else if (r == 3 && y < height - 1) {
        y++;
    }
}

int RandomNumber(int min, int max) {
    return rand() % (max - min) + min;
}

template <class Array2d>
uint32_t DrunkenBenchMs(Array2d& array, uint32_t steps, std::optional<char>* checksum) {
    size_t x = RandomNumber(0, width);
    size_t y = RandomNumber(0, height);
    char data = 0;

    TimePoint start = Timer::now();
    for (uint32_t i = 0; i < steps; ++i) {
        data += array.Get(x, y).data[0];
        array.GetMutable(x, y).data[0] = rand();
        RandomStep(x, y);
    }
    DurationMs elapsed = Timer::now() - start;

    if (checksum) {
        if (*checksum) {
            assert(data == *checksum);
        } else {
            checksum->emplace(data);
        }
    }

    return std::chrono::duration_cast<Ms>(elapsed).count();
}


template <class Array2d>
void Prepare(Array2d& array) {
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            array.GetMutable(x, y).data[0] = rand();
        }
    }
}

template <size_t ObjectSize>
void RunTests(uint32_t steps = 100'000'000) {
    std::optional<char> checksum;

    {
        srand(seed);
        Naive<ObjectMock<32>, width, height> array;
        Prepare(array);
        std::cout << "Naive<Obj" << ObjectSize << "Byte>: " << DrunkenBenchMs(array, steps, &checksum) << " [ms]" << std::endl;
    }

    {
        srand(seed);
        Chunk<ObjectMock<32>, width, height, 1> array;
        Prepare(array);
        std::cout << "Chunk<Obj" << ObjectSize << "Byte, 1>: " << DrunkenBenchMs(array, steps, &checksum) << " [ms]" << std::endl;
    }

    {
        srand(seed);
        Chunk<ObjectMock<32>, width, height, 2> array;
        Prepare(array);
        std::cout << "Chunk<Obj" << ObjectSize << "Byte, 2>: " << DrunkenBenchMs(array, steps, &checksum) << " [ms]" << std::endl;
    }

    {
        srand(seed);
        Chunk<ObjectMock<32>, width, height, 4> array;
        Prepare(array);
        std::cout << "Chunk<Obj" << ObjectSize << "Byte, 4>: " << DrunkenBenchMs(array, steps, &checksum) << " [ms]" << std::endl;
    }

    {
        srand(seed);
        Chunk<ObjectMock<32>, width, height, 8> array;
        Prepare(array);
        std::cout << "Chunk<Obj" << ObjectSize << "Byte, 8>: " << DrunkenBenchMs(array, steps, &checksum) << " [ms]" << std::endl;
    }

    {
        srand(seed);
        Chunk<ObjectMock<32>, width, height, 16> array;
        Prepare(array);
        std::cout << "Chunk<Obj" << ObjectSize << "Byte, 16>: " << DrunkenBenchMs(array, steps, &checksum) << " [ms]" << std::endl;
    }
}
    

int main() {
    std::optional<char> checksum;
    RunTests<1>();
    RunTests<4>();
    RunTests<16>();
    RunTests<64>();
    RunTests<256>();
}
