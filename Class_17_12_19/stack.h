#pragma once

#include <iostream>

template<typename T>
class Stack
{
	struct Data
	{
		T _data;
		Data* _previous = nullptr;
	};

public:
	Stack() = default;
	Stack(std::initializer_list<T> l);
	Stack(Stack<T> const& s);
	~Stack();

	void Push_back(T const& data);
	T Pop_back();
	void Insert(size_t itr, T const& data);
	T Remove(size_t itr);

	Stack<T> operator +(Stack<T> const& s);
	Stack<T> operator =(Stack<T> const& s);
	T& operator [](size_t itr);
	T& operator [](size_t itr) const;

	size_t Count();
	void Show();
	void ShowReverse();

private:
	void SRec(Data* item);
	void Rek_copy(Data* elem);
	Data* _last = nullptr;
};


template<typename T>
Stack<T>::~Stack()
{
	size_t size = Count();
	Data* temp;
	for (size_t i = 0; i < size; i++)
	{
		if (_last->_previous)
		{
			temp = _last->_previous;
			delete _last;
			_last = temp;
		}
		else
		{
			delete _last;
		}
	}
}

template<typename T>
Stack<T>::Stack(Stack<T> const& s)
{
	Rek_copy(s._last);
}

template<typename T>
Stack<T>::Stack(std::initializer_list<T> l)
{
	size_t size = l.size();
	for (auto& elem : l)
	{
		Push_back(elem);
	}
}

template<typename T>
void Stack<T>::Push_back(T const& data)
{
	Data* temp = new Data;
	temp->_data = data;
	temp->_previous = _last;
	_last = temp;
}

template<typename T>
T Stack<T>::Pop_back()
{
	if (_last)
	{
		T elem = std::move(_last->_data);
		Data* temp = _last;
		_last = temp->_previous;
		delete temp;
		return std::move(elem);
	}
	return T();
}

template<typename T>
Stack<T> Stack<T>::operator +(Stack<T> const& s)
{
	Rek_copy(s._last);
	return *this;
}

template<typename T>
Stack<T> Stack<T>::operator =(Stack<T> const& s)
{
	if (&s == this)
	{
		return *this;
	}
	Data* temp;
	while (_last)
	{
		if (_last->_previous)
		{
			temp = _last->_previous;
			delete _last;
			_last = temp;
		}
		else
		{
			delete _last;
			_last = nullptr;
		}
	}
	Rek_copy(s._last);
	return *this;
}

template<typename T>
void Stack<T>::Insert(size_t itr, T const& data)
{
	size_t size = Count();
	size -= itr;
	if (size == 0)
	{
		Push_back(data);
	}
	else if (size > 0)
	{
		Data* temp = _last;
		for (size_t i = 0; i < size; i++)
		{
			if (i != size - 1)
			{
				temp = temp->_previous;
			}			
		}
		Data* new_temp = new Data;
		new_temp->_data = data;
		new_temp->_previous = temp->_previous;
		temp->_previous = new_temp;
	}
}

template<typename T>
T Stack<T>::Remove(size_t itr)
{
	size_t size = Count();
	size -= itr+1;
	if (size == 0)
	{
		return std::move(Pop_back());
	}
	else if (size > 0)
	{
		Data* temp = _last;
		for (size_t i = 0; i < size; i++)
		{
			//std::cout << temp->_data << std::endl;
			if (i != size - 1)
			{
				temp = temp->_previous;
			}
		}
		Data* previous = temp->_previous;
		T data = previous->_data;
		temp->_previous = previous->_previous;
		delete previous;
		return std::move(data);
	}	
	return T();
}

template<typename T>
T& Stack<T>::operator [](size_t itr)
{
	size_t size = Count();
	size -= itr;
	Data* temp = _last;
	for (size_t i = 0; i < size; i++)
	{
		if (i != size - 1)
		{
			temp = temp->_previous;
		}
	}
	return temp->_data;
}

template<typename T>
T& Stack<T>::operator [](size_t itr) const
{
	size_t size = Count();
	size -= itr;
	Data* temp = _last;
	for (int i = 0; i < size; i++)
	{
		if (i != size - 1)
		{
			temp = temp->_previous;
		}
	}
	return temp->_data;
}

template<typename T>
void Stack<T>::Rek_copy(Data* elem)
{
	if (elem->_previous != nullptr) 
	{
		Rek_copy(elem->_previous);
	}
	Stack<T>::Push_back(elem->_data);
}

template<typename T>
size_t Stack<T>::Count()
{
	size_t size = 0;
	Data* temp = _last;
	while (true)
	{
		if (temp == nullptr)
		{
			return size;
		}
		temp = temp->_previous;
		++size;
	}
}

template<typename T>
void Stack<T>::ShowReverse()
{
	Data* temp = _last;
	while (true)
	{
		if (temp == nullptr)
		{
			std::cout << std::endl;
			break;
		}
		std::cout << temp->_data << "   ";
		temp = temp->_previous;
	}
}

template<typename T>
void Stack<T>::Show()
{
	SRec(_last);
	std::cout << std::endl;
}

template<typename T>
void Stack<T>::SRec(Data* item)
{
	if (item->_previous != nullptr)
	{
		SRec(item->_previous);
	}
	std::cout << item->_data << "   ";
}