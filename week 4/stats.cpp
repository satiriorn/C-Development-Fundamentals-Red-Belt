#include "stats.h"
#include <algorithm>
#include <iostream>

Stats::Stats() :
	MetStatus(
	{
		{"GET", 0},
		{"POST", 0},
		{"PUT", 0},
		{"DELETE", 0},
		{"UNKNOWN", 0},
	}),
	Uri_status(
	{
		{"/", 0},
		{"/order", 0},
		{"/product", 0},
		{"/basket", 0},
		{"/help", 0},
		{"unknown", 0}
	}){}

void Stats::AddMethod(string_view method){(MetStatus.count(method) == 0)?MetStatus["UNKNOWN"]+=1:MetStatus[method]+=1;}

void Stats::AddUri(string_view uri){(Uri_status.count(uri) == 0)?Uri_status["unknown"]+=1:Uri_status[uri]+=1;}

const map<string_view, int>& Stats::GetMethodStats() const{return MetStatus;}

const map<string_view, int>& Stats::GetUriStats() const{return Uri_status;}

HttpRequest ParseRequest(string_view line)
{
	HttpRequest res;
	line.remove_prefix(min(line.find_first_not_of(' '), line.size()));
	size_t l = line.find(' ');
	res.method = line.substr(0,l);
	line.remove_prefix(++l);
	l = line.find(' ');
	res.uri = line.substr(0, l);
	line.remove_prefix(++l);
	res.protocol = line.substr(0);
	return res;
}