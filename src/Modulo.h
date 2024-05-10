#pragma once

#include <cassert>
#include <cinttypes>
#include <cstdlib>

namespace Groebner {
namespace Details {
    constexpr bool IsPrime(int64_t n) {
        if (n <= 1 || (n > 2 && n % 2 == 0)) {
            return false;
        }

        for (size_t i = 3; i * i <= n; i++) {
            if (n % i == 0) {
                return false;
            }
        }

        return true;
    }

    using ModuloValueType = int64_t;

    ModuloValueType FindGcdExtended(ModuloValueType x, ModuloValueType y,
                                    ModuloValueType* coef_x,
                                    ModuloValueType* coef_y);
}  // namespace Details

template <int64_t N>
concept IsPrime = Details::IsPrime(N);

template <int64_t Modulus>
requires IsPrime<Modulus> class Modulo {
    public:
        using ValueType = Details::ModuloValueType;

        Modulo(ValueType value = 0) : value_(value) { Normalize(); }

        ValueType GetValue() const { return value_; }

        bool IsZero() const { return value_ == 0; }

        Modulo Abs() const {
            return *this;
        }

        Modulo operator-() const { return Modulo<Modulus>(-value_); }
        Modulo operator+() const { return *this; }

        Modulo& operator+=(const Modulo& other) {
            value_ += other.value_;
            Normalize();
            return *this;
        }

        Modulo& operator-=(const Modulo& other) {
            value_ -= other.value_;
            Normalize();
            return *this;
        }

        Modulo& operator*=(const Modulo& other) {
            value_ *= other.value_;
            Normalize();
            return *this;
        }

        Modulo& operator/=(const Modulo& other) {
            assert(other.value_ != 0 && "Can't divide by zero");
            value_ *= other.GetInverse().value_;
            Normalize();
            return *this;
        }

        Modulo operator+(const Modulo& other) const {
            Modulo temp(*this);
            temp += other;
            return temp;
        }

        Modulo operator-(const Modulo& other) const {
            Modulo temp(*this);
            temp -= other;
            return temp;
        }

        Modulo operator*(const Modulo& other) const {
            Modulo temp(*this);
            temp *= other;
            return temp;
        }

        Modulo operator/(const Modulo& other) const {
            Modulo temp(*this);
            temp /= other;
            return temp;
        }

        bool operator==(const Modulo& other) const {
            return value_ == other.value_;
        }

        bool operator!=(const Modulo& other) const { return !(*this == other); }

        bool operator<(const Modulo& other) const {
            return value_ < other.value_;
        }

        bool operator<=(const Modulo& other) const {
            return (*this < other) || (*this == other);
        }

        bool operator>(const Modulo& other) const { return other < *this; }

        bool operator>=(const Modulo& other) const { return other <= *this; }

    private:
        void Normalize() {
            if (value_ < 0) {
                value_ = (value_ % Modulus + Modulus) % Modulus;
            } else {
                value_ = value_ % Modulus;
            }
        }

        Modulo GetNormalized() { return Modulo(*this); }

        void Inverse() {
            assert(value_ != 0 && "No inverse element for zero");
            ValueType x, y;
            Details::FindGcdExtended(value_, Modulus, &x, &y);
            value_ = x;
            Normalize();
        }

        Modulo GetInverse() const {
            Modulo temp(*this);
            temp.Inverse();
            return temp;
        }

        // considering modulus_ to be positive and prime
        // 0 <= value_ < Modulus
        ValueType value_;
};

}  // namespace Groebner
