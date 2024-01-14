#include "gtest/gtest.h"
#include "GroebnerBuilder.h"

TEST(Builder, Lcm) {
    {
        Monomial x(Rational(3), MonomialDegree(3, {1, 2, 3}));
        Monomial y(Rational(3), MonomialDegree(3, {1, 2, 3}));
        Monomial z = GroebnerBuilder::FindLcm(x, y);

        EXPECT_EQ(z.GetDegree(), MonomialDegree(3, {1, 2, 3}));
        EXPECT_EQ(z.GetDegree().GetSumDegree(), 6);
    }

    {
        Monomial x(Rational(3), MonomialDegree(3, {1, 2, 3}));
        Monomial y(Rational(3), MonomialDegree(3, {2, 4, 5}));
        Monomial z = GroebnerBuilder::FindLcm(x, y);

        EXPECT_EQ(z.GetDegree(), MonomialDegree(3, {2, 4, 5}));
        EXPECT_EQ(z.GetDegree().GetSumDegree(), 11);
    }

    {
        Monomial x(Rational(3), MonomialDegree(3, {1, 2, 3}));
        Monomial y(Rational(3), MonomialDegree(3, {2, 2, 1}));
        Monomial z = GroebnerBuilder::FindLcm(x, y);

        EXPECT_EQ(z.GetDegree(), MonomialDegree(3, {2, 2, 3}));
        EXPECT_EQ(z.GetDegree().GetSumDegree(), 7);
    }

    {
        Monomial x(Modulo(7, 2), MonomialDegree(3, {1, 2, 3}));
        Monomial y(Modulo(7, 3), MonomialDegree(3, {2, 2, 1}));
        Monomial z = GroebnerBuilder::FindLcm(x, y);

        EXPECT_EQ(z.GetDegree(), MonomialDegree(3, {2, 2, 3}));
        EXPECT_EQ(z.GetDegree().GetSumDegree(), 7);
    }
}

