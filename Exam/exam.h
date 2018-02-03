#pragma once

#include "Date.h"

#include <chrono>
#include <string>
#include <iostream>
#include <ctime>
#include <utility>
#include <vector>

class Room
{
public:
	Room(size_t capacity, double cost_per_night, size_t number) : _capacity(capacity), _cost_per_night(cost_per_night), _number(number) {}
	Room(Room const& other) : _capacity(other._capacity), _cost_per_night(other._cost_per_night), _number(other._number), _booking(other._booking) {}
	Room(Room&& other) : _capacity(std::move(other._capacity)), _cost_per_night(std::move(other._cost_per_night)), _number(std::move(other._number)), _booking(std::move(other._booking)) {}

	size_t const& Get_capacity() { return _capacity; }
	double const& Get_cost() { return _cost_per_night; }
	size_t const& Get_number() { return _number; }
	std::pair<std::string, Date_t>* Get_book_data(); 
	size_t const& Get_book_amount();

	void Set_capacity(size_t capacity);
	void Set_cost(double cost);
	
	bool Add_booking(std::pair<std::string, Date_t> &&record);
	bool cancel_booking(Date_t &date);
	bool check_date_for_booking(Date_t &date);
	bool state(time_t date = time(NULL));

	void print_record(std::string access = "user");
	size_t clean_past(); // vyvod kol-va udalennyh zapisej

	Room operator = (Room const& other);

private:
	void _delete_booking(size_t index);

	std::vector<std::pair<std::string, Date_t>> _booking;
	
	size_t _capacity = 1;
	double _cost_per_night = 0;
	size_t _number = 0;
};

