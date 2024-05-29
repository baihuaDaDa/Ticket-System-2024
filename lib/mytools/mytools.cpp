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
        const ull M1 = 1e9 + 7;
        const ull b1 = 257;
        const ull M2 = 1e9 + 9;
        const ull b2 = 263;
        ull hashCode1 = 0, hashCode2 = 0;
        for (size_t i = 0; i < str.GetSize(); ++i) {
            hashCode1 = (hashCode1 * b1 + str[i]) % M1;
            hashCode2 = (hashCode2 * b2 + str[i]) % M2;
        }
        return (hashCode1 << 32) + hashCode2;
    }

    ull hash(const std::string &str) {
        const ull M1 = 1e9 + 7;
        const ull b1 = 257;
        const ull M2 = 1e9 + 9;
        const ull b2 = 263;
        ull hashCode1 = 0, hashCode2 = 0;
        for (char i : str) {
            hashCode1 = (hashCode1 * b1 + i) % M1;
            hashCode2 = (hashCode2 * b2 + i) % M2;
        }
        return (hashCode1 << 32) + hashCode2;
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