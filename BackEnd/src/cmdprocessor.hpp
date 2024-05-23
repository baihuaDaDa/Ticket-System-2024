#ifndef TICKET_SYSTEM_CMDPROCESSOR_HPP
#define TICKET_SYSTEM_CMDPROCESSOR_HPP

#include <iostream>
#include <sstream>
#include "../lib/mytools.hpp"
#include "../lib/constantLengthString.hpp"

namespace ticket {

    constexpr int argNum_Max = 26, uid_Max = 21, pwd_Max = 31, uName_Max = 21, mail_Max = 31, trainID_Max = 21, staName_Max = 41, staNum_Max = 101, cmdNum_Max = 16;

    typedef baihua::ConstLenStr<uid_Max> uidType;
    typedef baihua::ConstLenStr<pwd_Max> pwdType;
    typedef baihua::ConstLenStr<uName_Max> uNameType;
    typedef baihua::ConstLenStr<mail_Max> mailType;
    typedef baihua::ConstLenStr<trainID_Max> trainIDType;
    typedef baihua::ConstLenStr<staName_Max> staNameType;

    const std::string CMD[cmdNum_Max] = {"add_user", "login", "logout", "query_profile", "modify_profile", "add_train",
                                         "delete_train", "release_train", "query_train", "query_ticket",
                                         "query_transfer", "buy_ticket", "query_order", "refund_ticket", "clean",
                                         "exit"};

    struct ArgSet {
        int cmdNo, argNum, timeTag;
        std::string args[argNum_Max];
        ArgSet() : cmdNo(-1), argNum(0), timeTag(0), args() {}
    };

    ArgSet parse(const std::string &cmd) {
        ArgSet argSet;
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
        }
        argSet.argNum = argCnt;
        return argSet;
    }
}

#endif //TICKET_SYSTEM_CMDPROCESSOR_HPP
