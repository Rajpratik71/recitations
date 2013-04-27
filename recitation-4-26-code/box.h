#ifndef __BOX_H__
#define __BOX_H__

using namespace std;

#include <iostream>

class box {
	public:
		// Fields
		double height;
		double width;
		double length;
		
		// The big four
		// Default constructor
		box();
		// The copy constructor
		box(const box& b);
		// The compiler generated destructor, which is to do nothing, is fine for us.
		// Assignement operator
		box& operator=(const box& b);
		
		// A user defined constructor
		box(double h, double w, double l);
		
		// Some operators that kind of make sense
		
		// operator+
		friend box operator+(const box& b1, const box& b2);
		// operator-
		friend box operator-(const box& b1, const box& b2);
		// operator+=
		friend box& operator+=(const box& b1, const box& b2);
		// operator-=
		friend box& operator-=(const box& b1, const box& b2);
	
		// put-to operator
		friend ostream& operator<<(ostream& os, const box& b);
		
		// no box would be complete without some volume
		double volume() const { return length * width * height; };
		
		// And two boolean operators, just because
		
		// operator<
		friend int operator<(const box& b1, const box& b2);		
		// operator>
		friend int operator>(const box& b1, const box& b2);
};

#endif
