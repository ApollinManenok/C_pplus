#include "exam.h"
#include <iostream>
#include <fstream>


std::pair<std::string, Date_t>* Room::Get_book_data()
{
	return _booking.data();
}

size_t const& Room::Get_book_amount()
{
	return _booking.size();
}

void Room::Set_capacity(size_t capacity)
{
	_capacity = capacity;
}

void Room::Set_cost(double cost)
{
	_cost_per_night = cost;
}

bool Room::Add_booking(std::pair<std::string, Date_t> &&record)
{
	if (_booking.size() == 0)
	{
		_booking.push_back(std::move(record));
		return true;
	}
	for (size_t i = 0; i < _booking.size(); ++i)
	{
		if (record.second < _booking[i].second)
		{
			for (size_t j = _booking.size(); j>i; --j)
			{
				_booking[j] = std::move(_booking[j - 1]);
			}
			_booking[i] = std::move(record);
			return true;
		}
	}
	return false;
}

bool Room::cancel_booking(Date_t &date)
{
	for (size_t i = 0; i < _booking.size(); ++i)
	{
		if (date == _booking[i].second)
		{
			_delete_booking(i);
			return true;
		}
	}
	return false;
}

bool Room::check_date_for_booking(Date_t &date)
{
	for (size_t i = 0; i < _booking.size(); ++i)
	{
		if (!(_booking[i].second != date))
		{
			return false;
		}
	}
	return true;
}

bool Room::state(time_t date)
{
	for (size_t i = 0; i < _booking.size(); ++i)
	{
		if (_booking[i].second == date)
		{
			return true;
		}
	}
	return false;
}

void Room::print_record(std::string access)
{
	std::cout <<"===============================================================\n"<<
		"Room number " << _number << "\n\t\tCapacity: " << _capacity << "\tCost per night: " << _cost_per_night << "$\n\n";
	if (access == "admin")
	{
		if (_booking.empty())
		{
			std::cout << "No reserves\n";
		}
		else
		{
			for (size_t i = 0; i< _booking.size(); ++i)
			{
				std::cout << "Customer " << _booking[i].first << _booking[i].second.Get_string();
			}
		}
	}
	std::cout << "\n===============================================================";
}

size_t Room::clean_past() // return kol-va udalennyh zapisej
{
	time_t date = time(NULL);
	size_t count = 0;
	for (size_t i = 0; i < _booking.size(); ++i)
	{
		if (_booking[i].second < date)
		{
			if (this->cancel_booking(_booking[i].second))
			{
				count++;
				--i;
			}
		}
	}
	return count;
}

Room Room::operator = (Room const& other)
{
	_capacity = other._capacity;
	_cost_per_night = other._cost_per_night;
	_number = other._number;
	_booking = other._booking;
	return *this;
}

void Room::_delete_booking(size_t index)
{
	
	for (size_t i = index; i<_booking.size()-1; ++i)
	{
		_booking[i] = std::move(_booking[i + 1]);
	}
	_booking.pop_back();
}

/*
Room Room::Read_from_file(std::string &const file_name)
{
	std::ifstream fin(file_name);
	
	if (fin.is_open())
	{
		while (!fin.eof())
		{
			std::string s;///
		}
	}
	else
	{
		//fail
	}
	return *this;
}

bool Room::Write_to_file(std::string &const file_name) 
{
	std::ofstream fout(file_name);
	if (!fout.is_open())
	{
		std::cout << "Can't open file " << file_name << std::endl;
		return false;
	}
	else
	{
		for (auto record : rooms)
		{
			fout << record. << " " << record.second << std::endl;
			Sleep(3000);
		}
		fout.close();
		return true;
	}
	


}
*/

