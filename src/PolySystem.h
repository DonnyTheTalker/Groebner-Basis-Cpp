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
        explicit PolySystem(std::vector<LocalPolynomial>&& polys) {
            polynomials_.reserve(polys.size());
            for (auto& poly : polys) {
                if (!poly.IsZero()) {
                    polynomials_.push_back(std::move(poly));
                }
            }
        }

        PolySystem(std::initializer_list<LocalPolynomial> polys) {
            polynomials_.reserve(polys.size());
            for (auto& poly : polys) {
                if (!poly.IsZero()) {
                    polynomials_.push_back(poly);
                }
            }
        }

        template <Details::IsIterator It>
        PolySystem(It begin, It end) {
            polynomials_.reserve(std::distance(begin, end));
            for (auto it = begin; it != end; it++) {
                if (!(it->IsZero())) {
                    polynomials_.push_back(*it);
                }
            }
        }

        size_t GetSize() const { return polynomials_.size(); }
        bool IsEmpty() const { return GetSize() == 0; }

        const LocalPolynomial& operator[](size_t index) const {
            assert(index < polynomials_.size() && "Out of bounds");
            return polynomials_[index];
        }

        LocalPolynomial& operator[](size_t index) {
            assert(index < polynomials_.size() && "Out of bounds");
            return polynomials_[index];
        }

        void Add(const LocalPolynomial& other) {
            polynomials_.push_back(other);
        }
        void Add(LocalPolynomial&& other) {
            polynomials_.push_back(std::move(other));
        }

        // TODO add tests
        LocalPolynomial Pop(size_t index) {
            assert(index < polynomials_.size() && "Out of bounds");
            LocalPolynomial result(std::move(polynomials_[index]));
            polynomials_.erase(std::next(polynomials_.begin(), index));
            return result;
        }

        LocalPolynomial SwapAndPop(size_t index) {
            assert(index < polynomials_.size() && "Out of bounds");
            std::swap(polynomials_[index], polynomials_.back());
            LocalPolynomial result(std::move(polynomials_.back()));
            polynomials_.pop_back();
            return result;
        }

        void AddAndSwap(size_t index, const LocalPolynomial& polynomial) {
            assert(index <= polynomials_.size() && "Out of bounds");
            polynomials_.push_back(polynomial);
            std::swap(polynomials_[index], polynomials_.back());
        }

        void AddAndSwap(size_t index, LocalPolynomial&& polynomial) {
            assert(index <= polynomials_.size() && "Out of bounds");
            polynomials_.push_back(std::move(polynomial));
            std::swap(polynomials_[index], polynomials_.back());
        }

        // TODO add comparison operator (and one for system with different comparator
    friend class GroebnerAlgorithm;
    private:
        void Reduce() {
            size_t sz = polynomials_.size();
            for (size_t i = 0; i < sz;) {
                if (polynomials_[i].IsZero()) {
                    SwapAndPop(i);
                    sz--;
                } else {
                    i++;
                }
            }
        }

        std::vector<LocalPolynomial> polynomials_;
};
}  // namespace Groebner
