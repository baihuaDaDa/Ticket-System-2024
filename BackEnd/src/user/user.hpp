#ifndef INC_7_TICKET_SYSTEM_USER_HPP
#define INC_7_TICKET_SYSTEM_USER_HPP

#include "../cmdprocessor.hpp"

namespace ticket {

    struct User {
        pwdType pwd;
        uNameType uName;
        mailType mail;
        int privilege;

    };

    class UserManager {

    };

}



#endif //INC_7_TICKET_SYSTEM_USER_HPP
