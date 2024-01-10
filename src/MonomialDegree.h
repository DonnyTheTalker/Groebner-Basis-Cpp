#include <cinttypes>
#include <vector>

class MonomialDegree {
public:
    using DegreeType = uint64_t;
    using SizeType = size_t;
public:
    MonomialDegree() = delete;

    explicit MonomialDegree(SizeType n_variables);
    MonomialDegree(SizeType n_variables, const std::vector<DegreeType>& degrees);
    MonomialDegree(const MonomialDegree &other) = default;
    MonomialDegree(MonomialDegree &&other) noexcept ;
    MonomialDegree &operator=(const MonomialDegree &other);
    MonomialDegree &operator=(MonomialDegree &&other) noexcept;

public:
    SizeType GetSize() const;

    DegreeType operator[](size_t ind);
    DegreeType operator[](size_t ind) const;

    MonomialDegree &operator+=(const MonomialDegree &other);
    MonomialDegree &operator-=(const MonomialDegree &other);

    MonomialDegree operator+(const MonomialDegree &other);
    MonomialDegree operator-(const MonomialDegree &other);

    bool operator==(const MonomialDegree &other) const;

private:
    const SizeType n_variables_;
    std::vector<DegreeType> degrees_;
};