#include "area.h"
#include <vector>
#include <iostream>
#include <string>

double Triangle::area()
{
	std::cout << "Triangle: ";
	return double(_a*_h) / 2;
}

double Trapeze::area()
{
	std::cout << "Trapeze: ";
	return double(_a+_b) / 2 *_h;
}

double Circle::area()
{
	std::cout << "Circle: ";
	return 3.1415*(_r*_r);
}

double Square::area()
{
	std::cout << "Square: ";
	return double(_a*_b);
}

int main()
{
	std::vector<Area*> areas;
	Triangle A(3, 4);
	Circle B(9);
	Square D(5, 4);
	Area* a = &A;
	areas.push_back(a);
	areas.push_back(&B);
	areas.push_back(&D);
	areas.push_back(&Trapeze(2, 4, 3));
	for (auto elem : areas)
	{
		std::cout << elem->area() << std::endl;
	}
	

	return 0;
}