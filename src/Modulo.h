#pragma once

#include <cassert>
#include <cinttypes>
#include <cstdlib>

namespace Groebner {
namespace GroebnerDetails {
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
                                    ModuloValueType* coef_y) {
        assert(coef_x && coef_y);

        if (x == 0) {
            *coef_x = 0;
            *coef_y = 1;
            return y;
        }

        ModuloValueType temp_x, temp_y;
        ModuloValueType gcd = FindGcdExtended(y % x, x, &temp_x, &temp_y);
        *coef_x = temp_y - (y / x) * temp_x;
        *coef_y = temp_x;
        return gcd;
    }
}  // namespace GroebnerDetails

template <int64_t Modulus>
class Modulo {
    public:
        using ValueType = GroebnerDetails::ModuloValueType;

        explicit Modulo(ValueType value = 0) : value_(value) {
            assert(GroebnerDetails::IsPrime(Modulus) &&
                   "Modulus must be prime number");
            Normalize();
        }

        ValueType GetValue() const { return value_; }

        bool IsZero() const { return value_ == 0; }

        //        size_t GetModulus() const {
        //            return Modulus;
        //        }

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
            value_ *= other.GetInversed().value_;
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

        bool operator!=(const Modulo& other) const {
            return !(*this == other);
        }

        bool operator<(const Modulo& other) const {
            return value_ < other.value_;
        }

        bool operator<=(const Modulo& other) const {
            return (*this < other) || (*this == other);
        }

        bool operator>(const Modulo& other) const {
            return !(*this <= other);
        }

        bool operator>=(const Modulo& other) const {
            return (*this > other) || (*this == other);
        }

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
            GroebnerDetails::FindGcdExtended(value_, Modulus, &x, &y);
            value_ = x;
            Normalize();
        }

        Modulo GetInversed() const {
            Modulo temp(*this);
            temp.Inverse();
            return temp;
        }

    private:
        // considering modulus_ to be positive and prime
        // 0 <= value_ < Modulus
        ValueType value_;
};

}  // namespace Groebner