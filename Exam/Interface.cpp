#include "Interface.h"
#include <limits>
#include <stdlib.h>
#include <windows.h>
#include <fstream>
#include <list>
#include <algorithm>

#include "Date.h"

void sort_rooms(std::vector<Room>& rooms)
{
	for (int i = rooms.size()-1; i > 0; --i)
	{
		if (rooms[i].Get_number() < rooms[i - 1].Get_number())
		{
			Room temp = rooms[i];
			rooms[i] = rooms[i - 1];
			rooms[i - 1] = temp;
		}
		else
		{
			break;
		}
	}
}

bool write_to_f_user(std::vector<User_profile>& users, std::string &const file_name)
{
	std::ofstream fout(file_name);
	if (!fout.is_open())
	{
		std::cout << "Can't open file " << file_name << std::endl;
		return false;
	}
	else
	{
		fout << users.size() << "\n";
		for (auto record : users)
		{
			fout << record.login << "\n" << record.password << "\n" << record.sur_name << "\n" << record.bookings.size() << std::endl;
			for (size_t i = 0; i < record.bookings.size(); ++i)
			{
				fout << record.bookings[i].first << " " << record.bookings[i].second.Get_check_in() << " " << record.bookings[i].second.Get_check_in() << std::endl;
			}
		}
		fout.close();
		return true;
	}
}

bool read_from_f_user(std::vector<User_profile>& users, std::string &const file_name)
{
	size_t users_amount_f = 0;
	size_t users_amount = 0;
	std::string login = "\0";
	std::string password = "\0";
	std::string user_FIO = "\0";
	size_t book_amount_f = 0;
	size_t book_amount = 0;
	size_t room_number = 0;
	time_t check_in = 0;
	time_t check_out = 0;
	
	std::ifstream fin(file_name);
	if (!fin.is_open())
	{
		std::cout << "\nCan't open file \n" << file_name << std::endl;
		return false;
	}
	else
	{
		fin >> users_amount_f;
		if (users_amount_f == 0)
		{
			fin.close();
			std::cout << "File is empty";
			Sleep(2000);
			return false;
		}
		if (!users.empty())
		{
			users.clear();
		}

		while (!fin.eof())
		{
			if (users_amount == users_amount_f)
			{
				fin.close();
				return true;
			}
			std::string second_str = "\0";
			fin >> login >> password >> user_FIO >> second_str >> book_amount_f;
			user_FIO += " " + second_str;
			
			std::vector<std::pair<size_t, Date_t>> bookings;
			for (size_t i = 0; i < book_amount_f; ++i)
			{
				if (book_amount_f == book_amount)
				{
					break;
				}
				fin >> room_number >> check_in >> check_out;
				bookings.push_back(std::pair<size_t, Date_t>(room_number, Date_t(check_in, check_out)));
				++book_amount;
			}
			users.push_back(User_profile(user_FIO, login, password, bookings));
			++users_amount;
		}
		if (users_amount == users_amount_f)
		{
			return true;
		}
		else
		{
			std::cout << "Some records wasn't readed";
			Sleep(2000);
			return false;
		}		
	}
}

bool read_from_f_room(std::vector<Room>& rooms, std::string &const file_name)
{
	size_t count_record_f = 0;
	size_t count_record = 0;

	size_t capacity = 0;
	double cost = 0;
	size_t number = 0;
	std::string fio = "\0";
	//std::string fio = "\0";
	size_t book_amount = 0;
	time_t check_in = 0;
	time_t check_out = 0;
	std::vector<std::pair<std::string, Date_t>> booking;

	std::ifstream fin(file_name);
	if (!fin.is_open())
	{
		std::cout << "\nCan't open file \n" << file_name << std::endl;
		return false;
	}
	else
	{
		fin >> count_record_f;
		if (count_record_f == 0)
		{
			fin.close();
			std::cout << "File is empty";
			Sleep(2000);
			return false;
		}
		if (!rooms.empty())
		{
			rooms.clear();
		}

		while (!fin.eof())
		{
			if (count_record == count_record_f)
			{
				fin.close();
				return true;
			}
			fin >> number >> capacity >> cost >> book_amount;
			Room room(capacity, cost, number);
			size_t check_book_records = 0;
			for (size_t i = 0; i < book_amount; ++i)
			{
				if (check_book_records == book_amount)
				{
					break;
				}
				std::string second_str = "\0";
				fin >> fio >> second_str;
				fio += " " + second_str;
				fin >> check_in >> check_out;
				room.Add_booking(std::pair<std::string, Date_t>(fio, Date_t(check_in, check_out)));
				++check_book_records;
			}
			rooms.push_back(room);
			++count_record;
			if (fin.eof())
			{
				fin.close();
				return true;
			}
		}
	}
}

bool write_to_f_room(std::vector<Room>& rooms, std::string &const file_name)
{
	std::ofstream fout(file_name, std::ios::trunc);
	if (!fout.is_open())
	{
		std::cout << "Can't open file " << file_name << std::endl;
		return false;
	}
	else
	{
		fout << rooms.size() << "\n";
		for (size_t i = 0; i<rooms.size(); ++i)
		{
			size_t amount = rooms[i].Get_book_amount();
			fout << rooms[i].Get_number() << " " << rooms[i].Get_capacity() << " " << rooms[i].Get_cost() << " " << amount << std::endl;
			std::pair<std::string, Date_t>* book = rooms[i].Get_book_data();
			for (size_t i = 0; i < amount; ++i)
			{
				fout << book[i].first << "\n" << book[i].second.Get_check_in() << " " << book[i].second.Get_check_out() << std::endl;
			}
		}
		fout.close();
		return true;
	}

}

