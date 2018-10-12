
#pragma once

#include "annealing.hpp"
#include <functional>       // std::function
#include <vector>           // std::vector
#include <utility>          // std::pair
#include <fstream>          // std::ostream
#include <iomanip>          // std::setw
#include <algorithm>        // std::reverse
#include <stdexcept>        // std::invalid_argument
#include <limits>           // std::numeric_limits

// Class tsp:
template <typename  T>
double tsp<T>::_partialCost(const T& A, const T& B) const
{
    return _serviceTime(A) + _duration(A, B);
}

template <typename T>
double tsp<T>::_totalCost() const
{    
    double _cost = _partialCost(_depot, _elements.front());

    for (std::size_t j = 0; j < _elements.size() - 1UL; j++)
        _cost += _partialCost(_elements[j], _elements[j + 1UL]);

    _cost += _partialCost(_elements.back(), _depot);

    return _cost;
}

template <typename T>
tsp<T>::tsp()
:
_depot(0.0, 0.0),
_elements(),
_serviceTime([](const T& t) { return std::numeric_limits<double>().max(); }),
_duration([](const T& A, const T& B) { return std::numeric_limits<double>().max(); }),
_cost(std::numeric_limits<double>().max())
{
}

template <typename T>
tsp<T>::tsp
(
    const T& _depot,
    const std::vector<T>& _elements,
    const std::function<double(const T&)>& _serviceTime,
    const std::function<double(const T&, const T&)>& _duration
)
:
_depot(_depot),
_elements(_elements),
_serviceTime(_serviceTime),
_duration(_duration),
_cost(_totalCost())
{
    if (std::find(_elements.begin(), _elements.end(), _depot) != _elements.end())
        throw std::invalid_argument("depot amongst the elements");
}

template <typename T>
tsp<T>::tsp(const tsp<T>& other)
:
_depot(other._depot),
_elements(other._elements),
_serviceTime(other._serviceTime),
_duration(other._duration),
_cost(other._cost)
{
}

template <typename T>
tsp<T>::tsp(tsp<T>&& other) noexcept
:
_depot(std::move(other._depot)),
_elements(std::move(other._elements)),
_serviceTime(std::move(other._serviceTime)),
_duration(std::move(other._duration)),
_cost(std::move(other._cost))
{
}

template <typename T>
tsp<T>& tsp<T>::operator=(const tsp<T>& other)
{
    _depot       = other._depot;
    _elements    = other._elements;
    _serviceTime = other._serviceTime;
    _duration    = other._duration;
    _cost        = other._cost;

    return *this;
}

template <typename T>
tsp<T>& tsp<T>::operator=(tsp<T>&& other) noexcept
{
    _depot       = std::move(other._depot);
    _elements    = std::move(other._elements);
    _serviceTime = std::move(other._serviceTime);
    _duration    = std::move(other._duration);
    _cost        = std::move(other._cost);

    return *this;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const tsp<T>& path)
{
    std::size_t id = 0UL;
    for (const auto& element : path._elements)
    {
        os
        << std::setw(3) << std::setfill('0') << ++id << ". "
        << element << std::endl;
    }
    
    os << "\nCost: " << path.cost();

    return os;
}

template <typename T>
tsp<T> tsp<T>::nneighbour() const
{
    std::vector<T> oelements(this->_elements), nelements;

    auto nearestTo = [&oelements, this](const T& current)
    {
        auto it = std::min_element
        (
            oelements.begin(),
            oelements.end(),
            [&current, this](const T& A, const T& B)
            {
                return this->_partialCost(current, A) < this->_partialCost(current, B);
            }
        );

        T nearest(*it); oelements.erase(it); return nearest;
    };

    nelements.emplace_back(nearestTo(this->_depot));
    while (!oelements.empty())
        nelements.emplace_back(nearestTo(nelements.back()));

    return tsp<T>(this->_depot, nelements, this->_serviceTime, this->_duration);
}

template <typename T>
tsp<T> tsp<T>::opt2() const
{
    tsp<T> best(*this);

    auto opt2swap = [&best](std::size_t i, std::size_t k)
    {
        // 1. take route[0] to route[i-1] and add them in order to new_route
        // 2. take route[i] to route[k] and add them in reverse order to new_route
        // 3. take route[k+1] to end and add them in order to new_route

        std::vector<T> next(best._elements);

        std::reverse(next.begin() + i, next.begin() + k);

        return tsp<T>(best._depot, next, best._serviceTime, best._duration);
    };

    for (std::size_t i = 0; i < best._elements.size() - 1; i++)
    {
        for (std::size_t k = i + 1UL; k < best._elements.size(); k++)
        {
            const tsp<T> neighbour(opt2swap(i, k));

            if (neighbour._cost < best._cost)
                return neighbour.opt2();
        }
    }

    return best;
}

template <typename T>
tsp<T> tsp<T>::sannealing() const
{
    auto randomSwap = [](const tsp<T>& current)
    {
        std::vector<T> next(current._elements);

        const std::size_t i = std::rand() % next.size();
        const std::size_t j = std::rand() % next.size();

        const Vector2 element(next[i]);
        next[i] = next[j];
        next[j] = element;

        return tsp(current._depot, next, current._serviceTime, current._duration);
    };

    const double temperature = 100000.0, cooling = 0.000005;
    const std::size_t iterations = 1000000UL;

    return Annealing::simulated<tsp<T>>
    (
        *this,
        randomSwap,
        [](const tsp<T>& path) { return path._cost; },
        temperature,
        cooling,
        iterations
    );
}

