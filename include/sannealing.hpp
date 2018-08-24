
#pragma once

#include <functional>   // std::function

template <typename T>
T SimulatedAnnealing(
    const T&,
    const std::function<T(const T&)>&,
    const std::function<double(const T&)>&,
    double,
    double,
    std::size_t
);

#include "sannealing.ipp"
