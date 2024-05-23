#ifndef TICKET_SYSTEM_ORDER_HPP
#define TICKET_SYSTEM_ORDER_HPP

#include "../cmdprocessor.hpp"
#include "../time.hpp"
#include "../lib/STLite/utility.hpp"

namespace ticket {

    struct Order {
        enum Status {success, pending, refunded};
        trainIDType trainID;
        baihua::pair<staNameType, Time> from;
        baihua::pair<staNameType, Time> to;
        int price;
        int num;

    };

    class OrderManager {

    };

}


#endif //TICKET_SYSTEM_ORDER_HPP
