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
                std::stringstream ss(str);
                int ind = 0;
                while (std::getline(ss, _data[ind++], ch));
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
