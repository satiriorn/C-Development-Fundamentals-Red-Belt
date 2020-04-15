#include "airline_ticket.h"
#include "test_runner.h"

#include <algorithm>
#include <numeric>
using namespace std;

#define SORT_BY(field) [](const AirlineTicket& lhs, const AirlineTicket& rhs) {   \
                            return lhs.field < rhs.field;                         \
                       }
					   
bool operator==(const Date& lhs, const Date& rhs) {
   return (lhs.year == rhs.year) && (lhs.month == rhs.month) && (lhs.day == rhs.day);
}

bool operator<(const Date& lhs, const Date& rhs) {
	return (lhs.year != rhs.year)?lhs.year < rhs.year:(lhs.month != rhs.month)?lhs.month < rhs.month:lhs.day < rhs.day;
}

bool operator==(const Time& lhs, const Time& rhs) { return (lhs.hours == rhs.hours) && (lhs.minutes == rhs.minutes);}

bool operator<(const Time& lhs, const Time& rhs) { return (lhs.hours != rhs.hours)?lhs.hours < rhs.hours: lhs.minutes < rhs.minutes;}

ostream& operator<<(ostream& os, const Time& time) {  return os << time.hours << ':' << time.minutes;}

ostream& operator<<(ostream& os, const Date& date) {  return os << date.year << '.' << date.month << '.' << date.day;}

