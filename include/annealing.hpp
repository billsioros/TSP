
#pragma once

#include <functional>   // std::function

namespace Annealing
{
    template <typename T>
    T simulated(
        const T&,
        const std::function<T(const T&)>&,
        const std::function<double(const T&)>&,
        double,
        double,
        std::size_t
    );

    template <typename T>
    T compressed(
        const T& initial,
        const std::function<T(const T&)>&,
        const std::function<double(const T&)>&,
        const std::function<double(const T&)>&,
        double,
        double,
        double,
        double,
        double,
        std::size_t,
        std::size_t,
        std::size_t,
        std::size_t,
        std::size_t
    );
}

#include "annealing.ipp"
