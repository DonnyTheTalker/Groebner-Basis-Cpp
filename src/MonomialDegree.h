#include <cinttypes>
#include <vector>

class MonomialDegree {
public:
    using DegreeType = uint64_t;
    using SizeType = size_t;
public:
    MonomialDegree() = delete;

    explicit MonomialDegree(SizeType n_variables = 0);
    MonomialDegree(SizeType n_variables, const std::vector<DegreeType>& degrees);
    MonomialDegree(const MonomialDegree &other) = default;
    MonomialDegree(MonomialDegree &&other) noexcept ;
    MonomialDegree &operator=(const MonomialDegree &other);
    MonomialDegree &operator=(MonomialDegree &&other) noexcept;

public:
    SizeType GetSize() const;
    DegreeType GetSumDegree() const;

    DegreeType operator[](size_t ind) const;

    MonomialDegree &operator+=(const MonomialDegree &other);
    MonomialDegree &operator-=(const MonomialDegree &other);

    MonomialDegree operator+(const MonomialDegree &other) const;
    MonomialDegree operator-(const MonomialDegree &other) const;

    bool operator==(const MonomialDegree &other) const;

private:
    const SizeType n_variables_;
    DegreeType sum_degree_;
    std::vector<DegreeType> degrees_;
};