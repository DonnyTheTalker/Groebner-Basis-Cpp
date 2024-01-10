#include "Modulo.h"

#include <cassert>
#include <utility>

namespace {
    bool IsPrime(Modulo::ValueType x) {
        if (x < 2 || (x > 2 && x % 2 == 0)) {
            return false;
        }

        for (Modulo::ValueType temp = 3; temp * temp <= x; temp += 2) {
            if (x % temp == 0) {
                return false;
            }
        }

        return true;
    }

    Modulo::ValueType ExtendedFindGcd(Modulo::ValueType x, Modulo::ValueType y,
                                      Modulo::ValueType *coef_x, Modulo::ValueType *coef_y) {
        if (x == 0) {
            *coef_x = 0;
            *coef_y = 1;
            return y;
        }

        Modulo::ValueType temp_x, temp_y;
        Modulo::ValueType gcd = ExtendedFindGcd(y % x, x, &temp_x, &temp_y);
        *coef_x = temp_y - (y / x) * temp_x;
        *coef_y = temp_x;
        return gcd;
    }
}

Modulo::Modulo(Modulo::OrderType order, Modulo::ValueType value) : order_(order), value_(value) {
    assert(order > 0 && "Order must be positive");
    assert(IsPrime(order) && "Order must be prime number");

    Normalize();
}

Modulo::Modulo(Modulo &&other) noexcept: order_(other.order_), value_(std::exchange(other.value_, 0)) {
}

Modulo &Modulo::operator=(const Modulo &other) {
    assert(order_ == other.order_ && "Orders must be equal in '='");
    value_ = other.value_;
    return *this;
}

Modulo &Modulo::operator=(Modulo &&other) noexcept {
    assert(order_ == other.order_ && "Orders must be equal in '='");
    if (this != &other) {
        value_ = std::exchange(other.value_, 0);
    }
    return *this;
}

Modulo::ValueType Modulo::Value() const {
    return value_;
}

bool Modulo::IsZero() const {
    return value_ == 0;
}

Modulo Modulo::Inverse() const {
    assert(value_ != 0 && "Can't divide by zero");
    ValueType x, y;
    assert(ExtendedFindGcd(value_, order_, &x, &y) == 1 && "GCD of order and modulo must be 1");
    return Modulo(order_, x);
}

void Modulo::Normalize() {
    if (value_ < 0) {
        value_ = (value_ % order_ + order_) % order_;
    } else {
        value_ = value_ % order_;
    }
}

Modulo &Modulo::operator+=(const Modulo &other) {
    assert(order_ == other.order_ && "Orders must be equal in '+='");
    value_ += other.value_;
    Normalize();
    return *this;
}

Modulo &Modulo::operator-=(const Modulo &other) {
    assert(order_ == other.order_ && "Orders must be equal in '-='");
    value_ -= other.value_;
    Normalize();
    return *this;
}

Modulo &Modulo::operator*=(const Modulo &other) {
    assert(order_ == other.order_ && "Orders must be equal in '*='");
    value_ *= other.value_;
    Normalize();
    return *this;
}

Modulo &Modulo::operator/=(const Modulo &other) {
    assert(order_ == other.order_ && "Orders must be equal in '/='");
    value_ *= other.Inverse().value_;
    Normalize();
    return *this;
}

Modulo Modulo::operator+(const Modulo &other) const {
    assert(order_ == other.order_ && "Orders must be equal in '+'");
    Modulo temp(*this);
    return temp += other;
}

Modulo Modulo::operator-(const Modulo &other) const {
    assert(order_ == other.order_ && "Orders must be equal in '-'");
    Modulo temp(*this);
    return temp -= other;
}

Modulo Modulo::operator*(const Modulo &other) const {
    assert(order_ == other.order_ && "Orders must be equal in '*'");
    Modulo temp(*this);
    return temp *= other;
}

Modulo Modulo::operator/(const Modulo &other) const {
    assert(order_ == other.order_ && "Orders must be equal in '/'");
    Modulo temp(*this);
    return temp /= other;
}

bool Modulo::operator==(const Modulo &other) const {
    return value_ == other.value_ && order_ == other.order_;
}

bool Modulo::operator<(const Modulo &other) const {
    return order_ == other.order_ && value_ < other.value_;
}

bool Modulo::operator<=(const Modulo &other) const {
    return order_ == other.order_ && value_ <= other.value_;
}

bool Modulo::operator>(const Modulo &other) const {
    return order_ == other.order_ && value_ > other.value_;
}

bool Modulo::operator>=(const Modulo &other) const {
    return order_ == other.order_ && value_ >= other.value_;
}


