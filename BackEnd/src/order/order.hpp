#ifndef TICKET_SYSTEM_ORDER_HPP
#define TICKET_SYSTEM_ORDER_HPP

#include "../db/bpt.hpp"
#include "../typebase.hpp"
#include "../lib/STLite/utility.hpp"

namespace ticket {

    struct OrderAddr {
        int timeTag;
        int addr;

    };

    struct Order {
        int status;
        trainIDType trainID;
        Date startDate;
        ftType from;
        ftType to;
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
                        int price, const ftType &_f, const ftType &_t, baihua::pair<int, int> staNo, int timeTag);

        void query_order(std::ostream &os, const ull &_u);

        baihua::pair<bool, Order> refund_ticket(std::ostream &os, const ull &_u, int _n);

        void push_queue(seatsType &seats, const Order &order);

        void clear();

    };

}


#endif //TICKET_SYSTEM_ORDER_HPP
