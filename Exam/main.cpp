#include <windows.h>
#include <memory>
#include <iostream>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <utility>

#include "exam.h"
#include "Interface.h"



int main()
{
	std::vector<User_profile> users;
	std::vector<Room> rooms;
	std::string file_users = "profiles.txt";
	std::string file_hotel = "room_db.txt";
	Interface* I;
	if (!read_from_f_user(users, file_users))
	{
		users.push_back(User_profile("No name", "admin", "admin", std::vector<std::pair<size_t, Date_t>>()));
		I = &I_Admin(&rooms, &users, &users[0]);
		I->Menu();
	}
	else
	{
		read_from_f_room(rooms, file_hotel);
	}	
	int pause = 3000;
	bool exit = true;
	// Main
	while (exit == true)
	{
		system("CLS");
		int main_answer = 0;
		std::cout << "Welcome!\n\nHere you can book a room in Hotel\n\n\t\"Hampton by Hilton Minsk City Center\"" <<
			"\n\n-> To Log in \t\t press 1\n\n-> To Register \t\t press 2\n\n-> To continue as guest  press 3\n\n-> To exit program \t press 4\n\n>>> ";
		std::cin >> main_answer;
		switch (main_answer)
		{
		case 1: // Log In
		{
			std::string login = "\0";
			std::string password = "\0";
			while (true)
			{
				system("CLS");
				std::cout << "Enter \"exit\" if you want to return to main menu\n\nEnter your login (min size = 4): ";
				std::cin >> login;
				if (login == "exit")
				{
					break;
				}
				else if (login.size() < 4)
				{
					std::cout << "\nLogin must be longer than 4 characters!\n" << std::endl;
					Sleep(pause);
					continue;
				}
				else if (login.size() > 18)
				{
					std::cout << "\nLogin is too long\n" << std::endl;
					Sleep(pause);
					continue;
				}

				std::cout << "Enter your password (min size = 8): ";
				std::cin >> password;
				if (password.size() < 8 && password != users[0].password)
				{
					std::cout << "\nPassword must be longer than 8 characters!\n" << std::endl;
					Sleep(pause);
					continue;
				}
				else if (login.size() > 18)
				{
					std::cout << "\nPassword is too long\n" << std::endl;
					Sleep(pause);
					continue;
				}

				if (users[0].login == login)
				{
					if (users[0].password == password)
					{
						I = &I_Admin(&rooms, &users, &users[0]);
						I->Menu();
						write_to_f_room(rooms, file_hotel);
						write_to_f_user(users, file_users);
						break;
					}
				}
				bool exit = 0;
				for (size_t i = 0; i < users.size(); i++)
				{
					if (users[i].login == login)
					{
						if (users[i].password == password)
						{
							//dat' dostup USER
							I = &I_User(&rooms, &users, &users[i]);
							I->Menu();
							write_to_f_room(rooms, file_hotel);
							write_to_f_user(users, file_users);
							exit = 1;
							break;
						}
						else
						{
							std::cout << "Wrong password!\n" << std::endl;
							Sleep(pause);
							break;
						}
					}
				}
				if (exit)
				{
					break;
				}
				std::cout << "Wrong login!" << std::endl;
				Sleep(pause);
			}
			break;
		}
		case 2: // Registration
		{
			std::string login = "\0";
			std::string password = "\0";
			while (true)
			{
				system("CLS");
				std::cout << "Enter \"exit\" if you want to return to main menu\n\nEnter your login (min size = 4): ";
				std::cin >> login;
				if (login == "exit")
				{
					break;
				}
				else if (login.size() < 4)
				{
					std::cout << "\nLogin must be longer than 4 characters!\n" << std::endl;
					Sleep(pause);
					continue;
				}
				else if (login.size() > 18)
				{
					std::cout << "\nLogin is too long\n" << std::endl;
					Sleep(pause);
					continue;
				}
				bool check_ok = 1;
				for (size_t i = 0; i < users.size(); ++i)
				{
					if (users[i].login == login)
					{
						std::cout << "\nSame login already exists, choose another one\n" << std::endl;
						Sleep(3000);
						check_ok = 0;
						break;
					}
				}
				std::cout << "Enter your password (min size = 8): ";
				std::cin >> password;
				if (password.size() < 8)
				{
					std::cout << "\nPassword must be longer than 8 characters!\n" << std::endl;
					Sleep(pause);
					continue;
				}
				else if (password.size() > 18)
				{
					std::cout << "\nPassword is too long\n" << std::endl;
					Sleep(pause);
					continue;
				}
				std::string user_FIO = "\0"; 
				std::string second_str = "\0";
				for (;;)
				{
					std::cout << "\nEnter your initials, please (Name Surname): ";
					std::cin >> user_FIO >> second_str;
					user_FIO += " " + second_str;
					size_t counter = 0;
					for (size_t i = 0; i < user_FIO.size(); ++i)
					{
						//std::cout << user_FIO[i];
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
				users.push_back(User_profile(user_FIO, login, password, std::vector<std::pair<size_t, Date_t>>()));
				write_to_f_user(users, file_users);
				std::cout << "\nRegistration succeed!\n";
				Sleep(pause);
				break;
			}
			break;
		}
		case 3: // Guest
		{
			system("CLS");
			std::cout << "Welcome guest!" << std::endl;
			I = &I_Guest(&rooms);
			I->Menu();
			break;
		}
		case 4: // Exit program
		{
			write_to_f_room(rooms, file_hotel);
			write_to_f_user(users, file_users);
			system("CLS");
			exit = false;
			std::cout << "\n\n\n\n\n\n\n\n\n\n\n\t\t\t\tBye bye!\n\n\n\n\n\n\n" << std::endl;
			Sleep(pause);
			break;
		}
		default:
			std::cout << "\nWrong enter, try again!" << std::endl;
			Sleep(2000);
			break;
		}
	}

	return 0;
}

