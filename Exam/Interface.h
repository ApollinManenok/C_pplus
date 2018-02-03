#pragma once
#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include <utility>
#include "Date.h"
#include "exam.h"



/*
    Struct with user infomation.
	Conain: initials, login, password and booking vector(room number and booking date in pair)
*/
struct User_profile
{
	User_profile(std::string sur_name, std::string login, std::string password, std::vector<std::pair<size_t, Date_t>> bookings)
		: sur_name(sur_name), login(login), password(password), bookings(bookings) {}
	std::string sur_name = "\0"; // name surname
	std::string login = "\0";
	std::string password = "\0";
	std::vector<std::pair<size_t, Date_t>> bookings;
};

bool write_to_f_user(std::vector<User_profile>& users, std::string &const file_name); // Function for writing user profiles into database file
bool read_from_f_user(std::vector<User_profile>& users, std::string &const file_name); // Function for reading user profiles from database file
bool read_from_f_room(std::vector<Room>& rooms, std::string &const file_name); // Function for reading rooms information from database file
bool write_to_f_room(std::vector<Room>& rooms, std::string &const file_name); // Function for writing rooms information into database file
Date_t Create_book_date(); // Create booking in Date_t format
void sort_rooms(std::vector<Room>& rooms); //Place last room elementh into right plase (special for .push_back())
std::string split_string(std::string& old, std::string& second); // Split one string into two by first 32(" ") symbol. First part returns by function, second part placed in second function argument


class Interface
{
public:
	
virtual void Menu() = 0; // Main function wich define any user functionality

private:
virtual void _Show_rooms() = 0; 
virtual bool _Add_room() = 0; 
virtual bool _Delete_room() = 0;
virtual bool _New_booking() = 0;
virtual bool _Edit_room() = 0;
virtual bool _Change_profile_info() = 0;
virtual void _Update_to_DB_room() = 0;
virtual void _Update_to_DB_user() = 0;
virtual void _Update_from_DB_room() = 0;
virtual void _Update_from_DB_user() = 0;
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
	void _Show_rooms() override; // output all rooms
	bool _Add_room() override; // Add new room
	bool _Delete_room() override; // Delete room(only from container)
	bool _New_booking() override; // Create new booking
	bool _Edit_room() override; //
	bool _Change_profile_info() override;
	void _Update_to_DB_room() override;
	void _Update_to_DB_user() override;
	void _Update_from_DB_room() override;
	void _Update_from_DB_user() override ;
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
	void _Update_to_DB_user() override {}
	void _Update_from_DB_room() override {}
	void _Update_from_DB_user() override {}
	
	
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
	void _Update_to_DB_user() override {}
	void _Update_from_DB_room() override {}
	void _Update_from_DB_user() override {}

	void _Show_me() override {};
	

	std::vector<Room>* _rooms;
};

