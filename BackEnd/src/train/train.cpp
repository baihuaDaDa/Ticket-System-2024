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
        Train newTrain{_n, _s, _m, _p, _x, _t, _o, _d, _y};
        trainMap.Insert(_i, TrainAddr{trainData.SingleAppend(newTrain), false});
        os << 0;
    }

    void TrainManager::delete_train(std::ostream &os, const ull &_i) {
        auto trainAddr = trainMap.Find(_i);
        if (trainAddr.empty() || trainAddr[0].released) {
            os << -1;
            return;
        }
        trainMap.Delete(_i, trainAddr[0]);
        os << 0;
    }

    void TrainManager::release_train(std::ostream &os, const ull &_i) {
        auto trainAddr = trainMap.Find(_i);
        if (trainAddr.empty() || trainAddr[0].released) {
            os << -1;
            return;
        }
        static Train train;
        trainData.SingleRead(train, trainAddr[0].addr);
        static seatsType dailyTrains[trainNum_Max];
        static seatsType dailyTrain;
        for (int i = 0; i < train.staNum - 1; ++i)
            dailyTrain[i] = train.seatNum;
        int dates = train.saleDate.second - train.saleDate.first + 1;
        for (int i = 0; i < dates; ++i)
            dailyTrains[i] = dailyTrain;
        int seatAddr = dailyTrainData.RangeAppend(dailyTrains, dates);
        dailyTrainMap.Insert(_i, DailyTrainAddr{trainAddr[0].addr, seatAddr, train.saleDate});
        Clock clock = train.startTime;
        int time = 0;
        for (int i = 0; i < train.staNum; ++i) {
            int stopoverTime = (i == 0 || i == train.staNum - 1 ? 0 : train.stopoverTimes[i - 1]);
            Station station{_i, clock, stopoverTime, train.prices[i], time};
            staData.Insert(baihua::hash(train.stations[i]), station);
            if (i < train.staNum - 1) {
                clock.add(train.travelTimes[i] + stopoverTime);
                time += train.travelTimes[i] + stopoverTime;
            }
        }
        os << 0;
    }

    void TrainManager::query_train(std::ostream &os, const ull &_i, const Date &_d) {
        auto trainAddr = trainMap.Find(_i);
        if (trainAddr.empty()) {
            os << -1;
            return;
        }
        static Train train;
        trainData.SingleRead(train, trainAddr[0].addr);

        if (!trainAddr[0].released) {
            for (int i = 0; i < )
        } else {
            auto dailyTrainAddr = dailyTrainMap.Find(DailyTrainIndex{_i, _d});
        }
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

    int TrainManager::read_seats(seatsType &seats, const ull &_i, const Date &_d) {
        auto dailyTrainAddr = dailyTrainMap.Find(_i);
        int addr = dailyTrainAddr[0].seatAddr + (_d - dailyTrainAddr[0].saleDate.first);
        dailyTrainData.SingleRead(seats, addr);
        return addr;
    }

}