TEST(Builder, SPoly) {
    {
        Polynomial<Rational, LexOrder> x({Monomial(Rational(2), MonomialDegree(2, {2, 1})),
                                          Monomial(Rational(1), MonomialDegree(2, {0, 1}))});
        Polynomial<Rational, LexOrder> y({Monomial(Rational(1), MonomialDegree(2, {1, 1})),
                                          Monomial(Rational(1), MonomialDegree(2, {1, 0}))});

        Monomial lcm = GroebnerBuilder::FindLcm(x.GetLeader(), y.GetLeader());
        EXPECT_EQ(lcm, Monomial(Rational(2), MonomialDegree(2, {2, 1})));
        EXPECT_EQ(lcm.GetDegree().GetSumDegree(), 3);

        Polynomial<Rational, LexOrder> z = GroebnerBuilder::SPolynomial(x, y, lcm);
        Polynomial<Rational, LexOrder> expected({Monomial(Rational(-2), MonomialDegree(2, {2, 0})),
                                                 Monomial(Rational(1), MonomialDegree(2, {0, 1}))});
        EXPECT_EQ(z, expected);
    }

    {
        Polynomial<Modulo, LexOrder> x({Monomial(Modulo(3, 2), MonomialDegree(2, {2, 1})),
                                        Monomial(Modulo(3, 1), MonomialDegree(2, {0, 1}))});
        Polynomial<Modulo, LexOrder> y({Monomial(Modulo(3, 1), MonomialDegree(2, {1, 1})),
                                        Monomial(Modulo(3, 1), MonomialDegree(2, {1, 0}))});

        Monomial lcm = GroebnerBuilder::FindLcm(x.GetLeader(), y.GetLeader());
        EXPECT_EQ(lcm, Monomial(Modulo(3, 2), MonomialDegree(2, {2, 1})));
        EXPECT_EQ(lcm.GetDegree().GetSumDegree(), 3);

        Polynomial<Modulo, LexOrder> z = GroebnerBuilder::SPolynomial(x, y, lcm);
        Polynomial<Modulo, LexOrder> expected({Monomial(Modulo(3, -2), MonomialDegree(2, {2, 0})),
                                               Monomial(Modulo(3, 1), MonomialDegree(2, {0, 1}))});
        EXPECT_EQ(z, expected);
    }

    {
        Polynomial<Rational, LexOrder> x({Monomial(Rational(2), MonomialDegree(2, {1, 2}))});
        Polynomial<Rational, LexOrder> y({Monomial(Rational(1), MonomialDegree(2, {2, 1}))});

        Monomial lcm = GroebnerBuilder::FindLcm(x.GetLeader(), y.GetLeader());
        EXPECT_EQ(lcm, Monomial(Rational(2), MonomialDegree(2, {2, 2})));
        EXPECT_EQ(lcm.GetDegree().GetSumDegree(), 4);

        Polynomial<Rational, LexOrder> z = GroebnerBuilder::SPolynomial(x, y, lcm);
        Polynomial<Rational, LexOrder> expected;
        EXPECT_EQ(z, expected);
        EXPECT_TRUE(z.IsZero());
    }

    {
        Polynomial<Modulo, LexOrder> x({Monomial(Modulo(3, 2), MonomialDegree(2, {1, 2}))});
        Polynomial<Modulo, LexOrder> y({Monomial(Modulo(3, 1), MonomialDegree(2, {2, 1}))});

        Monomial lcm = GroebnerBuilder::FindLcm(x.GetLeader(), y.GetLeader());
        EXPECT_EQ(lcm, Monomial(Modulo(3, 2), MonomialDegree(2, {2, 2})));
        EXPECT_EQ(lcm.GetDegree().GetSumDegree(), 4);

        Polynomial<Modulo, LexOrder> z = GroebnerBuilder::SPolynomial(x, y, lcm);
        Polynomial<Modulo, LexOrder> expected;
        EXPECT_EQ(z, expected);
        EXPECT_TRUE(z.IsZero());
    }

    {
        Polynomial<Rational, ReverseLexOrder> x({Monomial(Rational(2), MonomialDegree(2, {2, 1})),
                                                 Monomial(Rational(1), MonomialDegree(2, {0, 1}))});
        Polynomial<Rational, ReverseLexOrder> y({Monomial(Rational(1), MonomialDegree(2, {1, 1})),
                                                 Monomial(Rational(1), MonomialDegree(2, {1, 0}))});

        Monomial lcm = GroebnerBuilder::FindLcm(x.GetLeader(), y.GetLeader());
        EXPECT_EQ(lcm, Monomial(Rational(1), MonomialDegree(2, {1, 1})));
        EXPECT_EQ(lcm.GetDegree().GetSumDegree(), 2);

        Polynomial<Rational, ReverseLexOrder> z = GroebnerBuilder::SPolynomial(x, y, lcm);
        Polynomial<Rational, ReverseLexOrder> expected({Monomial(Rational(2), MonomialDegree(2, {3, 1})),
                                                        Monomial(Rational(-1), MonomialDegree(2, {1, 2}))});
        EXPECT_EQ(z, expected);
    }

    {
        Polynomial<Rational, GrlexOrder> x({Monomial(Rational(2), MonomialDegree(2, {2, 1})),
                                            Monomial(Rational(1), MonomialDegree(2, {0, 1}))});
        Polynomial<Rational, GrlexOrder> y({Monomial(Rational(1), MonomialDegree(2, {0, 1})),
                                            Monomial(Rational(1), MonomialDegree(2, {1, 0}))});

        Monomial lcm = GroebnerBuilder::FindLcm(x.GetLeader(), y.GetLeader());
        EXPECT_EQ(lcm, Monomial(Rational(2), MonomialDegree(2, {2, 1})));
        EXPECT_EQ(lcm.GetDegree().GetSumDegree(), 3);

        Polynomial<Rational, GrlexOrder> z = GroebnerBuilder::SPolynomial(x, y, lcm);
        Polynomial<Rational, GrlexOrder> expected({Monomial(Rational(-2), MonomialDegree(2, {1, 2})),
                                                   Monomial(Rational(1), MonomialDegree(2, {0, 1}))});
        EXPECT_EQ(z, expected);
    }

    {
        Polynomial<Rational, GrevlexOrder> x({Monomial(Rational(2), MonomialDegree(2, {2, 1})),
                                              Monomial(Rational(1), MonomialDegree(2, {0, 1}))});
        Polynomial<Rational, GrevlexOrder> y({Monomial(Rational(1), MonomialDegree(2, {0, 1})),
                                              Monomial(Rational(1), MonomialDegree(2, {1, 0}))});

        Monomial lcm = GroebnerBuilder::FindLcm(x.GetLeader(), y.GetLeader());
        EXPECT_EQ(lcm, Monomial(Rational(2), MonomialDegree(2, {2, 1})));
        EXPECT_EQ(lcm.GetDegree().GetSumDegree(), 3);

        Polynomial<Rational, GrevlexOrder> z = GroebnerBuilder::SPolynomial(x, y, lcm);
        Polynomial<Rational, GrevlexOrder> expected({Monomial(Rational(-2), MonomialDegree(2, {1, 2})),
                                                     Monomial(Rational(1), MonomialDegree(2, {0, 1}))});
        EXPECT_EQ(z, expected);
    }
}

