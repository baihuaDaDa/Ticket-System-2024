#ifndef TICKET_SYSTEM_ORDER_HPP
#define TICKET_SYSTEM_ORDER_HPP

#include "../db/bpt.hpp"
#include "../typebase.hpp"
#include "../time.hpp"
#include "../lib/STLite/utility.hpp"
#include "../lib/mytools.hpp"

namespace ticket {

    struct OrderIndex {
        int addr;
        int orderNo;

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
        ull trainID;
        Date date;

    };

    struct QueueOrder {
        int timeTag;
        int addr;

    };

    struct ModifyOrder {
        staNameType from;
        staNameType to;
        int num;

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
        void buy_ticket(bool queue, const ull &_u, const trainIDType &_i, const Date &_d, const int _n,
                        const staNameType &_f, const staNameType &_t);

        void query_order(std::ostream &os, const ull &_u);

        baihua::pair<baihua::pair<bool, ModifyOrder>, baihua::pair<bool, ModifyOrder>>
        refund_ticket(std::ostream &os, const ull &_u, const int _n);

    };

}


#endif //TICKET_SYSTEM_ORDER_HPP
