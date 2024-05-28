#ifndef TICKET_SYSTEM_ARRAY_HPP
#define TICKET_SYSTEM_ARRAY_HPP

#include <iostream>
#include <sstream>
#include "STLite/exceptions.hpp"

namespace baihua {

    template<class T, std::size_t length> // length must be greater than zero
    class Array {
    public:
        Array() = default;

        Array(const Array &other) {
            for (size_t i = 0; i < length; ++i)
                _data[i] = other._data[i];
        }

        ~Array() = default;

        explicit Array(const std::string &str, const char ch = '|', const std::string &null_string = "_") {
            if (str != null_string) {
                int len = str.size(), ptr = 0;
                std::string substring;
                for (int i = 0; i < len; i++) {
                    if (str[i] != ch) substring += str[i];
                    if (str[i] == ch || i == len - 1) {
                        std::stringstream sstr(substring);
                        sstr >> _data[ptr++];
                        substring.clear();
                    }
                }
            }
        }

        T &operator[](std::size_t ind) {
            if (ind >= length) {
                throw index_out_of_bound();
            }
            return _data[ind];
        }

        const T &operator[](std::size_t ind) const {
            if (ind >= length) {
                throw index_out_of_bound();
            }
            return _data[ind];
        }

        T *data() {
            return *_data;
        }

        const T *data() const {
            return _data;
        }

        size_t size() const {
            return length;
        }

    private:
        T _data[length];
    };

}

#endif //TICKET_SYSTEM_ARRAY_HPP
