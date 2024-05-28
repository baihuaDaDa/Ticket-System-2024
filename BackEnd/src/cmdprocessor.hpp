#ifndef TICKET_SYSTEM_CMDPROCESSOR_HPP
#define TICKET_SYSTEM_CMDPROCESSOR_HPP

#include <iostream>
#include <sstream>
#include "../lib/mytools.hpp"
#include "typebase.hpp"

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

    void parse(const std::string &cmd, ArgSet &argSet) {
        argSet.initialize();
        int len = cmd.size();
        while (cmd[len - 1] == ' ' || cmd[len - 1] == '\t' || cmd[len - 1] == '\n' || cmd[len - 1] == '\r') --len;
        std::stringstream ss(cmd);
        std::string token;
        std::getline(ss, token, ' ');
        argSet.timeTag = baihua::string_to_int(token, 1, token.size() - 1);
        std::getline(ss, token, ' ');
        for (int i = 0; i < cmdNum_Max; ++i)
            if (token == CMD[i]) argSet.cmdNo = i;
        int argCnt = 0, argNo;
        while (std::getline(ss, token, ' ')) {
            ++argCnt;
            argNo = token[1] - 'a';
            std::getline(ss, token, ' ');
            argSet.args[argNo] = token;
            argSet.argsList[argNo] = true;
        }
        argSet.argNum = argCnt;
    }

}

#endif //TICKET_SYSTEM_CMDPROCESSOR_HPP