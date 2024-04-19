#pragma once

#include <cinttypes>
#include <iterator>
#include <numeric>
#include <vector>

#include "IteratorFwd.h"

namespace Groebner {

class Monomial {
    public:
        using DegreeType = uint64_t;

        Monomial(size_t size = 0);
        explicit Monomial(std::vector<DegreeType>&& degrees);
        Monomial(std::initializer_list<DegreeType> degrees);

        template <Details::IsIterator It>
        Monomial(It begin, It end) {
            degrees_.insert(degrees_.end(), begin, end);
            sum_degree_ =
                std::accumulate(degrees_.begin(), degrees_.end(), 0ULL);
        }

        size_t GetSize() const;
        DegreeType GetSumDegree() const;

        DegreeType GetDegree(size_t ind) const;
        void SetDegree(size_t ind, DegreeType val);

        Monomial& operator+=(const Monomial& other);
        Monomial& operator-=(const Monomial& other);

        Monomial operator+(const Monomial& other) const;
        Monomial operator-(const Monomial& other) const;

        bool operator==(const Monomial& other) const;
        bool operator!=(const Monomial& other) const;

    private:
        void Expand(size_t new_size);

        DegreeType sum_degree_ = 0;
        std::vector<DegreeType> degrees_;
};
}  // namespace Groebner