Date_t Create_book_date()
{
	std::time_t now = std::time(nullptr);
	char buffer[100] = { 0 };
	int day = 0;
	int month = 0;
	int year = 0;
	std::string answer = "\0";
	
	while (answer != "y")
	{
		system("CLS");
		std::cout << "\nEnter check-in date (dd mm yyyy): ";
		std::cin >> day >> month >> year;
		getchar();
		std::cout << "Do you need early check-in? (06:00 - 14:00) (y/n): ";
		std::cin >> answer;
		bool early = (answer == "y");
		std::time_t check_in = check_in_date(day, month, year, early);
		if (check_in < now)
		{
			std::cout << "Check-in date is past time, choose another date";
			getchar();
			Sleep(3000);
			continue;
		}
		else if (check_in < now + 604800)
		{
			std::cout << "Check-in date is too close, choose another date or contact reception";
			getchar();
			Sleep(3000);
			continue;
		}
		std::cout << "\nEnter check-out date (dd mm yyyy): ";
		std::cin >> day >> month >> year;
		getchar();
		std::cout << "Do you need late check-out? (12:00 - 23:00) (y/n): ";
		std::cin >> answer;
		bool late = (answer == "y");
		std::time_t check_out = check_out_date(day, month, year, late);
		if (difftime(check_out, check_in) < 0.0)
		{
			std::cout << "Check-out is earlier than check-in, check your dates carefully";
			getchar();
			Sleep(3000);
			continue;
		}
		else if (difftime(check_out, check_in) < 79200.0)
		{
			std::cout << "Time interval between check-in date and check-out date is too small, choose another date or contact reception";
			getchar();
			Sleep(3000);
			continue;
		}

		tm check;
		localtime_s(&check, &check_in);
		strftime(buffer, 100, "\nCheck-in date: %A %e %B %Y  after %R", &check);
		std::cout << buffer << "\n";

		localtime_s(&check, &check_out);
		strftime(buffer, 100, "\nCheck-out date: %A %e %B %Y  before %R", &check);
		std::cout << buffer << "\n\nAre this dates correct? (y/n) ";
		std::cin >> answer;
		if (answer == "y")
		{
			return std::move(Date_t(check_in, check_out));
		}		
	}	
}

std::string split_string(std::string& old, std::string& second)
{
	std::string temp = "\0";
	bool check_ok = 1;
	for (size_t i = 0; i < old.size(); ++i)
	{
		if (old[i] == 32)
		{
			check_ok = 0;
			continue;
		}
		if (check_ok)
		{
			temp += old[i];
		}
		else 
		{
			second += old[i];
		}
	}
	return std::move(temp);
}


//Admin

void I_Admin::Menu()
{
	bool exit = true;
	int answer = 0;
	while (exit)
	{
		system("CLS");
		std::cout << "\n-> To show all rooms \t press 1\n\n-> To add new room \t press 2\n\n" <<
			"-> To edit room \t press 3\n\n-> To delete room \t press 4\n\n-> To make new booking \t press 5\n\n" <<
			"-> To change admin login and password \t press 6\n\n-> To update database with rooms \t press 7\n\n" <<
			"-> To update from database with rooms \t press 8\n\n-> To update database with profiles \t press 9\n\n" <<
			"-> To update from database with profiles press 10\n\n-> To log out \t press 11\n\n>>> ";
		std::cin >> answer;
		switch (answer)
		{
		case 1:
		{
			this->_Show_rooms();
			break;
		}
		case 2:
		{
			this->_Add_room();
			break;
		}
		case 3:
		{
			this->_Edit_room();
			break;
		}
		case 4:
		{
			this->_Delete_room();
			break;
		}
		case 5:
		{
			this->_New_booking();
			break;
		}
		case 6:
		{
			this->_Change_profile_info();
			break;
		}
		case 7:
		{
			this->_Update_to_DB_room();
			break;
		}
		case 8:
		{
			this->_Update_from_DB_room();
			break;
		}
		case 9:
		{
			this->_Update_to_log_pass();
			break;
		}
		case 10:
		{
			this->_Update_from_log_pass();
			break;
		}
		case 11:
		{
			exit = false;
			break;
		}
		default:
		{
			std::cout << "\nWrong enter, try again!" << std::endl;
			Sleep(3000);
			break;
		}
		}
	}
}

void I_Admin::_Show_rooms()
{
	if (_rooms->empty())//.get()->
	{
		std::cout << "\nThere are no  registered rooms in hotel.\n\n";
	}
	else
	{
		system("CLS");
		for (size_t i = 0; i < _rooms->size(); ++i)//get()->
		{
			std::cout << "\n";
			_rooms->operator[](i).print_record("admin"); //.get()->operator[](i)
		}
		std::string answer = "\0";
		std::cout << "\n\nDo you want to clean old bookings? (y/n) ";
		std::cin >> answer;
		if (answer == "y")
		{
			size_t deleted_records = 0;
			for (size_t i = 0; i < _rooms->size(); ++i)//get()->
			{
				deleted_records += _rooms->operator[](i).clean_past();//.get()->operator[](i)
			}
			std::cout << deleted_records << " records were deleted.\n";			
		}
	}
	Sleep(3000);
}

