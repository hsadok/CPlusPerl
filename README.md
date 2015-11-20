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

That's it, easy as that!
Now suppose you want to give this function some parameters:

	interface << 2 << 3;
	int result = interface.sub("sum").get();

This passes the parameters to the interface and call it with them. Note that we use get() to get the result, this is important and will be clear in the next examples.

Perl allow us to return multiple values from a function, of course we may keep doing it:

	interface << 5 << 6 << 9;
	PerlStack ret = interface.sub("squareAll");
	int results[] = {ret.get(), ret.get(), ret.get()};//there is a smarter way of doing it, specially if you don't know how many parameters you will get

See what get() does? It gets the next value from a PerlStack. The return of the sub() method is also a PerlStack, when you know there is only one return from your Perl function or simply don't care about the others, you may simply use get() and store only the value you want. This is what we did in the previous example.

In fact the PerlInterface is also a PerlStack, that is why you can pass the parameters directly to the interface. An alternative way of doing it is to create a PerlStack and pass it to the sub method, check it out:

	PerlStack awesomeStack = interface.newStack();
	awesomeStack << 2 << 3;
	int result = interface.sub("sum", awesomeStack).get();

Uhm... If the sub() returns a PerlStack and also accepts one as parameter, we may pass the output of one function to another:

	interface.sub("function1", interface.sub("function2"));


Installation
------------
You will get C+Perl as easy as pie:

**Linux**

If you have **Debian**, **Ubuntu**, **Mint** or other Debian based OS you may simply type in the terminal::

	$ sudo apt-get install build-essential libperl-dev
	$ wget --no-check-certificate https://github.com/hugombarreto/CPlusPerl/releases/download/v1.0/CPlusPerl-Ubuntu-Debian.tar.gz -O - | tar xz

After that you will have the ``CPlusPerl.a`` and the ``CPLusPerl.h``

If you have **Fedora** or **RHEL** type::

	$ sudo yum install make gcc-c++ perl-devel.x86_64 perl-ExtUtils-Embed.noarch
	$ curl -L https://github.com/hugombarreto/CPlusPerl/releases/download/v1.0/CPlusPerl-Fedora-RHEL.tar.gz | tar zx

After that you will have the ``CPlusPerl.a`` and the ``CPlusPerl.h``

If you have other Linux dist, check that you have the dependencies (make, g++, libperl-dev) and download the source code::

	$ curl -LO https://github.com/hugombarreto/CPlusPerl/archive/master.zip && unzip master && cd master && make

_______________________________________________________________________________________________________________________
**Mac OS X**

Make sure you have Xcode and command line tools are enabled (in the terminal type: xcode-select --install)

	> curl -LO https://github.com/hugombarreto/CPlusPerl/releases/download/v1.0/CPlusPerl-MacOS10.9.zip && unzip CPlusPerl-MacOS10.9.zip
After that you will have the ``CPlusPerl.a`` and the ``CPlusPerl.h``

_______________________________________________________________________________________________________________________
**Windows** (not supported yet)


Using the Library
-----------------
Once you have the ``CPlusPerl.a` and the ``CPlusPerl.h`` you may be wondering how to compile your source code. There is a kind of "Makefile template" you can use to make your life easier.

Copy and edit the `Makefile template <docs/example_Makefile.make>`_. Of course you can adapt the Makefile to your needs but there are two lines you very likely need to change. They are marked with a comment in the beginning that reads::

	##### EDIT THE FOLLOWING LINES #####

Make sure it contains a list of your C++ source files (you shouldn't list your Perl files as they are not to be compiled!).
Now make sure you place this file with your source code and rename it to `Makefile`.

To compile your code just execute the following command in the same directory as your Makefile::

	make

If your code is right it should work, and a file with the executable name you specified should appear in the same directory. If you're lost with this Makefile thing you may consider reading a bit about it `here <https://www.gnu.org/software/make/manual/html_node/Introduction.html>`_.
