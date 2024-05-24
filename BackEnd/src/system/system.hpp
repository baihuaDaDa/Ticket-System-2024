#ifndef TICKET_SYSTEM_SYSTEM_HPP
#define TICKET_SYSTEM_SYSTEM_HPP

#include "../user/user.hpp"
#include "../train/train.hpp"
#include "../order/order.hpp"
#include "../cmdprocessor.hpp"

namespace ticket {

    class system {
    private:
        int timeTag;
        UserManager userManager;
        OrderManager orderManager;
        TrainManager trainManager;

    };

}


#endif //TICKET_SYSTEM_SYSTEM_HPP
