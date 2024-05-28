#ifndef TICKET_SYSTEM_SYSTEM_HPP
#define TICKET_SYSTEM_SYSTEM_HPP

#include "../user/user.hpp"
#include "../train/train.hpp"
#include "../order/order.hpp"
#include "../cmdprocessor.hpp"

namespace ticket {

    class System {
    private:
        UserManager userManager;
        OrderManager orderManager;
        TrainManager trainManager;

    public:
        System(const std::string &filename);

        void response(std::ostream &os, const std::string &buffer);

        void add_user(std::ostream &os, const ArgSet &argSet);

        void login(std::ostream &os, const ArgSet &argSet);

        void logout(std::ostream &os, const ArgSet &argSet);

        void query_profile(std::ostream &os, const ArgSet &argSet);

        void modify_profile(std::ostream &os, const ArgSet &argSet);

        void add_train(std::ostream &os, const ArgSet &argSet);

        void delete_train(std::ostream &os, const ArgSet &argSet);

        void release_train(std::ostream &os, const ArgSet &argSet);

        void query_train(std::ostream &os, const ArgSet &argSet);

        void query_ticket(std::ostream &os, const ArgSet &argSet);

        void query_transfer(std::ostream &os, const ArgSet &argSet);

        void buy_ticket(std::ostream &os, const ArgSet &argSet);

        void query_order(std::ostream &os, const ArgSet &argSet);

        void refund_ticket(std::ostream &os, const ArgSet &argSet);

        void exit(std::ostream &os, const ArgSet &argSet);

        void clean(std::ostream &os, const ArgSet &argSet);

    private:
        void (System::*interface[cmdNum_Max])(std::ostream &os, const ArgSet &argSet) = {&System::add_user,
                                                                                         &System::login,
                                                                                         &System::logout,
                                                                                         &System::query_profile,
                                                                                         &System::modify_profile,
                                                                                         &System::add_train,
                                                                                         &System::delete_train,
                                                                                         &System::release_train,
                                                                                         &System::query_train,
                                                                                         &System::query_ticket,
                                                                                         &System::query_transfer,
                                                                                         &System::buy_ticket,
                                                                                         &System::query_order,
                                                                                         &System::refund_ticket,
                                                                                         &System::clean,
                                                                                         &System::exit};

    };

}


#endif //TICKET_SYSTEM_SYSTEM_HPP
