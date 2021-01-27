#pragma once

#include <complex>
#include <valarray>

#include <SFML/System/Vector2.hpp>

constexpr double pi = 3.14159265359;

typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;

inline float len(sf::Vector2f& a) {
    return sqrt(a.x * a.x + a.y * a.y);
}

inline float dot(sf::Vector2f& a, sf::Vector2f& b) {
    return a.x * b.x + a.y * b.y;
}