bool I_Admin::_Add_room()
{	
	size_t capacity = 0;
	size_t number = 0;
	double cost = 0;
	
	while (true)
	{
		bool check_ok = 1;
		system("CLS");
		// number
		std::cout << "To cancel adding room press 0\n\nEnter new room number: ";
		std::cin >> number;
		getchar();

		if (number == 0)
		{
			return false;
		}
		else if (number < 0)
		{
			std::cout << "\nRoom number can't be less than 1.\n";
			Sleep(3000);
			continue;
		}

		for (size_t i = 0; i < _rooms->size(); ++i)//get()->
		{
			if (_rooms->operator[](i).Get_number() == number)//.get()->operator[](i)
			{
				std::cout << "\nSuch room already exists.\n";
				Sleep(3000);
				check_ok = false;
				break;
			}
		}
		if (!check_ok)
		{
			continue;
		}

		// capacity
		std::cout << "Enter new room capacity: ";
		std::cin >> capacity;
		getchar();

		if (capacity < 1 || capacity > 10)
		{
			std::cout << "\nWrong capacity value.\n";
			Sleep(3000);
			check_ok = false;
			break;
		}
		if (!check_ok)
		{
			continue;
		}

		// cost
		std::cout << "Enter new room cost per night: ";
		std::cin >> cost;
		getchar();

		if (cost < 1.0)
		{
			std::cout << "\nWrong cost value.\n";
			Sleep(3000);
			check_ok = false;
			break;
		}
		if (!check_ok)
		{
			continue;
		}
		else
		{
			std::string answer = "\0";
			//create
			_rooms->push_back(Room(capacity, cost, number));//get()->
			sort_rooms(*_rooms);//* ssylka? sort
			std::cout << "\nRoom " << number << " was added.\n\nDo you want to add more room? (y/n) "; 
			std::cin >> answer;
			if (answer == "y")
			{
				continue;
			}
			else 
			{
				return true;
			}			
		}		
	}
}

bool I_Admin::_Delete_room()
{
	size_t number = 0;
	while (true)
	{
		system("CLS");
		std::cout << "To delete room enter room number: ";
		std::cin >> number;
		getchar();
		
		for (size_t i = 0; i < _rooms->size(); ++i)//get()->
		{
			if (_rooms->operator[](i).Get_number() == number)//.get()->operator[](i)
			{
				for (size_t j = i; j < _rooms->size()-1; ++j)//get()->
				{
					_rooms[j] = _rooms[j+1];// .get()->operator[](j)  .get()->operator[](j + 1)
				}
				_rooms->pop_back(); //get()->
				std::cout << "Room number " << number << " was successfully deleted.";
				Sleep(3000);
				return true;
			}
		}
		std::string answer = "\0";
		std::cout << "\nRoom not found\nWould you try again? (y/n) ";
		std::cin >> answer;
		if (answer == "n")
		{
			return false;
		}
	}
}

bool I_Admin::_New_booking()
{
	Date_t booking(Create_book_date());
	size_t capacity_need = 0;
	std::cout << "\n\nEnter needed capacity ";
	std::cin >> capacity_need;
	getchar();	

	std::list<size_t> number_list;
	for (size_t i = 0; i < _rooms->size(); ++i)//get()->
	{
		if (_rooms->operator[](i).Get_capacity() >= capacity_need) //.get()->operator[](i)
		{
			if (_rooms->operator[](i).check_date_for_booking(booking))//.get()->operator[](i)
			{
				number_list.push_back(i);
			}
		}
	}

	if (number_list.size() == 0)
	{
		std::cout << "\nNo room matching your request, please try again.\n";
		Sleep(3000);
		return false;
	}
	system("CLS");
	std::cout << "All matching results: \n";
	for (auto ind : number_list)
	{
		double total_cost = count_total_cost(_rooms->operator[](ind).Get_cost(), booking);//.get()->operator[](ind)
		std::cout << " #index " << ind+1 << ": ";
		_rooms->operator[](ind).print_record();//.get()->operator[](ind)
		std::cout << "\n\t\t\t\t\tTotal cost: " << total_cost << "\n\n";
	}
	size_t index = 0;
	std::cout << "\nTo cancel booking press 0.\n\nChoose room #index  ";
	std::cin >> index;
	getchar();
	if (index == 0)
	{
		return false;
	}
	--index;
	if (index > _rooms->size())//get()->
	{
		std::cout << "Wrong index, enter again, please ";
		std::cin >> index;
		getchar();
	}
	if (index > _rooms->size())//get()->
	{
		std::cout << "Wrong index, try again later ";
		Sleep(2000);
		return false;
	}
	std::string user_FIO = "\0"; /// CHECK
	std::string second_str = "\0";
	for (;;)
	{
		std::cout << "\nEnter your initials, please (Name Surname): ";
		std::cin >> user_FIO >> second_str;
		user_FIO += " " + second_str;
		size_t counter = 0;
		for (size_t i = 0; i < user_FIO.size(); ++i)
		{
			if (user_FIO[i] == 32 || user_FIO[i] == 46 || user_FIO[i] == 44 || user_FIO[i] == 63 || user_FIO[i] == 33 || (user_FIO[i] > 48 && user_FIO[i] < 57))
			{
				++counter;
			}
		}
		if (counter > 1)
		{
			std::cout << "\nWrong initials were entered, enter must be like: Name Surname.\n Only literals and \"-\" are acceptable)n" << std::endl;
			Sleep(5000);
			continue;
		}
		break;
	}

	if (_rooms->operator[](index).Add_booking(std::pair<std::string, Date_t>(user_FIO, booking)))//.get()->operator[](index)
	{
		std::cout << "\nBooking was successfull.";
		Sleep(3000);
		return true;
	}
	std::cout << "\nSomething went wrong. Try again later.";
	Sleep(3000);
	return true;	
}

