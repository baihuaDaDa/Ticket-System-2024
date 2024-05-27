#include "train.hpp"

namespace ticket {

    int CmpTrainAddr(const TrainAddr &lhs, const TrainAddr &rhs) {

    }

    int CmpDailyTrainAddr(const DailyTrainAddr &lhs, const DailyTrainAddr &rhs) {

    }

    int CmpStation(const Station &lhs, const Station &rhs) {

    }

    void Train::print_train(std::ostream &os, int ind, const Date &startDate) {
        Time arriveTime = Time{startDate, startClock} + travelTimes[ind];
        os << stations[ind] << ' ';
        if (ind == 0) os << "xx-xx xx:xx -> " << arriveTime;
        else if (ind == staNum - 1) os << arriveTime << " -> xx-xx xx:xx";
        else os << arriveTime << " -> " << arriveTime + stopoverTimes[ind];
        os << ' ' << prices[ind];
    }

    void TrainManager::add_train(std::ostream &os, const ull &_i, int _n, int _m, const stationsType &_s,
                                 const pricesType &_p, const Clock &_x, const ttsType &_t, const oType &_o,
                                 const baihua::pair<Date, Date> &_d, char _y) {
        auto trainAddr = trainMap.Find(_i);
        if (!trainAddr.empty()) {
            os << -1;
            return;
        }
        static hashStaType __s;
        static ttsType __t;
        static pricesType __p;
        __t[0] = __p[0] = 0;
        for (int i = 1; i < _n; ++i) {
            __t[i] = __t[i - 1] + _t[i - 1] + _o[i - 1];
            __p[i] = __p[i - 1] + _p[i - 1];
            __s[i] = baihua::hash(_s[i]);
        }
        Train newTrain{_n, _s, __s, _m, __p, _x, __t, _o, _d, _y};
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
        Clock &clock = train.startClock;
        for (int i = 0; i < train.staNum; ++i) {
            Station station{_i, i, _add(clock, train.travelTimes[i]).first, train.stopoverTimes[i],
                            train.prices[i], train.travelTimes[i]};
            staData.Insert(baihua::hash(train.stations[i]), station);
        }
        os << 0;
    }

    void TrainManager::query_train(std::ostream &os, const ull &_i, const Date &_d, const trainIDType &trainID) {
        auto trainAddr = trainMap.Find(_i);
        if (trainAddr.empty()) {
            os << -1;
            return;
        }
        static Train train;
        trainData.SingleRead(train, trainAddr[0].addr);
        os << trainID << ' ' << train.type << '\n';
        if (!trainAddr[0].released) {
            for (int i = 0; i < train.staNum; ++i) {
                train.print_train(os, i, _d);
                os << ' ' << (i != train.staNum - 1 ? train.seatNum : 'x');
                if (i != train.staNum - 1) os << '\n';
            }
        } else {
            auto dailyTrainAddr = dailyTrainMap.Find(_i);
            static seatsType dailyTrain;
            dailyTrainData.SingleRead(dailyTrain, dailyTrainAddr[0].seatAddr + (_d - dailyTrainAddr[0].saleDate.first));
            for (int i = 0; i < train.staNum; ++i) {
                train.print_train(os, i, _d);
                os << ' ' << (i != train.staNum - 1 ? dailyTrain[i] : 'x');
                if (i != train.staNum - 1) os << '\n';
            }
        }
    }

    void TrainManager::query_ticket(std::ostream &os, const ull &_s, const ull &_t, const Date &_d, bool _p) {

    }

    void TrainManager::query_transfer(std::ostream &os, const ull &_s, const ull &_t, const Date &_d, bool _p) {

    }

    Ticket TrainManager::buy_ticket_if_success(std::ostream &os, const ull &_i, const Date &_d, int _n, const ull &_f,
                                             const ull &_t, bool _q) {
        Ticket ret(_d);
        auto dailyTrainAddr = dailyTrainMap.Find(_i);
        if (dailyTrainAddr.empty()) {
            os << -1;
            return ret;
        }
        static Train train;
        trainData.SingleRead(train, dailyTrainAddr[0].trainAddr);
        int start = -1, end = -1;
        for (int i = 0; i < train.staNum; ++i) {
            if (start == -1) {
                if (train.hashStations[i] == _f) start = i;
            } else if (end == -1 && train.hashStations[i] == _t) end = i;
        }
        if (start == -1 || end == -1) {
            os << -1;
            return ret;
        }
        ret.staNo = {start, end};
        ret.success_and_queue.first = true;
        ret.startDate.minus(_add(train.startClock, train.travelTimes[start]).second);
        Time startTime{ret.startDate, train.startClock};
        ret.from = startTime + train.travelTimes[start];
        ret.to = startTime + train.travelTimes[end];
        static seatsType dailyTrain;
        int addr = dailyTrainAddr[0].seatAddr + (ret.startDate - dailyTrainAddr[0].saleDate.first);
        dailyTrainData.SingleRead(dailyTrain, addr);
        for (int i = start; i < end; ++i)
            if (dailyTrain[i] < _n) {
                ret.success_and_queue.second = true;
                break;
            }
        if (ret.success_and_queue.second) {
            if (_q) {
                os << "queue";
                return ret;
            } else {
                ret.success_and_queue = {false, false};
                os << -1;
                return ret;
            }
        } else {
            for (int i = start; i < end; ++i)
                dailyTrain[i] -= _n;
            dailyTrainData.SingleUpdate(dailyTrain, addr);
            os << _n * (train.prices[end] - train.prices[start]);
            return ret;
        }
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