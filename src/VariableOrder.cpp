#include "VariableOrder.h"

#include <cassert>

namespace Groebner {

VariableOrder& VariableOrder::Instance() {
    static VariableOrder instance;
    return instance;
}

void VariableOrder::SetVar(size_t index, const std::string& name) {
    index_to_name_[index] = name;
    name_to_index_[name] = index;
}

void VariableOrder::SetVarList(std::vector<var_pair>&& var_list) {
    for (auto& [index, name] : var_list) {
        index_to_name_[index] = name;
        name_to_index_[std::move(name)] = index;
    }
}

void VariableOrder::SetVarList(std::initializer_list<var_pair> var_list) {
    for (auto& [index, name] : var_list) {
        index_to_name_[index] = name;
        name_to_index_[name] = index;
    }
}

void VariableOrder::ClearVarList() {
    index_to_name_.clear();
    name_to_index_.clear();
}

std::string VariableOrder::GetAt(size_t index) {
    assert(index_to_name_.contains(index) && "No such index");
    return index_to_name_[index];
}

size_t VariableOrder::GetAt(const std::string& name) {
    assert(name_to_index_.contains(name) && "No such variable");
    return name_to_index_[name];
}

}  // namespace Groebner