// Class tsptw:
template <typename T>
double tsptw<T>::_partialPenalty(double& arrivalTime, const T& A, const T& B) const
{
    arrivalTime += this->_partialCost(A, B);

    const double startOfService = std::max<double>
    (
        arrivalTime,
        this->_timewindow(B).x()
    );

    return std::max<double>
    (
        0.0,
        startOfService + this->_serviceTime(B) - this->_timewindow(B).y()
    );
}

template <typename T>
double tsptw<T>::_totalPenalty() const
{
    double arrivalTime = _departureTime, _penalty;
    
    _penalty = _partialPenalty(arrivalTime, this->_depot, this->_elements.front());
    
    for (std::size_t j = 0; j < this->_elements.size() - 1UL; j++)
        _penalty += _partialPenalty(arrivalTime, this->_elements[j], this->_elements[j + 1UL]);

    _penalty += _partialPenalty(arrivalTime, this->_elements.back(), this->_depot);

    return _penalty;
}

template <typename T>
tsptw<T>::tsptw()
:
tsp<T>(),
_departureTime(std::numeric_limits<double>().max()),
_timewindow([](const T& t) { return Vector2(0.0, 0.0) }),
_penalty(std::numeric_limits<double>().max())
{
}

template <typename T>
tsptw<T>::tsptw
(
    const T& _depot,
    const std::vector<T>& _elements,
    const std::function<double(const T&)>& _serviceTime,
    const std::function<double(const T&, const T&)>& _duration,
    double _departureTime,
    const std::function<tsptw::Timewindow(const T&)>& _timewindow
)
:
tsp<T>(_depot, _elements, _serviceTime, _duration),
_departureTime(_departureTime),
_timewindow(_timewindow),
_penalty(_totalPenalty())
{
}

template <typename T>
tsptw<T>::tsptw(const tsptw<T>& other)
:
tsp<T>(other),
_departureTime(other._departureTime),
_timewindow(other._timewindow),
_penalty(other._penalty)
{
}

template <typename T>
tsptw<T>::tsptw(tsptw<T>&& other) noexcept
:
tsp<T>(std::move(other)),
_departureTime(std::move(other._departureTime)),
_timewindow(std::move(other._timewindow)),
_penalty(std::move(other._penalty))
{
}

template <typename T>
tsptw<T>& tsptw<T>::operator=(const tsptw<T>& other)
{
    tsp<T>::operator=(other);

    _departureTime = other._departureTime;
    _timewindow    = other._timewindow;
    _penalty       = other._penalty;

    return *this;
}

template <typename T>
tsptw<T>& tsptw<T>::operator=(tsptw<T>&& other) noexcept
{
    tsp<T>::operator=(std::move(other));
    
    _departureTime = std::move(other._departureTime);
    _timewindow    = std::move(other._timewindow);
    _penalty       = std::move(other._penalty);

    return *this;
}

template <typename T>
tsptw<T>& tsptw<T>::operator=(const tsp<T>& other)
{
    tsp<T>::operator=(other);

    _penalty = _totalPenalty();

    return *this;
}

template <typename T>
tsptw<T>& tsptw<T>::operator=(tsp<T>&& other) noexcept
{
    tsp<T>::operator=(std::move(other));

    _penalty = _totalPenalty();

    return *this;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const tsptw<T>& path)
{
    os << static_cast<tsp<T>>(path) << " Penalty: " << path._penalty;

    return os;
}

template <typename T>
tsptw<T> tsptw<T>::cannealing() const
{
    auto shift1 = [](const tsptw<T>& current)
    {
        std::vector<T> next(current._elements);

        const std::size_t i = std::rand() % next.size();
        const std::size_t j = std::rand() % next.size();

        const T element(next[i]);
        next.erase(next.begin() + i);
        next.insert(next.begin() + j, element);

        return tsptw<T>
        (
            current._depot,
            next,
            current._serviceTime,
            current._duration,
            current._departureTime,
            current._timewindow
        );
    };

    // Parameter Initialization (Robust Set provided by the authors):
    const double COOLING    = 0.95,    // (1)  Cooling Coefficient
                ACCEPTANCE  = 0.94,    // (2)  Initial Acceptance Ratio
                PRESSURE0   = 0.0,     // (3)  Initial Pressure
                COMPRESSION = 0.06,    // (4)  Compression Coefficient
                PCR         = 0.9999;  // (5)  Pressure Cap Ratio

    const std::size_t IPT = 30000UL,    // (6)  Iterations per temperature
                    MTC   = 100UL,      // (7)  Minimum number of temperature changes
                    ITC   = 75UL,       // (8)  Maximum idle temperature changes
                    TLI   = IPT,        // (9)  Trial loop of iterations
                    TNP   = 5000UL;     // (10) Trial neighbour pairs

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    return Annealing::compressed<tsptw<T>>
    (
        *this,
        shift1,
        [](const tsptw<T>& path) { return path._cost; },
        [](const tsptw<T>& path) { return path._penalty; },
        COOLING,
        ACCEPTANCE,
        PRESSURE0,
        COMPRESSION,
        PCR,
        IPT,
        MTC,
        ITC,
        TLI,
        TNP
    );
}
