#include "Monomial.h"
#include "MonomialCompare.h"

#include <cassert>
#include <utility>

namespace Groebner {

Monomial::Monomial(size_t size) : degrees_(size) {}

Monomial::Monomial(std::vector<DegreeType>&& degrees)
    : degrees_(std::move(degrees)) {
    sum_degree_ = std::accumulate(degrees_.begin(), degrees_.end(), 0ULL);
}

Monomial::Monomial(std::initializer_list<DegreeType> degrees) {
    degrees_.insert(degrees_.end(), degrees.begin(), degrees.end());
    sum_degree_ = std::accumulate(degrees_.begin(), degrees_.end(), 0ULL);
}

size_t Monomial::GetSize() const {
    return degrees_.size();
}

Monomial::DegreeType Monomial::GetSumDegree() const {
    return sum_degree_;
}

Monomial::DegreeType Monomial::GetDegree(size_t ind) const {
    if (ind < GetSize()) {
        return degrees_[ind];
    }
    return 0;
}

void Monomial::SetDegree(size_t ind, Monomial::DegreeType val) {
    if (ind < GetSize()) {
        sum_degree_ -= degrees_[ind];
    } else {
        // TODO do more efficient expansion
        Expand(ind + 1);
    }

    sum_degree_ += val;
    degrees_[ind] = val;
}

Monomial& Monomial::operator+=(const Monomial& other) {
    if (GetSize() < other.GetSize()) {
        Expand(other.GetSize());
    }

    for (size_t i = 0; i < other.GetSize(); i++) {
        SetDegree(i, GetDegree(i) + other.GetDegree(i));
    }

    return *this;
}

Monomial& Monomial::operator-=(const Monomial& other) {
    assert(StraightCoordinateOrder::IsGreaterOrEqual(*this, other) &&
           "Can't substitute from lower degree");

    if (GetSize() < other.GetSize()) {
        Expand(other.GetSize());
    }

    for (size_t i = 0; i < other.GetSize(); i++) {
        SetDegree(i, GetDegree(i) - other.GetDegree(i));
    }

    return *this;
}

Monomial Monomial::operator+(const Monomial& other) const {
    Monomial temp(*this);
    temp += other;
    return temp;
}

Monomial Monomial::operator-(const Monomial& other) const {
    Monomial temp(*this);
    temp -= other;
    return temp;
}

bool Monomial::operator==(const Monomial& other) const {
    for (size_t i = 0; i < std::max(GetSize(), other.GetSize()); i++) {
        if (GetDegree(i) != other.GetDegree(i)) {
            return false;
        }
    }

    return true;
}

bool Monomial::operator!=(const Monomial& other) const {
    return !(*this == other);
}

void Monomial::Expand(size_t new_size) {
    assert(new_size >= GetSize() && "Trying to expand to lower size");
    degrees_.resize(new_size);
}
}  // namespace Groebner
