
#include "emploee.h"

void Manager::print() 
{ 
	std::cout << "Manager " << _name << "\nSalary: " << _salary << "\n" << std::endl; 
}

void Worker::print()
{
	std::cout << "Worker " << _name << "\nSalary: " << _salary << "\n" << std::endl;
}

void Prezident::print()
{
	std::cout << "Officer " << _name << "\nSalary: " << _salary << "\n" << std::endl;
}

int main()
{
	Manager A("Jane Miller", 150);
	Worker B("Pete Freeman", 250);
	Prezident C("Bob Wolf", 344);
	Employee D("Fred Pit", 99);
	A.print();
	B.print();
	C.print();
	D.print();
	return 0;
}