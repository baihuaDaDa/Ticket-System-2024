#ifndef TICKET_SYSTEM_TIME_HPP
#define TICKET_SYSTEM_TIME_HPP

#include <iostream>
#include <string>
#include "../lib/mytools.hpp"

namespace ticket {

    struct Clock {
        int hour;
        int minute;

        Clock() : hour(0), minute(0) {}

        Clock(const Clock &clock) : hour(clock.hour), minute(clock.minute) {}

        Clock(int hh, int mm) : hour(hh), minute(mm) {}

        Clock(const std::string &clock) {
            hour = baihua::string_to_int(clock, 0, 2);
            minute = baihua::string_to_int(clock, 3, 5);
        }

        friend std::ostream &operator<<(std::ostream &os, const Clock &clock) {
            std::string out = "00:00";
            out[0] += clock.hour / 10;
            out[1] += clock.hour % 10;
            out[3] += clock.minute / 10;
            out[4] += clock.minute % 10;
            os << out;
            return os;
        }

        Clock &operator=(const Clock &other) {
            if (this == &other) return *this;
            minute = other.minute;
            hour = other.hour;
            return *this;
        }

        // return the number of days
        friend baihua::pair<Clock, int> _add(const Clock &clock, const int &_minute) {
            Clock ret(clock);
            ret.minute += _minute;
            if (ret.minute < 60) return baihua::pair<Clock, int>{ret, 0};
            ret.hour += ret.minute / 60;
            ret.minute %= 60;
            if (ret.hour < 24) return baihua::pair<Clock, int>{ret, 0};
            int day = ret.hour / 24;
            ret.hour %= 24;
            return baihua::pair<Clock, int>{ret, day};
        }

        int add(const int &_minute) {
            auto result = _add(*this, _minute);
            *this = result.first;
            return result.second;
        }

        friend baihua::pair<Clock, int> _minus(const Clock &clock, const int &_minute) {
            Clock ret(clock);
            ret.minute -= _minute;
            if (ret.minute >= 0) return baihua::pair<Clock, int>{ret, 0};
            int dh = (-ret.minute + 59) / 60;
            ret.hour -= dh;
            ret.minute += dh * 60;
            if (ret.hour >= 0) return baihua::pair<Clock, int>{ret, 0};
            int dd = (-ret.hour + 23) / 24;
            ret.hour += dd * 24;
            return baihua::pair<Clock, int>{ret, dd};
        }

        int minus(const int &_minute) {
            auto result = _minus(*this, _minute);
            *this = result.first;
            return result.second;
        }

    };

    struct Date {
        // 2024
        static constexpr int months[13] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        int month;
        int day;

        Date() : month(0), day(0) {}

        Date(const Date &date) : month(date.month), day(date.day) {}

        Date(int mm, int dd) : month(mm), day(dd) {}

        Date(const std::string &date) {
            month = baihua::string_to_int(date, 0, 2);
            day = baihua::string_to_int(date, 3, 5);
        }

        friend std::ostream &operator<<(std::ostream &os, const Date &date) {
            std::string out = "00-00";
            out[0] += date.month / 10;
            out[1] += date.month % 10;
            out[3] += date.day / 10;
            out[4] += date.day % 10;
            os << out;
            return os;
        }

        friend Date _add(const Date &date, const int &_day) {
            Date ret(date);
            ret.day += _day;
            for (; ret.day <= months[ret.month]; ret.day -= months[ret.month++]);
            return ret;
        }

        void add(const int &_day) {
            *this = _add(*this, _day);
        }

        friend Date _minus(const Date &date, const int &_day) {
            Date ret(date);
            ret.day -= _day;
            for (; ret.day >= 1; ret.day += months[--ret.month]);
            return ret;
        }

        void minus(const int &_day) {
            *this = _minus(*this, _day);
        }

        friend int operator-(const Date &lhs, const Date &rhs) {
            int ret = lhs.day - rhs.day;
            for (int i = rhs.month; i < lhs.month; ++i)
                ret += months[i];
            return ret;
        }

    };

    struct Time {
        Clock clock;
        Date date;

        Time() : clock(), date() {}

        Time(const Date &_date, const Clock &_clock) : clock(_clock), date(_date) {}

        Time(int mm, int dd, int _hh, int _mm) : clock(_hh, _mm), date(mm, dd) {}

        Time(const std::string &time) : clock(time.substr(0, 5)), date(time.substr(6, 5)) {}

        Time &operator+=(const int &minute) {
            date.add(clock.add(minute));
            return *this;
        }

        friend Time operator+(const Time &x, const int &minute) {
            Time ret(x);
            ret += minute;
            return ret;
        }

        Time &operator-=(const int &minute) {
            date.minus(clock.minus(minute));
            return *this;
        }

        friend Time operator-(const Time &x, const int &minute) {
            Time ret(x);
            ret -= minute;
            return ret;
        }

        friend std::ostream &operator<<(std::ostream &os, const Time &time) {
            os << time.date << ' ' << time.clock;
            return os;
        }
    };

    int CmpClock(const Clock &lhs, const Clock &rhs) {
        if (lhs.hour != rhs.hour) return baihua::CmpInt(lhs.hour, rhs.hour);
        else return baihua::CmpInt(lhs.minute, rhs.minute);
    }

    int CmpDate(const Date &lhs, const Date &rhs) {
        if (lhs.month != rhs.month) return baihua::CmpInt(lhs.month, rhs.month);
        else return baihua::CmpInt(lhs.day, rhs.day);
    }

    int CmpTime(const Time &lhs, const Time &rhs) {
        int flag = CmpDate(lhs.date, rhs.date);
        if (flag != 0) return flag;
        else return CmpClock(lhs.clock, rhs.clock);
    }
}

#endif //TICKET_SYSTEM_TIME_HPP
