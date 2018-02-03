#include <ctime>
#include "Date.h"

bool Date_t::operator >(Date_t& other) //posle otrezka vremeni
{
	double difference = difftime(check_in, other.check_out);
	if (difference > 0.0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool Date_t::operator <(Date_t& other) // pered otrezkom vremeni
{
	double difference = difftime(other.check_in, check_out);
	if (difference > 0.0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool Date_t::operator !=(Date_t& other) // ne sovpadaut	
{
	if (this->operator>(other) || this->operator<(other))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool Date_t::operator ==(Date_t& other) // sovpadaut	
{
	if (date_str == other.date_str)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool Date_t::operator <(time_t& other) // proshedshee sobytie
{
	double difference = difftime(other, check_out);
	if (difference > 0.0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool Date_t::operator ==(time_t& other) // proshedshee sobytie
{
	double diff_in = difftime(other, check_in);
	if (diff_in > 0.0)
	{
		double diff_out = difftime(check_out, other);
		if (diff_out > 0.0)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

Date_t Date_t::operator =(Date_t const& other)
{
	check_in = other.check_in;
	check_out = other.check_out;
	date_str = other.date_str;
	return *this;
}

Date_t Date_t::operator =(Date_t&& other)
{
	check_in = std::move(other.check_in);
	check_out = std::move(other.check_out);
	date_str = std::move(other.date_str);
	return *this;
}

void Date_t::make_string()
{
	char buffer[100] = "\0";
	tm check;
	localtime_s(&check, &check_in);
	strftime(buffer, 100, "\nCheck-in date: %A %e %B %Y  after %R", &check);
	date_str = buffer;

	localtime_s(&check, &check_out);
	strftime(buffer, 100, "\nCheck-out date: %A %e %B %Y  before %R\n", &check);
	date_str += buffer;
}

time_t check_in_date(int day, int month, int year, bool early_check_in)
{
	std::time_t check_in = std::time(nullptr);
	char buffer[100] = { 0 };
	tm buf;
	localtime_s(&buf, &check_in);
	buf.tm_year = year - 1900;
	buf.tm_mon = month - 1;
	buf.tm_mday = day;
	buf.tm_hour = early_check_in == 1 ? 6 : 14;
	buf.tm_min = 0;
	buf.tm_sec = 0;
	check_in = mktime(&buf);

	return std::move(check_in);
}

time_t check_out_date(int day, int month, int year, bool late_check_out)
{
	std::time_t check_out = std::time(nullptr);
	char buffer[100] = { 0 };
	tm buf;
	localtime_s(&buf, &check_out);
	buf.tm_year = year - 1900;
	buf.tm_mon = month - 1;
	buf.tm_mday = day;
	buf.tm_hour = late_check_out == 1 ? 23 : 12;
	buf.tm_min = 0;
	buf.tm_sec = 0;
	check_out = mktime(&buf);

	return std::move(check_out);
}

double count_total_cost(double cost, Date_t date)
{
	double result = 0.0;
	double diff_sec = difftime(date.Get_check_out(), date.Get_check_in());
	int days_24h = (int)(diff_sec / 86400) + 1.0;
	int extra_hour = (int)(diff_sec - days_24h * 86400)/3600 + 2;
	if (extra_hour == 8.0 || extra_hour == 11.0)
	{
		result = (double)(cost*days_24h) + (double)(cost/2);
	}
	else if (extra_hour == 19.0)
	{
		result = cost + (double)(cost*days_24h);
	}
	else
	{
		result = (double)(cost*days_24h);
	}
	return result;
}

