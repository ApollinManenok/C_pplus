#pragma once
#include <iostream>
#include <string>


class Employee
{
public:
	Employee(std::string name, int salary) : _name(name), _salary(salary) { }
	virtual void print() { std::cout << "Employee " << _name << "\n Salary: " << _salary << "\n" << std::endl; }
protected:
	std::string _name = "None";
	int _salary = 0;
};


class Manager : public Employee
{
public:
	Manager(std::string name, int salary) : Employee(name, salary) {}
	void print() override;
};


class Worker : public Employee
{
public:
	Worker(std::string name, int salary) : Employee(name, salary) {}
	void print() override;
};


class Prezident : public Employee
{
public:
	Prezident(std::string name, int salary) : Employee(name, salary) {}
	void print() override;
};