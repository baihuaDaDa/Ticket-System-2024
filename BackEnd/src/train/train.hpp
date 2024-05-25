#ifndef INC_7_TICKET_SYSTEM_TRAIN_HPP
#define INC_7_TICKET_SYSTEM_TRAIN_HPP

#include "../db/bpt.hpp"
#include "../typebase.hpp"
#include "../time.hpp"
#include "../lib/STLite/utility.hpp"

namespace ticket {

    struct Train {
        int staNum;
        staNameType stations[staNum_Max];
        int seatNum;
        int prices[staNum_Max - 1];
        Clock startTime;
        int travelTimes[staNum_Max - 1];
        int stopoverTimes[staNum_Max - 2];
        baihua::pair<Date, Date> saleDate;
        char type;
        bool released;

    };

    struct DailyTrainIndex {
        ull trainID;
        Date startTime;

    };

    struct DailyTrainAddr {
        int trainAddr;
        int seatAddr;

    };

    struct DailyTrain {
        int seat[staNum_Max - 1];

    };

    struct Station {
        ull trainID;
        Clock arriveClock;
        int price; // from this station to the next
        int time; // from the start station to this one

    };

    int CmpDailyTrainIndex(const DailyTrainIndex &lhs, const DailyTrainIndex &rhs);

    int CmpDailyTrainAddr(const DailyTrainAddr &lhs, const DailyTrainAddr &rhs);

    int CmpStation(const Station &lhs, const Station &rhs);

    class TrainManager {
    private:
        baihua::BPT<ull, int, baihua::CmpUll, baihua::CmpInt> trainMap;
        baihua::Database<Train> trainData;
        baihua::BPT<DailyTrainIndex, DailyTrainAddr, CmpDailyTrainIndex, CmpDailyTrainAddr> dailyTrainMap;
        baihua::Database<DailyTrain> dailyTrainData;
        baihua::BPT<ull, Station, baihua::CmpUll, CmpStation> staData;

    public:
        void add_train(std::ostream &os, const ull &_i, const int _n, const int _m, const staNameType _s[],
                       const int _p[], const Clock &_x, const int _t[], const int _o[],
                       const baihua::pair<Date, Date> &_d, const char _y);

        void delete_train(const ull &_i);

        void release_train(const ull &_i);

        void query_train(const ull &_i, const Date &_d);

        void query_ticket(const ull &_s, const ull &_t, const Date &_d, const bool _p);

        void query_transfer(const ull &_s, const ull &_t, const Date &_d, const bool _p);

        bool buy_ticket_if_success(const ull &_i, const Date &_d, const int _n, const staNameType &_f,
                                   const staNameType &_t, const bool _q);

        void refund_ticket(const ull &_i, const Date &_d, const int _n_ref, const int _f_ref,
                           const int _t_ref, const int _n_que, const int _f_que,
                           const int _t_que);

    };

}


#endif //INC_7_TICKET_SYSTEM_TRAIN_HPP
