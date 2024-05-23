#ifndef INC_7_TICKET_SYSTEM_TRAIN_HPP
#define INC_7_TICKET_SYSTEM_TRAIN_HPP

#include "../cmdprocessor.hpp"
#include "../time.hpp"
#include "../lib/STLite/utility.hpp"

namespace ticket {

    struct Train {
        int staNum;
        staNameType stations[staNum_Max];
        int seatNum;
        int prices[staNum_Max - 1];
        Clock startTime;
        int travelTimes[staNum_Max - 1];
        int stopoverTimes[staNum_Max - 2];
        baihua::pair<Date, Date> saleDate;
        char type;

    };

    class TrainManager {

    };

}


#endif //INC_7_TICKET_SYSTEM_TRAIN_HPP
