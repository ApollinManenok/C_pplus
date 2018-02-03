// Date_t header
#pragma once

#include <string>
#include <iostream>
#include <ctime>

/*
Contain two time_t and string
Represents period of time between time_t check_in and check_out
*/

class Date_t
{
public:
	Date_t() = default;
	Date_t(time_t first, time_t second) : check_in(first), check_out(second) { make_string(); } // dve tochki vo vremeni
	Date_t(time_t first, size_t duration_days)
		: check_in(first)
	{
		tm buf;
		localtime_s(&buf, &first);
		buf.tm_mday += duration_days;
		check_out = mktime(&buf);
		make_string();
	} // nachal'naja tochka vo vremeni i dlitel'nost' prebyvanija 
	Date_t(Date_t const& other) : check_in(other.check_in), check_out(other.check_out), date_str(other.date_str) {} // copy constuctor
	Date_t(Date_t&& other) : check_in(std::move(other.check_in)), check_out(std::move(other.check_out)), date_str(std::move(other.date_str)) {} // move destructor

	std::string const& Get_string() { return date_str; }; //return date_str
	time_t const& Get_check_in() { return check_in; } // retrun check_in
	time_t const& Get_check_out() { return check_out; } //return check_out

	bool operator >(Date_t& other); //posle otrezka vremeni
	bool operator <(Date_t& other); // pered otrezkom vremeni
	bool operator !=(Date_t& other); // ne peresekauts'a	
	bool operator ==(Date_t& other); // sovpadaut	
	bool operator <(time_t& now); // proshedshee sobytie otnositel'no time_t
	bool operator ==(time_t& now); // nahodits'a li tochka vremeni v sobytii
	
	Date_t operator =(Date_t const& other); // priravn'at' otrezki vremeni
	Date_t operator =(Date_t&& other); // move prisvaivanie otrezki vremeni

private:
	void make_string();

	time_t check_in = 0; // data v'ezda
	time_t check_out = 0; // data vyezda
	std::string date_str = "\0"; // stroka s vyvodom perioda
	
};

time_t check_in_date(int day, int month, int year, bool early_check_in = false); // create check_in date from users input
time_t check_out_date(int day, int month, int year, bool late_check_out = false);  // create check_out date from users input
double count_total_cost(double cost, Date_t date); //count nights in "date" period