TEST(Reduction, Simple) {
    {
        Polynomial<Rational, LexOrder> x({Monomial(Rational(1), MonomialDegree(3, {1, 1, 1}))});
        Polynomial<Rational, LexOrder> y({Monomial(Rational(1), MonomialDegree(3, {1, 1, 1}))});

        Polynomial<Rational, LexOrder> z = GroebnerBuilder::ReducePolynomial(x, PolySystem<Rational, LexOrder>({y}));
        Polynomial<Rational, LexOrder> expected;

        EXPECT_TRUE(z.IsZero());
        EXPECT_EQ(z.GetMonomials().size(), 0);
        EXPECT_EQ(z, expected);
    }

    {
        Polynomial<Rational, LexOrder> x({Monomial(Rational(1), MonomialDegree(3, {1, 1, 1}))});
        Polynomial<Rational, LexOrder> y1({Monomial(Rational(1), MonomialDegree(3, {1, 0, 0}))});
        Polynomial<Rational, LexOrder> y2({Monomial(Rational(1), MonomialDegree(3, {0, 1, 0}))});
        Polynomial<Rational, LexOrder> y3({Monomial(Rational(1), MonomialDegree(3, {0, 0, 1}))});

        Polynomial<Rational, LexOrder> z = GroebnerBuilder::ReducePolynomial(x, PolySystem<Rational, LexOrder>(
                {y1, y2, y3}));
        Polynomial<Rational, LexOrder> expected;

        EXPECT_TRUE(z.IsZero());
        EXPECT_EQ(z.GetMonomials().size(), 0);
        EXPECT_EQ(z, expected);
    }

    {
        Polynomial<Rational, LexOrder> x({Monomial(Rational(1, 2), MonomialDegree(2, {2, 1})),
                                          Monomial(Rational(2), MonomialDegree(2, {0, 1}))});

        Polynomial<Rational, LexOrder> y1({Monomial(Rational(1), MonomialDegree(2, {1, 0}))});
        Polynomial<Rational, LexOrder> y2({Monomial(Rational(1), MonomialDegree(2, {0, 1}))});

        Polynomial<Rational, LexOrder> z = GroebnerBuilder::ReducePolynomial(x,
                                                                             PolySystem<Rational, LexOrder>({y1, y2}));
        Polynomial<Rational, LexOrder> expected;

        EXPECT_TRUE(z.IsZero());
        EXPECT_EQ(z.GetMonomials().size(), 0);
        EXPECT_EQ(z, expected);
    }

    {
        Polynomial<Rational, LexOrder> x({Monomial(Rational(1, 2), MonomialDegree(2, {2, 1})),
                                          Monomial(Rational(2), MonomialDegree(2, {0, 1}))});

        Polynomial<Rational, LexOrder> y({Monomial(Rational(1), MonomialDegree(2, {0, 1}))});

        Polynomial<Rational, LexOrder> z = GroebnerBuilder::ReducePolynomial(x, PolySystem<Rational, LexOrder>({y}));
        Polynomial<Rational, LexOrder> expected;

        EXPECT_TRUE(z.IsZero());
        EXPECT_EQ(z.GetMonomials().size(), 0);
        EXPECT_EQ(z, expected);
    }

    {
        Polynomial<Rational, GrevlexOrder> x({Monomial(Rational(1, 2), MonomialDegree(2, {2, 1})),
                                              Monomial(Rational(2), MonomialDegree(2, {0, 1}))});

        Polynomial<Rational, GrevlexOrder> y1({Monomial(Rational(1), MonomialDegree(2, {1, 0}))});
        Polynomial<Rational, GrevlexOrder> y2({Monomial(Rational(1), MonomialDegree(2, {0, 1}))});

        Polynomial<Rational, GrevlexOrder> z = GroebnerBuilder::ReducePolynomial(x, PolySystem<Rational, GrevlexOrder>(
                {y1, y2}));
        Polynomial<Rational, GrevlexOrder> expected;

        EXPECT_TRUE(z.IsZero());
        EXPECT_EQ(z.GetMonomials().size(), 0);
        EXPECT_EQ(z, expected);
    }
}

