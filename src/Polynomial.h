#pragma once

#include "ComparatorFwd.h"
#include "FieldFwd.h"

#include <algorithm>

template<IsField Field, IsComparator Comparator = LexOrder>
class Polynomial {
public:
    Polynomial() = default;

    explicit Polynomial(const std::vector<Monomial<Field>> &monomials);
    Polynomial(const Polynomial<Field, Comparator> &other) = default;
    Polynomial(Polynomial<Field, Comparator> &&other) noexcept = default;
    Polynomial &operator=(const Polynomial<Field, Comparator> &other) = default;
    Polynomial &operator=(Polynomial &&other) = default;

public:
    const std::vector<Monomial<Field>>& GetMonomials() const;

public:
    Polynomial<Field, Comparator> &operator+=(const Polynomial<Field, Comparator> &other);
    Polynomial<Field, Comparator> &operator-=(const Polynomial<Field, Comparator> &other);
    Polynomial<Field, Comparator> &operator*=(const Polynomial<Field, Comparator> &other);

    Polynomial<Field, Comparator> operator+(const Polynomial<Field, Comparator> &other) const;
    Polynomial<Field, Comparator> operator-(const Polynomial<Field, Comparator> &other) const;
    Polynomial<Field, Comparator> operator*(const Polynomial<Field, Comparator> &other) const;


    bool operator==(const Polynomial<Field, Comparator> &other) const;

private:
    void SortMonomials();

private:
    // assuming Monomials have different degrees (3x + 5x -> 8x)
    // no monomials with coefficient 0
    std::vector<Monomial<Field>> monomials_ = {};
};

template<IsField Field, IsComparator Comparator>
const std::vector<Monomial<Field>> &Polynomial<Field, Comparator>::GetMonomials() const {
    return monomials_;
}

template<IsField Field, IsComparator Comparator>
Polynomial<Field, Comparator>::Polynomial(const std::vector<Monomial<Field>> &monomials) : monomials_(monomials) {
    SortMonomials();
}

template<IsField Field, IsComparator Comparator>
Polynomial<Field, Comparator> &Polynomial<Field, Comparator>::operator+=(const Polynomial<Field, Comparator> &other) {
    std::vector<Monomial<Field>> result;
    result.reserve(monomials_.size());

    size_t left = 0;
    size_t right = 0;

    while (left < monomials_.size() && right < other.monomials_.size()) {
        if (Comparator::IsEqual(monomials_[left], other.monomials_[right])) {
            result.push_back(std::move(monomials_[left++]));
            result.back() += other.monomials_[right++];
            if (result.back().IsZero()) {
                result.pop_back();
            }
        } else if (Comparator::IsGreater(monomials_[left], other.monomials_[right])) {
            result.push_back(std::move(monomials_[left++]));
        } else {
            result.push_back(other.monomials_[right++]);
        }
    }

    while (left < monomials_.size()) {
        result.push_back(std::move(monomials_[left++]));
    }
    while (right < other.monomials_.size()) {
        result.push_back(other.monomials_[right++]);
    }

    monomials_ = std::move(result);
    return *this;
}

template<IsField Field, IsComparator Comparator>
Polynomial<Field, Comparator> &Polynomial<Field, Comparator>::operator-=(const Polynomial<Field, Comparator> &other) {
    std::vector<Monomial<Field>> result;
    result.reserve(monomials_.size());

    size_t left = 0;
    size_t right = 0;

    while (left < monomials_.size() && right < other.monomials_.size()) {
        if (Comparator::IsEqual(monomials_[left], other.monomials_[right])) {
            result.push_back(std::move(monomials_[left++]));
            result.back() -= other.monomials_[right++];
            if (result.back().IsZero()) {
                result.pop_back();
            }
        } else if (Comparator::IsGreater(monomials_[left], other.monomials_[right])) {
            result.push_back(std::move(monomials_[left++]));
        } else {
            result.push_back(-other.monomials_[right++]);
        }
    }

    while (left < monomials_.size()) {
        result.push_back(std::move(monomials_[left++]));
    }
    while (right < other.monomials_.size()) {
        result.push_back(-other.monomials_[right++]);
    }

    monomials_ = std::move(result);
    return *this;
}

template<IsField Field, IsComparator Comparator>
Polynomial<Field, Comparator> &Polynomial<Field, Comparator>::operator*=(const Polynomial<Field, Comparator> &other) {
    Polynomial<Field, Comparator> result;

    for (size_t i = 0; i < other.monomials_.size(); i++) {
        // TODO optimize
        std::vector<Monomial<Field>> row;
        row.reserve(monomials_.size());
        for (size_t j = 0; j < monomials_.size(); j++) {
            row.push_back(other.monomials_[i] * monomials_[j]);
        }
        result += Polynomial<Field, Comparator>(row);
    }

    monomials_ = std::move(result.monomials_);
    // TODO check if needed
    SortMonomials();
    return *this;
}

template<IsField Field, IsComparator Comparator>
Polynomial<Field, Comparator> Polynomial<Field, Comparator>::operator+(const Polynomial<Field, Comparator> &other) const {
    Polynomial<Field, Comparator> temp(*this);
    return temp += other;
}

template<IsField Field, IsComparator Comparator>
Polynomial<Field, Comparator> Polynomial<Field, Comparator>::operator-(const Polynomial<Field, Comparator> &other) const {
    Polynomial<Field, Comparator> temp(*this);
    return temp -= other;
}

template<IsField Field, IsComparator Comparator>
Polynomial<Field, Comparator> Polynomial<Field, Comparator>::operator*(const Polynomial<Field, Comparator> &other) const {
    Polynomial<Field, Comparator> temp(*this);
    return temp *= other;
}

template<IsField Field, IsComparator Comparator>
void Polynomial<Field, Comparator>::SortMonomials() {
    std::sort(monomials_.begin(), monomials_.end(), [](const Monomial<Field> &lhs, const Monomial<Field> &rhs) {
        return Comparator::IsGreater(lhs, rhs);
    });
}

template<IsField Field, IsComparator Comparator>
bool Polynomial<Field, Comparator>::operator==(const Polynomial<Field, Comparator> &other) const {
    return monomials_ == other.monomials_;
}
