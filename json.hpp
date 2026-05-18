// nlohmann/json single-header library stub for compilation
// For full functionality, download json.hpp from https://github.com/nlohmann/json/releases
// and replace this file with the official version.

#pragma once

#include <map>
#include <string>
#include <vector>
#include <initializer_list>
#include <stdexcept>
#include <type_traits>
#include <iostream>
#include <fstream>

namespace nlohmann {

    class json {
    public:
        // Minimal stub for is_array()
        bool is_array() const { return false; }
        // Minimal stub for operator[]
        json& operator[](size_t) { return *this; }
        const json& operator[](size_t) const { return *this; }
        // Minimal stub for at()
        const json& at(const std::string&) const { return *this; }
        // Minimal stub for get<T>()
        template<typename T>
        T get() const { return T(); }
        // Minimal stub for begin/end
        auto begin() const { return std::vector<json>().begin(); }
        auto end() const { return std::vector<json>().end(); }
        // Minimal stub for input stream
        friend std::istream& operator>>(std::istream& is, json&) { return is; }
    };

} // namespace nlohmann
