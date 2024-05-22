#pragma once

#include <array>

#include <cstddef>

template <typename T, size_t W, size_t H>
class Naive {
public:
    Naive()
        : _data(W * H) {

    }

public:
    const T& Get(size_t x, size_t y) const {
        return _data[x + y * W];
    }

    T& GetMutable(size_t x, size_t y) {
        return _data[x + y * W];
    }

private:
    std::vector<T> _data;
};
