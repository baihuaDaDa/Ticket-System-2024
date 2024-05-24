#ifndef TICKET_SYSTEM_TYPEBASE_HPP
#define TICKET_SYSTEM_TYPEBASE_HPP

#include "../lib/constantLengthString.hpp"

namespace ticket {

    constexpr int argNum_Max = 26, uid_Max = 21, pwd_Max = 31, uName_Max = 21, mail_Max = 31, trainID_Max = 21, staName_Max = 41, staNum_Max = 101, cmdNum_Max = 16;

    using ull = unsigned long long;

    typedef baihua::ConstLenStr<uid_Max> uidType;
    typedef baihua::ConstLenStr<pwd_Max> pwdType;
    typedef baihua::ConstLenStr<uName_Max> uNameType;
    typedef baihua::ConstLenStr<mail_Max> mailType;
    typedef baihua::ConstLenStr<trainID_Max> trainIDType;
    typedef baihua::ConstLenStr<staName_Max> staNameType;

    constexpr std::string STATUS[3] = {"success", "pending", "refunded"};

}

#endif //TICKET_SYSTEM_TYPEBASE_HPP
