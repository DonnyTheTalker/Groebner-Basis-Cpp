#pragma once

#include <cinttypes>

namespace Groebner {

class Rational {
    public:
        using ValueType = int64_t;

        explicit Rational(ValueType numerator = 0, ValueType denominator = 1);

        ValueType GetNumerator() const;
        ValueType GetDenominator() const;

        Rational operator-() const;
        Rational operator+() const;

        bool IsZero() const;

        Rational& operator+=(const Rational& other);
        Rational& operator-=(const Rational& other);
        Rational& operator*=(const Rational& other);
        Rational& operator/=(const Rational& other);

        Rational operator+(const Rational& other) const;
        Rational operator-(const Rational& other) const;
        Rational operator*(const Rational& other) const;
        Rational operator/(const Rational& other) const;

        bool operator==(const Rational& other) const;
        bool operator!=(const Rational& other) const;
        bool operator<(const Rational& other) const;
        bool operator<=(const Rational& other) const;
        bool operator>(const Rational& other) const;
        bool operator>=(const Rational& other) const;

    private:
        void Reduce();

    private:
        // considering denominator > 0
        // and gcd(numerator, denominator) = 1
        // zero is represented as 0 / 1
        ValueType numerator_ = 0;
        ValueType denominator_ = 1;
};

}  // namespace Groebner