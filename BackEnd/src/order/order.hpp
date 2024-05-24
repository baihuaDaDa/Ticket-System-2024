#ifndef TICKET_SYSTEM_ORDER_HPP
#define TICKET_SYSTEM_ORDER_HPP

#include "../db/bpt.hpp"
#include "../typebase.hpp"
#include "../time.hpp"
#include "../lib/STLite/utility.hpp"

namespace ticket {

    struct OrderIndex {
        int timeTag;
        int addr;

    };

    struct Order {
        int status;
        trainIDType trainID;
        baihua::pair<staNameType, Time> from;
        baihua::pair<staNameType, Time> to;
        int price;
        int num;

    };

    struct QueueOrderIndex {
        int trainID;
        Date date;

    };

    struct QueueOrder {
        int timeTag;
        int addr;

    };

    int CmpOrderIndex(const OrderIndex &lhs, const OrderIndex &rhs);

    int CmpQueueOrderIndex(const QueueOrderIndex &lhs, const QueueOrderIndex &rhs);

    int CmpQueueOrder(const QueueOrder &lhs, const QueueOrder &rhs);

    class OrderManager {
    private:
        baihua::BPT<ull, OrderIndex, baihua::CmpUll, CmpOrderIndex> orderMap;
        baihua::BPT<QueueOrderIndex, QueueOrder, CmpQueueOrderIndex, CmpQueueOrder> queueOrderMap;
        baihua::Database<Order> orderData;

    public:

    };

}


#endif //TICKET_SYSTEM_ORDER_HPP
