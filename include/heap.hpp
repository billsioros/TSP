
#pragma once

#include <functional>       // const std::function<bool(const T&, const T&)>

template <typename T>
class heap
{
    const std::function<bool(const T&, const T&)> priority;

    const std::size_t max; std::size_t size;

    T * items;

public:
    
    static bool less(const T&, const T&);
    static bool greater(const T&, const T&);

    heap(std::size_t, const std::function<bool(const T&, const T&)>& priority);
    ~heap();

    bool push(const T&);
    bool pop(T&);
};

#include "heap.ipp"
