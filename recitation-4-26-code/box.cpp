#include "box.h"
#include <cmath>

// The default constructor
box::box()
{
	height = 0;
	length = 0;
	width = 0;
}

// The copy constructor
box::box(const box& b)
{
	height = b.height;
	length = b.length;
	width = b.width;
}

// Assignement operator
box& box::operator=(const box& b)
{
	// If the memory address of the box and the box we are to assign to are the
	// same, don't do anything
	if (this == &b) {
		return *this;
    }
    // The copy operator is essentially what we want to do here.
	height = b.height;
	length = b.length;
	width = b.width;
	// now return
	return *this;
}


// User defined constructor
box::box(double h, double w, double l)
{
	height = h;
	width = w;
	length = l;
}
		
// operator+
box operator+(const box& b1, const box& b2)
{
	// Let's copy the value of b1.
	box b3(b1);
	// And add the value of b2 to it.
	b3.length += b2.length;
	b3.width += b2.width;
	b3.height += b2.height;
	// Return the box we just made
	return b3;
}

// operator-
box operator-(const box& b1, const box& b2)
{
	// Let's make a new box
	box b3;
	// And get the absolute difference in dimensions of the box.
	b3.length = fabs(b1.length - b2.length);
	b3.width = fabs(b1.width - b2.width);
	b3.height = fabs(b1.height - b2.height);
	// Return the box we just made
	return b3;
}

// operator+= We'll write it in terms of operator+
box& operator+=(const box& b1, const box& b2)
{
	// We're going to lie, and we'll actually modify b1
	box& b11 = (box&) b1;
	b11 = b1 + b2;
	return b11;
}

// operator-= We'll write it in terms of operator+
box& operator-=(const box& b1, const box& b2)
{
	// We're going to lie, and we'll actually modify b1
	box& b11 = (box&) b1;
	b11 = b1 - b2;
	return b11;
}
	
// put-to operator
ostream& operator<<(ostream& os, const box& b)
{
	// This is essentially the toString() method, so let's format our box appropriately
	os << "Length: " << b.length << " | Width: " << b.width << " | Height: " <<
		b.height << " | Volume: " << b.volume();
	return os;
}

int operator>(const box& b1, const box& b2)
{
	return b1.volume() > b2.volume();	
}

int operator<(const box& b1, const box& b2)
{
	return b1.volume() < b2.volume();
}

