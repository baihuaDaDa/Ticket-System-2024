#include "order.hpp"

namespace ticket {

    int CmpOrderIndex(const OrderIndex &lhs, const OrderIndex &rhs) {

    }

    int CmpQueueOrderIndex(const QueueOrderIndex &lhs, const QueueOrderIndex &rhs) {

    }

    int CmpQueueOrder(const QueueOrder &lhs, const QueueOrder &rhs) {

    }

    void OrderManager::buy_ticket(bool queue, const ull &_u, const trainIDType &_i, const Date &_d, const int _n,
                                  const staNameType &_f, const staNameType &_t) {

    }

    void OrderManager::query_order(std::ostream &os, const ticket::ull &_u) {

    }

    // TODO 已经退订的应该也算订单吧？
    baihua::pair<baihua::pair<bool, ModifyOrder>, baihua::pair<bool, ModifyOrder>>
    OrderManager::refund_ticket(std::ostream &os, const ticket::ull &_u, const int _n) {
        baihua::pair<baihua::pair<bool, ModifyOrder>, baihua::pair<bool, ModifyOrder>> ret;
        auto orderIndex = orderMap.Find(_u);
        if (orderIndex.size() < _n) {
            os << -1;
            ret.first.first = false;
            return ret;
        }
        Order order;
        orderData.SingleRead(order, orderIndex[_n - 1].addr);
        switch (order.status) {
            case 0:

                break;
            case 1:
                order.status = 2;
                orderData.SingleUpdate(order, orderIndex[_n - 1].addr);
                QueueOrderIndex queueOrderIndex{baihua::hash(order.trainID), order.};
                auto queueOrder = queueOrderMap.Find(QueueOrderIndex());
                break;
            case 2:
                os << -1;
                ret.first.first = false;
                return ret;
        }
    }

}