#include "order.hpp"

namespace ticket {

    // 按orderNo降序
    int CmpOrderIndex(const OrderIndex &lhs, const OrderIndex &rhs) {
        if (lhs.orderNo != rhs.orderNo) return -baihua::CmpInt(lhs.orderNo, rhs.orderNo);
        else return baihua::CmpInt(lhs.addr, rhs.addr);
    }

    int CmpQueueOrderIndex(const QueueOrderIndex &lhs, const QueueOrderIndex &rhs) {
        if (lhs.trainID != rhs.trainID) return baihua::CmpUll(lhs.trainID, rhs.trainID);
        else return CmpDate(lhs.date, rhs.date);
    }

    // 按timeTag升序
    int CmpQueueOrder(const QueueOrder &lhs, const QueueOrder &rhs) {
        if (lhs.timeTag != rhs.timeTag) return baihua::CmpInt(lhs.timeTag, rhs.timeTag);
        else return baihua::CmpInt(lhs.addr, rhs.addr);
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
        if (order.status == 0) {
            order.status = 2;
            orderData.SingleUpdate(order, orderIndex[_n - 1].addr);
            QueueOrderIndex queueOrderIndex{baihua::hash(order.trainID), order.startTime};
        } else if (order.status == 1) {
            order.status = 2;
            orderData.SingleUpdate(order, orderIndex[_n - 1].addr);
            QueueOrderIndex queueOrderIndex{baihua::hash(order.trainID), order.startTime};

            auto queueOrder = queueOrderMap.Find(QueueOrderIndex());
        } else {
            os << -1;
            ret.first.first = false;
            return ret;
        }
    }

}