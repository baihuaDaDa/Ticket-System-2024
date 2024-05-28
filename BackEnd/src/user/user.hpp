#ifndef INC_7_TICKET_SYSTEM_USER_HPP
#define INC_7_TICKET_SYSTEM_USER_HPP

#include "../db/bpt.hpp"
#include "../typebase.hpp"

namespace ticket {

    struct UserAddr {
        int addr;
        int privilege; // privilege也可以放在User里

    };

    struct User {
        pwdType pwd;
        uNameType uName;
        mailType mail;

    };

    int CmpUserIndex(const UserAddr &lhs, const UserAddr &rhs);

    int CmpUser(const User &lhs, const User &rhs);

    class UserManager {
    private:
        baihua::BPT<ull, UserAddr, baihua::CmpUll, CmpUserIndex> userMap;
        baihua::Database<User, 1, 1> userData;
        baihua::map<ull, int> userList; // <username, privilege>
        int userNum;

    public:
        explicit UserManager(const std::string &filename);

        ~UserManager();

        void add_user(std::ostream &os, const ull &_c, const ull &_u,
                      const pwdType &_p, const uNameType &_n, const mailType &_m, int _g);

        void login(std::ostream &os, const ull &_u, const pwdType &_p);

        void logout(std::ostream &os, const ull &_u);

        void query_profile(std::ostream &os, const ull &_c, const ull &_u, const uidType &username);

        void modify_profile(std::ostream &os, const ull &_c, const ull &_u, const uidType &username,
                            const baihua::pair<bool, pwdType> &_p, const baihua::pair<bool, uNameType> &_n,
                            const baihua::pair<bool, mailType> &_m, const baihua::pair<bool, int> &_g);

        bool if_login(const ull &_u);

        void clear();

    };

}


#endif //INC_7_TICKET_SYSTEM_USER_HPP
