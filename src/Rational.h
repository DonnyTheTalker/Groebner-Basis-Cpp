#pragma once

#include <cinttypes>

class Rational {
public:
    using ValueType = int64_t;
public:
    explicit Rational(ValueType numerator = 0, ValueType denominator = 1);
    Rational(const Rational &other) = default;
    Rational(Rational &&other) noexcept;
    Rational &operator=(const Rational &other) = default;
    Rational &operator=(Rational &&other) noexcept;

public:
    ValueType Numerator() const;
    ValueType Denominator() const;
    bool IsZero() const;

    Rational &operator+=(const Rational &other);
    Rational &operator-=(const Rational &other);
    Rational &operator*=(const Rational &other);
    Rational &operator/=(const Rational &other);

    Rational operator+(const Rational &other) const;
    Rational operator-(const Rational &other) const;
    Rational operator*(const Rational &other) const;
    Rational operator/(const Rational &other) const;

    bool operator==(const Rational &other) const;

private:
    void Reduce();

private:
    // considering denominator > 0
    // and gcd(numerator, denominator) = 1
    // zero as represented as 0 / 1
    ValueType numerator_ = 0;
    ValueType denominator_ = 1;
};



