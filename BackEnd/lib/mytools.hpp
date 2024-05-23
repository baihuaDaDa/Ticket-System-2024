#ifndef TICKET_SYSTEM_MYTOOLS_HPP
#define TICKET_SYSTEM_MYTOOLS_HPP

#include "STLite/exceptions.hpp"
#include <string>

namespace baihua {

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

}

#endif //TICKET_SYSTEM_MYTOOLS_HPP