TEST(Reduction, Remainder) {
    {
        Polynomial<Rational, LexOrder> x({Monomial(Rational(1), MonomialDegree(2, {2, 1})),
                                          Monomial(Rational(1), MonomialDegree(2, {0, 1}))});

        Polynomial<Rational, LexOrder> y1({Monomial(Rational(1), MonomialDegree(2, {1, 0}))});

        Polynomial<Rational, LexOrder> z = GroebnerBuilder::ReducePolynomial(x, PolySystem<Rational, LexOrder>({y1}));
        Polynomial<Rational, LexOrder> expected({Monomial(Rational(1), MonomialDegree(2, {0, 1}))});

        EXPECT_FALSE(z.IsZero());
        EXPECT_EQ(z.GetMonomials().size(), 1);
        EXPECT_EQ(z, expected);
    }

    {
        Polynomial<Rational, GrevlexOrder> x({Monomial(Rational(1), MonomialDegree(2, {2, 1})),
                                              Monomial(Rational(1), MonomialDegree(2, {0, 1}))});

        Polynomial<Rational, GrevlexOrder> y1({Monomial(Rational(1), MonomialDegree(2, {1, 0}))});

        Polynomial<Rational, GrevlexOrder> z = GroebnerBuilder::ReducePolynomial(x, PolySystem<Rational, GrevlexOrder>(
                {y1}));
        Polynomial<Rational, GrevlexOrder> expected({Monomial(Rational(1), MonomialDegree(2, {0, 1}))});

        EXPECT_FALSE(z.IsZero());
        EXPECT_EQ(z.GetMonomials().size(), 1);
        EXPECT_EQ(z, expected);
    }

    {
        Polynomial<Rational, LexOrder> x({Monomial(Rational(1), MonomialDegree(2, {2, 1})),
                                          Monomial(Rational(1), MonomialDegree(2, {0, 1}))});

        Polynomial<Rational, LexOrder> y1({Monomial(Rational(1), MonomialDegree(2, {1, 1}))});

        Polynomial<Rational, LexOrder> z = GroebnerBuilder::ReducePolynomial(x, PolySystem<Rational, LexOrder>({y1}));
        Polynomial<Rational, LexOrder> expected({Monomial(Rational(1), MonomialDegree(2, {0, 1}))});

        EXPECT_FALSE(z.IsZero());
        EXPECT_EQ(z.GetMonomials().size(), 1);
        EXPECT_EQ(z, expected);
    }

    {
        Polynomial<Rational, LexOrder> x({Monomial(Rational(1), MonomialDegree(2, {2, 1})),
                                          Monomial(Rational(1), MonomialDegree(2, {0, 1}))});

        Polynomial<Rational, LexOrder> y1({Monomial(Rational(1), MonomialDegree(2, {3, 1}))});

        Polynomial<Rational, LexOrder> z = GroebnerBuilder::ReducePolynomial(x, PolySystem<Rational, LexOrder>({y1}));

        EXPECT_FALSE(z.IsZero());
        EXPECT_EQ(z.GetMonomials().size(), 2);
        EXPECT_EQ(z, x);
    }

    {
        Polynomial<Rational, LexOrder> x({Monomial(Rational(1), MonomialDegree(3, {2, 1, 0})),
                                          Monomial(Rational(1), MonomialDegree(3, {0, 0, 1}))});

        Polynomial<Rational, LexOrder> y1({Monomial(Rational(1), MonomialDegree(3, {1, 0, 0}))});
        Polynomial<Rational, LexOrder> y2({Monomial(Rational(1), MonomialDegree(3, {0, 1, 0}))});

        Polynomial<Rational, LexOrder> z = GroebnerBuilder::ReducePolynomial(x, PolySystem<Rational, LexOrder>({y1}));
        Polynomial<Rational, LexOrder> expected({Monomial(Rational(1), MonomialDegree(3, {0, 0, 1}))});

        EXPECT_FALSE(z.IsZero());
        EXPECT_EQ(z.GetMonomials().size(), 1);
        EXPECT_EQ(z, expected);
    }

    {
        Polynomial<Modulo, LexOrder> x({Monomial(Modulo(3, 1), MonomialDegree(3, {2, 1, 0})),
                                        Monomial(Modulo(3, 1), MonomialDegree(3, {0, 0, 1}))});

        Polynomial<Modulo, LexOrder> y1({Monomial(Modulo(3, 1), MonomialDegree(3, {1, 0, 0}))});
        Polynomial<Modulo, LexOrder> y2({Monomial(Modulo(3, 1), MonomialDegree(3, {0, 1, 0}))});

        Polynomial<Modulo, LexOrder> z = GroebnerBuilder::ReducePolynomial(x, PolySystem<Modulo, LexOrder>({y1}));
        Polynomial<Modulo, LexOrder> expected({Monomial(Modulo(3, 1), MonomialDegree(3, {0, 0, 1}))});

        EXPECT_FALSE(z.IsZero());
        EXPECT_EQ(z.GetMonomials().size(), 1);
        EXPECT_EQ(z, expected);
    }
}

