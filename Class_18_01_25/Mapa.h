#pragma once
#include <iostream>
#include <string>
#include <initializer_list>

template <typename K, typename V >
class Mapa
{
	template<typename p_K, typename p_V>
	struct Para
	{
		p_K first;
		p_V second;
	};
public:
	Mapa() = default;
	Mapa(K key, V val);
	Mapa(std::pair<K, V> pair);
	Mapa(Mapa<K, V> &const Map); //copy constructor
	Mapa(Mapa<K, V>&& Map); //move constructor
	Mapa(std::initializer_list<std::pair<K, V>> l);
	~Mapa();

	size_t GetSize() const; //vydelennaja pam'at'
	size_t GetUpperBound() const; //kol-vo zapolnennyh elementov
	Para<const K, V>* begin() const; //poluchenie ssylki na nachalo mapy, modifikacija key v const key
	Para<const K, V>* end() const; //poluchenie poslednego dopustimogo indexsa, modifikacija key v const key
	
	bool Empty(); // Pust _map?
	bool findIf(K key); //Est' key?

	size_t Insert(K key, V val); // dobavit' paru s proverkoj na nalichie kl'ucha, sortirovka
	void Erase(K key); //Udalit' po indexsu so smesheniem
	V& At(K key); //dostup k value po key ili nulevoe znachenie, bez sozdanija pary
	void Append(Mapa<K, V> &const Map, bool overwrite = false); //slozhenie dvuh map, default bez perezapisi
	void Swap(Mapa<K, V> &Map); // obmen znachenij 
	void FreeExtra(); //udalenie vsej pam'ati sverh poslednej pary
	void Clear(); // udalit' vse i zanulit'
	void Show(); // vyvod na ekran

	V& operator [](K key); // dostup po kluchu ili sozdanie s zanuleniem
	Mapa<K, V>& operator =(const Mapa<K,V>& Map); //return *this
	Mapa<K, V>& operator = (Mapa<K, V>&& Map); //return *this

private:
	void Grow(size_t size); // Uvelichenie _map
	void Decrease(size_t size); // Umen'shenie _map
	Para<K, V> *_map = nullptr;
	size_t _size = 0; // dopustimoe kol-vo elementov
	size_t _filled = 0; // zapolneno
	size_t _grow = 10; //ne imeet vneshnego dostupa
};

template<typename K, typename V>
Mapa<K, V>::Mapa(K key, V val)
{
	_map = new Para<K, V>[_grow];
	_map[0].first = key;
	_map[0].second = val;
	_size = 1;
	_filled = 1; 
}

template<typename K, typename V>
Mapa<K, V>::Mapa(std::pair<K, V> pair)
{
	_map = new Para<K, V>[_grow];
	_map[0].first = pair.first;
	_map[0].second = pair.second;
	_size = 1;
	_filled = 1;
}

template<typename K, typename V>
Mapa<K, V>::Mapa(Mapa<K, V> &const Map) //copy constructor
{
	_size = Map._size;
	_filled = Map._filled;
	_map = new Para<K, V>[_size];
	memcpy(_map, Map._map, sizeof(Para<K,V>)*(_filled));
}

template<typename K, typename V>
Mapa<K, V>::Mapa(Mapa<K, V>&& Map) //move constructor
{
	_map = Map._map;
	_size = Map._size;
	_filled = Map._filled; 
	Map._map = nullptr;
	Map.~Mapa();
}

template<typename K, typename V>
Mapa<K, V>::Mapa(std::initializer_list<std::pair<K, V>> l)
{
	_size = l.size();
	_size += _size%_grow ? (_grow - (_size%_grow)) : 0;
	_map = new Para<K, V>[_size];
	for (auto elem : l)
	{
		this->Insert(elem.first, elem.second);
	}
}

template<typename K, typename V>
Mapa<K, V>::~Mapa()
{
	this->Clear();
}

template<typename K, typename V>
size_t Mapa<K, V>::GetSize() const //vydelennaja pam'at'
{
	return _size;
}

template<typename K, typename V>
size_t Mapa<K, V>::GetUpperBound() const //poslednij dopustimyj element
{
	return _filled-1;
}

template<typename K, typename V>
Mapa<K, V>::Para<const K, V>* Mapa<K, V>::begin() const //poluchenie ssylki na nachalo mapy
{
	return &_map[0];
}

template<typename K, typename V>
Mapa<K, V>::Para<const K, V>* Mapa<K, V>::end() const //poluchenie poslednego dopustimogo indexsa
{
	return &_map[_filled];
}

