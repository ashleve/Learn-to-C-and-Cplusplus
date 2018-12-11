#ifndef __EMPLOYEE_H__
#define __EMPLOYEE_H__
#include <iostream>

using namespace std;


class Employee {
public:
	string Name;
	string Position;
	unsigned int Age;


	Employee (string n = "", string j = "", unsigned int a = 0): Name(n), Position(j), Age(a) {};


	void show() const
	{
		cout << "Name: " << this->Name << endl;
		cout << "Position: " << this->Position << endl;
		cout << "Age: " << this->Age << endl;
	}

};

#endif