TEST(BasisReduction, Simple) {
    {
        Polynomial<Rational, LexOrder> x({Monomial(Rational(3), MonomialDegree(2, {1, 0}))});
        Polynomial<Rational, LexOrder> y({Monomial(Rational(4), MonomialDegree(2, {0, 1}))});

        PolySystem<Rational, LexOrder> basis({x, y});
        PolySystem<Rational, LexOrder> expected(
                {Polynomial<Rational, LexOrder>({Monomial(Rational(1), MonomialDegree(2, {1, 0}))}),
                 Polynomial<Rational, LexOrder>({Monomial(Rational(1), MonomialDegree(2, {0, 1}))})
                });

        basis = GroebnerBuilder::ReduceBasis(basis);
        EXPECT_EQ(basis, expected);
        EXPECT_FALSE(basis.IsEmpty());
        EXPECT_EQ(basis.GetSize(), 2);
    }

    {
        Polynomial<Rational, LexOrder> x({Monomial(Rational(3), MonomialDegree(2, {2, 0}))});
        Polynomial<Rational, LexOrder> y({Monomial(Rational(4), MonomialDegree(2, {1, 0}))});

        PolySystem<Rational, LexOrder> basis({x, y});
        PolySystem<Rational, LexOrder> expected(
                {Polynomial<Rational, LexOrder>({Monomial(Rational(1), MonomialDegree(2, {1, 0}))})
                });

        basis = GroebnerBuilder::ReduceBasis(basis);
        EXPECT_EQ(basis, expected);
        EXPECT_FALSE(basis.IsEmpty());
        EXPECT_EQ(basis.GetSize(), 1);
    }

    {
        Polynomial<Rational, LexOrder> x({Monomial(Rational(5), MonomialDegree(2, {1, 0})),
                                          Monomial(Rational(1), MonomialDegree(2, {0, 1}))});
        Polynomial<Rational, LexOrder> y({Monomial(Rational(4), MonomialDegree(2, {1, 0}))});
        Polynomial<Rational, LexOrder> z({Monomial(Rational(1, 2), MonomialDegree(2, {0, 1}))});

        PolySystem<Rational, LexOrder> basis({x, y, z});
        PolySystem<Rational, LexOrder> expected(
                {Polynomial<Rational, LexOrder>({Monomial(Rational(1), MonomialDegree(2, {1, 0}))}),
                 Polynomial<Rational, LexOrder>({Monomial(Rational(1), MonomialDegree(2, {0, 1}))})
                });

        basis = GroebnerBuilder::ReduceBasis(basis);
        EXPECT_EQ(basis, expected);
        EXPECT_FALSE(basis.IsEmpty());
        EXPECT_EQ(basis.GetSize(), 2);
    }
}