template<typename K, typename V>
bool Mapa<K, V>::Empty() // Pust?
{
	if (_filled > 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

template<typename K, typename V>
bool Mapa<K, V>::findIf(K key) //Est'?
{
	for (size_t i = 0; i<_filled; i++)
	{
		if (_map[i].first == key)
		{
			return i;
		}
	}
	return 0;
}

template<typename K, typename V>
size_t Mapa<K, V>::Insert(K key, V val) // dobavit' paru s proverkoj na nalichie kl'ucha;	
{
	if (!this->findIf(key))
	{
		size_t result = _filled;
		if (_filled == _size)
		{
			this->Grow(_size + _grow);
		}
		for (size_t i = 0; i < _filled; i++)
		{
			if (_map[i].first > key)
			{
				result = i;
				break;
			}
		}
		for (size_t j = _filled; j > result; j--)
		{
			_map[j] = _map[j - 1];
		}
		_map[result].first = key;
		_map[result].second = val;
		_filled++;
		return std::move(result);
	}
	else
	{
		return std::move(this->findIf(key));
	}
}

template<typename K, typename V>
void Mapa<K, V>::Erase(K key) //Udalit' po indexsu so smesheniem
{
	size_t result = _filled;
	for (size_t i = 0; i < _filled; i++)
	{
		if (_map[i].first == key)
		{
			result = i;
			break;
		}
	}
	if (result != _filled)
	{
		for (size_t j = result; j < _filled - 1; j++)
		{
			_map[j] = _map[j + 1];
		}
		_filled -= 1;
		_map[_filled].first = K();
		_map[_filled].second = V();
		if (_size - _filled >= _grow)
		{
			this->Decrease(_size - _grow);
		}
	}
}

template<typename K, typename V>
V& Mapa<K, V>::At(K key) //dostup k key ili null bez sozdanija
{
	for (size_t i = 0; i<_filled; i++)
	{
		if (_map[i].first == key)
		{
			return _map[i].second;
		}
	}
	V result = V();
	return result;
}

template<typename K, typename V>
void Mapa<K, V>::Append(Mapa<K, V> &const Map, bool overwrite = false) //slozhenie dvuh map
{
	if (overwrite)
	{
		for (size_t i = 0; i < Map._filled; i++)
		{
			this->operator[](Map._map[i].first) = Map._map[i].second;
		}
	}
	else
	{
		for (size_t i = 0; i < Map._filled; i++)
		{
			this->Insert(Map._map[i].first, Map._map[i].second);
		}
	}
}

template<typename K, typename V>
void Mapa<K, V>::Swap(Mapa<K, V> &Map) // obmen znachenij
{
	Para<K, V> *tmp = _map;
	size_t tmp_size = _size;
	size_t tmp_filled = _filled;
	_map = Map._map;
	_size = Map.GetSize();
	_filled = Map.GetUpperBound() + 1;
	Map._map = tmp;
	Map._size = tmp_size;
	Map._filled = tmp_filled;
}

template<typename K, typename V>
void Mapa<K, V>::FreeExtra() //udalenie vsej pam'ati sverh poslednego indexsa
{
	Para<K, V>* tmp = new Para<K, V>[_filled];
	memcpy(tmp, _map, sizeof(Para<K, V>)*_filled);
	delete[] _map;
	_map = tmp;
	_size = _filled;
}

template<typename K, typename V>
void Mapa<K, V>::Clear() //udalit' vse
{
	if (_map != nullptr)
	{
		delete[] _map;
		_map = nullptr;
	}
	_size = 0;
	_filled = 0;
}

template<typename K, typename V>
void Mapa<K, V>::Show()
{
	for (size_t i = 0; i<_filled; i++)
	{
		std::cout << "\nElement #" << i+1 << "\nKey " << _map[i].first << "   Val " << _map[i].second << std::endl;
	}
}

template<typename K, typename V>
V& Mapa<K, V>::operator [](K key) // dostup po kluchu	
{
	for (size_t i = 0; i<_filled; i++)
	{
		if (_map[i].first == key)
		{
			return _map[i].second;
		}
	}
	size_t itr = this->Insert(key, V());
	return _map[itr].second;
}

template<typename K, typename V>
Mapa<K, V>& Mapa<K, V>::operator =(const Mapa<K, V>& Map)//return *this
{
	this->Clear();
	_size = Map._size;
	_filled = Map._filled;
	_map = new Para<K, V>[_size];
	memcpy(_map, Map._map, sizeof(Para<K, V>)*(_filled));
	return *this;
}

template<typename K, typename V>
Mapa<K, V>& Mapa<K, V>::operator = (Mapa<K, V>&& Map) //return *this
{
	this->Clear();
	_map = Map._map;
	_size = Map._size;
	_filled = Map._filled;
	Map._map = nullptr;
	Map.~Mapa();
	return *this;
}

template<typename K, typename V>
void Mapa<K, V>::Grow(size_t size)
{
	Para<K,V> *tmp = new Para<K, V>[size];
	memcpy(tmp, _map, sizeof(Para<K, V>)*_size);
	delete[] _map;
	_map = tmp;
	_size = size;
}

template<typename K, typename V>
void Mapa<K, V>::Decrease(size_t size)
{
	Para<K, V> *tmp = new Para<K, V>[size];
	_filled = _filled > size ? size : _filled;
	memcpy(tmp, _map, sizeof(Para<K, V>)*_filled);
	delete[] _map;
	_map = tmp;
	_size = size;
}