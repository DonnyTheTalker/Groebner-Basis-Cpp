#include "Modulo.h"

namespace Groebner::Details {

ModuloValueType FindGcdExtended(ModuloValueType x, ModuloValueType y,
                                ModuloValueType* coef_x,
                                ModuloValueType* coef_y) {
    assert(coef_x && coef_y);

    if (x == 0) {
        *coef_x = 0;
        *coef_y = 1;
        return y;
    }

    ModuloValueType temp_x, temp_y;
    ModuloValueType gcd = FindGcdExtended(y % x, x, &temp_x, &temp_y);
    *coef_x = temp_y - (y / x) * temp_x;
    *coef_y = temp_x;
    return gcd;
}
}  // namespace Groebner::Details
