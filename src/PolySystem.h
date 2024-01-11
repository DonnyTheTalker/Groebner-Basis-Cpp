#pragma once

#include "ComparatorFwd.h"
#include "FieldFwd.h"
#include "Polynomial.h"

// TODO add template for container type
// to add smth like std::set, ordering by Leading monomial
template<IsField Field, IsComparator Comparator>
class PolySystem {
public:
    // TODO add constructors for other Comparator
    PolySystem() = default;
    PolySystem(const PolySystem<Field, Comparator> &other) = default;
    PolySystem &operator=(const PolySystem<Field, Comparator> &other) = default;
    PolySystem(PolySystem<Field, Comparator> &&other) noexcept = default;
    PolySystem &operator=(PolySystem<Field, Comparator> &&other) noexcept = default;
public:
    size_t GetSize();
    bool IsEmpty() const;
    const Polynomial<Field, Comparator> &operator[](size_t index) const;
    Polynomial<Field, Comparator> &operator[](size_t index);

    void Add(const Polynomial<Field, Comparator> &other);
    void Add(Polynomial<Field, Comparator> &&other);

    // TODO move in private and give access wia friend
    void SwapAndPop(size_t index);
    void AddAndSwap(size_t index, const Polynomial<Field, Comparator> &polynomial);

private:
    std::vector<Polynomial<Field, Comparator>> polynomials_;
};

template<IsField Field, IsComparator Comparator>
void PolySystem<Field, Comparator>::AddAndSwap(size_t index, const Polynomial<Field, Comparator> &polynomial) {
    assert(index <= polynomials_.size() && "Out of bounds");
    polynomials_.push_back(polynomial);
    std::swap(polynomials_[index], polynomials_.back());
}

template<IsField Field, IsComparator Comparator>
void PolySystem<Field, Comparator>::SwapAndPop(size_t index) {
    assert(index < polynomials_.size() && "Out of bounds");
    std::swap(polynomials_[index], polynomials_.back());
    polynomials_.pop_back();
}

template<IsField Field, IsComparator Comparator>
Polynomial<Field, Comparator> &PolySystem<Field, Comparator>::operator[](size_t index) {
    assert(index < polynomials_.size() && "Out of bounds");
    return polynomials_[index];
}

template<IsField Field, IsComparator Comparator>
bool PolySystem<Field, Comparator>::IsEmpty() const {
    return polynomials_.empty();
}

template<IsField Field, IsComparator Comparator>
size_t PolySystem<Field, Comparator>::GetSize() {
    return polynomials_.size();
}

template<IsField Field, IsComparator Comparator>
const Polynomial<Field, Comparator> &PolySystem<Field, Comparator>::operator[](size_t index) const {
    assert(index < polynomials_.size() && "Out of bounds");
    return polynomials_[index];
}

template<IsField Field, IsComparator Comparator>
void PolySystem<Field, Comparator>::Add(const Polynomial<Field, Comparator> &other) {
    polynomials_.push_back(other);
}

template<IsField Field, IsComparator Comparator>
void PolySystem<Field, Comparator>::Add(Polynomial<Field, Comparator> &&other) {
    polynomials_.push_back(std::move(other));
}
