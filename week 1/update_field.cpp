#include "airline_ticket.h"
#include "test_runner.h"
#include <tuple>

using namespace std;

#define UPDATE_FIELD(ticket, field, values) {  \
  map<string, string>::const_iterator it;      \
  it = values.find(#field);                    \
  if (it != values.end()) {                    \
    istringstream is(it->second);              \
    is >> ticket.field;                        \
  }                                            \
}

bool operator==(const Date& lhs, const Date& rhs) {return tie(lhs.year, lhs.month, lhs.day)==tie(rhs.year, rhs.month, rhs.day);}

bool operator<(const Date& lhs, const Date& rhs) {
	return (lhs.year != rhs.year)?lhs.year < rhs.year:(lhs.month != rhs.month)?lhs.month < rhs.month:lhs.day < rhs.day;
}

ostream& operator<<(ostream& os, const Time& time) {  return os << time.hours << ':' << time.minutes;}

ostream& operator<<(ostream& os, const Date& date) {  return os << date.year << '.' << date.month << '.' << date.day;}

bool operator==(const Time& lhs, const Time& rhs) {return(lhs.hours == rhs.hours) && (lhs.minutes == rhs.minutes);}

bool operator<(const Time& lhs, const Time& rhs) { return tie(lhs.hours, lhs.minutes) < tie(rhs.hours, rhs.minutes);}

istream& operator>>(istream& is, Time& time) {
    is >> time.hours;
    is.ignore(1);
    is >> time.minutes;

    return is;
}

istream& operator>>(istream& is, Date& date) {
    is >> date.year;
    is.ignore(1);
    is >> date.month;
    is.ignore(1);
    is >> date.day;

    return is;
}