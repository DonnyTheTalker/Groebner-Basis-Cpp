#pragma once

#include "FieldFwd.h"
#include "MonomialDegree.h"

#include <cassert>

template<IsField Field>
class Monomial {
public:
    Monomial() = delete;

    Monomial(Field coef, const MonomialDegree &degree);
    Monomial(const Monomial<Field> &other) = default;
    Monomial(Monomial<Field> &&other) noexcept = default;
    Monomial<Field> &operator=(const Monomial<Field> &other);
    Monomial<Field> &operator=(Monomial<Field> &&other) noexcept;

public:
    Field GetCoef() const;
    MonomialDegree::SizeType GetSize() const;

    MonomialDegree &GetDegree();
    const MonomialDegree &GetDegree() const;

    bool IsZero() const;

public:
    Monomial<Field> &operator+=(const Monomial<Field> &other);
    Monomial<Field> &operator-=(const Monomial<Field> &other);
    Monomial<Field> &operator*=(const Monomial<Field> &other);
    Monomial<Field> &operator/=(const Monomial<Field> &other);

    Monomial<Field> operator+(const Monomial<Field> &other);
    Monomial<Field> operator-(const Monomial<Field> &other);
    Monomial<Field> operator*(const Monomial<Field> &other);
    Monomial<Field> operator/(const Monomial<Field> &other);

    bool operator==(const Monomial<Field> &other) const;

private:
    Field coef_;
    MonomialDegree degree_;
};

template<IsField Field>
Monomial<Field>::Monomial(Field coef, const MonomialDegree &degree) : coef_(coef), degree_(degree) {

}

template<IsField Field>
Monomial<Field> &Monomial<Field>::operator=(const Monomial<Field> &other) {
    assert(GetSize() == other.GetSize() && "Monomial sizes must be equal");
    coef_ = other.coef_;
    degree_ = other.degree_;
    return *this;
}

template<IsField Field>
Monomial<Field> &Monomial<Field>::operator=(Monomial<Field> &&other) noexcept {
    assert(GetSize() == other.GetSize() && "Monomial sizes must be equal");
    if (*this != other) {
        coef_ = std::move(other.coef_);
        degree_ = std::move(other.degree_);
    }
    return *this;
}

template<IsField Field>
Field Monomial<Field>::GetCoef() const {
    return coef_;
}


template<IsField Field>
MonomialDegree::SizeType Monomial<Field>::GetSize() const {
    return degree_.GetSize();
}

template<IsField Field>
MonomialDegree &Monomial<Field>::GetDegree() {
    return degree_;
}

template<IsField Field>
const MonomialDegree &Monomial<Field>::GetDegree() const {
    return degree_;
}

template<IsField Field>
bool Monomial<Field>::IsZero() const {
    return coef_.IsZero();
}

template<IsField Field>
Monomial<Field> &Monomial<Field>::operator+=(const Monomial<Field> &other) {
    assert(GetSize() == other.GetSize() && "Can't add monomials with different degree");
    coef_ += other.coef_;
    return *this;
}

template<IsField Field>
Monomial<Field> &Monomial<Field>::operator-=(const Monomial<Field> &other) {
    assert(GetSize() == other.GetSize() && "Can't substitute monomials with different degree");
    coef_ -= other.coef_;
    return *this;
}

template<IsField Field>
Monomial<Field> &Monomial<Field>::operator*=(const Monomial<Field> &other) {
    assert(GetSize() == other.GetSize() && "Can't multiply monomials with different degree");
    coef_ *= other.coef_;
    degree_ += other.degree_;
    return *this;
}

template<IsField Field>
Monomial<Field> &Monomial<Field>::operator/=(const Monomial<Field> &other) {
    // TODO add assertion for lhs.degree >= rhs.degree at every index
    assert(GetSize() == other.GetSize() && "Can't divide monomials with different degree");
    coef_ /= other.coef_;
    degree_ -= other.degree_;
    return *this;
}

template<IsField Field>
Monomial<Field> Monomial<Field>::operator+(const Monomial<Field> &other) {
    assert(GetSize() == other.GetSize() && "Can't add monomials with different degrees");
    Monomial<Field> temp(*this);
    return temp += other;
}

template<IsField Field>
Monomial<Field> Monomial<Field>::operator-(const Monomial<Field> &other) {
    assert(GetSize() == other.GetSize() && "Can't substitute monomials with different degrees");
    Monomial<Field> temp(*this);
    return temp -= other;
}

template<IsField Field>
Monomial<Field> Monomial<Field>::operator*(const Monomial<Field> &other) {
    Monomial<Field> temp(*this);
    return temp *= other;
}

template<IsField Field>
Monomial<Field> Monomial<Field>::operator/(const Monomial<Field> &other) {
    // TODO add assert lhs.degree >= rhs.degree at every index
    Monomial<Field> temp(*this);
    return temp /= other;
}

// TODO add == for monomials with different fields - and return false
template<IsField Field>
bool Monomial<Field>::operator==(const Monomial<Field> &other) const {
    return coef_ == other.coef_ && degree_ == other.degree_;
}