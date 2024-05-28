#ifndef TICKET_SYSTEM_MYTOOLS_HPP
#define TICKET_SYSTEM_MYTOOLS_HPP

#include "STLite/exceptions.hpp"
#include "constantLengthString.hpp"
#include <string>

namespace baihua {

    using ull = unsigned long long;

    // left-closed and right-open
    // unsigned
    int string_to_int(const std::string &str, size_t beg, size_t end);

    int default_string_to_int(const std::string &str);

    // hash
    template<size_t length>
    ull hash(const ConstLenStr<length> &str);

    ull hash(const std::string &str);

    // @return -1, 0, 1
    int CmpUll(const ull &lhs, const ull &rhs);

    int CmpInt(const int &lhs, const int &rhs);

}

#endif //TICKET_SYSTEM_MYTOOLS_HPP
