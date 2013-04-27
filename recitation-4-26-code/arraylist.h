#ifndef __ARRAYLIST_H__
#define __ARRAYLIST_H__

#include <algorithm>
#include <iostream>
#include <stdio.h>

using namespace std;

// class declaration
template <typename T>
class ArrayList;

// put-to operator
template <typename T>
ostream& operator<<(ostream& os, const ArrayList<T>&);

// class definition
template <typename T>
class ArrayList {
	public:
		// The constructors
		ArrayList<T>();
		ArrayList<T>(const ArrayList&);
		~ArrayList<T>();
		// Adding to our list
		ArrayList<T>& addBack(const T&);
		// Two function types I haven't gone over
		T& operator[](int);
		const T& operator[](int) const;
		// To print, we need to access list, which is a private member, even
		// though << is a global operator.
		friend ostream& operator<< <T>(ostream& os, const ArrayList<T>&);
		// Addition (concatenation) operator
		ArrayList<T> operator+(const ArrayList<T>&) const;
		// Getting the size
		int length() const { return size; };
	private:
		T *arr;
		int size;
		int maxLength;
		void doubleArray();
};

// Default constructor
template <typename T>
ArrayList<T>::ArrayList()
{
	#ifdef BASIC3TRACE
		fprintf(stderr, "BASIC3TRACE: (%p)->Constructor\n", this);
	#endif
	arr = new T[10];
	size = 0;
	maxLength = 10;
}

// Copy Constructor
template <typename T>
ArrayList<T>::ArrayList(const ArrayList& list)
{
	#ifdef BASIC3TRACE
		fprintf(stderr, "BASIC3TRACE: (%p)->Copy Constructor\n", this);
	#endif
	arr = new T[list.size + 10];
	size = list.size;
	maxLength = size + 10;
	for(int i = 0; i < list.maxLength; i++)
		arr[i] = list.arr[i];
}

// Destructor
template <typename T>
ArrayList<T>::~ArrayList()
{
	#ifdef BASIC3TRACE
		fprintf(stderr, "BASIC3TRACE: (%p)->Destructor\n", this);
	#endif
	delete[] arr;
}

// Adding to our list
template <typename T>
ArrayList<T>& ArrayList<T>::addBack(const T& t)
{
	if(maxLength == size)
		doubleArray();
	arr[size++] = t;
	return *this;
}

// The array operator for this class
template <typename T>
T& ArrayList<T>::operator[](int i)
{
	// Just return the object at that position
	return arr[i];
}

// The const array operator for this class
template <typename T>
const T& ArrayList<T>::operator[](int i) const
{
	return ((ArrayList<T>&) *this)[i];
}

template <typename T>
void ArrayList<T>::doubleArray()
{
	// Make a new array of double the length of the old
	T *newArr = new T[maxLength * 2];
	// Fill it
	for(int i = 0; i < size; i++)
		newArr[i] = arr[i];
	// Get rid of the old one
	delete[] arr;
	// Mark the new length
	maxLength *= 2;
	// Assign the new one
	arr = newArr;
	// size still stays the same
}

// The "toString()" method of this class.
template <typename T>
ostream& operator<<(ostream& os, const ArrayList<T>& list)
{
	// First, print the open bracket
	os << "{ ";
	// Then traverse the array and print out each object.
	for(int i = 0; i < list.size; i++)
		os << list[i] << " ";
	// Close the bracket
	os << "}";
	return os;
}


// Addition (concatenation) operator
template <typename T>
ArrayList<T> ArrayList<T>::operator+(const ArrayList<T>& l1) const
{
	// Make a list to return
	ArrayList<T> l2;	
	// Delete the old array, as we'll be overwritting it with a new one
	delete[] l2.arr;
	// Make a new array that's the same size as both and has room for 10 more
	// items before it needs to expand
	l2.arr = new T[l1.size + size + 10];
	// Fill the array with the stuff from the first list, which would be "this"
	// one
	for (int i = 0; i < size; i++)
		l2.arr[i] = arr[i];
	// Then add the stuff from the other array
	for (int i = 0; i < l1.size; i++)
		l2.arr[size + i] = l1.arr[i];
	l2.size = l1.size + size;
	l2.maxLength = l1.size + size + 10;
	return l2;
}

#endif
