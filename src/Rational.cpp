#include "Rational.h"

#include <cassert>
#include <cmath>
#include <numeric>
#include <utility>

namespace Groebner {

Rational::Rational(Rational::ValueType numerator,
                   Rational::ValueType denominator)
    : numerator_(numerator), denominator_(denominator) {
    assert(denominator_ != 0 && "Denominator must not be zero");
    Reduce();
}

Rational::ValueType Rational::GetNumerator() const {
    return numerator_;
}

Rational::ValueType Rational::GetDenominator() const {
    return denominator_;
}

Rational Rational::operator-() const {
    return Rational(-numerator_, denominator_);
}

Rational Rational::operator+() const {
    return *this;
}

bool Rational::IsZero() const {
    return numerator_ == 0;
}

Rational& Rational::operator+=(const Rational& other) {
    ValueType lcm = std::lcm(denominator_, other.denominator_);
    numerator_ = numerator_ * (lcm / denominator_) +
                 other.numerator_ * (lcm / other.denominator_);
    denominator_ = lcm;
    Reduce();
    return *this;
}

Rational& Rational::operator-=(const Rational& other) {
    ValueType lcm = std::lcm(denominator_, other.denominator_);
    numerator_ = numerator_ * (lcm / denominator_) -
                 other.numerator_ * (lcm / other.denominator_);
    denominator_ = lcm;
    Reduce();
    return *this;
}

Rational& Rational::operator*=(const Rational& other) {
    numerator_ *= other.numerator_;
    denominator_ *= other.denominator_;
    Reduce();
    return *this;
}

Rational& Rational::operator/=(const Rational& other) {
    assert(other.numerator_ != 0 && "Can't divide by zero");

    // self-division case
    ValueType temp = other.numerator_;
    numerator_ *= other.denominator_;
    denominator_ *= temp;

    Reduce();
    return *this;
}

Rational Rational::operator+(const Rational& other) const {
    Rational temp(*this);
    temp += other;
    return temp;
}

Rational Rational::operator-(const Rational& other) const {
    Rational temp(*this);
    temp -= other;
    return temp;
}

Rational Rational::operator*(const Rational& other) const {
    Rational temp(*this);
    temp *= other;
    return temp;
}

Rational Rational::operator/(const Rational& other) const {
    Rational temp(*this);
    temp /= other;
    return temp;
}

bool Rational::operator==(const Rational& other) const {
    return numerator_ == other.numerator_ && denominator_ == other.denominator_;
}

bool Rational::operator!=(const Rational& other) const {
    return !(*this == other);
}

bool Rational::operator<(const Rational& other) const {
    ValueType lcm = std::lcm(denominator_, other.denominator_);
    return numerator_ * (lcm / denominator_) <
           other.numerator_ * (lcm / other.denominator_);
}

bool Rational::operator<=(const Rational& other) const {
    return (*this < other) || (*this == other);
}

bool Rational::operator>(const Rational& other) const {
    return other < *this;
}

bool Rational::operator>=(const Rational& other) const {
    return other <= *this;
}

void Rational::Reduce() {
    assert(denominator_ != 0 && "Denominator must not be zero");
    if (numerator_ == 0) {
        denominator_ = 1;
        return;
    }

    if (denominator_ < 0) {
        numerator_ *= -1;
        denominator_ *= -1;
    }

    ValueType gcd = std::gcd(numerator_, denominator_);
    numerator_ /= gcd;
    denominator_ /= gcd;
}

}  // namespace Groebner
