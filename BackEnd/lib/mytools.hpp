#ifndef TICKET_SYSTEM_MYTOOLS_HPP
#define TICKET_SYSTEM_MYTOOLS_HPP

#include "STLite/exceptions.hpp"
#include "constantLengthString.hpp"
#include <string>

namespace baihua {

    using ull = unsigned long long;

    // left-closed and right-open
    // unsigned
    unsigned int string_to_int(const std::string &str, size_t beg, size_t end) {
        unsigned int result = 0;
        for (size_t i = beg; i < end; ++i) {
            if (!std::isdigit(str[i])) throw invalid_string();
            result <<= 1;
            result += str[i] - '0';
        }
        return result;
    }

    // hash
    template<size_t length>
    ull hash(const ConstLenStr<length> &str) {
        const ull M = 1e9 + 7;
        const ull b = 197;
        ull hashCode = 0;
        for (size_t i = 0; i < str.GetSize(); ++i)
            hashCode = (hashCode * b + str[i]) % M;
        return hashCode;
    }

    ull hash(const std::string &str) {
        const ull M = 1e9 + 7;
        const ull b = 197;
        ull hashCode = 0;
        for (size_t i = 0; i < str.size(); ++i)
            hashCode = (hashCode * b + str[i]) % M;
        return hashCode;
    }

    // @return -1, 0, 1
    int CmpUll(const ull &lhs, const ull &rhs) {
        if (lhs == rhs) return 0;
        else if (lhs < rhs) return -1;
        else return 1;
    }

    int CmpInt(const int &lhs, const int &rhs) {
        if (lhs == rhs) return 0;
        else if (lhs < rhs) return -1;
        else return 1;
    }

}

#endif //TICKET_SYSTEM_MYTOOLS_HPP
