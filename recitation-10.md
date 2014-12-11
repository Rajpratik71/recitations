# Recitation 10 #

# C++ templates and containers #

At this point hopefully you've gotten used to writing basic classes in C++ from
our experience with MyString. At this point we can move in one of two
directions. First, we could take a more typical Object Oriented track and
discuss inheritance, polymorphism, and those sorts of things that companies love
to ask about during tech interviews. However if you've used Java at length
you're probably already somewhat comfortable with this, even though it's
incredibly complex and confusing (multiple inheritance = head asplode).

Instead, in 3157, we're going to discuss templates and generics. The interaction
between generics and C++ is very interesting, and they work differently than in
most other languages. Consequently they're incredibly powerful, and often very
useful for programming fast and flexible libraries.

#### Recall how we created executables in C (and C++ so far) ####

If you go back to the beginning of the course, we discussed compilation as the
following steps:

1. Preprocessing (`#include`, `#define`, other `#` commands)
2. Compiling (turning raw .c files into .o object files)
    * Recall, we could also create a library `.a` file from our `.o` files,
      if we wanted. This would be an extra step after compiling, before linking
3. Linking (combining 1+ .o files into a single executable)

In this step we can throw away the source code after 2, and as long as we have
the .o and .a files we can link successfully.

#### Drawbacks to void * ####

This approach is feasible in C, because it's a small language with
relatively few types. The prominence of pointers and casting - you can't write
basically any C without using them - means that using `void *` as a way of
handling unknown types is fairly natural. This is what we did in our linked
list in lab 3.

However it has some problems: it doesn't provide any type safety, so you have to
constantly cast your void pointers to whatever type they really should be. This
means that the compiler is essentially helpless - it can't provide the static
checking that catches errors at compile time. Additionally, the list can't
provide any guarantees about the lifetime of things to which it points. As you
may have seen, it's very easy to end up with invalid pointers.

### Templates ###

Templates are (one way) C++ "solves" the problems with `void *` pointers.
Templates let you write code that accepts generic data types and provide
compile time type checks. In 3157 we'll primarily focus on class templates (like
`vector`), not function templates.

