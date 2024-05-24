#ifndef TICKET_SYSTEM_TIME_HPP
#define TICKET_SYSTEM_TIME_HPP

#include <iostream>
#include <string>
#include "../lib/mytools.hpp"

namespace ticket {

    struct Clock {
        int hour;
        int minute;

        Clock(const Clock &clock) : hour(clock.hour), minute(clock.minute) {}

        Clock(int hh, int mm) : hour(hh), minute(mm) {}

        Clock(const std::string &clock) {
            hour = baihua::string_to_int(clock, 0, 2);
            minute = baihua::string_to_int(clock, 3, 5);
        }

        friend std::ostream &operator<<(std::ostream &os, const Clock &clock) {
            std::string out = "00:00";
            out[0] += clock.hour / 10; out[1] += clock.hour % 10;
            out[3] += clock.minute / 10; out[4] += clock.minute % 10;
            os << out;
            return os;
        }

    };

    struct Date {
        int month;
        int day;

        Date(const Date &date) : month(date.month), day(date.day) {}

        Date(int mm, int dd) : month(mm), day(dd) {}

        Date(const std::string &date) {
            month = baihua::string_to_int(date, 0, 2);
            day = baihua::string_to_int(date, 3, 5);
        }

        friend std::ostream &operator<<(std::ostream &os, const Date &date) {
            std::string out = "00-00";
            out[0] += date.month / 10; out[1] += date.month % 10;
            out[3] += date.day / 10; out[4] += date.day % 10;
            os << out;
            return os;
        }

    };

    struct Time {
        Clock clock;
        Date date;

        Time(const Date &_date, const Clock &_clock) : clock(_clock), date(_date) {}

        Time(int mm, int dd, int _hh, int _mm) : clock(_hh, _mm), date(mm, dd) {}

        Time(const std::string &time) : clock(time.substr(0, 5)), date(time.substr(6, 5)) {}

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

        friend std::ostream &operator<<(std::ostream &os, const Time &time) {
            os << time.date << ' ' << time.clock;
            return os;
        }
    };
}

#endif //TICKET_SYSTEM_TIME_HPP
