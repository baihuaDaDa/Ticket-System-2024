#ifndef TICKET_SYSTEM_TIME_HPP
#define TICKET_SYSTEM_TIME_HPP

#include <iostream>
#include <string>
#include "../lib/mytools/mytools.hpp"
#include "../lib/STLite/utility.hpp"

namespace ticket {

    struct Clock {
        int hour;
        int minute;

        Clock();

        Clock(const Clock &clock);

        Clock(int hh, int mm);

        Clock(const std::string &clock);

        friend std::ostream &operator<<(std::ostream &os, const Clock &clock);

        Clock &operator=(const Clock &other);

        // return the number of days
        friend baihua::pair<Clock, int> _add(const Clock &clock, const int &_minute);

        int add(int _minute);

        friend baihua::pair<Clock, int> _minus(const Clock &clock, int _minute);

        int minus(int _minute);

        friend int operator-(const Clock &x, const Clock &y);

    };

    struct Date {
        // 2024
        static constexpr int months[13] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        int month;
        int day;

        Date();

        Date(const Date &date);

        Date(int mm, int dd);

        Date(const std::string &date);

        friend std::ostream &operator<<(std::ostream &os, const Date &date);

        friend Date _add(const Date &date, int _day);

        void add(int _day);

        friend Date _minus(const Date &date, int _day);

        void minus(int _day);

        friend int operator-(const Date &lhs, const Date &rhs);

    };

    struct Time {
        Clock clock;
        Date date;

        Time();

        Time(const Date &_date, const Clock &_clock);

        Time(int mm, int dd, int _hh, int _mm);

        Time(const std::string &time);

        Time &operator+=(int minute);

        friend Time operator+(const Time &x, int minute);

        Time &operator-=(int minute);

        friend Time operator-(const Time &x, int minute);

        friend int operator-(const Time &x, const Time &y);

        friend std::ostream &operator<<(std::ostream &os, const Time &time);
    };

    int CmpClock(const Clock &lhs, const Clock &rhs);

    int CmpDate(const Date &lhs, const Date &rhs);

    int CmpTime(const Time &lhs, const Time &rhs);
}

#endif //TICKET_SYSTEM_TIME_HPP
