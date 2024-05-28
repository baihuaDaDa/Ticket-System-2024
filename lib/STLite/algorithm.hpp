#ifndef TICKET_SYSTEM_ALGORITHM_HPP
#define TICKET_SYSTEM_ALGORITHM_HPP

#include <iostream>

namespace baihua {

    template<class T>
    void sort(T *beg, T *end, int (*cmp)(const T &, const T &)) {
        if (beg + 1 == end || beg == end) return;
        int l = 0, r = end - beg - 1, mid = (l + r) / 2;
        T pivot = beg[mid];
        while (l < r) {
            if (l < mid) {
                if (cmp(pivot, beg[l]) == -1) {
                    beg[mid] = beg[l];
                    mid = l;
                }
                else l++;
            }
            if (r > mid) {
                if (cmp(beg[r], pivot) == -1) {
                    beg[mid] = beg[r];
                    mid = r;
                }
                else r--;
            }
        }
        beg[mid] = pivot;
        sort(beg, beg + mid, cmp);
        sort(beg + mid + 1, end, cmp);
    }

}

#endif //TICKET_SYSTEM_ALGORITHM_HPP
