#pragma once

#include <array>

#include <cstddef>
#include <iostream>

template <typename T, size_t W, size_t H, size_t ChunkSize = 8>
class Chunk {
public:
    static constexpr size_t Mod = ChunkSize - 1;

    static_assert(W % ChunkSize == 0);
    static_assert(H % ChunkSize == 0);
    static_assert((Mod & ChunkSize) == 0);

    static constexpr size_t GridW = (W + ChunkSize - 1) / ChunkSize;
    static constexpr size_t GridH = (H + ChunkSize - 1) / ChunkSize;

    static constexpr size_t AdjW = GridW * ChunkSize;
    static constexpr size_t AdjH = GridH * ChunkSize;

    static constexpr size_t ChunkVolume = ChunkSize * ChunkSize;

public:
    Chunk()
        : _data(AdjW * AdjH) {

    }

    T& GetMutable(size_t x, size_t y) {
        size_t chunkX = x / ChunkSize;
        size_t chunkY = y / ChunkSize;
        size_t position = (chunkX + chunkY * GridW) * ChunkVolume;

        size_t inChunkX = x & Mod;
        size_t inChunkY = y & Mod;
        size_t offset = inChunkX + inChunkY * ChunkSize;

        return _data[position + offset];
    }

    const T& Get(size_t x, size_t y) const {
        size_t chunkX = x / ChunkSize;
        size_t chunkY = y / ChunkSize;
        size_t position = (chunkX + chunkY * GridW) * ChunkVolume;

        size_t inChunkX = x & Mod;
        size_t inChunkY = y & Mod;
        size_t offset = inChunkX + inChunkY * ChunkSize;

        return _data[position + offset];

    }

private:
    std::vector<T> _data;
};
