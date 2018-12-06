#ifndef __BOOK_H__
#define __BOOK_H__

#include <iostream>

using namespace std;


enum C {ScienceFiction = 0, Drama, Comedy, Adventure, Education, Horror, Fantasy, Undefined};
const char* CategoryNames[] = {"Science Fiction", "Drama", "Comedy", "Adventure", "Education", "Horror", "Fantasy", "Undefined"};
enum S {Wypozyczona = 0, NaPolce};
const char* StatusNames[] = {"Wypozyczona", "Na polce"};


class Book {
public:
	string Autor;

	C Category;
	S Status;
	unsigned int NumberOfPages;

	Book(string a = "", C c = Undefined, S s = NaPolce, unsigned int n = 0): Autor(a), Category(c), Status(s), NumberOfPages(n) {};

	void show()
	{
		cout << "Autor: " << Autor << endl;
		cout << "Category: " << CategoryNames[Category] << endl;
		cout << "Status: " << StatusNames[Status] << endl;
		cout << "Number Of Pages: " << NumberOfPages << endl;
	}

};

#endif