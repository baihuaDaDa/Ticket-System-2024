#ifndef TICKET_SYSTEM_TIME_HPP
#define TICKET_SYSTEM_TIME_HPP

#include <iostream>

namespace ticket {

    struct Clock {
        int hour;
        int minute;

        Clock(const Clock &clock) : hour(clock.hour), minute(clock.minute) {}

        Clock(int hh, int mm) : hour(hh), minute(mm) {}

        std::string to_string() {

        }
    };

    struct Date {
        int month;
        int day;

        Date(const Date &date) : month(date.month), day(date.day) {}

        Date(int mm, int dd) : month(mm), day(dd) {}

        std::string to_string() {

        }
    };

    struct Time {
        Clock clock;
        Date date;

        Time(const Date &_date, const Clock &_clock) : clock(_clock), date(_date) {}

        Time(int mm, int dd, int _hh, int _mm) : clock(_hh, _mm), date(mm, dd) {}

        Time &operator+=(const int minute) {

        }

        friend Time operator+(const Time &x, const int minute) {

        }

        Time &operator-=(const int minute) {

        }

        friend Time operator-(const Time &x, const int minute) {

        }

        friend bool operator<(const Time &lhs, const Time &rhs) {

        }

        friend bool operator<=(const Time &lhs, const Time &rhs) {

        }

        friend bool operator>(const Time &lhs, const Time &rhs) {

        }

        friend bool operator>=(const Time &lhs, const Time &rhs) {

        }

        std::string to_string() {

        }
    };
}

#endif //TICKET_SYSTEM_TIME_HPP
