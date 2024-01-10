#pragma once

#include "Modulo.h"
#include "Rational.h"

#include <type_traits>

template<typename T>
concept IsField = std::is_same_v<T, Modulo> || std::is_same_v<T, Rational>;