Before we proceed into talking about templates, a word of warning. Templates
break, or at least abuse, the standard mental model of compiling and linking
that we've established so far. They do things that "feel" like linking, but it's
actually happening during compiling. So just be aware that you'll need a new
mental bucket for thinking about templates. (We'll
[discuss](#templates-under-the-hood) that more soon.)

Templates are kind of like madlibs. They contain almost the whole story, but
leave certain things blank. Whatever you put in the blank will complete the
story. Using templates is generally quite easy. Say we want a vector (discussed
more below): we just define a vector and specify the type of object it holds.
Let's say we want to store a bunch of ints and then a bunch of MdbRecs. That's
easy:

```cpp

#include <vector>
//...

vector<int> ivec;
vector<MdbRec> mdbvec;
```

But there's no need to stop there! What if we want a vector of vectors of
MyStrings?

```cpp
vector<vector<MyString> > msvecvec;
```

So, what's the type of `ivec`? `vector`? Nope! It's type `vector<int>`. The type
of the template'd thing includes the type of the things being used in the
template. This is because templates are handled in the compilation step, and
basically generate a new type. This **isn't** exactly what happens, but mentally you
can think of the compiler as seeing your `vector<int>` and creating a normal,
non-template type `vector_int`; then it goes through and replaces every instance
of the `vector<int>` template type with the `vector_int` normal type. If
elsewhere you use `vector<MyString>`, it creates an entirely separate class
called `vector_mystring`. (Again, nothign is literally called `vector_int` but
it can be a good way to think about this)

It helps to think about how the implementation is written. 

```cpp
template <typename T>
class vector {
  public:
    //......
    void push_back(const T& x);
        //......

  private:
    T *a;
    size_t size;
    size_t capacity;
    void grow();
};
```

Note that `push_back` takes a constant reference to type T (`const T&`). If
you've done lab 9, you'll know that it's generally better to take constant
references for a couple of reasons:

1. By taking `const`, you increase the range of things you can take (both
`const` and non-`const`)
2. By taking a reference, you avoid the overhead associated with copy
constructors, temporaries, etc.

Using vector:

```cpp
vector<string> students(200); //a vector of 200 strings
vector<int> grades; //an empty vector or grades

students[0] = "Peter J. Awn";
grades.push_back(100);
```

#### Templates: under the hood ####

So we recalled above how compiling worked in C. We create a struct, like `struct
List list`, and call a function like `initList(&list)`. The compiler looks at our
source code and our included headers to see if the type `struct List` and the
function `initList(struct list *)` are defined anywhere. It doesn't care about
the implementation at this point, only whether they're defined as valid, legal
types. If so, it creates an object file. Later the linker goes through and links
the separately compiled objects together into a single executable.

This all breaks down with templates. As we discussed previously, there is no
type `vector`. It's code that can work with *any* type. So whenever you use a
`vector`, the compiler needs to see the entire definition because it has two
jobs to do: 1) check that you're using it validly, and 2) *actually generate the
code*. When you use the template with a type, the compiler is actually creating
entirely new types, functions, etc. To do that it needs to have the full code
available. **This is why we put templates entirely in the .h files.**

A comment from Jae about this: 

> Template compilation is very tricky.  The crux of the problem is that when a
piece of code *uses* a template, the compiler needs to see the whole definition
of the template because it needs to generate a typed instance.  This is
different from C, where the compiler does not need to see the body of a function
when it's compiling a piece of code that calls it.  Basically, the C model of
separating interface (.h) and implementation (.c) doesn't really work in C++.
This is one of the things in C++ that is fundamentally incompatible with C, and
no matter what you do you'll end up with a kludge.

> We take the easiest way (as most people do): each compilation unit (i.e. each
cpp file that *uses* a template function or a template class) will include the
generated typed instances of a template.  That is, if foo.cpp and bar.cpp both
use Stack<int>, foo.o and bar.o will each have the generated Stack<int> machine
code in there.  The linker will throw away the duplicates at link time.
Wasteful, but simple.  This is known as the "Borland" model.  (Borland is a
compiler vendor that most of you are too young to know. I actually remember
installing Borland C++ compiler on my PC from 3.5-inch floppy disks, 22 of
them.)

> This gcc manual page explains the Borland model and other more fancier
alternatives: http://gcc.gnu.org/onlinedocs/gcc/Template-Instantiation.html


See the end of Lippman 5th ed 16.1.1, "Template Compilation" for a more thorough
explanation.

#### Alternate Template Motivation ####

If you'd like another reason for templates, the canonical example is writing
these two functions:

```cpp
// returns 0 if the values are equal, -1 if v1 is smaller, 1 if v2 is smaller
int compare(const string &v1, const string &v2)
{
    if (v1 < v2)
        return -1;
    if (v2 < v1)
        return 1;
    return 0;
} 
int compare(const double &v1, const double &v2)
{
    if (v2 < v1)
        return 1;
    if (v1 < v2)
        return -1;
    return 0;
}
```

You might realize that functions are nearly identical, but still have two
implementations: one for strings and one for doubles. And all of this doesn't
help you at all if you want to write a `compare` for ints! If only there was a
way to do the same logic, and use the already existing operator overloading to
just call whatever the appropriate `operator<` function...luckily ,templates do
this!

The template'd version of this is:

```cpp
template <typename T>
int compare(const T &v1, const T &v2)
{
    if (v2 < v1)
        return 1;
    if (v1 < v2)
        return -1;
    return 0;
}
```


#### What's with that weird double template stuff for ostream? ####

http://publib.boulder.ibm.com/infocenter/comphelp/v8v101/index.jsp?topic=%2Fcom.ibm.xlcpp8a.doc%2Flanguage%2Fref%2Ffriends_and_templates.htm

Sometimes you need to define FUNCTION templates in association with a CLASS
template. This is a friend function that depends on the specific type of the
class. If we look at Jae's stack example from lecture note 22, he declares 

```cpp
template <typename T>
ostream& operator<<(ostream& os, const Stack<T>& rhs)
```

There's one function for each type used, ie there's a
operator<<_for_stack_of_int, and operator<<_for_stack_of_strings. Those are
different functions, and each needs to be created as separate function
templates. However they're friends, they aren't member functions, hence they
need to be declared as separate templates.


## Containers ##

The C standard library, STL, provides
[containers](http://www.cplusplus.com/reference/stl/). Containers are a standard
set of library classes for containing a bunch of objects of some type. They're
implemented with templates, and are pretty much the canonical example of
templates in the language.

#### Value Semantics ####

One key feature of containers is that they provide **value semantics**. That is,
containers at least appear to make copies of the things you store into them, and
manage the lifetime of those copies. They guarantee that even if you destruct
the original, the copy it stored is safe. And once you destruct the vector, the
copies it made are destructed as well.

Understanding value semantics is key for lab 10, and helps explain why they're
so handy. Something about STL containers could easily be a final question. 

Note that Jae's lecture note 22 does a good job of explaining containers and
iterators.

#### What can you do with containers? ####

Each container will support slightly different operations based on the semantic
meaing it has, and what it can do in a computationally efficient manner. However,
generally they can:

* Iterate
* Sort
* Insert new elements
* Remove old elements
* Swap around elements
* Search for things
* Splice 
* Merge multiple containers
* Be passed into functions that will do things, like printing them in a certain way


#### Vector ####

Vector is the prototypical container in this class. It's a sequential container,
supporting O(1) random access to elements, and O(1) amortized appends
(`push_back()`). Under the hood it's an array with some blank spaces on the
right;  whenever it runs out of blanks it creates a new (larger) array, and
copies the data from the old to new. It manages that underlying array
internally.

Vector is a lot like Java's ArrayList. 

#### Deque ####

Also known as dequeue, and pronounced "deck", it's a doubly ended queue: like a
vector with space on both the left and the right. This means that `push_front()`
and `pop_front()` are O(1) operations. 

Note: Deque is actually implemented differently. However, a vector with empty
space on  both ends is a good mental model. In reality it uses something like
a vector of vectors, and therefore it wastes more memory and it does not guarantee
that the elements will be stored contiguosly in memory.


#### List ####

List is a linked list. It's a fancier version of what you wrote for lab 3. In
particular, the list
class is doubly linked, so every node points not just to next, but also to
previous (i.e., the node that points to it). It also maintains a tail pointer in
addition to head, so it can addBack in O(1) constant time. It can also add/remove
in O(1) time anywhere in the list, as long as you already have a pointer/reference
to the right node.

However, searching requires it to step through the whole list. Therefore 
list does NOT define `operator[]` or anything similar - it has
no random access. Additionally, its iterator is much more complex than vector,
because vector is basically just a pointer. Here we need to map `++it` to advance
to the next node.

### Derived Containers ###

Based on vector, list, and deque, there are several derived containers that use
one of those as the underlying container. They include queue (uses either list
or deque), priority queue (using vector or deque), and stack (using any).

#### Set ####

Sets provide very efficient lookup to see if an element is in the set, along
with mathematical set operations like union, intersection, etc. They keep the
elements sorted using the operator< function. (Unimportant detail: internally
they use some type of self balancing binary search tree.)

There's also an unordered set (unimportant detail: that uses a hash table). 

#### Pair and Map ####

We discuss pair and map because it's useful to understand the flexibility and
total genericness of the STL containers. Absolute understanding of how they work
isn't important for 3157.

A pair is a class that lets you group two objects together, potentially of
different types. A simple implementation is just:

```cpp
    template <class T1, class T2>
    struct pair {
        T1 first;
        T2 second;
    };

    pair<string,int>  prez("obama", 1961);
    pair<string,int>  veep("biden", 1942);
```

Pairs are useful because they're often a natural extension of generic containers.
We want to store an int plus some value instead of just an int. Pairs
provide a convenient way to do that. 

One use is in map, which maps keys to values. It will return pairs when it wants
to return (key, value) tuples.

```cpp
  map<string,int>  word_count;
  string word;
  while (cin >> word)
      word_count[word]++;

  map<string,int>::iterator it;
  for (it = word_count.begin(); it != word_count.end(); ++it) {
      cout << it->first << " occurs ";          //recall that it->first is (*it).first
      cout << it->second << " times." << endl;
  }
```





## Iterators ##

Iterators are the key feature that makes containers so useful as a group. Every
container specifies an iterator type, providing a consistent way to access every
element stored in the container. Compare the same iterator code, one written for
a vector, and the other for a dequeue: 

```cpp

vector<string> v;
v.push_back("Hello");
v.push_back("world");

for (vector<string>::iterator it = v.begin(); it != v.end(); ++it)
    *it = "";

//Now for deque
deque<string> v{ "Hello", "World" };

for (deque<string>::iterator it = v.begin(); it != v.end(); ++it)
    *it = "";

// C++11 version
for (auto it = v.begin(); it != v.end(); ++it)
    *it = "";
```

`iterator` is a type member, which is a typedef defined *inside* a class.

`iterator`s act like pointers: they must provide the basics that pointers
provide (some do provide other features). In particular you must be able to get
the beginning, `v.begin()` and *one past the end* with `v.end()`. Both of these
functions return `iterator`s. However it's only valid to dereference the
returned value of `begin()`, because `end()` points past the end of the
container.

The iterator has to define three functions to be useful: `*`, `++` and `!=`.
With only those three we can do our entire iteration with any container, even
ones like trees that aren't strictly sequential.

- `operator*` returns a reference to the object to which the iterator is
currently pointing. In a `vector` the iterator is actually a pointer, so it
works without any code. In another class, say a linked list, the code has to do
more work to figure out what the object being stored is, and return that.

- `operator++` advances the iterator to the next element. Again, how it actually
happens depends entirely on what the container holds.

- `operator!=` is important, because for some container types the idea of
`operator<` doesn't really make sense. Hashmaps, for example, are entirely
unordered, so we can only really test whether two iterators are not equal, not
whether one is less than the other.

A container can create an iterator of different types depending on what sort of
behavior it wants to support. For example, a singly linked list (exactly like lab3)
might have a *forward* iterator. It can only go forward, and therefore would
support `++` but not `--`.

The 3 types to worry about are: *forward*, *bidirectional*, and *random access*.
Each can do all the things the ones to the left can do, and more. For example,
a doubly linked list (like the normal STL list) has bidirectional iterators,
because you can easily do `--it`. However only something like vector or deque
offers a random access iterator, which can jump around arbitrarily, in addition
to supporting `--` and `++`. 

There are also `const_iterator`, which gives you const references. However it
behaves exactly the same way conceptually.



## Clang ##

Clang is a newer alternative to gcc. It was designed primarily by Apple to be a
drop in replacement to gcc, but with a license apple prefers, and to support
better debugging and static analysis. Apple uses it for XCode, and it's
available on clic as well. So if you're getting crazy error messages you don't
understand, you can try changing `g++` to `clang++` in your Makefile, and seeing
if you get more easily interpreted error messages.




#### Anecdote: Template metaprogramming to an extreme ####

One really nice feature about templates is that, although they take a long time
to compile, they're incredibly fast running. There's literally no overhead
during execution for a templated class, because they're just turned into the
code that you would have written yourself anyways.

There's a really cool Bayesian statistics project at Columbia run by
Andrew Gelman's group called Stan http://mc-stan.org/. It's very fast to run
complicated statistical models (under a second for moderate sizes, where
competitors would be several minutes), in part because the whole thing very
extensively uses C++ templates. However it comes at the cost of 30-60 second
compilations every time you want to create a new model (even just a tiny tweak).
This is also because of the C++ templates, which mean it has to recompile a huge
amount of code every time. By using templates the code is a little trickier to
write, longer to compile, but the end result is execution times that are as fast
as hand written code, with significantly more flexibility for the end user. This
lets them have end users write whatever models they want, at basically no cost
in slowness.

A relatively simple example they have is:

```cpp
DoubleVectorView<include_summand<propto,T_scale>::value,is_vector<T_scale>::value> log_sigma(length(sigma));
```

Quoting Daniel Lee, one of the main developers of STAN on one of his favorite examples:

> If you look at the normal_log function (line 44 is the actual prototype), that function calculates the sum of log of the normal distribution given y, mu, sigma. So mathematically that's:
log(0) - (sigma * sqrt(2 * pi)) - (y - mu)^2 / (2 * sigma^2)

> I'll just enumerate some ways we're doing things really slick:
> 1) Each of our arguments, {y, mu, sigma} can be double or stan::agrad::var, our auto-diff variable, or any other type that has the basic operators defined. The template types can handle that all (T_y, T_loc, T_scale).
> 2) Each of our arguments, {y, mu, sigma} can be vectorized. That is, they can be a single value: double. Or a vector: std::vector<double>. Or an array: double[]. Or an Eigen Vector: Eigen::Matrix<double, 1, -1>. Or an Eigen Row Vector: Eigen::Matrix<double, -1, 1>. Oh yeah... since we've done the template magic, we can also replace double with our auto-diff type. The template function that handles the vectorization is hidden behind VectorView (lines 84-87). That is a struct that's defined in this file: https://github.com/stan-dev/stan/blob/develop/src/stan/meta/traits.hpp. The pattern is slick: expose operator[] to the struct. If it's a vector type, access the right element. If it's a double type, recycle the single element over and over again.
> 3) Efficiency: we decide not to compute "constants" at compile time. A constant, in our usage, is anything that's a double. A parameter is an autodiff variable. If you look at line 111 of normal.hpp, that's a compile time if check. If we're not computing constants at all, we don't do the += operator. In fact, the real power is in the loop in lines 91 - 95. For each sigma value we have (1 if it's a single value, if it's a vector type, the length of the values it has), we calculate an inv_sigma[n] for each value, and if and only if sigma is an autodiff type (that's what the if statement is saying in line 93), we calculate log_sigma, but only once per sigma value we have. Now, every time we need a log_sigma, we use the operator[] on the log_sigma struct to grab values. For example, if y is a std::vector<double> of length 10, and sigma is a stan::agrad::var, we only calculate log_sigma once, but when we loop over 10 elements in the for loop on line 97, we are reading the same value over and over again on line 114. If y is a std::vector<double> of length 10 and sigma is a std::vector<stan::agrad::var> of length 10, then we calculate log_sigma for each individual value. In our typical use case, we typically have a single variance parameter and a lot of data points (think 100s or 1000s or even millions). So calculating the log(sigma) once is a large savings. And cutting out other calculations actually does matter.

