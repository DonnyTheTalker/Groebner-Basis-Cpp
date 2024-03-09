#include "MonomialDegree.h"

#include <cassert>
#include <numeric>
#include <utility>

MonomialDegree::MonomialDegree(MonomialDegree::SizeType n_variables) : n_variables_(n_variables),
                                                                       degrees_(n_variables, 0),
                                                                       sum_degree_(0) {
}

MonomialDegree::MonomialDegree(MonomialDegree::SizeType n_variables, const std::vector<DegreeType> &degrees) :
        n_variables_(n_variables), sum_degree_(std::accumulate(degrees.begin(), degrees.end(),
                                                               static_cast<DegreeType>(0))), degrees_(degrees) {
}

MonomialDegree::MonomialDegree(MonomialDegree &&other) noexcept: n_variables_(other.n_variables_),
                                                                 sum_degree_(std::exchange(other.sum_degree_, 0)),
                                                                 degrees_(std::move(other.degrees_)) {
    other.degrees_.clear();
    other.degrees_.resize(n_variables_);

}

MonomialDegree &MonomialDegree::operator=(const MonomialDegree &other) {
    assert(n_variables_ == other.n_variables_ && "Number of variables must be equal in '='");
    sum_degree_ = other.sum_degree_;
    degrees_ = other.degrees_;
    return *this;
}

MonomialDegree &MonomialDegree::operator=(MonomialDegree &&other) noexcept {
    assert(n_variables_ == other.n_variables_ && "Number of variables must");
    if (this != &other) {
        sum_degree_ = std::exchange(other.sum_degree_, 0);
        degrees_ = std::move(other.degrees_);
        other.degrees_.clear();
        other.degrees_.resize(n_variables_);
    }
    return *this;
}

MonomialDegree::SizeType MonomialDegree::GetSize() const {
    return n_variables_;
}

MonomialDegree::DegreeType MonomialDegree::GetSumDegree() const {
    return sum_degree_;
}

MonomialDegree::DegreeType& MonomialDegree::operator[](size_t ind) {
    assert(ind < n_variables_ && "Out of bounds");
    return degrees_[ind];
}

const MonomialDegree::DegreeType& MonomialDegree::operator[](size_t ind) const {
    assert(ind < n_variables_ && "Out of bounds");
    return degrees_[ind];
}

MonomialDegree &MonomialDegree::operator+=(const MonomialDegree &other) {
    assert(n_variables_ == other.n_variables_ && "Number of variables must be equal");
    for (SizeType i = 0; i < n_variables_; ++i) {
        degrees_[i] += other.degrees_[i];
        sum_degree_ += other.degrees_[i];
    }
    return *this;
}

MonomialDegree &MonomialDegree::operator-=(const MonomialDegree &other) {
    assert(n_variables_ == other.n_variables_ && "Number of variables must be equal");
    for (SizeType i = 0; i < n_variables_; ++i) {
        assert(degrees_[i] >= other.degrees_[i] && "Can't substitute from lower degree");
        degrees_[i] -= other.degrees_[i];
        sum_degree_ -= other.degrees_[i];
    }
    return *this;
}

MonomialDegree MonomialDegree::operator+(const MonomialDegree &other) const {
    assert(n_variables_ == other.n_variables_ && "Number of variables must be equal");
    MonomialDegree temp(*this);
    return temp += other;
}

MonomialDegree MonomialDegree::operator-(const MonomialDegree &other) const {
    assert(n_variables_ == other.n_variables_ && "Number of variables must be equal");
    MonomialDegree temp(*this);
    return temp -= other;
}

bool MonomialDegree::operator==(const MonomialDegree &other) const {
    return n_variables_ == other.n_variables_ && degrees_ == other.degrees_;
}

// TODO remove this completely
void MonomialDegree::SetSumDegree(MonomialDegree::DegreeType degree) {
    assert(degree == std::accumulate(degrees_.begin(), degrees_.end(), static_cast<DegreeType>(0)) && "Be careful");
    sum_degree_ = degree;
}







