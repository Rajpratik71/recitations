#include "box.h"
#include <iostream>
#include <cstdio>

int main()
{
	box b1;
	box b2(1, 2, 3);
	// Print out the state of b1 and b2
	cout << b1 << endl << b2 << endl;
	// Make another box that is the sum of b1 and b2
	box b3 = b1 + b2;
	cout << b3 << endl;
	// This is perfectly valid
	b2 += b2;
	cout << b2 << endl;
	// save the old value of b3
	b1 = b3;
	// reassign b3
	b3 = b1 + b2;
	cout << b3 << endl;
	// b3
	b2 = b2 - b1;
	cout << b2 << endl;
	cout << (b1 > b2) << endl;
	cout << (b1 < b2) << endl;
	// Just for that old throw back
	printf("%d\n", (b1 < b2));
	return 0;
}
