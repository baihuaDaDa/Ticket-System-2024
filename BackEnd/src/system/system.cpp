#include "system.hpp"

namespace ticket {

    System::System(const std::string &filename) : run(true), userManager(filename + "User"),
                                                  orderManager(filename + "Order"),
                                                  trainManager(filename + "Train") {}

    void System::response(std::ostream &os, const std::string &cmd) {
        static ArgSet argSet;
        parse(cmd, argSet);
        os << '[' << argSet.timeTag << "] ";
        (this->*interface[argSet.cmdNo])(os, argSet);
        os << '\n';
    }

    void System::add_user(std::ostream &os, const ArgSet &argSet) {
        userManager.add_user(os, baihua::hash(argSet.args['c' - 'a']), baihua::hash(argSet.args['u' - 'a']),
                             argSet.args['p' - 'a'], argSet.args['n' - 'a'], argSet.args['m' - 'a'],
                             baihua::default_string_to_int(argSet.args['g' - 'a']));
    }

    void System::login(std::ostream &os, const ArgSet &argSet) {
        userManager.login(os, baihua::hash(argSet.args['u' - 'a']), argSet.args['p' - 'a']);
    }

    void System::logout(std::ostream &os, const ArgSet &argSet) {
        userManager.logout(os, baihua::hash(argSet.args['u' - 'a']));
    }

    void System::query_profile(std::ostream &os, const ArgSet &argSet) {
        userManager.query_profile(os, baihua::hash(argSet.args['c' - 'a']), baihua::hash(argSet.args['u' - 'a']),
                                  argSet.args['u' - 'a']);
    }

    void System::modify_profile(std::ostream &os, const ArgSet &argSet) {
        userManager.modify_profile(os, baihua::hash(argSet.args['c' - 'a']),
                                   baihua::hash(argSet.args['u' - 'a']), argSet.args['u' - 'a'],
                                   baihua::pair<bool, pwdType>{argSet.argsList['p' - 'a'], argSet.args['p' - 'a']},
                                   baihua::pair<bool, uNameType>{argSet.argsList['n' - 'a'], argSet.args['n' - 'a']},
                                   baihua::pair<bool, mailType>{argSet.argsList['m' - 'a'], argSet.args['m' - 'a']},
                                   baihua::pair<bool, int>{argSet.argsList['g' - 'a'],
                                                           baihua::default_string_to_int(argSet.args['g' - 'a'])});
    }

    void System::add_train(std::ostream &os, const ArgSet &argSet) {
        stationsType _s{argSet.args['s' - 'a']};
        pricesType _p{argSet.args['p' - 'a']};
        ttsType _t{argSet.args['t' - 'a']};
        oType _o{argSet.args['o' - 'a']};
        baihua::pair<Date, Date> _d{argSet.args['d' - 'a'].substr(0, 5), argSet.args['d' - 'a'].substr(6, 5)};
        trainManager.add_train(os, baihua::hash(argSet.args['i' - 'a']),
                               baihua::default_string_to_int(argSet.args['n' - 'a']),
                               baihua::default_string_to_int(argSet.args['m' - 'a']), _s, _p, argSet.args['x' - 'a'],
                               _t, _o, _d, argSet.args['y' - 'a'][0]);
    }

    void System::delete_train(std::ostream &os, const ArgSet &argSet) {
        trainManager.delete_train(os, baihua::hash(argSet.args['i' - 'a']));
    }

    void System::release_train(std::ostream &os, const ArgSet &argSet) {
        trainManager.release_train(os, baihua::hash(argSet.args['i' - 'a']), argSet.args['i' - 'a']);
    }

    void System::query_train(std::ostream &os, const ArgSet &argSet) {
        trainManager.query_train(os, baihua::hash(argSet.args['i' - 'a']), argSet.args['d' - 'a'],
                                 argSet.args['i' - 'a']);
    }

    void System::query_ticket(std::ostream &os, const ArgSet &argSet) {
        bool _p = true;
        if (argSet.argsList['p' - 'a'] && argSet.args['p' - 'a'] == "cost") _p = false;
        trainManager.query_ticket(os, baihua::hash(argSet.args['s' - 'a']), argSet.args['s' - 'a'],
                                  baihua::hash(argSet.args['t' - 'a']), argSet.args['t' - 'a'], argSet.args['d' - 'a'],
                                  _p);
    }

    void System::query_transfer(std::ostream &os, const ArgSet &argSet) {
        bool _p = true;
        if (argSet.argsList['p' - 'a'] && argSet.args['p' - 'a'] == "cost") _p = false;
        trainManager.query_transfer(os, baihua::hash(argSet.args['s' - 'a']), argSet.args['s' - 'a'],
                                    baihua::hash(argSet.args['t' - 'a']), argSet.args['t' - 'a'],
                                    argSet.args['d' - 'a'],
                                    _p);
    }

    void System::buy_ticket(std::ostream &os, const ArgSet &argSet) {
        ull _u = baihua::hash(argSet.args['u' - 'a']);
        if (!userManager.if_login(_u)) {
            os << -1;
            return;
        }
        Date _d{argSet.args['d' - 'a']};
        int _n = baihua::default_string_to_int(argSet.args['n' - 'a']);
        bool _q = false;
        if (argSet.argsList['q' - 'a'] && argSet.args['q' - 'a'] == "true") _q = true;
        ticket::TicketOrder ticketOrder = trainManager.buy_ticket_if_success(os, baihua::hash(argSet.args['i' - 'a']),
                                                                             _d, _n,
                                                                             baihua::hash(argSet.args['f' - 'a']),
                                                                             baihua::hash(argSet.args['t' - 'a']), _q);
        if (ticketOrder.success_and_queue.first)
            orderManager.buy_ticket(ticketOrder.success_and_queue.second, _u,
                                    argSet.args['i' - 'a'], ticketOrder.startDate, _n, ticketOrder.price,
                                    {argSet.args['f' - 'a'], ticketOrder.from},
                                    {argSet.args['t' - 'a'], ticketOrder.to}, ticketOrder.staNo, argSet.timeTag);
    }

    void System::query_order(std::ostream &os, const ArgSet &argSet) {
        ull _u = baihua::hash(argSet.args['u' - 'a']);
        if (!userManager.if_login(_u)) {
            os << -1;
            return;
        }
        orderManager.query_order(os, _u);
    }

    void System::refund_ticket(std::ostream &os, const ArgSet &argSet) {
        ull _u = baihua::hash(argSet.args['u' - 'a']);
        if (!userManager.if_login(_u)) {
            os << -1;
            return;
        }
        int _n = (argSet.argsList['n' - 'a'] ? baihua::default_string_to_int(argSet.args['n' - 'a']) : 1);
        auto result = orderManager.refund_ticket(os, _u, _n);
        if (result.first) {
            static seatsType seats;
            int addr = trainManager.read_seats(seats, baihua::hash(result.second.trainID), result.second.startDate);
            orderManager.push_queue(seats, result.second);
            trainManager.write_seats(addr, seats);
        }
    }

    void System::exit(std::ostream &os, const ArgSet &argSet) {
        os << "bye";
        run = false;
    }

    void System::clean(std::ostream &os, const ArgSet &argSet) {
        os << 0;
        userManager.clear();
        trainManager.clear();
        orderManager.clear();
    }

    bool System::if_run() const {
        return run;
    }

}