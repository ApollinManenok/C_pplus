#pragma once
#include <iostream>
#include <exception>


class key_exception : public std::exception
{
public:
	key_exception(char const* e = "Key exception") throw() : exception(e) {}
};

class data_exception : public std::exception
{
public:
	data_exception(char const* e = "Data exception") throw() : exception(e) {}
};




template<typename Key, typename Value>
class BTree
{
	struct TreeItem
	{
		Key _key;
		Value _value;
		TreeItem* _left = nullptr;
		TreeItem* _right = nullptr;
	};

public:
	BTree() = default;
	BTree(Key key, Value value);
	~BTree();
	void ItemRemove(Key key);
	void ItemAdd(Key key, Value value);
	Value& GetAt(Key key);
	bool InTree(Key key) const;
	size_t Count();
	void Show();

private:
	void RekDel(TreeItem* item);
	size_t RekCount(TreeItem* item);
	void RekShow(TreeItem* item, size_t num = 0);
	void RekInsert(TreeItem* item);
	TreeItem* _root = nullptr;
};


////////////////////////////////////////////////////////////////
//                       FUNCTIONS
////////////////////////////////////////////////////////////////


template<typename Key, typename Value>
BTree<Key, Value>::BTree(Key key, Value value)
{
	_root = new TreeItem;
	_root->_key = key;
	_root->_value = value;
}

template<typename Key, typename Value>
BTree<Key, Value>::~BTree()
{
	if (_root != nullptr)
	{
		RekDel(_root);
	}
}

template<typename Key, typename Value>
void BTree<Key, Value>::RekDel(TreeItem* item)
{
	if (item->_left != nullptr)
	{
		RekDel(item->_left);
	}
	if (item->_right != nullptr)
	{
		RekDel(item->_right);
	}
	delete item;
}


template<typename Key, typename Value>
void BTree<Key, Value>::ItemRemove(Key key)
{
	if (_root == nullptr)
	{
		throw data_exception("Object is empty");
	}
	TreeItem* add = nullptr;
	size_t l_items = 1;
	size_t r_items = 1;
	if (_root->_key == key)
	{
		TreeItem* temp = _root;
		l_items += RekCount(_root->_left);
		r_items += RekCount(_root->_right);
		add = (l_items >= r_items) ? _root->_right : _root->_left;
		_root = (l_items < r_items) ? _root->_right : _root->_left;
		delete temp;
	}
	TreeItem* item = _root;
	while (true)
	{		
		if (item->_key > key)
		{
			if (item->_left == nullptr)
			{
				throw key_exception("Key not found");
			}
			else
			{
				if (item->_left->_key == key)
				{
					TreeItem* temp = item->_left;
					l_items += RekCount(temp->_left);
					r_items += RekCount(temp->_right);
					add = (l_items >= r_items) ? temp->_right : temp->_left;
					item->_left = (l_items < r_items) ? temp->_right : temp->_left;
					delete temp;
					break;
				}
				item = item->_left;
			}
		}
		else
		{
			if (item->_right == nullptr)
			{
				throw key_exception("Key not found");
				break;
			}
			else
			{
				if (item->_right->_key == key)
				{
					TreeItem* temp = item->_right;
					l_items += RekCount(temp->_left);
					r_items += RekCount(temp->_right);
					add = (l_items >= r_items) ? temp->_right : temp->_left;
					item->_right = (l_items < r_items) ? temp->_right : temp->_left;
					delete temp;
					break;
				}
				item = item->_right;
			}
		}
	}
	RekInsert(add);
}

template<typename Key, typename Value>
void BTree<Key, Value>::RekInsert(TreeItem* item)
{
	if (item->_left != nullptr)
	{
		RekInsert(item->_left);
	}
	if (item->_right != nullptr)
	{
		RekInsert(item->_right);
	}
	ItemAdd(item->_key, item->_value);
	delete item;
}


template<typename Key, typename Value>
void BTree<Key, Value>::ItemAdd(Key key, Value value)
{
	TreeItem* temp = new TreeItem;
	temp->_key = key;
	temp->_value = value;
	if (_root == nullptr)
	{
		_root = temp;
	}
	else
	{
		TreeItem* item = _root;
		while (true)
		{
			if (item->_key == key)
			{
				item->_value = value;
				break;
			}
			else if (item->_key > key)
			{
				if (item->_left == nullptr)
				{
					item->_left = temp;
					break;
				}
				else
				{
					item = item->_left;
				}
			}
			else
			{
				if (item->_right == nullptr)
				{
					item->_right = temp;
					break;
				}
				else
				{
					item = item->_right;
				}
			}
		}
	}
}

template<typename Key, typename Value>
Value& BTree<Key, Value>::GetAt(Key key)
{
	if(_root!=nullptr)
	{
		TreeItem* item = _root;
		while (true)
		{
			if (item->_key == key)
			{
				return item->_value;
			}
			else if (item->_key > key)
			{
				if (item->_left == nullptr)
				{
					throw key_exception();
				}
				else
				{
					item = item->_left;
				}
			}
			else
			{
				if (item->_right == nullptr)
				{
					throw key_exception();
				}
				else
				{
					item = item->_right;
				}
			}
		}
	}
	throw data_exception("Object is empty");
}


template<typename Key, typename Value>
bool BTree<Key, Value>::InTree(Key key) const
{
	if (_root != nullptr)
	{
		TreeItem* item = _root;
		while (true)
		{
			if (item->_key == key)
			{
				return true;
			}
			else if (item->_key > key)
			{
				if (item->_left == nullptr)
				{
					return false;
				}
				else
				{
					item = item->_left;
				}
			}
			else
			{
				if (item->_right == nullptr)
				{
					return false; 
				}
				else
				{
					item = item->_right;
				}
			}
		}
	}
	return false;
}


template<typename Key, typename Value>
size_t BTree<Key, Value>::Count()
{
	size_t count = 0;
	if (_root != nullptr)
	{
		count++;
		count += RekCount(_root);
	}
	return count;
}


template<typename Key, typename Value>
size_t BTree<Key, Value>::RekCount(TreeItem* item)
{
	size_t temp_c = 0;
	if (item->_left != nullptr)
	{
		temp_c++;
		temp_c += RekCount(item->_left);
	}
	if (item->_right != nullptr)
	{
		temp_c++;
		temp_c += RekCount(item->_right);
	}
	return temp_c;
}


template<typename Key, typename Value>
void BTree<Key, Value>::Show()
{
	if (_root != nullptr)
	{
		RekShow(_root);
	}
	else
	{
		std::cout << "Object is emty";
	}
}


template<typename Key, typename Value>
void BTree<Key, Value>::RekShow(TreeItem* item, size_t num = 0)
{
	std::cout << item->_key;
	if (item->_right != nullptr)
	{
		std::cout << "-->";
		RekShow(item->_right, num+1);
	}
	if (item->_left != nullptr)
	{
		std::cout << "\n";
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < (num + 1) * 2 - 2; j++)
			{
				std::cout << "  ";
			}
			if (i == 0)
			{
				std::cout << " |\n";
			}
			else
			{
				std::cout << "  \\->";
			}			
		}
		RekShow(item->_left, num + 1);
	}	
}