bool I_Admin::_Edit_room()
{
	size_t number = 0;
	while (true)
	{
		system("CLS");
		std::cout << "To edit room enter its number: ";
		std::cin >> number;
		getchar();

		for (size_t i = 0; i < _rooms->size(); ++i)
		{
			if (_rooms->operator[](i).Get_number() == number)
			{
				bool exit = true;
				int answer = 0;
				while (exit)
				{
					system("CLS");
					std::cout << "\n-> To view room information press 1\n\n-> To change capasity \t    press 2\n\n" <<
						"-> To change cost \t    press 3\n\n-> To cancel booking \t    press 4\n\n\-> To finish editig \t    press 5\n\n>>> ";
					std::cin >> answer;
					switch (answer)
					{
					case 1:
					{
						system("CLS");
						_rooms->operator[](i).print_record("admin");
						std::cout << "\nPress to continue " << std::endl;
						getchar();
						getchar();					
						
						break;
					}
					case 2:
					{
						size_t new_capacity = 0;
						std::cout << "\nEnter new capacity: ";
						std::cin >> new_capacity;
						getchar();
						if (new_capacity < 1)
						{
							std::cout << "\nWrong enter! Enter again, please  ";
							std::cin >> new_capacity;
							getchar();
						}
						if (new_capacity < 1)
						{
							std::cout << "\nWrong enter, try again later ";
							Sleep(2000);
							break;
						}
						_rooms->operator[](i).Set_capacity(new_capacity);
						std::cout << "\nChanging capacity was successfull.";
						Sleep(2000);
						break;
					}
					case 3:
					{
						size_t new_cost = 0;
						std::cout << "\nEnter new cost: ";
						std::cin >> new_cost;
						getchar();
						if (new_cost < 1)
						{
							std::cout << "\nWrong enter! Enter again, please  ";
							std::cin >> new_cost;
							getchar();
						}
						if (new_cost < 1)
						{
							std::cout << "\nWrong enter, try again later ";
							Sleep(2000);
							break;
						}
						_rooms->operator[](i).Set_cost(new_cost);
						std::cout << "\nChanging cost was successfull.";
						Sleep(2000);
						break;
					}
					case 4:
					{
						if (_rooms->operator[](i).Get_book_amount() == 0)
						{
							std::string answer = "\0";
							std::cout << "\nThere is no reservations for this room ";
							Sleep(3000);
							break;
						}
						Date_t booking_del(Create_book_date());
						if (_rooms->operator[](i).cancel_booking(booking_del))
						{
							std::cout << "\nCancelling was successfull.";
							Sleep(3000);
							break;
						}
						std::cout << "\nCancelling was unsuccessfull.";
						Sleep(3000);
						break;
					}
					case 5:
					{
						exit = false;
						break;
					}
					default:
					{
						std::cout << "\nWrong enter, try again!" << std::endl;
						Sleep(3000);
						break;
					}
					}
				}
				std::cout << "\nRoom number " << number << " was successfully edited.";
				Sleep(3000);
				return true;
			}
		}
		std::string answer = "\0";
		std::cout << "\nRoom not found\nWould you try again? (y/n) ";
		std::cin >> answer;
		if (answer == "n")
		{
			return false;
		}	
	}
}

bool I_Admin::_Change_profile_info()
{
	std::string login = "\0";
	std::string password = "\0";
	while (true)
	{
		system("CLS");
		std::cout << "Enter \"exit\" if you want to return to menu\n\nEnter new login (min size = 4): ";
		std::cin >> login;
		if (login == "exit")
		{
			return false;
		}
		else if (login.size() < 4)
		{
			std::cout << "\nLogin must be longer than 4 characters!\n" << std::endl;
			Sleep(3000);
			continue;
		}
		else if (login.size() > 18)
		{
			std::cout << "\nLogin is too long\n" << std::endl;
			Sleep(3000);
			continue;
		}
		bool check_ok = 1;
		for (size_t i = 0; i < _users->size(); ++i)
		{
			if (_users->operator[](i).login == login)
			{
				std::cout << "\nSame login already exists, choose another one\n" << std::endl;
				Sleep(3000);
				check_ok = 0;
				break;
			}
		}
		if (!check_ok)
		{
			continue;
		}
		std::cout << "Enter your password (min size = 8): ";
		std::cin >> password;
		if (password.size() < 8)
		{
			std::cout << "\nPassword must be longer than 8 characters!\n" << std::endl;
			Sleep(3000);
			continue;
		}
		else if (login.size() > 18)
		{
			std::cout << "\nPassword is too long\n" << std::endl;
			Sleep(3000);
			continue;
		}
		profile->login = login;
		profile->password = password;
		std::cout << "Login and password was successfully changed.";
		Sleep(3000);
		return true;
	}
}

void I_Admin::_Update_to_DB_room()
{
	system("CLS");
	std::string filename = "\0";
	std::cout << "Enter file name (or default) ";
	std::cin >> filename;
	if (filename == "default")
	{
		filename = "room_db.txt";
	}

	if (write_to_f_room(*_rooms, filename))//*
	{
		std::cout << "\nUpdate file " << filename << " was successfull\n";
		Sleep(3000);
	}
	else
	{
		std::cout << "\nUpdate file " << filename << " was unsuccessfull\n";
		Sleep(3000);
	}
}

void I_Admin::_Update_to_log_pass()
{
	system("CLS");
	std::string filename = "\0";
	std::cout << "Enter file name (or default) ";
	std::cin >> filename;
	if (filename == "default")
	{
		filename = "profiles.txt";
	}

	if (write_to_f_user(*_users, filename))//*
	{
		std::cout << "\nUpdate file " << filename << " was successfull\n";
		Sleep(3000);
	}
	else
	{
		std::cout << "\nUpdate file " << filename << " was unsuccessfull\n";
		Sleep(3000);
	}
}

void I_Admin::_Update_from_DB_room()
{
	system("CLS");
	std::string filename = "\0";
	std::cout << "Enter file name (or default) ";
	std::cin >> filename;
	if (filename == "default")
	{
		filename = "room_db.txt";
	}

	if (read_from_f_room(*_rooms, filename))//*
	{
		std::cout << "\nUpdate file " << filename << " was successfull\n";
		Sleep(3000);
	}
	else
	{
		std::cout << "\nUpdate file " << filename << " was unsuccessfull\n";
		Sleep(3000);
	}

}

