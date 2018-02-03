#pragma once
#include <iostream>
#include <string>
#include <initializer_list>
#include <exception>


class data_exception : public std::exception
{
public:
	data_exception(char const* e = "Data exception") throw() : exception(e) {}
};

	////////////////////////////////////////////////////////////////
	//                       VECTOR
	////////////////////////////////////////////////////////////////

template <typename T>
class Vector
{
public:
	Vector();
	Vector(T el);
	Vector(std::initializer_list<T> l);
	~Vector();
	size_t GetSize() const; //vydelennaja pam'at'
	void SetSize(size_t size, size_t grow = 1); //vydelenie pam'ati s shgom na uvelichenie
	size_t GetUpperBound() const; //poluchenie poslednego dopustimogo indexsa
	bool IsEmpty(); // Pust?
	void FreeExtra(); //udalenie vsej pam'ati sverh poslednego indexsa
	void RemoveAll(); //udalit' vse
	T GetAt(size_t ind) const; //poluchit' po indexu
	void SetAt(size_t ind, T val); //zapisat' po indexsu
								   //operator=; operator[];
	void Add(T value); // dobavit' v konec;
	void Add(std::initializer_list<T> l); // dobavit' v konec init_list;
	void Append(Vector& const v); //slozhenie dvuh massivov
	T* GetData(); //poluchenie ssylki
	void InsertAt(size_t ind, T value); //Vstavit' po indexsu so smesheniem
	void RemoveAt(size_t ind); //Udalit' po indexsu so smesheniem
	void Show();

	T& operator [](size_t ind);
	void operator =(Vector const& v);
private:
	T* _vec = NULL; // ukazatel' na massiv
	size_t _filled = 0; // zapolneno
	size_t _size = 0; // vydelennaja pam'at' (ispol'zovat' size_t)
	size_t _grow = 10; // shag uvelichenija 
};

////////////////////////////////////////////////////////////////
//                       FUNCTIONS
////////////////////////////////////////////////////////////////

template<typename T>
Vector<T>::Vector()
{
	_vec = new T[_grow];
	_size = _grow;
}

template<typename T>
Vector<T>::Vector(std::initializer_list<T> l)
{
	_size = l.size();
	_size += _size%_grow ? (_grow - (_size%_grow)) : 0;
	_vec = new T[_size];
	for (auto elem : l)
	{
		_vec[_filled++] = elem;
	}
}

template<typename T>
Vector<T>::Vector(T el)
{
	_vec = new T[_grow];
	_vec[0] = el;
	_size = _grow;
	_filled = 1;
}

template<typename T>
Vector<T>::~Vector()
{
	delete[] _vec;
}

template<typename T>
size_t Vector<T>::GetSize() const //vydelennaja pam'at'
{
	return _size;
}

template<typename T>
void Vector<T>::SetSize(size_t size, size_t grow = 10) //vydelenie pam'ati s shagom na uvelichenie
{
	size_t c_size = _size < size ? _size : size;
	T* tmp = new T[size];
	memcpy(tmp, _vec, sizeof(T)*c_size);
	delete[] _vec;
	_vec = tmp;
	_size = size;
	_grow = grow;
	_filled = _filled > size ? size : _filled;
}

template<typename T>
size_t Vector<T>::GetUpperBound() const //poluchenie poslednego dopustimogo indexsa
{
	return _filled>0 ? _filled-1 : 0;
}

template<typename T>
bool Vector<T>::IsEmpty() // Pust?
{
	if (_filled == 0)
	{
		return true;
	}
	return false;
}

template<typename T>
void Vector<T>::FreeExtra() //udalenie vsej pam'ati sverh poslednego indexsa
{
	T* tmp = new T[_filled];
	memcpy(tmp, _vec, sizeof(T)*_filled);
	delete[] _vec;
	_vec = tmp;
	_size = _filled;
}

template<typename T>
void Vector<T>::RemoveAll() //udalit' vse
{
	delete[] _vec;
	_vec = NULL;
	_size = 0;
	_filled = 0;
}

template<typename T>
T& Vector<T>::operator [](size_t ind)
{
	return _vec[ind];
}

template<typename T>
T Vector<T>::GetAt(size_t ind) const //poluchit' po indexu
{
	if (ind > _filled-1) { throw data_exception("Index out of filled range"); }
	else
	{
		return _vec[ind];
	}
}

template<typename T>
void Vector<T>::SetAt(size_t ind, T value) //zapisat' po indexsu
{
	if (ind > _filled) { throw data_exception("Index out of range"); }
	else
	{
		_vec[ind] = value;
	}
}

template<typename T>
void Vector<T>::Add(T value)  // dobavit' v konec;
{
	if (_filled == _size)
	{
		this->SetSize(_size + _grow, _grow);
	}
	_vec[_filled++] = value;
}

template<typename T>
void Vector<T>::Add(std::initializer_list<T> l)
{
	size_t fill_size = l.size()+_filled;
	_size += _size%_grow ? (_grow - (_size%_grow)) : 0;
	
	if (fill_size >= _size)
	{
		fill_size += fill_size%_grow ? (_grow - (fill_size%_grow)) : 0;
		this->SetSize(fill_size, _grow);
	}
	for (auto elem : l)
	{
		_vec[_filled++] = elem;
	}
}

template<typename T>
void Vector<T>::Append(Vector& const v) //slozhenie dvuh massivov
{
	if (_filled + v._filled > _size)
	{
		this->SetSize(_filled + v._filled + _grow-((_filled + v._filled)%_grow), _grow);
	}
	for (size_t i = 0; i < v._filled; i++)
	{
		this->Add(v[i]);
	}
}

template<typename T>
T* Vector<T>::GetData()  //poluchenie ssylki
{
	return _vec;
}

template<typename T>
void Vector<T>::InsertAt(size_t ind, T value)  //Vstavit' po indexsu so smesheniem
{
	if (ind > _filled-1) { throw data_exception("Index out of filled range"); }
	if (_filled == _size)
	{
		this->SetSize(_size + _grow, _grow);
	}
	for (size_t i = _filled; i > ind; i--)
	{
		_vec[i] = _vec[i-1];		
	}
	_vec[ind] = value;
	_filled++;
}

template<typename T>
void Vector<T>::RemoveAt(size_t ind)  //Udalit' po indexsu so smesheniem
{
	if (ind > _filled-1){ throw data_exception("Index out of filled range"); }
	for (int i = ind; i < _filled-1; i++)
	{
		_vec[i] = _vec[i+1];
	}
	_vec[--_filled] = 0;
		if (_size - _filled >= _grow)
	{
		this->SetSize(_size - _grow, _grow);
	}
}

template<typename T>
void Vector<T>::operator =(Vector const& v)
{
	delete[] _vec;
	_size = v.GetUpperBound()+1;
	_size += _size%_grow ? (_grow - (_size%_grow)) : 0;
	_vec = new T[_size];
	memcpy(_vec, v._vec, sizeof(T)*v._filled);
	_filled = v._filled;
}

template<typename T>
void Vector<T>::Show()
{
	for (size_t i = 0; i < _filled; i++)
	{
		std::cout << _vec[i] << "  ";
	}
	std::cout << std::endl;
}