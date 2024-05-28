#ifndef TICKET_SYSTEM_TYPEBASE_HPP
#define TICKET_SYSTEM_TYPEBASE_HPP

#include "time/time.hpp"
#include "../lib/mytools/mytools.hpp"
#include "../lib/constantLengthString.hpp"
#include "../lib/array.hpp"

namespace ticket {

    // 0-base
    constexpr int argNum_Max = 26, uid_Max = 21, pwd_Max = 31, uName_Max = 21, mail_Max = 31, trainID_Max = 21, staName_Max = 41, staNum_Max = 100, cmdNum_Max = 16, trainNum_Max = 92;

    using ull = unsigned long long;

    typedef baihua::ConstLenStr<uid_Max> uidType;
    typedef baihua::ConstLenStr<pwd_Max> pwdType;
    typedef baihua::ConstLenStr<uName_Max> uNameType;
    typedef baihua::ConstLenStr<mail_Max> mailType;
    typedef baihua::ConstLenStr<trainID_Max> trainIDType;
    typedef baihua::ConstLenStr<staName_Max> staNameType;

    constexpr std::string STATUS[3] = {"success", "pending", "refunded"};

    typedef baihua::Array<staNameType, staNum_Max> stationsType;
    typedef baihua::Array<ull, staNum_Max> hashStaType;
    typedef baihua::Array<int, staNum_Max> pricesType;
    typedef baihua::Array<int, staNum_Max> ttsType;
    typedef baihua::Array<int, staNum_Max> oType;
    typedef baihua::Array<int, staNum_Max - 1> seatsType;
    typedef baihua::pair<staNameType, Time> ftType;

}

#endif //TICKET_SYSTEM_TYPEBASE_HPP