void I_Admin::_Update_from_log_pass()
{
	system("CLS");
	std::string filename = "\0";
	std::cout << "Enter file name (or default) ";
	std::cin >> filename;
	if (filename == "default")
	{
		filename = "profiles.txt";
	}

	if (read_from_f_user(*_users, filename))//*
	{
		std::cout << "\nUpdate file " << filename << " was successfull\n";
		Sleep(3000);
	}
	else
	{
		std::cout << "\nUpdate file " << filename << " was unsuccessfull\n";
		Sleep(3000);
	}
}


//User

void I_User::Menu()
{
	bool exit = true;
	int answer = 0;
	while (exit)
	{
		system("CLS");
		std::cout << "\n-> To view your profile   press 1\n\n-> To change your profile press 2\n\n" <<
			"-> To make new booking \t  press 3\n\n-> To cancel your booking press 4\n\n-> To log out \t\t  press 5\n\n>>> ";
		std::cin >> answer;
		switch (answer)
		{
		case 1:
		{
			this->_Show_me();
			break;
		}
		case 2:
		{
			this->_Change_profile_info();
			break;
		}
		case 3:
		{
			this->_New_booking();
			break;
		}
		case 4:
		{
			this->_Cancel_booking();
			break;
		}
		case 5:
		{
			exit = false;
			break;
		}
		default:
		{
			std::cout << "\nWrong enter, try again!" << std::endl;
			Sleep(3000);
			break;
		}
		}
	}
}

bool I_User::_New_booking()
{
	Date_t booking(Create_book_date());
	size_t capacity_need = 0;
	std::cout << "\n\nEnter needed capacity ";
	std::cin >> capacity_need;
	getchar();

	std::list<size_t> number_list;
	for (size_t i = 0; i < _rooms->size(); ++i)//get()->
	{
		if (_rooms->operator[](i).Get_capacity() >= capacity_need) //.get()->operator[](i)
		{
			if (_rooms->operator[](i).check_date_for_booking(booking))//.get()->operator[](i)
			{
				number_list.push_back(i);
			}
		}
	}

	if (number_list.size() == 0)
	{
		std::cout << "\nNo room matching your request, please try again.\n";
		Sleep(3000);
		return false;
	}
	system("CLS");
	std::cout << "All matching results: \n";
	for (auto ind : number_list)
	{
		double total_cost = count_total_cost(_rooms->operator[](ind).Get_cost(), booking);//.get()->operator[](ind)
		std::cout << " #index " << ind + 1 << ": ";
		_rooms->operator[](ind).print_record();//.get()->operator[](ind)
		std::cout << "\n\t\t\t\t\tTotal cost: " << total_cost << "\n\n";
	}
	size_t index = 0;
	std::cout << "\nTo cancel booking press 0.\n\nChoose room #index  ";
	std::cin >> index;
	getchar();
	if (index == 0)
	{
		return false;
	}
	--index;
	if (index > _rooms->size())//get()->
	{
		std::cout << "Wrong index, enter again, please ";
		std::cin >> index;
		getchar();
	}
	if (index > _rooms->size())//get()->
	{
		std::cout << "Wrong index, try again later ";
		Sleep(2000);
		return false;
	}
	if (_rooms->operator[](index).Add_booking(std::pair<std::string, Date_t>(profile->sur_name, booking)))
	{
		profile->bookings.push_back(std::pair<size_t, Date_t>(_rooms->operator[](index).Get_number(), booking));
		std::cout << "\nBooking was successfull.";
		Sleep(3000);
		return true;
	}
	std::cout << "\nSomething went wrong. Try again later.";
	Sleep(3000);
	return true;
}

bool I_User::_Cancel_booking()
{
	system("CLS");
	if (profile->bookings.empty())
	{
		std::cout << "\nThere is no booking you can cancel.";
	}
	else
	{
		int index;
		for (size_t i = 0; i < profile->bookings.size(); ++i)
		{
			std::cout << "#" << i + 1 << "\nRoom number " << profile->bookings[i].first << "\n\n" << profile->bookings[i].second.Get_string() << std::endl;
		}
		std::cout << "\nIf you don't want to cancel booking press 0\n\nChoose record #num which you want to cancel ";
		std::cin >> index;
		if (index == 0)
		{
			return false;
		}
		--index;
		time_t now = time(NULL);
		time_t first_date = profile->bookings[index].second.Get_check_in();
		if (first_date < now + 604800)
		{
			std::cout << "\nYou can't cancel this booking, it is less than a week.\n\nContact reception for more help.";
			Sleep(3000);
			return false;
		}
		size_t number = profile->bookings[index].first;
		for (size_t i = 0; i < _rooms->size(); ++i)
		{
			if (_rooms->operator[](i).Get_number() == number)
			{
				if (_rooms->operator[](i).Get_book_amount() == 0)
				{
					std::string answer = "\0";
					std::cout << "\nThere is no reservations for this room.\n\nMost likely your booking was alredy cancelled by administration.\nStill you can contact administration or reception to get more info";
					for (size_t i = index; i < profile->bookings.size() - 1; ++i)
					{
						profile->bookings[i] = profile->bookings[i + 1];
					}
					profile->bookings.pop_back();
					Sleep(6000);
					break;
				}
				if (_rooms->operator[](i).cancel_booking(profile->bookings[index].second))
				{
					std::cout << "\nCancelling was successfull.";
					for (size_t i = index; i < profile->bookings.size() - 1; ++i)
					{
						profile->bookings[i] = profile->bookings[i + 1];
					}
					profile->bookings.pop_back();
					Sleep(3000);
					break;
				}
				std::cout << "\nCancelling was unsuccessfull. There is no matching reservations for this room.\n\nMost likely your booking was alredy cancelled by administration.\nYou can contact administration or reception to get more info";
				for (size_t i = index; i < profile->bookings.size() - 1; ++i)
				{
					profile->bookings[i] = profile->bookings[i + 1];
				}
				profile->bookings.pop_back();
				Sleep(6000);
				break;
			}
		}
	}
}

