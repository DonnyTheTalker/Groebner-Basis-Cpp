#pragma once

#include "MonomialCompare.h"

#include <type_traits>

template <typename T>
concept IsComparator = std::is_base_of_v<StraightOrder, T>;