#include "src/system/system.hpp"

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    ticket::System system("TicketSystem");
    std::string cmd;
    while (std::getline(std::cin, cmd))
        system.response(std::cout, cmd);
    return 0;
}