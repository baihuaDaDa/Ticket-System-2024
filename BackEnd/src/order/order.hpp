#ifndef TICKET_SYSTEM_ORDER_HPP
#define TICKET_SYSTEM_ORDER_HPP

#include "../db/bpt.hpp"
#include "../typebase.hpp"
#include "../time.hpp"
#include "../lib/STLite/utility.hpp"
#include "../lib/mytools.hpp"

namespace ticket {

    struct OrderAddr {
        int timeTag;
        int addr;

    };

    struct Order {
        int status;
        trainIDType trainID;
        Date startDate;
        baihua::pair<staNameType, Time> from;
        baihua::pair<staNameType, Time> to;
        baihua::pair<int, int> staNo;
        int price;
        int num;

        friend std::ostream &operator<<(std::ostream &os, const Order &order);

    };

    struct QueueOrderIndex {
        ull trainID;
        Date date;

    };

    struct QueueOrderAddr {
        int timeTag;
        int addr;

    };

    int CmpOrderIndex(const OrderAddr &lhs, const OrderAddr &rhs);

    int CmpQueueOrderIndex(const QueueOrderIndex &lhs, const QueueOrderIndex &rhs);

    int CmpQueueOrder(const QueueOrderAddr &lhs, const QueueOrderAddr &rhs);

    class OrderManager {
    private:
        baihua::BPT<ull, OrderAddr, baihua::CmpUll, CmpOrderIndex> orderMap;
        baihua::BPT<QueueOrderIndex, QueueOrderAddr, CmpQueueOrderIndex, CmpQueueOrder> queueOrderMap;
        baihua::Database<Order> orderData;

    public:
        explicit OrderManager(const std::string &filename);

        void buy_ticket(bool queue, const ull &_u, const trainIDType &_i, const Date &startDate, int _n,
                        int prices, const baihua::pair<staNameType, Time> &_f,
                        const baihua::pair<staNameType, Time> &_t, baihua::pair<int, int> staNo,
                        int timeTag);

        void query_order(std::ostream &os, const ull &_u);

        bool refund_ticket(std::ostream &os, const ull &_u, int _n, seatsType &seats);

    };

}


#endif //TICKET_SYSTEM_ORDER_HPP
