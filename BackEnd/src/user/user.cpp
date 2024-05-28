#include "user.hpp"

namespace ticket {

    int CmpUser(const User &lhs, const User &rhs) {
        int flag = baihua::CmpStr(lhs.pwd, rhs.pwd);
        if (flag != 0) return flag;
        else if ((flag = baihua::CmpStr(lhs.uName, rhs.uName)) != 0) return flag;
        else return baihua::CmpStr(lhs.mail, rhs.mail);
    }

    int CmpUserIndex(const UserAddr &lhs, const UserAddr &rhs) {
        if (lhs.addr != rhs.addr) return baihua::CmpInt(lhs.addr, rhs.addr);
        else return baihua::CmpInt(lhs.privilege, rhs.privilege);
    }

    UserManager::UserManager(const std::string &filename) : userMap(filename + "Map"), userData(filename + "Data.bin") {
        if (!userData.isFileExist()) {
            userData.initialize();
            userNum = 0;
        } else userData.ReadInfo(userNum, 1);
    }

    UserManager::~UserManager() {
        userData.WriteInfo(userNum, 1);
    }

    void UserManager::add_user(std::ostream &os, const ull &_c, const ull &_u,
                  const pwdType &_p, const uNameType &_n, const mailType &_m, int _g) {
        if (userNum != 0) {
            auto iter = userList.find(_c);
            if (iter == userList.end() || iter->second <= _g || !userMap.Find(_u).empty()) {
                os << -1;
                return;
            }
        }
        User newUser(_p, _n, _m);
        UserAddr newUserIndex(userData.SingleAppend(newUser), (userNum == 0 ? 10 : _g));
        userMap.Insert(_u, newUserIndex);
        ++userNum;
        os << 0;
    }

    void UserManager::login(std::ostream &os, const ull &_u, const pwdType &_p) {
        if (userList.find(_u) != userList.end()) {
            os << -1;
            return;
        }
        auto userIndex = userMap.Find(_u);
        if (userIndex.empty()) {
            os << -1;
            return;
        }
        static User user;
        userData.SingleRead(user, userIndex[0].addr);
        if (_p != user.pwd) {
            os << -1;
            return;
        }
        userList[_u] = userIndex[0].privilege;
        os << 0;
    }

    void UserManager::logout(std::ostream &os, const ticket::ull &_u) {
        auto iter = userList.find(_u);
        if (iter == userList.end()) os << -1;
        else {
            userList.erase(iter);
            os << 0;
        }
    }

    void UserManager::query_profile(std::ostream &os, const ticket::ull &_c, const ticket::ull &_u, const uidType &username) {
        auto iter = userList.find(_c);
        if (iter == userList.end()) {
            os << -1;
            return;
        }
        auto userIndex = userMap.Find(_u);
        if (userIndex.empty() || (userIndex[0].privilege >= iter->second && _u != _c)) {
            os << -1;
            return;
        }
        static User user;
        userData.SingleRead(user, userIndex[0].addr);
        os << username << ' ' << user.uName << ' ' << user.mail << ' ' << userIndex[0].privilege;
    }

    // 已登录的userList别忘了改权限
    // 如果自己修改自己的权限，但是修改成同样的值，是否算修改失败？ 当然算，按规则来。
    // 如果修改成同样的值修改成功后是否需要输出？ 当然。
    void UserManager::modify_profile(std::ostream &os, const ticket::ull &_c, const ticket::ull &_u, const uidType &username,
                                     const baihua::pair<bool, pwdType> &_p, const baihua::pair<bool, uNameType> &_n,
                                     const baihua::pair<bool, mailType> &_m, const baihua::pair<bool, int> &_g) {
        auto iter = userList.find(_c);
        if (iter == userList.end() || (_g.first && _g.second >= iter->second)) {
            os << -1;
            return;
        }
        auto userIndex = userMap.Find(_u);
        if (userIndex.empty() || (userIndex[0].privilege >= iter->second && _u != _c)) {
            os << -1;
            return;
        }
        static User user;
        userData.SingleRead(user, userIndex[0].addr);
        if (_p.first || _n.first || _m.first) {
            if (_p.first) user.pwd = _p.second;
            if (_n.first) user.uName = _n.second;
            if (_m.first) user.mail = _m.second;
            userData.SingleUpdate(user, userIndex[0].addr);
        }
        if (_g.first) {
            auto uIter = userList.find(_u);
            if (uIter != userList.end()) uIter->second = _g.second;
            userMap.Delete(_u, userIndex[0]);
            userIndex[0].privilege = _g.second;
            userMap.Insert(_u, userIndex[0]);
        }
        os << username << ' ' << user.uName << ' ' << user.mail << ' ' << userIndex[0].privilege;
    }

    bool UserManager::if_login(const ull &_u) {
        if (userList.find(_u) == userList.end()) return false;
        else return true;
    }

    void UserManager::clear() {
        userMap.Clear();
        userData.clear();
        userList.clear();
        userNum = 0;
    }

}