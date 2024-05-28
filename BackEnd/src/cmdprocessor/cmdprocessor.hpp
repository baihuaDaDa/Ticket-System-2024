#ifndef TICKET_SYSTEM_CMDPROCESSOR_HPP
#define TICKET_SYSTEM_CMDPROCESSOR_HPP

#include <iostream>
#include <sstream>
#include "../lib/mytools/mytools.hpp"
#include "../typebase.hpp"

namespace ticket {

    const std::string CMD[cmdNum_Max] = {"add_user", "login", "logout", "query_profile", "modify_profile", "add_train",
                                         "delete_train", "release_train", "query_train", "query_ticket",
                                         "query_transfer", "buy_ticket", "query_order", "refund_ticket", "clean",
                                         "exit"};

    struct ArgSet {
        int cmdNo, argNum, timeTag;
        std::string args[argNum_Max];
        bool argsList[argNum_Max];
        ArgSet() : cmdNo(-1), argNum(0), timeTag(0), args() {}
        void initialize() {
            for (int i = 0; i < argNum_Max; ++i)
                argsList[i] = false;
        }
    };

    void parse(const std::string &cmd, ArgSet &argSet);

}

#endif //TICKET_SYSTEM_CMDPROCESSOR_HPP