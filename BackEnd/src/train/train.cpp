#include "train.hpp"

namespace ticket {

    // TODO 有些比较函数的第一个value就可以确定顺序，可以不用比较后面的value
    int CmpTrainAddr(const TrainAddr &lhs, const TrainAddr &rhs) {
        return baihua::CmpInt(lhs.addr, rhs.addr);
    }

    int CmpDailyTrainAddr(const DailyTrainAddr &lhs, const DailyTrainAddr &rhs) {
        return baihua::CmpInt(lhs.seatAddr, rhs.seatAddr);
    }

    int CmpStation(const Station &lhs, const Station &rhs) {
        return baihua::CmpUll(lhs.hashTrainID, rhs.hashTrainID);
    }

    int CmpTicketTime(const Ticket &lhs, const Ticket &rhs) {
        int flag = baihua::CmpInt(lhs.time, rhs.time);
        if (flag != 0) return flag;
        else return baihua::CmpStr(lhs.trainID, rhs.trainID);
    }

    int CmpTicketCost(const Ticket &lhs, const Ticket &rhs) {
        int flag = baihua::CmpInt(lhs.price, rhs.price);
        if (flag != 0) return flag;
        else return baihua::CmpStr(lhs.trainID, rhs.trainID);
    }

    int CmpTransferTime(const Transfer &lhs, const Transfer &rhs) {
        int flag = baihua::CmpInt(lhs.time, rhs.time);
        if (flag != 0) return flag;
        else if ((flag = baihua::CmpInt(lhs.first.price + lhs.second.price, rhs.first.price + rhs.second.price)) != 0)
            return flag;
        else if ((flag = baihua::CmpStr(lhs.first.trainID, rhs.first.trainID)) != 0) return flag;
        else return baihua::CmpStr(lhs.second.trainID, rhs.second.trainID);
    }

    int CmpTransferCost(const Transfer &lhs, const Transfer &rhs) {
        int flag = baihua::CmpInt(lhs.first.price + lhs.second.price, rhs.first.price + rhs.second.price);
        if (flag != 0) return flag;
        else if ((flag = baihua::CmpInt(lhs.time, rhs.time)) != 0)
            return flag;
        else if ((flag = baihua::CmpStr(lhs.first.trainID, rhs.first.trainID)) != 0) return flag;
        else return baihua::CmpStr(lhs.second.trainID, rhs.second.trainID);    }

    void Train::print_train(std::ostream &os, int ind, const Date &startDate) {
        Time arriveTime = Time{startDate, startClock} + travelTimes[ind];
        os << stations[ind] << ' ';
        if (ind == 0) os << "xx-xx xx:xx -> " << arriveTime;
        else if (ind == staNum - 1) os << arriveTime << " -> xx-xx xx:xx";
        else os << arriveTime << " -> " << arriveTime + stopoverTimes[ind];
        os << ' ' << prices[ind];
    }

    std::ostream &operator<<(std::ostream &os, const Ticket &ticket) {
        os << ticket.trainID << ' ' << ticket.from.first << ' ' << ticket.from.second << " -> " << ticket.to.first
           << ticket.to.second << ticket.price << ' ' << ticket.seatNum;
    }

