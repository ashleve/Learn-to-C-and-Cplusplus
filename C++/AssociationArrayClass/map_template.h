#ifndef __MAP_TEMPLATE_H__
#define __MAP_TEMPLATE_H__
#include <iostream>
#include <vector>

using namespace std;


template<class aType, class E> class map_template {
private:
	vector<aType> ids;
	vector<E> employees;
	unsigned int size = 0;

public:
	void Add(aType id, E e)
	{
		ids.push_back(id);
		employees.push_back(e);
		size++;
	}


	map_template& operator= (const map_template& m)
	{
		this->ids.resize(0);
		this->employees.resize(0);
		this->size = 0;

		for (int i = 0; i < m.size; i++)
		{
			this->ids.push_back(m.ids[i]);
			this->employees.push_back(m.employees[i]);
			this->size++;
		}
		return *this;
	}


	E* Find(aType id)
	{
		for (unsigned int i = 0; i < size; i++)
		{
			if (ids[i] == id)return &employees[i];

		}
		return NULL;
	}


	friend ostream & operator << (ostream & s, map_template& m)
	{
		cout << "------------------------------------" << endl;
		for (unsigned int i = 0; i < m.size; i++)
		{
			cout << i + 1 << ".";
			cout << "ID: " << m.ids[i] << endl;
			m.employees[i].show();
		}
		cout << "------------------------------------" << endl << endl;
	}

};

#endif