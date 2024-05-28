#include "src/system/system.hpp"

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
//    freopen("../testcases/TicketSystemData/data/17.in", "r", stdin);
//    freopen("../testcases/TicketSystemData/ans.out", "w", stdout);
    ticket::System system("");
    std::string cmd;
    while (std::getline(std::cin, cmd)) {
        system.response(std::cout, cmd);
        if (!system.if_run()) return 0;
    }
    return 0;
}