    std::ostream &operator<<(std::ostream &os, const Transfer &transfer) {
        os << transfer.first << '\n' << transfer.second;
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

    void TrainManager::release_train(std::ostream &os, const ull &_i, const trainIDType &trainID) {
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
            Station station{_i, trainID, i, train.saleDate, _add(clock, train.travelTimes[i]).first,
                            train.stopoverTimes[i],
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

    void TrainManager::query_ticket(std::ostream &os, const ull &_s, const staNameType &start, const ull &_t,
                                    const staNameType &to, const Date &_d, bool _p) {
        auto sStations = staData.Find(_s);
        auto tStations = staData.Find(_t);
        baihua::vector<Ticket> tickets;
        int sp = 0, tp = 0;
        while (sp < sStations.size() && tp < tStations.size()) {
            Station &ss = sStations[sp], &ts = tStations[tp];
            int flag = CmpStation(ss, ts);
            if (flag == 1) ++tp;
            if (flag == -1) ++sp;
            if (flag == 0) {
                if (ss.staNo < ts.staNo) {
                    Date startDate = _minus(_d, _minus(ss.arriveClock, ss.time).second);
                    if (CmpDate(startDate, ss.saleDate.first) >= 0 &&
                        CmpDate(startDate, ss.saleDate.second) <= 0) {
                        Ticket ticket{ss.trainID, {start, Time{_d, ss.arriveClock} + ss.stopoverTime},
                                      {to, Time{}}, ts.price - ss.price, 0, 0};
                        ticket.time = ts.time - ss.time - ss.stopoverTime;
                        ticket.to.second = ticket.from.second + ticket.time;
                        auto dailyTrainAddr = dailyTrainMap.Find(ss.hashTrainID);
                        static seatsType dailyTrain;
                        dailyTrainData.SingleRead(dailyTrain, dailyTrainAddr[0].seatAddr +
                                                              (startDate - dailyTrainAddr[0].saleDate.first));
                        int seatNum = 1e5 + 1;
                        for (int i = ss.staNo; i < ts.staNo; ++i)
                            seatNum = std::min(seatNum, dailyTrain[i]);
                        ticket.seatNum = seatNum;
                        tickets.push_back(ticket);
                    }
                }
                ++sp;
                ++tp;
            }
        }
        os << tickets.size() << '\n';
        if (_p) baihua::sort(tickets.data(), tickets.data() + tickets.size(), CmpTicketTime);
        else baihua::sort(tickets.data(), tickets.data() + tickets.size(), CmpTicketCost);
        for (int i = 0; i < tickets.size(); ++i) {
            os << tickets[i];
            if (i != tickets.size() - 1) os << '\n';
        }
    }

    void TrainManager::query_transfer(std::ostream &os, const ull &_s, const staNameType &start, const ull &_t,
                                      const staNameType &to, const Date &_d, bool _p) {
        auto sStations = staData.Find(_s);
        auto tStations = staData.Find(_t);
        bool exist = false;
        static Transfer transfer;
        for (auto &ss: sStations) {
            Date sStartDate = _minus(_d, _minus(ss.arriveClock, ss.time).second);
            if (CmpDate(sStartDate, ss.saleDate.first) >= 0 &&
                CmpDate(sStartDate, ss.saleDate.second) <= 0) {
                auto sDailyTrainAddr = dailyTrainMap.Find(ss.hashTrainID);
                static seatsType sDailyTrain;
                dailyTrainData.SingleRead(sDailyTrain, sDailyTrainAddr[0].seatAddr +
                                                       (sStartDate - sDailyTrainAddr[0].saleDate.first));
                static Train sTrain;
                trainData.SingleRead(sTrain, sDailyTrainAddr[0].trainAddr);
                Time sStartTime{sStartDate, sTrain.startClock};
                ftType firstFrom{start, Time{_d, ss.arriveClock} + sTrain.stopoverTimes[ss.staNo]};
                for (auto &ts: tStations) {
                    if (ts.hashTrainID != ss.hashTrainID) {
                        auto trainAddr = trainMap.Find(ts.hashTrainID);
                        static Train tTrain;
                        trainData.SingleRead(tTrain, trainAddr[0].addr);
                        for (int i = ss.staNo + 1; i < sTrain.staNum; ++i) {
                            for (int j = 0; j < ts.staNo; ++j) {
                                if (sTrain.hashStations[i] == tTrain.hashStations[j]) {
                                    Time sTranArriveTime{sStartTime + sTrain.travelTimes[i]};
                                    auto result = _minus(ts.arriveClock,
                                                         ts.time - tTrain.travelTimes[j] - tTrain.stopoverTimes[j]);
                                    Time tTranStartTime{sTranArriveTime.date, result.first};
                                    if (CmpClock(sTranArriveTime.clock, tTranStartTime.clock) == 1)
                                        tTranStartTime.date.add(1);
                                    Date tStartDate = _minus(tTranStartTime.date, _minus(tTranStartTime.clock,
                                                                                         tTrain.travelTimes[j] +
                                                                                         tTrain.stopoverTimes[j]).second);
                                    if (CmpDate(tStartDate, ts.saleDate.first) >= 0 &&
                                        CmpDate(tStartDate, ts.saleDate.second) <= 0) {
                                        Time tArriveTime{_add(tTranStartTime.date, result.second), ts.arriveClock};
                                        Ticket first{ss.trainID, firstFrom, {sTrain.stations[i], sTranArriveTime},
                                                     sTrain.prices[i] - ss.price, 100001, 0};
                                        Ticket second{ts.trainID, {tTrain.stations[j], tTranStartTime}, {to, tArriveTime}, ts.price - tTrain.prices[j], 100001, 0};
                                        Transfer newTransfer{first, second, tArriveTime - firstFrom.second};
                                        auto tDailyTrainAddr = dailyTrainMap.Find(ts.hashTrainID);
                                        static seatsType tDailyTrain;
                                        dailyTrainData.SingleRead(tDailyTrain, tDailyTrainAddr[0].seatAddr +
                                                                               (tStartDate - tDailyTrainAddr[0].saleDate.first));
                                        for (int k = ss.staNo; k < i; ++k)
                                            first.seatNum = std::min(first.seatNum, sDailyTrain[k]);
                                        for (int k = j; k < ts.staNo; ++k)
                                            second.seatNum = std::min(second.seatNum, tDailyTrain[k]);
                                        if (exist) {
                                            if (_p && CmpTransferTime(transfer, newTransfer) == -1)
                                                transfer = newTransfer;
                                            else if (!_p && CmpTransferCost(transfer, newTransfer) == -1)
                                                transfer = newTransfer;
                                        } else {
                                            transfer = newTransfer;
                                            exist = true;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        if (exist) os << transfer;
        else os << 0;
    }

    TicketOrder TrainManager::buy_ticket_if_success(std::ostream &os, const ull &_i, const Date &_d, int _n,
                                                    const ull &_f, const ull &_t, bool _q) {
        TicketOrder ret(_d);
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
        ret.startDate.minus(_add(train.startClock, train.travelTimes[start]).second);
        if (CmpDate(ret.startDate, dailyTrainAddr[0].saleDate.first) == -1 ||
            CmpDate(ret.startDate, dailyTrainAddr[0].saleDate.second) == 1) {
            os << -1;
            return ret;
        }
        ret.staNo = {start, end};
        ret.success_and_queue.first = true;
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