#pragma once

#include "ComparatorFwd.h"
#include "FieldFwd.h"
#include "IteratorFwd.h"
#include "Polynomial.h"

namespace Groebner {

template <IsSupportedField Field, IsComparator Comparator>
class PolySystem {
    private:
        using LocalPolynomial = Polynomial<Field, Comparator>;

    public:
        PolySystem() = default;

        // TODO add constructors for other Comparator
        // and check for 0 polynomials
        explicit PolySystem(std::vector<LocalPolynomial>&& polys);
        PolySystem(std::initializer_list<LocalPolynomial> polys);
        template <Details::IsIterator It>
        PolySystem(It begin, It end);

        size_t GetSize() const;
        bool IsEmpty() const;

        const LocalPolynomial& operator[](size_t index) const;
        LocalPolynomial& operator[](size_t index);

        void Add(const LocalPolynomial& other);
        void Add(LocalPolynomial&& other);

        // TODO add slow pop from any position

        LocalPolynomial SwapAndPop(size_t index);
        void AddAndSwap(size_t index, const LocalPolynomial& polynomial);
        void AddAndSwap(size_t index, LocalPolynomial&& polynomial);

        // TODO add comparison operator (and one for system with different comparator

    private:
        std::vector<LocalPolynomial> polynomials_;
};

template <IsSupportedField Field, IsComparator Comparator>
PolySystem<Field, Comparator>::PolySystem(std::vector<LocalPolynomial>&& polys)
    : polynomials_(std::move(polys)) {}

template <IsSupportedField Field, IsComparator Comparator>
PolySystem<Field, Comparator>::PolySystem(
    std::initializer_list<LocalPolynomial> polys) {
    polynomials_.insert(polynomials_.begin(), polys.begin(), polys.end());
}

template <IsSupportedField Field, IsComparator Comparator>
template <Details::IsIterator It>
PolySystem<Field, Comparator>::PolySystem(It begin, It end) {
    polynomials_.insert(polynomials_.begin(), begin, end);
}

template <IsSupportedField Field, IsComparator Comparator>
size_t PolySystem<Field, Comparator>::GetSize() const {
    return polynomials_.size();
}

template <IsSupportedField Field, IsComparator Comparator>
bool PolySystem<Field, Comparator>::IsEmpty() const {
    return GetSize() == 0;
}

template <IsSupportedField Field, IsComparator Comparator>
PolySystem<Field, Comparator>::LocalPolynomial&
PolySystem<Field, Comparator>::operator[](size_t index) {
    assert(index < polynomials_.size() && "Out of bounds");
    return polynomials_[index];
}

template <IsSupportedField Field, IsComparator Comparator>
const PolySystem<Field, Comparator>::LocalPolynomial&
PolySystem<Field, Comparator>::operator[](size_t index) const {
    assert(index < polynomials_.size() && "Out of bounds");
    return polynomials_[index];
}

template <IsSupportedField Field, IsComparator Comparator>
void PolySystem<Field, Comparator>::Add(const LocalPolynomial& other) {
    polynomials_.push_back(other);
}

template <IsSupportedField Field, IsComparator Comparator>
void PolySystem<Field, Comparator>::Add(LocalPolynomial&& other) {
    polynomials_.push_back(std::move(other));
}

template <IsSupportedField Field, IsComparator Comparator>
PolySystem<Field, Comparator>::LocalPolynomial
PolySystem<Field, Comparator>::SwapAndPop(size_t index) {
    assert(index < polynomials_.size() && "Out of bounds");
    std::swap(polynomials_[index], polynomials_.back());
    LocalPolynomial result(std::move(polynomials_.back()));
    polynomials_.pop_back();
    return result;
}

template <IsSupportedField Field, IsComparator Comparator>
void PolySystem<Field, Comparator>::AddAndSwap(
    size_t index, const LocalPolynomial& polynomial) {
    assert(index <= polynomials_.size() && "Out of bounds");
    polynomials_.push_back(polynomial);
    std::swap(polynomials_[index], polynomials_.back());
}

template <IsSupportedField Field, IsComparator Comparator>
void PolySystem<Field, Comparator>::AddAndSwap(
    size_t index, PolySystem::LocalPolynomial&& polynomial) {
    assert(index <= polynomials_.size() && "Out of bounds");
    polynomials_.push_back(std::move(polynomial));
    std::swap(polynomials_[index], polynomials_.back());
}

}  // namespace Groebner
