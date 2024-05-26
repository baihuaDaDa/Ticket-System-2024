#include "train.hpp"

namespace ticket {


    void TrainManager::add_train(std::ostream &os, const ull &_i, int _n, int _m, const stationsType &_s,
                                 const pricesType &_p, const Clock &_x, const ttsType &_t, const oType &_o,
                                 const baihua::pair<Date, Date> &_d, char _y) {
        auto trainAddr = trainMap.Find(_i);
        if (!trainAddr.empty()) {
            os << -1;
            return;
        }
        Train newTrain;
        newTrain.
    }

    void TrainManager::delete_train(const ull &_i) {

    }

    void TrainManager::release_train(const ull &_i) {

    }

    void TrainManager::query_train(const ull &_i, const Date &_d) {

    }

    void TrainManager::query_ticket(const ull &_s, const ull &_t, const Date &_d, bool _p) {

    }

    void TrainManager::query_transfer(const ull &_s, const ull &_t, const Date &_d, bool _p) {

    }

    bool TrainManager::buy_ticket_if_success(const ull &_i, const Date &_d, int _n, const staNameType &_f,
                                             const staNameType &_t, bool _q) {

        return false;
    }

    void TrainManager::write_seats(int addr, seatsType &seats) {
        dailyTrainData.SingleUpdate(seats, addr);
    }

    void TrainManager::read_seats(seatsType &seats, const ull &_i, const Date &_d) {
        auto dailyTrainAddr = dailyTrainMap.Find(DailyTrainIndex{_i, _d});
        dailyTrainData.SingleRead(seats, dailyTrainAddr[0].seatAddr);
    }

}