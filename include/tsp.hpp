
#pragma once

#include "vector2.hpp"
#include <functional>   // std::function
#include <vector>       // std::vector
#include <iosfwd>       // std::ostream
#include <utility>      // std::pair

template <typename T>
class tsp;

template <typename T>
std::ostream& operator<<(std::ostream&, const tsp<T>&);

template <typename T>
class tsp
{
protected:

    T _depot;

    std::vector<T> _elements;
    
    std::function<double(const T&)> _serviceTime;

    std::function<double(const T&, const T&)> _duration;

    double _cost;

    double _partialCost(const T&, const T&) const;
    double _totalCost() const;

public:

    tsp();
    tsp
    (
        const T&,
        const std::vector<T>&,
        const std::function<double(const T&)>&,
        const std::function<double(const T&, const T&)>&
    );

    tsp(const tsp&);
    tsp(tsp&&) noexcept;

    tsp& operator=(const tsp&);
    tsp& operator=(tsp&&) noexcept;

    const T& depot() const { return _depot; }
    const std::vector<T>& elements() const { return _elements; }

    double cost() const { return _cost; }

    friend std::ostream& operator<< <T>(std::ostream&, const tsp&);

    tsp nneighbour() const;
    tsp opt2() const;
    tsp sannealing() const;
};

template <typename T>
class tsptw;

template <typename T>
std::ostream& operator<<(std::ostream&, const tsptw<T>&);

template <typename T>
class tsptw : public tsp<T>
{
    double _departureTime;

    std::function<Vector2(const T&)> _timewindow;

    double _penalty;

    double _partialPenalty(double&, const T&, const T&) const;
    double _totalPenalty() const;

public:

    using Timewindow = Vector2;

    tsptw();
    tsptw
    (
        const T& depot,
        const std::vector<T>&,
        const std::function<double(const T&)>&,
        const std::function<double(const T&, const T&)>&,
        double,
        const std::function<Timewindow(const T&)>&
    );
    
    tsptw(const tsptw&);
    tsptw(tsptw&&) noexcept;

    tsptw& operator=(const tsptw&);
    tsptw& operator=(tsptw&&) noexcept;

    tsptw& operator=(const tsp<T>&);
    tsptw& operator=(tsp<T>&&) noexcept;

    double penalty() const { return _penalty; }

    friend std::ostream& operator<< <T>(std::ostream&, const tsptw&);

    tsptw cannealing() const;
};

#include "tsp.ipp"
