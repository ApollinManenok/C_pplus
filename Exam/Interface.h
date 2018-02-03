#pragma once
#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include <utility>
#include "Date.h"
#include "exam.h"

struct User_profile
{
	User_profile(std::string sur_name, std::string login, std::string password, std::vector<std::pair<size_t, Date_t>> bookings)
		: sur_name(sur_name), login(login), password(password), bookings(bookings) {}
	std::string sur_name = "\0";
	std::string login = "\0";
	std::string password = "\0";
	std::vector<std::pair<size_t, Date_t>> bookings;
};

bool write_to_f_user(std::vector<User_profile>& users, std::string &const file_name);
bool read_from_f_user(std::vector<User_profile>& users, std::string &const file_name);
bool read_from_f_room(std::vector<Room>& rooms, std::string &const file_name);
bool write_to_f_room(std::vector<Room>& rooms, std::string &const file_name);
Date_t Create_book_date();
void sort_rooms(std::vector<Room>& rooms);
std::string split_string(std::string& old, std::string& second);


class Interface
{
public:
	
virtual void Menu() = 0;

private:
virtual void _Show_rooms() = 0;
virtual bool _Add_room() = 0;
virtual bool _Delete_room() = 0;
virtual bool _New_booking() = 0;
virtual bool _Edit_room() = 0;
virtual bool _Change_profile_info() = 0;
virtual void _Update_to_DB_room() = 0;
virtual void _Update_to_log_pass() = 0;
virtual void _Update_from_DB_room() = 0;
virtual void _Update_from_log_pass() = 0;
virtual bool _Cancel_booking() = 0;
virtual void _Show_me() = 0;

User_profile* profile;
std::vector<Room>* _rooms;
std::vector<User_profile>* _users;
};


class I_Admin : public Interface
{
public:
	I_Admin(std::vector<Room>* rooms, std::vector<User_profile>* users, User_profile* set_profile)
		: _rooms(rooms), _users(users), profile(set_profile) {}
	void Menu();
		
private:
	void _Show_rooms() override;
	bool _Add_room() override;
	bool _Delete_room() override;
	bool _New_booking() override;
	bool _Edit_room() override;
	bool _Change_profile_info() override;
	void _Update_to_DB_room() override;
	void _Update_to_log_pass() override;
	void _Update_from_DB_room() override;
	void _Update_from_log_pass() override ;
	bool _Cancel_booking() override { return false; }
	void _Show_me() override {};
	
	User_profile* profile;
	std::vector<Room>* _rooms;
	std::vector<User_profile>* _users;
};



class I_User : public Interface
{
public:
	I_User(std::vector<Room>* rooms, std::vector<User_profile>* users,  User_profile* set_profile)
		: _rooms(rooms), _users(users), profile(set_profile) {}

	void Menu();

private:
	bool _New_booking() override;
	bool _Cancel_booking() override;
	bool _Change_profile_info() override;
	void _Show_me() override;
	void _Show_rooms() override {};
	bool _Add_room() override { return 0; }
	bool _Delete_room() override { return 0; }
	bool _Edit_room() override { return 0; }
	void _Update_to_DB_room() override {}
	void _Update_to_log_pass() override {}
	void _Update_from_DB_room() override {}
	void _Update_from_log_pass() override {}
	
	
	User_profile* profile;
	std::vector<Room>* _rooms;
	std::vector<User_profile>* _users;
};


class I_Guest : public Interface
{
public:
	I_Guest(std::vector<Room>* rooms) : _rooms(rooms){}
	void Menu();

private:
	bool _New_booking() override;
	bool _Add_room() override { return 0; }
	bool _Delete_room()  override { return 0; }
	bool _Edit_room()  override { return 0; }
	bool _Change_profile_info() override { return 0; }
	bool _Cancel_booking() override { return 0; }
	void _Show_rooms() override { }
	void _Update_to_DB_room() override {}
	void _Update_to_log_pass() override {}
	void _Update_from_DB_room() override {}
	void _Update_from_log_pass() override {}

	void _Show_me() override {};
	

	std::vector<Room>* _rooms;
};