bool I_User::_Change_profile_info()
{
	std::string surname = "\0";
	std::string name = split_string(profile->sur_name, surname);
	std::string login = "\0";
	std::string password = "\0";
	
	int answer = 0;
	bool exit = true;
	
	while (exit)
	{
		system("CLS");
		std::cout << "\n-> To change name \t press 1\n\n-> To change surname \t press 2\n\n" <<
			"-> To change login \t press 3\n\n-> To change password \t press 4\n\n\-> To finish editig \t press 5\n\n>>> ";
		std::cin >> answer;
		getchar();

		switch (answer)
		{
		case 1:
		{
			system("CLS");

			std::cout << "Enter new name ";
			std::cin >> name;
			profile->sur_name = name + " " + surname;
			std::cout << "Name changed successfully. " << std::endl;
			Sleep(3000);
			break;
		}
		case 2:
		{
			system("CLS");
			std::cout << "Enter new surname ";
			std::cin >> surname;
			profile->sur_name = name + " " + surname;
			std::cout << "Surame changed successfully. " << std::endl;
			Sleep(3000);
			break;
		}
		case 3:
		{
			while (true)
			{
				system("CLS");
				std::cout << "Enter \"exit\" if you want to return to menu\n\nEnter your login (min size = 4): ";
				std::cin >> login;
				if (login == "exit")
				{
					break;
				}
				else if (login.size() < 4)
				{
					std::cout << "\nLogin must be longer than 4 characters!\n" << std::endl;
					Sleep(3000);
					continue;
				}
				else if (login.size() > 18)
				{
					std::cout << "\nLogin is too long\n" << std::endl;
					Sleep(3000);
					continue;
				}
				bool check_ok = 1;
				for (size_t i = 0; i < _users->size(); ++i)
				{
					if (_users->operator[](i).login == login)
					{
						std::cout << "\nSame login already exists, choose another one\n" << std::endl;
						Sleep(3000);
						check_ok = 0;
						break;
					}
				}
				if (!check_ok)
				{
					continue;
				}
				profile->login = login;
				break;
			}
			std::cout << "Login changed successfully. " << std::endl;
			Sleep(3000);
			break;
		}
		case 4:
		{
			while (true)
			{
				system("CLS");
				std::cout << "Enter \"exit\" if you want to return to menu\n\nEnter your password (min size = 8): ";
				std::cin >> password;
				if (password == "exit")
				{
					break;
				}
				else if (password.size() < 8)
				{
					std::cout << "\nPassword must be longer than 8 characters!\n" << std::endl;
					Sleep(3000);
					continue;
				}
				else if (password.size() > 18)
				{
					std::cout << "\nPassword is too long\n" << std::endl;
					Sleep(3000);
					continue;
				}
				profile->password = password;
				break;
			}
			std::cout << "Password changed successfully. " << std::endl;
			Sleep(3000);
			break;
		}
		case 5:
		{
			exit = false;
			break;
		}
		default:
		{
			std::cout << "\nWrong enter, try again!" << std::endl;
			Sleep(3000);
			break;
		}
		}
	}
	return true;
}

void I_User::_Show_me()
{
	system("CLS");
	std::string surname = "\0";
	std::string name = split_string(profile->sur_name, surname);
	std::cout << "\nName " << name << "\n\nSurname " << surname << std::endl;
	if (profile->bookings.empty())
	{
		std::cout << "\nNo booking yet.";
	}
	else
	{
		for (size_t i = 0; i < profile->bookings.size(); ++i)
		{
			std::cout << "\n#" << i + 1 << "\tRoom number " << profile->bookings[i].first << "\n" << profile->bookings[i].second.Get_string() << std::endl;			
		}
	}
	std::cout << "\n\nPress to continue ";
	getchar();
	getchar();
}



//Guest

void I_Guest::Menu()
{
	bool exit = true;
	int answer = 0;
	while (exit)
	{
		system("CLS");
		std::cout << "\n-> To make new booking press 1\n\n-> To log out \t       press 2\n\n>>> ";
		std::cin >> answer;
		switch (answer)
		{
		case 1:
		{
			this->_New_booking();
			break;
		}
		case 2:
		{
			exit = false;
			break;
		}
		default:
		{
			std::cout << "\nWrong enter, try again!" << std::endl;
			Sleep(3000);
			break;
		}
		}
	}
}

