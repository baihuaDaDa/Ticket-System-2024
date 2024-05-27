#include "order.hpp"

namespace ticket {

    // TODO timeTag在重启系统之后会重置吗？如果会，就不能用timeTag来表示订单先后顺序

    // 按orderNo降序
    int CmpOrderIndex(const OrderAddr &lhs, const OrderAddr &rhs) {
        if (lhs.timeTag != rhs.timeTag) return -baihua::CmpInt(lhs.timeTag, rhs.timeTag);
        else return baihua::CmpInt(lhs.addr, rhs.addr);
    }

    int CmpQueueOrderIndex(const QueueOrderIndex &lhs, const QueueOrderIndex &rhs) {
        if (lhs.trainID != rhs.trainID) return baihua::CmpUll(lhs.trainID, rhs.trainID);
        else return CmpDate(lhs.date, rhs.date);
    }

    // 按timeTag升序
    int CmpQueueOrder(const QueueOrderAddr &lhs, const QueueOrderAddr &rhs) {
        if (lhs.timeTag != rhs.timeTag) return baihua::CmpInt(lhs.timeTag, rhs.timeTag);
        else return baihua::CmpInt(lhs.addr, rhs.addr);
    }

    std::ostream &operator<<(std::ostream &os, const Order &order) {
        os << '[' << STATUS[order.status] << ']' << ' ' << order.trainID << ' ' << order.from << ' ' << "->" << order.to
           << ' ' << order.price << ' ' << order.num;
        return os;
    }

    OrderManager::OrderManager(const std::string &filename) : orderMap(filename + "Map"),
                                                              queueOrderMap(filename + "QueueMap"),
                                                              orderData(filename + "Data") {
        if (!orderData.isFileExist()) orderData.initialize();
    }

    void OrderManager::buy_ticket(bool queue, const ull &_u, const trainIDType &_i, const Date &startDate, int _n,
                                  int price, const ftType &_f, const ftType &_t, baihua::pair<int, int> staNo,
                                  int timeTag) {
        Order newOrder{queue, _i, startDate, _f, _t, staNo, price, _n};
        int addr = orderData.SingleAppend(newOrder);
        orderMap.Insert(_u, OrderAddr{timeTag, addr});
        if (queue) queueOrderMap.Insert(QueueOrderIndex{baihua::hash(_i), startDate}, QueueOrderAddr{timeTag, addr});
    }

    void OrderManager::query_order(std::ostream &os, const ticket::ull &_u) {
        auto orderAddr = orderMap.Find(_u);
        static Order order;
        os << orderAddr.size() << '\n';
        for (int i = 0; i < orderAddr.size(); ++i) {
            orderData.SingleRead(order, orderAddr[i].addr);
            os << order;
            if (i < orderAddr.size() - 1) os << '\n';
        }
    }

    // TODO 已经退订的应该也算订单吧？
    bool OrderManager::refund_ticket(std::ostream &os, const ticket::ull &_u, int _n, seatsType &seats) {
        auto orderIndex = orderMap.Find(_u);
        if (orderIndex.size() < _n) {
            os << -1;
            return false;
        }
        static Order order;
        orderData.SingleRead(order, orderIndex[_n - 1].addr);
        if (order.status == 0) {
            // TODO 可用线段树优化
            order.status = 2;
            orderData.SingleUpdate(order, orderIndex[_n - 1].addr);
            for (int i = order.staNo.first; i <= order.staNo.second; ++i)
                seats[i] += order.num;
            QueueOrderIndex queueOrderIndex{baihua::hash(order.trainID), order.startDate};
            auto queueOrderAddr = queueOrderMap.Find(queueOrderIndex);
            static Order queueOrder;
            for (auto &elem: queueOrderAddr) {
                bool success = true;
                orderData.SingleRead(queueOrder, elem.addr);
                for (int i = queueOrder.staNo.first; i <= queueOrder.staNo.second; ++i)
                    if (seats[i] < queueOrder.num) {
                        success = false;
                        break;
                    }
                if (success) {
                    for (int i = queueOrder.staNo.first; i <= queueOrder.staNo.second; ++i)
                        seats[i] -= queueOrder.num;
                    queueOrder.status = 0;
                    orderData.SingleUpdate(queueOrder, elem.addr);
                    queueOrderMap.Delete(queueOrderIndex, elem);
                }
            }
            os << 0;
            return true;
        } else if (order.status == 1) {
            order.status = 2;
            orderData.SingleUpdate(order, orderIndex[_n - 1].addr);
            QueueOrderIndex queueOrderIndex{baihua::hash(order.trainID), order.startDate};
            auto queueOrderAddr = queueOrderMap.Find(queueOrderIndex);
            for (auto elem: queueOrderAddr)
                if (elem.timeTag == orderIndex[_n - 1].timeTag) {
                    queueOrderMap.Delete(queueOrderIndex, elem);
                    break;
                }
            os << 0;
            return false;
        } else {
            os << -1;
            return false;
        }
    }

}