#ifndef INC_7_TICKET_SYSTEM_USER_HPP
#define INC_7_TICKET_SYSTEM_USER_HPP

#include "../db/bpt.hpp"
#include "../typebase.hpp"
#include "../lib/mytools.hpp"
#include "../lib/STLite/set.hpp"

namespace ticket {

    struct UserIndex {
        int addr;
        int privilege;

    };

    struct User {
        pwdType pwd;
        uNameType uName;
        mailType mail;

    };

    int CmpUserIndex(const UserIndex &lhs, const UserIndex &rhs);

    int CmpUser(const User &lhs, const User &rhs);

    class UserManager {
    private:
        baihua::BPT<ull, UserIndex, baihua::CmpUll, CmpUserIndex> userMap;
        baihua::Database<User, 1, 1> userData;
        baihua::map<ull, int> userList; // <username, privilege>
        int userNum;

    public:
        UserManager(const std::string &filename);

        ~UserManager();

        void add_user(std::ostream &os, const ull &_c, const ull &_u,
                      const pwdType &_p, const uNameType &_n, const mailType &_m, const int _g);

        void login(std::ostream &os, const ull &_u, const pwdType &_p);

        void logout(std::ostream &os, const ull &_u);

        void query_profile(std::ostream &os, const ull &_c, const ull &_u, const uidType &username);

        // _g == 11 if there is no _g input.
        void modify_profile(std::ostream &os, const ull &_c, const ull &_u, const baihua::pair<bool, pwdType> &_p,
                            const baihua::pair<bool, uNameType> &_n, const baihua::pair<bool, mailType> &_m,
                            const int _g);

    };

}


#endif //INC_7_TICKET_SYSTEM_USER_HPP
