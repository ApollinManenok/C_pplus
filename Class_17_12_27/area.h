#pragma once

#include <iostream>
#include <string>

class Area
{
public:
	virtual double area() = 0;
};

class Triangle : public Area
{
public:
	Triangle(int a, int h) : _a(a), _h(h){}
	double area() override;
private:
	int _a = 0;
	int _h = 0;
};

class Square : public Area
{
public:
	Square(int a, int b) : _a(a), _b(b) {}
	double area() override;
private:
	int _a = 0;
	int _b = 0;
};

class Trapeze : public Area
{
public:
	Trapeze(int a, int b, int h) : _a(a), _b(b), _h(h) {}
	double area() override;
private:
	int _a = 0;
	int _b = 0;
	int _h = 0;
};

class Circle : public Area
{
public:
	Circle(int r) : _r(r) {}
	double area() override;
private:
	int _r = 0;
};