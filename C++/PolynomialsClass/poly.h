#ifndef __poly_H__
#define __poly_H__
#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

class poly{
private:
	vector<float> v;
public:
	poly(float value=0)
	{
		if(v.size()<1)
			v.resize(1);
		v[0] = value;
	}

	float & operator [] (unsigned int pos)
	{
		if(pos >= v.size())
		{
			v.resize(pos+1);
		}

		return v[pos];
	}
	
	
	poly operator+ (poly p)
	{
		poly tmp;
		for(unsigned int i=0; i<p.v.size(); i++)
			tmp[i] = p[i];
		for(unsigned int i=0; i<v.size(); i++)
			tmp[i] += v[i];

		return tmp;
	}
	
	friend poly operator* (const poly &p1, const poly &p2)
	{
		poly tmp;
        for(unsigned int i=0; i<p1.v.size(); i++)
        {
		    for(unsigned int j=0; j<p2.v.size(); j++)
            {
                tmp[i+j] += p1.v[i]*p2.v[j];
            }
	    }

		return tmp;
	}
	
	double operator() (float x)
	{
		double res = 0;
		for(unsigned int i=0; i<v.size(); i++)
			res += pow(x,i)*v[i];

		return res;
	}

	friend ostream & operator << (ostream & s, poly p)
  	{
			int i = p.v.size()-1;
    		while(i!=0)
    		{
    			s<<p[i]<<"*(x^"<<i<<") + ";
				i--;
    		}
			s<<p[i]<<"*(x^"<<i<<")";

    		return s;
 	 }

};

#endif /* __poly_H__ */


