#ifndef INC_7_TICKET_SYSTEM_TRAIN_HPP
#define INC_7_TICKET_SYSTEM_TRAIN_HPP

#include "../db/bpt.hpp"
#include "../typebase.hpp"
#include "../time.hpp"
#include "../lib/STLite/utility.hpp"

namespace ticket {

    struct Train {
        int staNum;
        stationsType stations;
        int seatNum;
        pricesType prices;
        Clock startTime;
        ttsType travelTimes;
        oType stopoverTimes;
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
        baihua::Database<seatsType> dailyTrainData;
        baihua::BPT<ull, Station, baihua::CmpUll, CmpStation> staData;

    public:
        void add_train(std::ostream &os, const ull &_i, int _n, int _m, const stationsType &_s,
                       const pricesType &_p, const Clock &_x, const ttsType &_t, const oType &_o,
                       const baihua::pair<Date, Date> &_d, char _y);

        void delete_train(const ull &_i);

        void release_train(const ull &_i);

        void query_train(const ull &_i, const Date &_d);

        void query_ticket(const ull &_s, const ull &_t, const Date &_d, bool _p);

        void query_transfer(const ull &_s, const ull &_t, const Date &_d, bool _p);

        bool buy_ticket_if_success(const ull &_i, const Date &_d, int _n, const staNameType &_f,
                                   const staNameType &_t, bool _q);

        void write_seats(int addr, seatsType &seats);

        void read_seats(seatsType &seats, const ull &_i, const Date &_d);

    };

}


#endif //INC_7_TICKET_SYSTEM_TRAIN_HPP