TEST(BasisReduction, Advanced) {
    {
        Polynomial<Rational, LexOrder> x({Monomial(Rational(1), MonomialDegree(2, {2, 0})),
                                          Monomial(Rational(1), MonomialDegree(2, {1, 1})),
                                          Monomial(Rational(1), MonomialDegree(2, {0, 0}))});
        Polynomial<Rational, LexOrder> y({Monomial(Rational(1), MonomialDegree(2, {1, 1})),
                                          Monomial(Rational(-1), MonomialDegree(2, {0, 2}))});
        Polynomial<Rational, LexOrder> z({Monomial(Rational(2), MonomialDegree(2, {0, 3})),
                                          Monomial(Rational(1), MonomialDegree(2, {0, 1}))});

        PolySystem<Rational, LexOrder> basis({x, y, z});

        Polynomial<Rational, LexOrder> x1({Monomial(Rational(1), MonomialDegree(2, {2, 0})),
                                          Monomial(Rational(1), MonomialDegree(2, {0, 2})),
                                          Monomial(Rational(1), MonomialDegree(2, {0, 0}))});
        Polynomial<Rational, LexOrder> y1({Monomial(Rational(1), MonomialDegree(2, {1, 1})),
                                          Monomial(Rational(-1), MonomialDegree(2, {0, 2}))});
        Polynomial<Rational, LexOrder> z1({Monomial(Rational(1), MonomialDegree(2, {0, 3})),
                                          Monomial(Rational(1, 2), MonomialDegree(2, {0, 1}))});
        PolySystem<Rational, LexOrder> expected({x1, y1, z1});

        basis = GroebnerBuilder::ReduceBasis(basis);
        EXPECT_EQ(basis, expected);
        EXPECT_FALSE(basis.IsEmpty());
        EXPECT_EQ(basis.GetSize(), 3);
    }

    // TODO add example with another monomial ordering and with modulo field
}

TEST(BasisBuild, BasisAlready) {
    {
        Polynomial<Rational, LexOrder> x({Monomial(Rational(1), MonomialDegree(3, {1, 0, 0}))});
        Polynomial<Rational, LexOrder> y({Monomial(Rational(1), MonomialDegree(3, {0, 1, 0}))});
        Polynomial<Rational, LexOrder> z({Monomial(Rational(1), MonomialDegree(3, {0, 0, 1}))});

        PolySystem<Rational, LexOrder> basis({x, y, z});
        PolySystem<Rational, LexOrder> expected({x, y, z});

        basis = GroebnerBuilder::Build(basis);
        EXPECT_EQ(basis, expected);
        EXPECT_FALSE(basis.IsEmpty());
        EXPECT_EQ(basis.GetSize(), 3);
    }

    {
        Polynomial<Rational, LexOrder> x({Monomial(Rational(1), MonomialDegree(3, {1, 0, 0}))});
        Polynomial<Rational, LexOrder> y({Monomial(Rational(1), MonomialDegree(3, {0, 1, 0}))});
        Polynomial<Rational, LexOrder> z({Monomial(Rational(1), MonomialDegree(3, {0, 0, 1}))});

        PolySystem<Rational, LexOrder> basis({x, y, z});
        PolySystem<Rational, LexOrder> expected({x, y, z});

        basis = GroebnerBuilder::Build(basis, true);
        EXPECT_EQ(basis, expected);
        EXPECT_FALSE(basis.IsEmpty());
        EXPECT_EQ(basis.GetSize(), 3);
    }
}

