#ifndef TICKET_SYSTEM_ALGORITHM_HPP
#define TICKET_SYSTEM_ALGORITHM_HPP

#include <iostream>

template<class T, int (*CmpT)(const T &, const T &)>
void merge(T *a, size_t la, T *b, size_t lb) {
}

template<class T, int (*CmpT)(const T &, const T &)>
void sort(T *a, size_t la) {
}

#endif //TICKET_SYSTEM_ALGORITHM_HPP
