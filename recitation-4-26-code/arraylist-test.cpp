#include <iostream>
#include <string>
#include <cstring>
#include "arraylist.h"

using namespace std;

int main(int argc, char **argv)
{
	ArrayList<int> list;
	for(int i = 0; i < 11; i++)
		list.addBack(i + 1);
	cout << "list: " << list << endl;
	cout << "list length: " << list.length() << endl;
	cout << "number at position 6 in list: " << list[5] << endl;
	// Copy the ArrayList
	ArrayList<int> list2(list);
	cout << "list2 : " << list2 << endl;
	// A more complex list
	ArrayList<string> list3;
	for(int i = 0; i < 11; i++) {
		string s = "high";
		list3.addBack(s);
	}
	cout << "list3: " << list3 << endl;
	list3[0][0] = 's';
	cout << "list3: " << list3 << endl;
	// A more interesting list
	ArrayList<char *> list4;
	char *c = (char *)malloc(sizeof(char) * 5);
	strcpy(c, "high");
	for(int i = 0; i < 11; i++) {
		list4.addBack(c);
	}
	list4[1][0] = 's';
	cout << "list4: " << list4 << endl;
	// This is the copy constructor, not the assignment operator.
	ArrayList<int> list5 = list + list2;
	cout << "list5: " << list5 << endl;
	free(c);
	return 0;
}
