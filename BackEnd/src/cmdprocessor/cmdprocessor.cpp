#include "cmdprocessor.hpp"

namespace ticket {

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
            if (token == CMD[i]) {
                argSet.cmdNo = i;
                break;
            }
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