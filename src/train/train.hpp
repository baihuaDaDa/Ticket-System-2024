#ifndef INC_7_TICKET_SYSTEM_TRAIN_HPP
#define INC_7_TICKET_SYSTEM_TRAIN_HPP

#include "../db/bpt.hpp"
#include "../typebase.hpp"
#include "../lib/STLite/utility.hpp"
#include "../lib/STLite/algorithm.hpp"

namespace ticket {

    struct TrainAddr {
        int addr;
        bool released;

    };

    struct Train {
        int staNum;
        stationsType stations;
        hashStaType hashStations; // TODO maybe unnecessary
        int seatNum;
        pricesType prices; // from the start station
        Clock startClock;
        ttsType travelTimes; // from the start station
        oType stopoverTimes;
        baihua::pair<Date, Date> saleDate;
        char type;

        void print_train(std::ostream &os, int ind, const Date &startDate);

    };

    // save the head index of a sequence of daily trains
    struct DailyTrainAddr {
        int trainAddr;
        int seatAddr;
        baihua::pair<Date, Date> saleDate; //

    };

    struct Station {
        ull hashTrainID;
        trainIDType trainID;
        int staNo;
        baihua::pair<Date, Date> saleDate;
        Clock arriveClock;
        int stopoverTime;
        int price; // from the start to this station
        int time; // from the start station to this one

    };

    struct Ticket {
        trainIDType trainID;
        ftType from;
        ftType to;
        int price;
        int seatNum;
        int time;

        friend std::ostream &operator<<(std::ostream &os, const Ticket &ticket);

    };

    struct Transfer {
        Ticket first;
        Ticket second;
        int time;

        friend std::ostream &operator<<(std::ostream &os, const Transfer &transfer);

    };

    struct TicketOrder {
        baihua::pair<bool, bool> success_and_queue;
        Date startDate;
        Time from;
        Time to;
        baihua::pair<int, int> staNo;
        int price; // total

        TicketOrder(const Date &date) : success_and_queue(false, false), startDate(date), from(), to(), staNo(), price() {}

    };

    int CmpTrainAddr(const TrainAddr &lhs, const TrainAddr &rhs);

    int CmpDailyTrainAddr(const DailyTrainAddr &lhs, const DailyTrainAddr &rhs);

    int CmpStation(const Station &lhs, const Station &rhs);

    int CmpTicketTime(const Ticket &lhs, const Ticket &rhs);

    int CmpTicketCost(const Ticket &lhs, const Ticket &rhs);

    int CmpTransferTime(const Transfer &lhs, const Transfer &rhs);

    int CmpTransferCost(const Transfer &lhs, const Transfer &rhs);

    class TrainManager {
    private:
        baihua::BPT<ull, TrainAddr, baihua::CmpUll, CmpTrainAddr> trainMap;
        baihua::Database<Train> trainData;
        baihua::BPT<ull, DailyTrainAddr, baihua::CmpUll, CmpDailyTrainAddr> dailyTrainMap;
        baihua::Database<seatsType> dailyTrainData;
        baihua::BPT<ull, Station, baihua::CmpUll, CmpStation> staData;

    public:
        TrainManager(const std::string &filename);

        void add_train(std::ostream &os, const ull &_i, int _n, int _m, const stationsType &_s,
                       const pricesType &_p, const Clock &_x, const ttsType &_t, const oType &_o,
                       const baihua::pair<Date, Date> &_d, char _y);

        void delete_train(std::ostream &os, const ull &_i);

        void release_train(std::ostream &os, const ull &_i, const trainIDType &trainID);

        void query_train(std::ostream &os, const ull &_i, const Date &_d, const trainIDType &trainID);

        void query_ticket(std::ostream &os, const ull &_s, const staNameType &start, const ull &_t,
                          const staNameType &to, const Date &_d, bool _p);

        void query_transfer(std::ostream &os, const ull &_s, const staNameType &start, const ull &_t,
                            const staNameType &to, const Date &_d, bool _p);

        TicketOrder buy_ticket_if_success(std::ostream &os, const ull &_i, const Date &_d,
                                          int _n, const ull &_f, const ull &_t, bool _q);

        void write_seats(int addr, seatsType &seats);

        int read_seats(seatsType &seats, const ull &_i, const Date &_d);

        void clear();

    };

}


#endif //INC_7_TICKET_SYSTEM_TRAIN_HPP
