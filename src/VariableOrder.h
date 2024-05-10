#pragma once

#include <cstdlib>
#include <string>
#include <unordered_map>
#include <vector>

#include "IteratorFwd.h"

namespace Groebner {
class VariableOrder {
    private:
        VariableOrder() = default;
        VariableOrder(const VariableOrder& other);

        using var_pair = std::pair<size_t, std::string>;

    public:
        static VariableOrder& Instance();

        // TODO add checks for no conflicts
        void SetVar(size_t index, const std::string& name);
        void SetVarList(std::vector<var_pair>&& var_list);
        void SetVarList(std::initializer_list<var_pair> var_list);

        template <Details::IsIterator It>
        void SetVarList(It begin, It end) {
            for (auto it = begin; it != end; it++) {
                index_to_name_[it->first] = it->second;
                name_to_index_[it->second] = it->first;
            }
        }

        void ClearVarList();
        std::string GetAt(size_t index);
        size_t GetAt(const std::string& name);

    private:
        std::unordered_map<size_t, std::string> index_to_name_;
        std::unordered_map<std::string, size_t> name_to_index_;
};
}  // namespace Groebner
