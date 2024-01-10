#include "Rational.h"

#include <cassert>
#include <cmath>
#include <utility>

namespace {
    Rational::ValueType FindGcd(Rational::ValueType x, Rational::ValueType y) {
        while (y) {
            x %= y;
            std::swap(x, y);
        }
        return x;
    }

    Rational::ValueType FindLcm(Rational::ValueType x, Rational::ValueType y) {
        return x * y / FindGcd(std::abs(x), std::abs(y));
    }
}

Rational::Rational(Rational::ValueType numerator, Rational::ValueType denominator) :
        numerator_(numerator), denominator_(denominator) {
    assert(denominator_ != 0 && "Denominator must not be zero");
    Reduce();
}

Rational::Rational(Rational &&other) noexcept: numerator_(std::exchange(other.numerator_, 0)),
                                               denominator_(std::exchange(other.denominator_, 1)) {

}

Rational &Rational::operator=(Rational &&other) noexcept {
    if (this != &other) {
        numerator_ = std::exchange(other.numerator_, 0);
        denominator_ = std::exchange(other.denominator_, 1);
    }

    return *this;
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

    ValueType gcd = FindGcd(std::abs(numerator_), std::abs(denominator_));
    numerator_ /= gcd;
    denominator_ /= gcd;
}

Rational::ValueType Rational::Numerator() const {
    return numerator_;
}

Rational::ValueType Rational::Denominator() const {
    return denominator_;
}

bool Rational::IsZero() const {
    return numerator_ == 0;
}

Rational &Rational::operator+=(const Rational &other) {
    ValueType lcm = FindLcm(denominator_, other.denominator_);
    numerator_ = numerator_ * (lcm / denominator_) + other.numerator_ * (lcm / other.denominator_);
    denominator_ = lcm;
    Reduce();
    return *this;
}

Rational &Rational::operator-=(const Rational &other) {
    ValueType lcm = FindLcm(denominator_, other.denominator_);
    numerator_ = numerator_ * (lcm / denominator_) - other.numerator_ * (lcm / other.denominator_);
    denominator_ = lcm;
    Reduce();
    return *this;
}

Rational &Rational::operator*=(const Rational &other) {
    numerator_ *= other.numerator_;
    denominator_ *= other.denominator_;
    Reduce();
    return *this;
}

Rational &Rational::operator/=(const Rational &other) {
    assert(other.numerator_ != 0 && "Can't divide by zero");

    // self-division case
    ValueType temp = other.numerator_;

    numerator_ *= other.denominator_;
    denominator_ *= temp;
    Reduce();
    return *this;
}

Rational Rational::operator+(const Rational &other) const {
    Rational temp(*this);
    return temp += other;
}

Rational Rational::operator-(const Rational &other) const {
    Rational temp(*this);
    return temp -= other;
}

Rational Rational::operator*(const Rational &other) const {
    Rational temp(*this);
    return temp *= other;
}

Rational Rational::operator/(const Rational &other) const {
    Rational temp(*this);
    return temp /= other;
}

bool Rational::operator==(const Rational &other) const {
    return numerator_ == other.numerator_ && denominator_ == other.denominator_;
}

bool Rational::operator<(const Rational &other) const {
    ValueType lcm = FindLcm(denominator_, other.denominator_);
    return numerator_ * (lcm / denominator_) < other.numerator_ * (lcm / other.denominator_);
}

bool Rational::operator<=(const Rational &other) const {
    ValueType lcm = FindLcm(denominator_, other.denominator_);
    return numerator_ * (lcm / denominator_) <= other.numerator_ * (lcm / other.denominator_);

}

bool Rational::operator>(const Rational &other) const {
    ValueType lcm = FindLcm(denominator_, other.denominator_);
    return numerator_ * (lcm / denominator_) > other.numerator_ * (lcm / other.denominator_);

}

bool Rational::operator>=(const Rational &other) const {
    ValueType lcm = FindLcm(denominator_, other.denominator_);
    return numerator_ * (lcm / denominator_) >= other.numerator_ * (lcm / other.denominator_);
}

Rational Rational::operator-() const {
    return Rational(-numerator_, denominator_);
}
