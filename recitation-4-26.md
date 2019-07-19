# Making an ArrayList

In this example, I'll be going over how to make an [ArrayList](http://docs.oracle.com/javase/6/docs/api/java/util/ArrayList.html "Java 6 ArrayList"). C++ already has an ArrayList implementation, called [vector](http://www.cplusplus.com/reference/vector/vector/ "std::vector"), but making one from scratch is both instructive and useful in completing lab 9 and lab 10.

The complete source, test file, and Makefile are included in [this](https://github.com/kugurst/cs3157-recitations/tree/master/recitation-4-26-code "Recitation Code") directory, but looking at the solution without trying is just cheating yourself.

Without further ado, let's get to it.

### Making the File

Go ahead and make a file called `arraylist.h`. It doesn't matter what you call the file, unlike in Java, but it's still a good idea to name a file after its class. We won't be writing the accompanying `arralyst.cpp`, as the code we're writing are templates, and are thus not fully compiled until they are actually typed by some other function (more on this later). 

- - -

*You should separate your declarations (`arraylist.h`) from your definitions (`arraylist.cpp`) in real design, but we're not doing that in this case since it's not strictly necessary.*

- - -

## Writing Our First Lines

As with all header files, always make sure to create your include guards:

```cpp
#ifndef __ARRAYLIST_H__
#define __ARRAYLIST_H__
    // Class code goes here
#endif
```
We'll be filling in the class code. Now, our include statements:

```cpp
#include <iostream>
#include <stdio.h>
```

We'll be using `iostream` for `cout` and `ostream`, and we'll be using `stdio.h` for `fprintf`. Next, we'll change our namespace:

```cpp
using namespace std;
```

This tells `g++` that it should treat all variables and functions declared in `std` as though they were declared in our current scope, from this point on. So that means instead of writing `std::cout << "Hello World!" << std::endl;`, we can write `cout << "Hello World!" << endl;`.

---

*You can make your own namespace by doing something like this:*

```cpp
namespace myspace {
    int a, b;
    // Other stuff in namespace...
}
```

---

## The Meat of the Class

Now, we'll declare our class:

```cpp
// class declaration
template <typename T>
class ArrayList;
```

The second line is pretty much self evident, though the first line needs a bit of explanation. What we're doing here is called a class template declaration. We're telling the compiler that we have a class called `ArrayList` that works with some type, `T`. We're saying that our class will work for any type `T`, which is not always true, but if the people using our class are good programmers, then there's nothing to fear (more on that later).

Now, we define our operators that require access to our class's private members but are global functions:

```cpp
// put-to operator
template <typename T>
ostream& operator<<(ostream& os, const ArrayList<T>&);
```

Here, we have a function template declaration, that is for some type `T`. Note that this is different from mystring.h, where all the operator functions were member functions. This owes to the fact that this operator is for all ArrayLists (the class template), not for a particular template class (a particular typed instance of our class, which all of our functions defined inside of the class definition are).

Now, let's get on to defining our class:

```cpp
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
		// To print, we need to access list, which is a private member, even though
		// << is a global operator.
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
```

For this case, the default constructors are not enough. We have to keep track of an array of type `T`, and it must persist, so it can't be allocated to the stack, and by default, a pointer just points to `NULL`. So, we must define our own constructor that does what we need it to. Also, our class is the only thing that knows of the existence of `arr`, so we must delete it when our class is to be destroyed; thus, we must define our own destructor. Finally, it's not just enough to copy the value of `arr` (as would happen by default); we want to allocate a completely new array and copy over the values from the other array, thus we must define our own copy constructor.

Before I continue, I would like to turn your attention to the first line of code in this snippet, `template <typename T>`. I haven't particularly stated the scope of this expression, so let me do it now. `template <typename T>` basically tells the compiler that for the following statement, any reference to some type, `T`, is referring to this particular type. In the case of the put-to operator, this statement was the prototype of the function. In the class definition case, the statement is the definition (most statements end in `;`. Our ArrayList definition really just looks like: `template <typename T> ArrayList { // some stuff };`).

We define all the functions as we do in mystring.h, and we define some private variables that we need to implement our ArrayList that the users of our list do not need to know. We also have a function, `doubleArray()`, that is to be used only internally.

Finally, we have prototyped our put-to operator again. It would be better to defer explanation of why to this [forum post](http://www.ozzu.com/cpp-tutorials/tutorial-defining-and-overloading-operators-t85776.html "C++ Operators"). The put-to operator is pretty low in the post (search for "operator<<").

## Defining the Functions

Let's go ahead and define our constructors as we defined them above:

```cpp
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
```

I've included print statements that will only print if the macro, `BASIC3TRACE` is defined at compilation time (take a look at the Makefile to see how). The destructor is very straightforward, just un-allocate anything that we allocated. Since `arr` was created via `new []`, it must be un-allocated via `delete[]` (`malloc`/`calloc` is to `free` as `new` is to `delete` as `new []` is to `delete[]`). The default constructor and copy constructors are also equally simple. Just make an array and set the sizes appropriately (`size` refers to the number of elements in the array, `arr`). If we're copying an array, just copy whatever the other array contains (if the contents of the other array are `char *`, then our array will have its own set of pointers that point to the same location as the original `char *`. This is what it means to follow pointer semantics).

Let's make the function to add to our list:

```cpp
// Adding to our list
template <typename T>
ArrayList<T>& ArrayList<T>::addBack(const T& t)
{
	if(maxLength == size)
		doubleArray();
	arr[size++] = t;
	return *this;
}
```

If `arr` is full, then double the array. Otherwise, just add `t` to the end of our array, increment the number of items in out array, and continue. Returning the same ArrayList allows us to chain `addBack` calls like so: `list.addBack(5).addBack(6);` (where `list` is an `ArrayList<int>`). The first `ArrayList<T>&` tells the compiler that our function returns a typed ArrayList reference, and then `ArrayList<T>::` tells the compiler that this function is defined in the ArrayList<T> scope (that is, this is the function template for type `T`).

Now, let's define the array operator:

```cpp
// The array operator for this class
template <typename T>
T& ArrayList<T>::operator[](int i)
{
	// Just return the object at that position
	return arr[i];
}
```

Not much to see here. If we we're serious about this, we would do some sort of error checking to make sure that `i` is within bounds, but this is just an example.

In fact, all the other functions follow pretty naturally:

```cpp
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
```

The only quirk is that `operator<<` is not preceded by the scope operator (`ArrayList<T>::`), but this follows from the fact that `operator<<` is defined as a global function.

## Putting It All Together

Our whole class looks like this:

```cpp
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
```

It's not particularly long, but there are so many potential syntactic pitfalls, that not having a design guideline makes creating template classes more difficult than they should be. Mainly, this walkthrough is supposed to show you the syntax for creating templates, not particularly how you would make an ArrayList class. In particular, it should make lab 10 easier, and perhaps shed some additional light on why `MyString` is defined the way it is.