bool I_Guest::_New_booking()
{
	system("CLS");
	Date_t booking(Create_book_date());
	
	size_t capacity_need = 0;
	std::cout << "\nEnter needed capacity ";
	std::cin >> capacity_need;
	getchar();

	std::list<size_t> number_list;
	for (size_t i = 0; i < _rooms->size(); ++i)//get()->
	{
		if (_rooms->operator[](i).Get_capacity() >= capacity_need) //.get()->operator[](i)
		{
			if (_rooms->operator[](i).check_date_for_booking(booking))//.get()->operator[](i)
			{
				number_list.push_back(i);
			}
		}
	}
	system("CLS");
	if (number_list.size() == 0)
	{
		std::cout << "\nNo room matching your request, please try again.\n";
		Sleep(3000);
		return false;
	}
	std::cout << "All matching results: \n\n";
	for (auto ind : number_list)
	{
		double total_cost = count_total_cost(_rooms->operator[](ind).Get_cost(), booking);//.get()->operator[](ind)
		std::cout << " #index " << ind + 1 << ":      ";
		_rooms->operator[](ind).print_record();//.get()->operator[](ind)
		std::cout << " Total cost: " << total_cost << "$\n\n";
	}
	size_t index = 0;
	std::cout << "\nTo cancel booking press 0.\n\nChoose room #index  ";
	std::cin >> index;
	getchar();
	if (index == 0)
	{
		return false;
	}
	--index;
	if (index > _rooms->size())//get()->
	{
		std::cout << "Wrong index, enter again, please ";
		std::cin >> index;
		getchar();
	}
	if (index > _rooms->size())//get()->
	{
		std::cout << "Wrong index, try again later ";
		Sleep(2000);
		return false;
	}
	std::string user_FIO = "\0";
	std::string second_str = "\0";
	for(;;)
	{
		std::cout << "\nEnter your initials, please (Name Surname): ";
		std::cin >> user_FIO >> second_str;
		user_FIO += " " + second_str;
		size_t counter = 0;
		for (size_t i = 0; i < user_FIO.size(); ++i)
		{
			if (user_FIO[i] == 32 || user_FIO[i] == 46 || user_FIO[i] == 44 || user_FIO[i] == 63 || user_FIO[i] == 33 || (user_FIO[i] > 48 && user_FIO[i] < 57))
			{
				++counter;
			}
		}
		if (counter > 1)
		{
			std::cout << "\nWrong initials were entered, enter must be like: Name Surname.\n Only literals and \"-\" are acceptable)n" << std::endl;
			Sleep(4000);
			continue;
		}
		break;
	}
	
	if (_rooms->operator[](index).Add_booking(std::pair<std::string, Date_t>(user_FIO, booking)))//.get()->operator[](index)
	{
		std::cout << "\nBooking was successfull.";
		Sleep(3000);
		return true;
	}
	std::cout << "\nSomething went wrong. Try again later.";
	Sleep(3000);
	return true;
}




