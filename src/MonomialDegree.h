#include <cinttypes>
#include <iterator>
#include <numeric>
#include <vector>

namespace Groebner {

namespace Details {
    template <typename T, typename = void>
    struct IsIteratorV {
            static constexpr bool value = false;
    };

    template <typename T>
    struct IsIteratorV<
        T,
        typename std::enable_if<!std::is_same<
            typename std::iterator_traits<T>::value_type, void>::value>::type> {
            static constexpr bool value = true;
    };

    template <typename T>
    concept IsIterator = IsIteratorV<T>::value;
}  // namespace Details

class MonomialDegree {
    public:
        using DegreeType = uint64_t;

        MonomialDegree(size_t size = 0);
        explicit MonomialDegree(std::vector<DegreeType>&& degrees);
        MonomialDegree(std::initializer_list<DegreeType> degrees);

        template <Details::IsIterator It>
        MonomialDegree(It begin, It end) {
            degrees_.insert(degrees_.end(), begin, end);
            sum_degree_ =
                std::accumulate(degrees_.begin(), degrees_.end(), 0ULL);
        }

        size_t GetSize() const;
        DegreeType GetSumDegree() const;

        DegreeType GetDegree(size_t ind) const;
        void SetDegree(size_t ind, DegreeType val);

        MonomialDegree& operator+=(const MonomialDegree& other);
        MonomialDegree& operator-=(const MonomialDegree& other);

        MonomialDegree operator+(const MonomialDegree& other) const;
        MonomialDegree operator-(const MonomialDegree& other) const;

        bool operator==(const MonomialDegree& other) const;
        bool operator!=(const MonomialDegree& other) const;

    private:
        void Expand(size_t new_size);

        DegreeType sum_degree_ = 0;
        std::vector<DegreeType> degrees_;
};
}  // namespace Groebner