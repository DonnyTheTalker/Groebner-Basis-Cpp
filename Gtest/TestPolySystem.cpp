#include "gtest/gtest.h"
#include "PolySystem.h"


TEST(PolySystemBasic, FastPushPop) {
    Polynomial<Rational, LexOrder> x({Monomial(Rational(3), MonomialDegree(2, {2, 0})),
                                      Monomial(Rational(1), MonomialDegree(2, {1, 1})),
                                      Monomial(Rational(2), MonomialDegree(2, {0, 1}))});
    Polynomial<Rational, LexOrder> y({Monomial(Rational(3), MonomialDegree(2, {2, 0})),
                                      Monomial(Rational(1), MonomialDegree(2, {1, 1})),
                                      Monomial(Rational(2), MonomialDegree(2, {0, 1}))});
    Polynomial<Rational, LexOrder> z({Monomial(Rational(3), MonomialDegree(2, {2, 0})),
                                      Monomial(Rational(1), MonomialDegree(2, {1, 1})),
                                      Monomial(Rational(2), MonomialDegree(2, {0, 1}))});

    PolySystem<Rational, LexOrder> sys({x, y, z});

    PolySystem<Rational, LexOrder> copy(sys);

    {
        PolySystem<Rational, LexOrder> temp({y, z});
        sys.SwapAndPop(0);
        EXPECT_EQ(sys, temp);
        sys.AddAndSwap(0, x);
        EXPECT_EQ(sys, copy);
    }

    {
        PolySystem<Rational, LexOrder> temp({x, y});
        sys.SwapAndPop(2);
        EXPECT_EQ(sys, temp);
        sys.AddAndSwap(2, z);
        EXPECT_EQ(sys, copy);
    }

    {
        EXPECT_DEATH(sys.SwapAndPop(4), "Out of bounds");
        EXPECT_DEATH(sys.AddAndSwap(4, x), "Out of bounds");
    }
}