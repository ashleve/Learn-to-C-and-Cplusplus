#include <iostream>
using namespace std;
#include "complex.h"


int main()
{
  Complex a(0.0,15),b(0,13),c;
  c = 10;
  cout << c <<endl;
  c = -a;
  cout << c <<endl;
  c = a + b;
  c = c - Complex(10);
  cout << c <<endl;
/*
  c = 10 - a;
  (c -=b) -=10;
  cout << c <<endl;
*/
  c=c/2;
  cout << c << endl;
  c/=3;
  cout << c << endl;
  b = a;

  cout << c.abs() << endl;
  cout << a << endl;

}
