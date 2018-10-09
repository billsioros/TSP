
#pragma once

#include <iosfwd>       // std::istream & std::ostream

class Vector2
{
    double _x, _y;

public:

    Vector2();
    Vector2(double, double);
    Vector2(const Vector2&);
    Vector2(const Vector2&&) noexcept;

    // Access:
    double x() const { return _x; }
    double y() const { return _y; }

    // IO:
    friend std::ostream& operator<<(std::ostream&, const Vector2&);
    friend std::istream& operator>>(std::istream&, Vector2&);

    // Operations:
    Vector2& operator=(const Vector2&);
    Vector2& operator=(const Vector2&&);

    friend Vector2 operator+(const Vector2&, const Vector2&);
    friend Vector2 operator-(const Vector2&, const Vector2&);
    friend double  operator*(const Vector2&, const Vector2&);
    friend Vector2 operator*(const Vector2&, double);
    friend Vector2 operator/(const Vector2&, double);

    Vector2& operator+=(const Vector2&);
    Vector2& operator-=(const Vector2&);
    Vector2& operator*=(double);
    Vector2& operator/=(double);

    friend bool operator< (const Vector2&, const Vector2&);
    friend bool operator> (const Vector2&, const Vector2&);
    friend bool operator==(const Vector2&, const Vector2&);
    friend bool operator!=(const Vector2&, const Vector2&);
};
