C+Perl
======

The minimal Perl Wrapper for C++
This Library makes possible to call Perl functions (sub) from a C++ program in a very easy way.

Code Usage 
----------
*See docs/example.cpp for a working code example.*

If you have a Perl function in a file "example.pl" you want to use in your C++ program, first you will do:

	PerlInterface interface("example.pl");

Now you have your interface with Perl, just call your function by its name as it appears in the Perl file:

	interface.sub("superFuction");

This is it, easy as that!
Now suppose you want to give this function some parameters:

	interface << 2 << 3;
	int result = interface.sub("sum").get();

This passes the parameters to the interface and call it with them. Note that we use get() to get the result, this is important and will be clear in the next examples.

Perl allow us to return multiple values from a function, of course we may keep doing it:

	interface << 5 << 6 << 9;
	PerlStack ret = interface.sub("squareAll");
	int results[] = {ret.get(), ret.get(), ret.get()};//there is a smarter way of doing it, specially when you don't know how many parameters you will get

See what get() does? It gets the next value from a PerlStack, the sub() method returns a PerlStack, when you know there is only one return from your Perl function or simply don't care about the others, you don't need to keep the PerlStack, you may simply use get() and store only the value you want. This is what we did in the previous example.

In fact the PerlInterface is also a PerlStack, that is why you can pass the parameters directly to the interface, an alternative way to do it is to create a PerlStack and pass it to the sub method, check it out:

	PerlStack awesomeStack = interface.newStack();
	awesomeStack << 2 << 3;
	int result = interface.sub("sum", awesomeStack).get();

Uhm.. If the sub() returns a PerlStack and also accepts one as parameter, we may pass the output of one function to another:

	interface.sub("function1", interface.sub("function2"));


Installation
------------
You will get C+Perl as easy as pie:

**Linux**

If you have **Debian**, **Ubuntu**, **Mint** or other Debian based OS you may simply type in the terminal:

	> sudo apt-get install build-essential libperl-dev
	> wget --no-check-certificate https://github.com/hugombarreto/CPlusPerl/releases/download/v1.0/CPlusPerl-Ubuntu-Debian.tar.gz -O - | tar xz
After that you will have the CPlusPerl.a and the CPLusPerl.h

If you have **Fedora** or **RHEL** type:

	> sudo yum install make gcc-c++ perl-devel.x86_64 perl-ExtUtils-Embed.noarch
	> curl -L https://github.com/hugombarreto/CPlusPerl/releases/download/v1.0/CPlusPerl-Fedora-RHEL.tar.gz | tar zx

After that you will have the CPlusPerl.a and the CPlusPerl.h

If you have other Linux dist, check that you have the dependencies (make, g++, libperl-dev) and download the source code:

	> curl -LO https://github.com/hugombarreto/CPlusPerl/archive/master.zip && unzip master && cd master && make

_______________________________________________________________________________________________________________________
**Mac OS X**

Make sure you have Xcode and have command line tools enabled (in the Xcode menu go to Preferences > Downloads, in the box install the Command Line Tools)

	> curl -LO https://github.com/hugombarreto/CPlusPerl/releases/download/v1.0/CPlusPerl-MacOS10.9.zip && unzip CPlusPerl-MacOS10.9.zip
After that you will have the CPlusPerl.a and the CPLusPerl.h

_______________________________________________________________________________________________________________________
**Windows** (not supported yet)

	