/*

void I_Admin::Menu()
{
bool exit = true;
int answer = 0;
while (exit)
{
system("CLS");
std::cout << "\n-> To show all rooms \t press 1\n\n-> To add new room \t press 2\n\n" <<
"-> To edit room \t press 3\n\n-> To delete room \t press 4\n\n-> To make new booking \t press 5\n\n" <<
"-> To change admin login and password \t press 6\n\n-> To update database with rooms \t press 7\n\n" <<
"-> To update from database with rooms \t press 8\n\n-> To update file with login-password \t press 9\n\n" <<
"-> To update from file login-password \t press 10\n\n-> To log out \t press 11\n\n>>> ";
std::cin >> answer;
switch (answer)
{
case 1:
{
this->_Show_rooms();
break;
}
case 2:
{
this->_Add_room();
break;
}
case 3:
{
this->_Edit_room();
break;
}
case 4:
{
this->_Delete_room();
break;
}
case 5:
{
this->_New_booking();
break;
}
case 6:
{
this->_Change_log_info();
break;
}
case 7:
{
this->_Update_to_DB_room();
break;
}
case 8:
{
this->_Update_from_DB_room();
break;
}
case 9:
{
this->_Update_to_log_pass();
break;
}
case 10:
{
this->_Update_from_log_pass();
break;
}
case 11:
{
exit = false;
break;
}
default:
{
std::cout << "\nWrong enter, try again!" << std::endl;
Sleep(3000);
break;
}
}
}

}

void I_Admin::_Show_rooms()
{
if (_rooms->empty())//.get()->
{
std::cout << "\nThere are no  registered rooms in hotel.\n\n";
}
else
{
system("CLS");
for (size_t i = 0; i < _rooms->size(); ++i)//get()->
{
std::cout << "\n";
_rooms->operator[](i).print_record(access); //.get()->operator[](i)
}
std::string answer = "\0";
std::cout << "\n\nDo you want to clean old bookings? (y/n) ";
std::cin >> answer;
if (answer == "y")
{
size_t deleted_records = 0;
for (size_t i = 0; i < _rooms->size(); ++i)//get()->
{
deleted_records += _rooms->operator[](i).clean_past();//.get()->operator[](i)
}
std::cout << deleted_records << " records were deleted.\n";
}
}
Sleep(3000);
}

bool I_Admin::_Add_room()
{
size_t capacity = 0;
size_t number = 0;
double cost = 0;

while (true)
{
bool check_ok = 1;
system("CLS");
// number
std::cout << "To cancel adding room press 0\n\nEnter new room number: ";
std::cin >> number;
getchar();

if (number == 0)
{
return false;
}
else if (number < 0)
{
std::cout << "\nRoom number can't be less than 1.\n";
Sleep(3000);
continue;
}

for (size_t i = 0; i < _rooms->size(); ++i)//get()->
{
if (_rooms->operator[](i).Get_number() == number)//.get()->operator[](i)
{
std::cout << "\nSuch room already exists.\n";
Sleep(3000);
check_ok = false;
break;
}
}
if (!check_ok)
{
continue;
}

// capacity
std::cout << "Enter new room capacity: ";
std::cin >> capacity;
getchar();

if (capacity < 1 || capacity > 10)
{
std::cout << "\nWrong capacity value.\n";
Sleep(3000);
check_ok = false;
break;
}
if (!check_ok)
{
continue;
}

// cost
std::cout << "Enter new room cost per night: ";
std::cin >> cost;
getchar();

if (cost < 1.0)
{
std::cout << "\nWrong cost value.\n";
Sleep(3000);
check_ok = false;
break;
}
if (!check_ok)
{
continue;
}
else
{
std::string answer = "\0";
//create
_rooms->push_back(Room(capacity, cost, number));//get()->
sort_rooms(*_rooms);//* ssylka? sort
std::cout << "\nRoom " << number << " was added.\n\nDo you want to add more room? (y/n) ";
std::cin >> answer;
if (answer == "y")
{
continue;
}
else
{
return true;
}
}
}
}

bool I_Admin::_Delete_room()
{
size_t number = 0;
while (true)
{
system("CLS");
std::cout << "To delete room enter room number: ";
std::cin >> number;
getchar();

for (size_t i = 0; i < _rooms->size(); ++i)//get()->
{
if (_rooms->operator[](i).Get_number() == number)//.get()->operator[](i)
{
for (size_t j = i; j < _rooms->size()-1; ++j)//get()->
{
_rooms[j] = _rooms[j+1];// .get()->operator[](j)  .get()->operator[](j + 1)
}
_rooms->pop_back(); //get()->
std::cout << "Room number " << number << " was successfully deleted.";
Sleep(3000);
return true;
}
}
std::string answer = "\0";
std::cout << "\nRoom not found\nWould you try again? (y/n) ";
std::getline(std::cin, answer);
if (answer == "n")
{
return false;
}
}
}

bool I_Admin::_New_booking()
{
Date_t booking(Create_book_date());
system("CLS");
size_t capacity_need = 0;
std::cout << "\nEnter needed capacity ";
std::cin >> capacity_need;
getchar();

std::list<size_t> number_list;
for (size_t i = 0; i < _rooms->size(); ++i)//get()->
{
if (_rooms->operator[](i).Get_capacity() >= capacity_need) //.get()->operator[](i)
{
if (_rooms->operator[](i).check_date_for_booking(booking))//.get()->operator[](i)
{
number_list.push_back(i);
}
}
}

if (number_list.size() == 0)
{
std::cout << "\nNo room matching your request, please try again.\n";
Sleep(3000);
return false;
}
system("CLS");
std::cout << "All matching results: \n";
for (auto ind : number_list)
{
double total_cost = count_total_cost(_rooms->operator[](ind).Get_cost(), booking);//.get()->operator[](ind)
std::cout << " #index " << ind+1 << ": ";
_rooms->operator[](ind).print_record();//.get()->operator[](ind)
std::cout << "\n\t\t\t\t\tTotal cost: " << total_cost << "\n\n";
}
size_t index = 0;
std::cout << "\nTo cancel booking press 0.\n\nChoose room #index  ";
std::cin >> index;
getchar();
if (index == 0)
{
return false;
}
--index;
if (index > _rooms->size())//get()->
{
std::cout << "Wrong index, enter again, please ";
std::cin >> index;
getchar();
}
if (index > _rooms->size())//get()->
{
std::cout << "Wrong index, try again later ";
Sleep(2000);
return false;
}
std::string user_FIO = "\0"; /// CHECK
std::string second_str = "\0";
std::cout << "\nEnter customer initials, please (Surname Name): ";
std::cin >> user_FIO >> second_str;
user_FIO += " " + second_str;
if (_rooms->operator[](index).Add_booking(std::pair<std::string, Date_t>(user_FIO, booking)))//.get()->operator[](index)
{
std::cout << "\nBooking was successfull.";
Sleep(3000);
return true;
}
std::cout << "\nSomething went wrong. Try again later.";
Sleep(3000);
return true;
}

bool I_Admin::_Edit_room()
{
//menu
return 0;
}

bool I_Admin::_Change_log_info()
{
//password and login;
return 0;
}

void I_Admin::_Update_to_DB_room()
{
system("CLS");
std::string filename = "\0";
std::cout << "Enter file name (or default) ";
std::cin >> filename;
if (filename == "default")
{
filename = "room_db.txt";
}

if (write_to_f_room(*_rooms, filename))//*
{
std::cout << "\nUpdate file " << filename << " was successfull\n";
Sleep(3000);
}
else
{
std::cout << "\nUpdate file " << filename << " was unsuccessfull\n";
Sleep(3000);
}
}

void I_Admin::_Update_to_log_pass()
{
system("CLS");
std::string filename = "\0";
std::cout << "Enter file name (or default) ";
std::cin >> filename;
if (filename == "default")
{
filename = "log_pass.txt";
}

if (write_to_f_log(*_logs, filename))//*
{
std::cout << "\nUpdate file " << filename << " was successfull\n";
Sleep(3000);
}
else
{
std::cout << "\nUpdate file " << filename << " was unsuccessfull\n";
Sleep(3000);
}
}

void I_Admin::_Update_from_DB_room()
{
system("CLS");
std::string filename = "\0";
std::cout << "Enter file name (or default) ";
std::cin >> filename;
if (filename == "default")
{
filename = "room_db.txt";
}

if (read_from_f_room(*_rooms, filename))//*
{
std::cout << "\nUpdate file " << filename << " was successfull\n";
Sleep(3000);
}
else
{
std::cout << "\nUpdate file " << filename << " was unsuccessfull\n";
Sleep(3000);
}

}

void I_Admin::_Update_from_log_pass()
{
system("CLS");
std::string filename = "\0";
std::cout << "Enter file name (or default) ";
std::cin >> filename;
if (filename == "default")
{
filename = "log_pass.txt";
}

if (read_from_f_log(*_logs, filename))//*
{
std::cout << "\nUpdate file " << filename << " was successfull\n";
Sleep(3000);
}
else
{
std::cout << "\nUpdate file " << filename << " was unsuccessfull\n";
Sleep(3000);
}
}

//Sleep(3000);
*/