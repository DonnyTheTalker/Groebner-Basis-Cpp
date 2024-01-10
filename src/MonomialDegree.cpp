#include "MonomialDegree.h"

#include <cassert>
#include <utility>

MonomialDegree::MonomialDegree(MonomialDegree::SizeType n_variables) : n_variables_(n_variables),
                                                                       degrees_(n_variables, 0) {
}

MonomialDegree::MonomialDegree(MonomialDegree::SizeType n_variables, const std::vector<DegreeType> &degrees) :
        n_variables_(n_variables), degrees_(degrees) {
}

MonomialDegree::MonomialDegree(MonomialDegree &&other) noexcept: n_variables_(other.n_variables_),
                                                                 degrees_(std::move(other.degrees_)) {
    other.degrees_.clear();
    other.degrees_.resize(n_variables_);
}

MonomialDegree &MonomialDegree::operator=(const MonomialDegree &other) {
    assert(n_variables_ == other.n_variables_ && "Number of variables must be equal in '='");
    degrees_ = other.degrees_;
    return *this;
}

MonomialDegree &MonomialDegree::operator=(MonomialDegree &&other) noexcept {
    assert(n_variables_ == other.n_variables_ && "Number of variables must");
    if (this != &other) {
        degrees_ = std::move(other.degrees_);
        other.degrees_.clear();
        other.degrees_.resize(n_variables_);
    }
    return *this;
}

MonomialDegree::SizeType MonomialDegree::GetSize() const {
    return n_variables_;
}

MonomialDegree::DegreeType MonomialDegree::operator[](size_t ind) const {
    assert(ind < n_variables_ && "Out of bounds");
    return degrees_[ind];
}

MonomialDegree &MonomialDegree::operator+=(const MonomialDegree &other) {
    assert(n_variables_ == other.n_variables_ && "Number of variables must be equal");
    for (SizeType i = 0; i < n_variables_; ++i) {
        degrees_[i] += other.degrees_[i];
    }
    return *this;
}

MonomialDegree &MonomialDegree::operator-=(const MonomialDegree &other) {
    assert(n_variables_ == other.n_variables_ && "Number of variables must be equal");
    for (SizeType i = 0; i < n_variables_; ++i) {
        // TODO change to general check inside function
        assert(degrees_[i] >= other.degrees_[i] && "Can't substitute from lower degree");
        degrees_[i] -= other.degrees_[i];
    }
    return *this;
}

MonomialDegree MonomialDegree::operator+(const MonomialDegree &other) {
    assert(n_variables_ == other.n_variables_ && "Number of variables must be equal");
    MonomialDegree temp(*this);
    return temp += other;
}

MonomialDegree MonomialDegree::operator-(const MonomialDegree &other) {
    assert(n_variables_ == other.n_variables_ && "Number of variables must be equal");
    MonomialDegree temp(*this);
    return temp -= other;
}

bool MonomialDegree::operator==(const MonomialDegree &other) const {
    return n_variables_ == other.n_variables_ && degrees_ == other.degrees_;
}






