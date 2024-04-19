#include "GroebnerAlgorithm.h"
#include "gtest/gtest.h"

namespace Groebner::Test {

template <IsSupportedField Field, IsComparator Comparator>
void CheckEqual(const PolySystem<Field, Comparator>& lhs,
                const PolySystem<Field, Comparator>& rhs) {
    ASSERT_EQ(lhs.GetSize(), rhs.GetSize());
    for (size_t i = 0; i < lhs.GetSize(); i++) {
        ASSERT_EQ(lhs[i], rhs[i]);
    }
}

TEST(Builder, Lcm) {
    {
        Monomial x{1, 2, 3};
        Monomial y{1, 2, 3};
        Monomial z = GroebnerAlgorithm::FindMinimalCommonDegree(x, y);

        ASSERT_EQ(z, Monomial({1, 2, 3}));
        ASSERT_EQ(z.GetSumDegree(), 6);
    }

    {
        Monomial x{1, 2, 3};
        Monomial y{2, 4, 5};
        Monomial z = GroebnerAlgorithm::FindMinimalCommonDegree(x, y);

        ASSERT_EQ(z, Monomial({2, 4, 5}));
        ASSERT_EQ(z.GetSumDegree(), 11);
    }

    {
        Monomial x({1, 2, 3});
        Monomial y({2, 2, 1});
        Monomial z = GroebnerAlgorithm::FindMinimalCommonDegree(x, y);

        ASSERT_EQ(z, Monomial({2, 2, 3}));
        ASSERT_EQ(z.GetSumDegree(), 7);
    }
}

TEST(Builder, SPoly) {
    {
        Polynomial<Rational, LexOrder> x{{2, {2, 1}}, {1, {0, 1}}};
        Polynomial<Rational, LexOrder> y{{1, {1, 1}}, {1, {1, 0}}};

        SPolyInfo z = GroebnerAlgorithm::SPolynomial(x, y);
        ASSERT_EQ(z.common_degree, Monomial({2, 1}));
        Polynomial<Rational, LexOrder> expected_poly{{-2, {2, 0}}, {1, {0, 1}}};
        ASSERT_EQ(z.s_poly, expected_poly);
    }

    {
        Polynomial<Modulo<3>, LexOrder> x{{2, {2, 1}}, {1, {0, 1}}};
        Polynomial<Modulo<3>, LexOrder> y{{1, {1, 1}}, {1, {1, 0}}};

        SPolyInfo z = GroebnerAlgorithm::SPolynomial(x, y);
        ASSERT_EQ(z.common_degree, Monomial({2, 1}));
        Polynomial<Modulo<3>, LexOrder> expected_poly{{-2, {2, 0}},
                                                      {1, {0, 1}}};
        ASSERT_EQ(z.s_poly, expected_poly);
    }

    {
        Polynomial<Rational, LexOrder> x({{2, {1, 2}}});
        Polynomial<Rational, LexOrder> y({{1, {2, 1}}});
        SPolyInfo z = GroebnerAlgorithm::SPolynomial(x, y);
        ASSERT_EQ(z.common_degree, Monomial({2, 2}));
        Polynomial<Rational, LexOrder> expected_poly;
        ASSERT_EQ(z.s_poly, expected_poly);
        ASSERT_TRUE(z.s_poly.IsZero());
    }

    {
        Polynomial<Modulo<3>, LexOrder> x({{2, {1, 2}}});
        Polynomial<Modulo<3>, LexOrder> y({{1, {2, 1}}});
        SPolyInfo z = GroebnerAlgorithm::SPolynomial(x, y);
        ASSERT_EQ(z.common_degree, Monomial({2, 2}));
        Polynomial<Modulo<3>, LexOrder> expected_poly;
        ASSERT_EQ(z.s_poly, expected_poly);
        ASSERT_TRUE(z.s_poly.IsZero());
    }

    {
        Polynomial<Rational, ReverseLexOrder> x{{2, {2, 1}}, {1, {0, 1}}};
        Polynomial<Rational, ReverseLexOrder> y{{1, {1, 1}}, {1, {1, 0}}};

        SPolyInfo z = GroebnerAlgorithm::SPolynomial(x, y);
        ASSERT_EQ(z.common_degree, Monomial({1, 1}));
        Polynomial<Rational, ReverseLexOrder> expected_poly{{2, {3, 1}},
                                                            {-1, {1, 2}}};
        ASSERT_EQ(z.s_poly, expected_poly);
    }

    {
        Polynomial<Rational, GrlexOrder> x{{2, {2, 1}}, {1, {0, 1}}};
        Polynomial<Rational, GrlexOrder> y{{1, {1, 1}}, {1, {1, 0}}};

        SPolyInfo z = GroebnerAlgorithm::SPolynomial(x, y);
        ASSERT_EQ(z.common_degree, Monomial({2, 1}));
        Polynomial<Rational, GrlexOrder> expected_poly{{-2, {2, 0}},
                                                       {1, {0, 1}}};
        ASSERT_EQ(z.s_poly, expected_poly);
    }

    {
        Polynomial<Rational, GrevlexOrder> x{{2, {2, 1}}, {1, {0, 1}}};
        Polynomial<Rational, GrevlexOrder> y{{1, {1, 1}}, {1, {1, 0}}};

        SPolyInfo z = GroebnerAlgorithm::SPolynomial(x, y);
        ASSERT_EQ(z.common_degree, Monomial({2, 1}));
        Polynomial<Rational, GrevlexOrder> expected_poly{{-2, {2, 0}},
                                                         {1, {0, 1}}};
        ASSERT_EQ(z.s_poly, expected_poly);
    }
}

TEST(Reduction, Simple) {
    {
        Polynomial<Rational, LexOrder> x({{1, {1, 1, 1}}});
        Polynomial<Rational, LexOrder> y({{1, {1, 1, 1}}});

        Polynomial<Rational, LexOrder> z = GroebnerAlgorithm::ReducePolynomial(
            x, PolySystem<Rational, LexOrder>({y}));
        Polynomial<Rational, LexOrder> expected;

        ASSERT_TRUE(z.IsZero());
        ASSERT_EQ(z, expected);
    }

    {
        Polynomial<Rational, LexOrder> x({{1, {1, 1, 1}}});
        Polynomial<Rational, LexOrder> y({{1, {1, 1, 1}}});

        Polynomial<Rational, LexOrder> z = GroebnerAlgorithm::ReducePolynomial(
            std::move(x), PolySystem<Rational, LexOrder>({y}));
        Polynomial<Rational, LexOrder> expected;

        ASSERT_TRUE(z.IsZero());
        ASSERT_EQ(z, expected);
    }

    {
        Polynomial<Rational, LexOrder> x{{1, {1, 1, 1}}};
        Polynomial<Rational, LexOrder> y1{{1, {1, 0, 0}}};
        Polynomial<Rational, LexOrder> y2{{1, {0, 1, 0}}};
        Polynomial<Rational, LexOrder> y3{{1, {0, 0, 1}}};
        Polynomial<Rational, LexOrder> z = GroebnerAlgorithm::ReducePolynomial(
            x, PolySystem<Rational, LexOrder>({y1, y2, y3}));
        Polynomial<Rational, LexOrder> expected;

        ASSERT_TRUE(z.IsZero());
        ASSERT_EQ(z, expected);
    }

    {
        Polynomial<Rational, LexOrder> x{{{1, 2}, {2, 1}}, {2, {0, 1}}};
        Polynomial<Rational, LexOrder> y1{{1, {1, 0}}};
        Polynomial<Rational, LexOrder> y2{{1, {0, 1}}};

        Polynomial<Rational, LexOrder> z = GroebnerAlgorithm::ReducePolynomial(
            x, PolySystem<Rational, LexOrder>({y1, y2}));
        Polynomial<Rational, LexOrder> expected;

        ASSERT_TRUE(z.IsZero());
        ASSERT_EQ(z, expected);
    }

    {
        Polynomial<Modulo<3>, LexOrder> x{{2, {2, 1}}, {2, {0, 1}}};
        Polynomial<Modulo<3>, LexOrder> y1{{1, {1, 0}}};
        Polynomial<Modulo<3>, LexOrder> y2{{1, {0, 1}}};

        Polynomial<Modulo<3>, LexOrder> z = GroebnerAlgorithm::ReducePolynomial(
            x, PolySystem<Modulo<3>, LexOrder>({y1, y2}));
        Polynomial<Modulo<3>, LexOrder> expected;

        ASSERT_TRUE(z.IsZero());
        ASSERT_EQ(z, expected);
    }

    {
        Polynomial<Rational, ReverseLexOrder> x{{{1, 2}, {2, 1}}, {2, {0, 1}}};
        Polynomial<Rational, ReverseLexOrder> y1{{1, {1, 0}}};
        Polynomial<Rational, ReverseLexOrder> y2{{1, {0, 1}}};

        Polynomial<Rational, ReverseLexOrder> z =
            GroebnerAlgorithm::ReducePolynomial(
                x, PolySystem<Rational, ReverseLexOrder>({y1, y2}));
        Polynomial<Rational, ReverseLexOrder> expected;

        ASSERT_TRUE(z.IsZero());
        ASSERT_EQ(z, expected);
    }

    {
        Polynomial<Rational, GrlexOrder> x{{{1, 2}, {2, 1}}, {2, {0, 1}}};
        Polynomial<Rational, GrlexOrder> y1{{1, {1, 0}}};
        Polynomial<Rational, GrlexOrder> y2{{1, {0, 1}}};

        Polynomial<Rational, GrlexOrder> z =
            GroebnerAlgorithm::ReducePolynomial(
                x, PolySystem<Rational, GrlexOrder>({y1, y2}));
        Polynomial<Rational, GrlexOrder> expected;

        ASSERT_TRUE(z.IsZero());
        ASSERT_EQ(z, expected);
    }

    {
        Polynomial<Rational, GrevlexOrder> x{{{1, 2}, {2, 1}}, {2, {0, 1}}};
        Polynomial<Rational, GrevlexOrder> y1{{1, {1, 0}}};
        Polynomial<Rational, GrevlexOrder> y2{{1, {0, 1}}};

        Polynomial<Rational, GrevlexOrder> z =
            GroebnerAlgorithm::ReducePolynomial(
                x, PolySystem<Rational, GrevlexOrder>({y1, y2}));
        Polynomial<Rational, GrevlexOrder> expected;

        ASSERT_TRUE(z.IsZero());
        ASSERT_EQ(z, expected);
    }

    {
        Polynomial<Rational, LexOrder> x{{{1, 2}, {2, 1}}, {2, {0, 1}}};
        Polynomial<Rational, LexOrder> y2{{1, {0, 1}}};

        Polynomial<Rational, LexOrder> z = GroebnerAlgorithm::ReducePolynomial(
            x, PolySystem<Rational, LexOrder>({y2}));
        Polynomial<Rational, LexOrder> expected;

        ASSERT_TRUE(z.IsZero());
        ASSERT_EQ(z, expected);
    }
}

TEST(Reduction, Remainder) {
    {
        Polynomial<Rational, LexOrder> x{{1, {2, 1}}, {1, {0, 1}}};
        Polynomial<Rational, LexOrder> y1{{1, {1, 0}}};

        Polynomial<Rational, LexOrder> z = GroebnerAlgorithm::ReducePolynomial(
            x, PolySystem<Rational, LexOrder>({y1}));
        Polynomial<Rational, LexOrder> expected{{1, {0, 1}}};

        ASSERT_FALSE(z.IsZero());
        ASSERT_EQ(z, expected);
        ASSERT_EQ(z.GetLeader(), RationalTerm({1, {0, 1}}));
        ASSERT_EQ(z.GetAt(0), RationalTerm({1, {0, 1}}));
        ASSERT_EQ(z.GetSize(), 1);
    }

    {
        Polynomial<Rational, GrlexOrder> x{{1, {2, 1}}, {1, {0, 1}}};
        Polynomial<Rational, GrlexOrder> y1{{1, {1, 0}}};

        Polynomial<Rational, GrlexOrder> z =
            GroebnerAlgorithm::ReducePolynomial(
                x, PolySystem<Rational, GrlexOrder>({y1}));
        Polynomial<Rational, GrlexOrder> expected{{1, {0, 1}}};

        ASSERT_FALSE(z.IsZero());
        ASSERT_EQ(z, expected);
        ASSERT_EQ(z.GetLeader(), RationalTerm({1, {0, 1}}));
        ASSERT_EQ(z.GetAt(0), RationalTerm({1, {0, 1}}));
        ASSERT_EQ(z.GetSize(), 1);
    }

    {
        Polynomial<Rational, GrevlexOrder> x{{1, {2, 1}}, {1, {0, 1}}};
        Polynomial<Rational, GrevlexOrder> y1{{1, {1, 0}}};

        Polynomial<Rational, GrevlexOrder> z =
            GroebnerAlgorithm::ReducePolynomial(
                x, PolySystem<Rational, GrevlexOrder>({y1}));
        Polynomial<Rational, GrevlexOrder> expected{{1, {0, 1}}};

        ASSERT_FALSE(z.IsZero());
        ASSERT_EQ(z, expected);
        ASSERT_EQ(z.GetLeader(), RationalTerm({1, {0, 1}}));
        ASSERT_EQ(z.GetAt(0), RationalTerm({1, {0, 1}}));
        ASSERT_EQ(z.GetSize(), 1);
    }

    {
        Polynomial<Rational, ReverseLexOrder> x{{1, {2, 1}}, {1, {0, 1}}};
        Polynomial<Rational, ReverseLexOrder> y1{{1, {1, 0}}};

        Polynomial<Rational, ReverseLexOrder> z =
            GroebnerAlgorithm::ReducePolynomial(
                x, PolySystem<Rational, ReverseLexOrder>({y1}));
        Polynomial<Rational, ReverseLexOrder> expected{{1, {0, 1}}};

        ASSERT_FALSE(z.IsZero());
        ASSERT_EQ(z, expected);
        ASSERT_EQ(z.GetLeader(), RationalTerm({1, {0, 1}}));
        ASSERT_EQ(z.GetAt(0), RationalTerm({1, {0, 1}}));
        ASSERT_EQ(z.GetSize(), 1);
    }

    {
        Polynomial<Rational, LexOrder> x{{1, {2, 1}}, {1, {0, 1}}};
        Polynomial<Rational, LexOrder> y1{{1, {1, 1}}};

        Polynomial<Rational, LexOrder> z = GroebnerAlgorithm::ReducePolynomial(
            x, PolySystem<Rational, LexOrder>({y1}));
        Polynomial<Rational, LexOrder> expected{{1, {0, 1}}};

        ASSERT_FALSE(z.IsZero());
        ASSERT_EQ(z, expected);
        ASSERT_EQ(z.GetLeader(), RationalTerm({1, {0, 1}}));
        ASSERT_EQ(z.GetAt(0), RationalTerm({1, {0, 1}}));
        ASSERT_EQ(z.GetSize(), 1);
    }

    {
        Polynomial<Rational, LexOrder> x{{1, {2, 1}}, {1, {0, 1}}};
        Polynomial<Rational, LexOrder> y1{{1, {3, 0}}};

        Polynomial<Rational, LexOrder> z = GroebnerAlgorithm::ReducePolynomial(
            x, PolySystem<Rational, LexOrder>({y1}));

        ASSERT_FALSE(z.IsZero());
        ASSERT_EQ(z, x);
        ASSERT_EQ(z.GetSize(), 2);
    }

    {
        Polynomial<Rational, LexOrder> x{{1, {2, 1, 1}}, {1, {0, 1, 1}}};
        Polynomial<Rational, LexOrder> y1{{1, {2, 1}}};
        Polynomial<Rational, LexOrder> y2({1, {1, 1}});

        Polynomial<Rational, LexOrder> z = GroebnerAlgorithm::ReducePolynomial(
            x, PolySystem<Rational, LexOrder>({y1, y2}));

        ASSERT_TRUE(z.IsZero());
        ASSERT_EQ(z.GetSize(), 0);
    }

    {
        Polynomial<Rational, LexOrder> x{{1, {1, 1}}, {1, {1, 0}}};
        Polynomial<Rational, LexOrder> y1{{1, {0, 1}}, {1, {0, 0, 1}}};

        Polynomial<Rational, LexOrder> z = GroebnerAlgorithm::ReducePolynomial(
            x, PolySystem<Rational, LexOrder>({y1}));
        Polynomial<Rational, LexOrder> expected{{1, {1, 0}}, {-1, {1, 0, 1}}};

        ASSERT_FALSE(z.IsZero());
        ASSERT_EQ(z, expected);
        ASSERT_EQ(z.GetLeader(), RationalTerm({-1, {1, 0, 1}}));
        ASSERT_EQ(z.GetAt(0), RationalTerm({-1, {1, 0, 1}}));
        ASSERT_EQ(z.GetAt(1), RationalTerm({1, {1, 0}}));
        ASSERT_EQ(z.GetSize(), 2);
    }
}

TEST(BasisReduction, Simple) {
    {
        Polynomial<Rational, LexOrder> x{{3, {1, 0}}};
        Polynomial<Rational, LexOrder> y{{4, {0, 1}}};

        PolySystem<Rational, LexOrder> basis({x, y});
        PolySystem<Rational, LexOrder> expected{{{{1, {1, 0}}}},
                                                {{{1, {0, 1}}}}};

        basis = GroebnerAlgorithm::ReduceBasis(basis);
        CheckEqual(basis, expected);
        ASSERT_FALSE(basis.IsEmpty());
        ASSERT_EQ(basis.GetSize(), 2);
    }

    {
        Polynomial<Rational, LexOrder> x{{3, {1, 0}}};
        Polynomial<Rational, LexOrder> y{{4, {0, 1}}};

        PolySystem<Rational, LexOrder> basis({x, y});
        PolySystem<Rational, LexOrder> expected{{{{1, {1, 0}}}},
                                                {{{1, {0, 1}}}}};

        GroebnerAlgorithm::ReduceBasisInplace(basis);
        CheckEqual(basis, expected);
        ASSERT_FALSE(basis.IsEmpty());
        ASSERT_EQ(basis.GetSize(), 2);
    }

    {
        Polynomial<Rational, LexOrder> x{{3, {2, 0}}};
        Polynomial<Rational, LexOrder> y{{4, {1, 0}}};

        PolySystem<Rational, LexOrder> basis({x, y});
        PolySystem<Rational, LexOrder> expected{{{{1, {1, 0}}}}};

        basis = GroebnerAlgorithm::ReduceBasis(basis);
        CheckEqual(basis, expected);
        ASSERT_FALSE(basis.IsEmpty());
        ASSERT_EQ(basis.GetSize(), 1);
    }

    {
        Polynomial<Rational, LexOrder> x{{5, {1, 0}}, {1, {0, 1}}};
        Polynomial<Rational, LexOrder> y{{4, {1, 0}}};
        Polynomial<Rational, LexOrder> z{{2, {0, 1}}};

        PolySystem<Rational, LexOrder> basis({x, y, z});
        PolySystem<Rational, LexOrder> expected{
            {Polynomial<Rational, LexOrder>{{1, {1, 0}}}},
            {Polynomial<Rational, LexOrder>{{1, {0, 1}}}}};

        basis = GroebnerAlgorithm::ReduceBasis(basis);
        CheckEqual(basis, expected);
        ASSERT_FALSE(basis.IsEmpty());
        ASSERT_EQ(basis.GetSize(), 2);
    }

    {
        Polynomial<Rational, LexOrder> x{{5, {1, 0}}, {1, {0, 1}}};
        Polynomial<Rational, LexOrder> y{{4, {1, 0}}};
        Polynomial<Rational, LexOrder> z{{2, {0, 1}}};

        PolySystem<Rational, LexOrder> basis({x, y, z});
        PolySystem<Rational, LexOrder> expected{
            {Polynomial<Rational, LexOrder>{{1, {1, 0}}}},
            {Polynomial<Rational, LexOrder>{{1, {0, 1}}}}};

        GroebnerAlgorithm::ReduceBasisInplace(basis);
        CheckEqual(basis, expected);
        ASSERT_FALSE(basis.IsEmpty());
        ASSERT_EQ(basis.GetSize(), 2);
    }
}

TEST(BasisReduction, Advanced) {
    {
        Polynomial<Rational, LexOrder> x{{1, {2, 0}}, {1, {1, 1}}, {1, {0, 0}}};
        Polynomial<Rational, LexOrder> y{{1, {1, 1}}, {-1, {0, 2}}};
        Polynomial<Rational, LexOrder> z{{2, {0, 3}}, {1, {0, 1}}};

        PolySystem<Rational, LexOrder> basis({x, y, z});

        Polynomial<Rational, LexOrder> x1{{1, {2, 0}}, {1, {0, 2}}, {1, {0}}};
        Polynomial<Rational, LexOrder> y1{{1, {1, 1}}, {-1, {0, 2}}};
        Polynomial<Rational, LexOrder> z1{{1, {0, 3}}, {{1, 2}, {0, 1}}};

        PolySystem<Rational, LexOrder> expected({x1, y1, z1});

        GroebnerAlgorithm::ReduceBasisInplace(basis);
        CheckEqual(basis, expected);
        ASSERT_FALSE(basis.IsEmpty());
        ASSERT_EQ(basis.GetSize(), 3);
    }

    // TODO add example with another degree ordering and with modulo field
}

TEST(BasisBuild, BasisAlready) {
    {
        Polynomial<Rational, LexOrder> x{{1, {1, 0, 0}}};
        Polynomial<Rational, LexOrder> y{{1, {0, 1, 0}}};
        Polynomial<Rational, LexOrder> z{{1, {0, 0, 1}}};

        PolySystem<Rational, LexOrder> basis({x, y, z});
        PolySystem<Rational, LexOrder> expected({x, y, z});

        GroebnerAlgorithm::BuildGBInplace(basis);
        CheckEqual(basis, expected);
        ASSERT_FALSE(basis.IsEmpty());
        ASSERT_EQ(basis.GetSize(), 3);
    }

    {
        Polynomial<Rational, LexOrder> x{{1, {1, 0, 0}}};
        Polynomial<Rational, LexOrder> y{{1, {0, 1, 0}}};
        Polynomial<Rational, LexOrder> z{{1, {0, 0, 1}}};

        PolySystem<Rational, LexOrder> basis({x, y, z});
        PolySystem<Rational, LexOrder> expected({x, y, z});

        GroebnerAlgorithm::BuildGBInplace(basis, AutoReduction::Enabled);
        CheckEqual(basis, expected);
        ASSERT_FALSE(basis.IsEmpty());
        ASSERT_EQ(basis.GetSize(), 3);
    }
}

TEST(BasisBuild, Advanced) {
        {
            Polynomial<Rational, LexOrder> x{{1, {2, 0}}, {1, {1, 1}}, {1, {0, 0}}};
            Polynomial<Rational, LexOrder> y{{1, {1, 1}}, {-1, {0, 2}}};


            Polynomial<Rational, LexOrder> x1{{1, {2, 0}}, {1, {1, 1}}, {1, {0, 0}}};
            Polynomial<Rational, LexOrder> y1{{1, {1, 1}}, {-1, {0, 2}}};
            Polynomial<Rational, LexOrder> z1{{1, {0, 3}}, {{1, 2}, {0, 1}}};

            PolySystem<Rational, LexOrder> basis({x, y});
            PolySystem<Rational, LexOrder> expected({x1, y1, z1});

            basis = GroebnerAlgorithm::BuildGB(basis);
            CheckEqual(basis, expected);
            ASSERT_FALSE(basis.IsEmpty());
            ASSERT_EQ(basis.GetSize(), 3);
        }

    {
        Polynomial<Rational, LexOrder> x{{1, {2, 0}}, {1, {1, 1}}, {1, {0, 0}}};
        Polynomial<Rational, LexOrder> y{{1, {1, 1}}, {-1, {0, 2}}};

        Polynomial<Rational, LexOrder> x1{
            {1, {2, 0}}, {1, {0, 2}}, {1, {0, 0}}};
        Polynomial<Rational, LexOrder> y1{{1, {1, 1}}, {-1, {0, 2}}};
        Polynomial<Rational, LexOrder> z1{{1, {0, 3}}, {{1, 2}, {0, 1}}};

        PolySystem<Rational, LexOrder> basis({x, y});
        PolySystem<Rational, LexOrder> expected({x1, y1, z1});

        basis = GroebnerAlgorithm::BuildGB(basis, AutoReduction::Enabled);
        CheckEqual(basis, expected);
        ASSERT_FALSE(basis.IsEmpty());
        ASSERT_EQ(basis.GetSize(), 3);
    }
    // TODO add example with another degree ordering and with modulo field
}

}  // namespace Groebner::Test
