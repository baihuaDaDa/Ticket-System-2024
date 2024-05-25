#include "train.hpp"

namespace ticket {


    void TrainManager::add_train(std::ostream &os, const ull &_i, const int _n, const int _m, const staNameType *_s,
                                 const int *_p, const Clock &_x, const int *_t, const int *_o,
                                 const baihua::pair<Date, Date> &_d, const char _y) {

    }

    void TrainManager::delete_train(const ull &_i) {

    }

    void TrainManager::release_train(const ull &_i) {

    }

    void TrainManager::query_train(const ull &_i, const Date &_d) {

    }

    void TrainManager::query_ticket(const ull &_s, const ull &_t, const Date &_d, const bool _p) {

    }

    void TrainManager::query_transfer(const ull &_s, const ull &_t, const Date &_d, const bool _p) {

    }

    bool TrainManager::buy_ticket_if_success(const ull &_i, const Date &_d, const int _n, const staNameType &_f,
                                             const staNameType &_t, const bool _q) {

        return false;
    }

    void TrainManager::refund_ticket(const ull &_i, const Date &_d, const int _n_ref, const int _f_ref,
                                     const int _t_ref, const int _n_que, const int _f_que, const int _t_que) {

    }
}