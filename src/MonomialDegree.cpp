#include "MonomialDegree.h"

#include <cassert>
#include <utility>

namespace Groebner {

MonomialDegree::MonomialDegree(size_t size) : degrees_(size) {
}

MonomialDegree::MonomialDegree(std::vector<DegreeType>&& degrees)
    : degrees_(std::move(degrees)) {
    sum_degree_ = std::accumulate(degrees_.begin(), degrees_.end(), 0ULL);
}

MonomialDegree::MonomialDegree(std::initializer_list<DegreeType> degrees) {
    degrees_.insert(degrees_.end(), degrees.begin(), degrees.end());
    sum_degree_ = std::accumulate(degrees_.begin(), degrees_.end(), 0ULL);
}

size_t MonomialDegree::GetSize() const {
    return degrees_.size();
}

MonomialDegree::DegreeType MonomialDegree::GetSumDegree() const {
    return sum_degree_;
}

MonomialDegree::DegreeType MonomialDegree::GetDegree(size_t ind) const {
    if (ind < GetSize()) {
        return degrees_[ind];
    }
    return 0;
}

void MonomialDegree::SetDegree(size_t ind, MonomialDegree::DegreeType val) {
    if (ind < GetSize()) {
        sum_degree_ -= degrees_[ind];
    } else {
        Expand(ind + 1);
    }

    sum_degree_ += val;
    degrees_[ind] = val;
}

MonomialDegree& MonomialDegree::operator+=(const MonomialDegree& other) {
    if (GetSize() < other.GetSize()) {
        Expand(other.GetSize());
    }

    for (size_t i = 0; i < other.GetSize(); i++) {
        SetDegree(i, GetDegree(i) + other.GetDegree(i));
    }

    return *this;
}

MonomialDegree& MonomialDegree::operator-=(const MonomialDegree& other) {
    if (GetSize() < other.GetSize()) {
        Expand(other.GetSize());
    }

    for (size_t i = 0; i < other.GetSize(); i++) {
        // TODO add comparison
        assert(GetDegree(i) >= other.GetDegree(i) && "Can't substitute from lower degree");
        SetDegree(i, GetDegree(i) - other.GetDegree(i));
    }

    return *this;
}

MonomialDegree MonomialDegree::operator+(const MonomialDegree& other) const {
    MonomialDegree temp(*this);
    temp += other;
    return temp;
}

MonomialDegree MonomialDegree::operator-(const MonomialDegree& other) const {
    MonomialDegree temp(*this);
    temp -= other;
    return temp;
}

bool MonomialDegree::operator==(const MonomialDegree& other) const {
    auto lhs_size = GetSize();
    auto rhs_size = other.GetSize();

    for (size_t i = 0; i < std::max(lhs_size, rhs_size); i++) {
        DegreeType lhs_degree = (i < lhs_size) ? GetDegree(i) : 0;
        DegreeType rhs_degree = (i < rhs_size) ? other.GetDegree(i) : 0;
        if (lhs_degree != rhs_degree) {
            return false;
        }
    }

    return true;
}

bool MonomialDegree::operator!=(const MonomialDegree& other) const {
    return !(*this == other);
}

void MonomialDegree::Expand(size_t new_size) {
    assert(new_size >= GetSize() && "Trying to expand to lower size");
    degrees_.resize(new_size);
}
}  // namespace Groebner
