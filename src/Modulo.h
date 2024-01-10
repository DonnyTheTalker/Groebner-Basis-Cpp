#pragma once

#include <cinttypes>

class Modulo {
public:
    using OrderType = int64_t;
    using ValueType = int64_t;
public:
    Modulo() = delete;
    explicit Modulo(OrderType order, ValueType value = 0);
    Modulo(const Modulo &other) = default;
    Modulo(Modulo &&other) noexcept;
    Modulo &operator=(const Modulo &other);
    Modulo &operator=(Modulo &&other) noexcept;

public:
    ValueType Value() const;
    bool IsZero() const;

    Modulo &operator+=(const Modulo &other);
    Modulo &operator-=(const Modulo &other);
    Modulo &operator*=(const Modulo &other);
    Modulo &operator/=(const Modulo &other);

    Modulo operator+(const Modulo &other) const;
    Modulo operator-(const Modulo &other) const;
    Modulo operator*(const Modulo &other) const;
    Modulo operator/(const Modulo &other) const;

    bool operator==(const Modulo &other) const;
    bool operator<(const Modulo &other) const;
    bool operator<=(const Modulo &other) const;
    bool operator>(const Modulo &other) const;
    bool operator>=(const Modulo &other) const;

private:
    void Normalize();
    Modulo Inverse() const;

private:
    // considering order_ to be positive and prime
    // 0 <= value_ < order_
    const OrderType order_;
    ValueType value_;
};