TEST(BasisBuild, Advanced) {
    {
        Polynomial<Rational, LexOrder> x({Monomial(Rational(1), MonomialDegree(2, {2, 0})),
                                          Monomial(Rational(1), MonomialDegree(2, {1, 1})),
                                          Monomial(Rational(1), MonomialDegree(2, {0, 0}))});
        Polynomial<Rational, LexOrder> y({Monomial(Rational(1), MonomialDegree(2, {1, 1})),
                                          Monomial(Rational(-1), MonomialDegree(2, {0, 2}))});

        PolySystem<Rational, LexOrder> basis({x, y});

        Polynomial<Rational, LexOrder> x1({Monomial(Rational(1), MonomialDegree(2, {2, 0})),
                                          Monomial(Rational(1), MonomialDegree(2, {1, 1})),
                                          Monomial(Rational(1), MonomialDegree(2, {0, 0}))});
        Polynomial<Rational, LexOrder> y1({Monomial(Rational(1), MonomialDegree(2, {1, 1})),
                                          Monomial(Rational(-1), MonomialDegree(2, {0, 2}))});
        Polynomial<Rational, LexOrder> z1({Monomial(Rational(-2), MonomialDegree(2, {0, 3})),
                                          Monomial(Rational(-1), MonomialDegree(2, {0, 1}))});
        PolySystem<Rational, LexOrder> expected({x1, y1, z1});

        basis = GroebnerBuilder::Build(basis);
        EXPECT_EQ(basis, expected);
        EXPECT_FALSE(basis.IsEmpty());
        EXPECT_EQ(basis.GetSize(), 3);
    }

    {
        Polynomial<Rational, LexOrder> x({Monomial(Rational(1), MonomialDegree(2, {2, 0})),
                                          Monomial(Rational(1), MonomialDegree(2, {1, 1})),
                                          Monomial(Rational(1), MonomialDegree(2, {0, 0}))});
        Polynomial<Rational, LexOrder> y({Monomial(Rational(1), MonomialDegree(2, {1, 1})),
                                          Monomial(Rational(-1), MonomialDegree(2, {0, 2}))});

        PolySystem<Rational, LexOrder> basis({x, y});

        Polynomial<Rational, LexOrder> x1({Monomial(Rational(1), MonomialDegree(2, {2, 0})),
                                           Monomial(Rational(1), MonomialDegree(2, {0, 2})),
                                           Monomial(Rational(1), MonomialDegree(2, {0, 0}))});
        Polynomial<Rational, LexOrder> y1({Monomial(Rational(1), MonomialDegree(2, {1, 1})),
                                           Monomial(Rational(-1), MonomialDegree(2, {0, 2}))});
        Polynomial<Rational, LexOrder> z1({Monomial(Rational(1), MonomialDegree(2, {0, 3})),
                                           Monomial(Rational(1, 2), MonomialDegree(2, {0, 1}))});
        PolySystem<Rational, LexOrder> expected({x1, y1, z1});

        basis = GroebnerBuilder::Build(basis, true);
        EXPECT_EQ(basis, expected);
        EXPECT_FALSE(basis.IsEmpty());
        EXPECT_EQ(basis.GetSize(), 3);
    }

    // TODO add example with another monomial ordering and with modulo field
}