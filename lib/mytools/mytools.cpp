#include "mytools.hpp"

namespace baihua {

    // left-closed and right-open
    // unsigned
    int string_to_int(const std::string &str, size_t beg, size_t end) {
        int result = 0;
        for (size_t i = beg; i < end; ++i) {
            if (!std::isdigit(str[i])) throw invalid_string();
            result *= 10;
            result += str[i] - '0';
        }
        return result;
    }

    int default_string_to_int(const std::string &str) {
        int result = 0;
        for (char i : str) {
            if (!std::isdigit(i)) throw invalid_string();
            result *= 10;
            result += i - '0';
        }
        return result;
    }

    // hash
    template<size_t length>
    ull hash(const ConstLenStr<length> &str) {
        const ull M = 998244353;
        const ull b = 257;
        ull hashCode = 0;
        for (size_t i = 0; i < str.GetSize(); ++i)
            hashCode = (hashCode * b + str[i]) % M;
        return hashCode;
    }

    ull hash(const std::string &str) {
        const ull M = 998244353;
        const ull b = 257;
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

    template ull hash<21ull>(const ConstLenStr<21ull> &str);

    template ull hash<41ull>(const ConstLenStr<41ull> &str);

}