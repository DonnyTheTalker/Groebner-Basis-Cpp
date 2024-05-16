# Groebner-Basis-Cpp
Simple implementation of the groebner basis construction algorithm

Supported Modulo and Rational fields. Lexicographic, graded lexicographic and graded reverse lexicographic monomial orders. 

Easy polynomial definition:
```cpp
// 2x^2 + 3xy
Polynomial<Rational> poly = {{2, {2}}, {3, {1, 1}}};
```

Examples:

Groebner basis with reduction:
```cpp
// 2x^2 + 3xy
Polynomial<Rational> poly1 = {{2, {2}}, {3, {1, 1}}};
// xy^2 + 1
Polynomial<Rational> poly2 = {{1, {1, 2}}, {1, {}}};

auto basis = BuildGB({poly1, poly2}, AutoReduction::Enabled};
```

Check if polynomial belongs to ideal
```cpp
// 2x^2 + 3xy
Polynomial<Rational> poly1 = {{2, {2}}, {3, {1, 1}}};
// xy^2 + 1
Polynomial<Rational> poly2 = {{1, {1, 2}}, {1, {}}};

// xyz
Polynomial<Rational> aim = {{1, {1, 1, 1}}};
bool belongs = IsInIdeal(aim, {poly1, poly2});
```

Check if ideals are equal
```cpp
// 2x^2 + 3xy
Polynomial<Rational> poly1 = {{2, {2}}, {3, {1, 1}}};
// xy^2 + 1
Polynomial<Rational> poly2 = {{1, {1, 2}}, {1, {}}};

// xyz
Polynomial<Rational> poly3 = {{1, {1, 1, 1}}};
// x^2
Polynomial<Rationanl> poly4 = {{1, {2}}};
bool equal = AreEqualIdeals({poly1, poly2}, {poly3, poly4});
```

With Latex visualization support:

![image](https://github.com/DonnyTheTalker/Groebner-Basis-Cpp/assets/62959945/9cade9dc-67d4-4af6-9182-b5b7d8590a2b)
