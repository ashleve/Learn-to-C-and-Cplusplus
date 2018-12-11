#ifndef __MAP_TEMPLATE_H__
#define __MAP_TEMPLATE_H__
#include <iostream>
#include <vector>

using namespace std;


template<typename aType, class E> class map_template {
private:
	vector<aType> ids;
	vector<E> employees;
	unsigned int size;

public:
	map_template():size(0){};

	void Add(const aType& id, const E& e)
	{

		ids.push_back(id);

		try
		{
			employees.push_back(e);
		}
		catch (bad_alloc& ba)
		{
			ids.pop_back();
			throw;
		}

		size++;
	}


	E* Find(const aType& id)
	{
		for (unsigned int i = 0; i < size; i++)
		{
			if (ids[i] == id)return &employees[i];

		}
		return NULL;
	}


	friend ostream & operator << (ostream & s,const map_template& m)
	{
		cout << "------------------------------------" << endl;
		for (unsigned int i = 0; i < m.size; i++)
		{
			cout << endl << i + 1 << ".";
			cout << m.ids[i] << endl;
			m.employees[i].show();
			cout << endl;
		}
		cout << "------------------------------------" << endl << endl;
	}

};

#endif
