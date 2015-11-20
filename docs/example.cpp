
#include <iostream>

#include "CPlusPerl.h"

using namespace std;

void teste();

int main(int argc, char **argv)
{
	PerlInterface interface("example1Perl1.pl");//Starting the Perl in Interpreter in this Perl File

	cout << "Function 1 (no parameters and no return)" << endl;
	interface.sub("showtime");

	cout << endl << "Function 2 (with parameters and one return)" << endl;
	interface  << 2 << 3;
	int result = interface.sub("sum").get();
	cout << "Result: " << result << endl << endl;

	cout << "Function 3 (Returning multiple values)" << endl;
	interface << 5 << 6 << 9;
	PerlStack ret = interface.sub("squareAll");
	int results[] = {ret.get(), ret.get(), ret.get()};
	cout << "Results: " << results[0] << ", " << results[1] << ", " << results[2] << endl << endl;

	cout << "Function 4 (Passing the return of a function to another)" << endl;
	interface << 2 << 4 << 9;
	PerlStack stackToAdd = interface.newStack();
	stackToAdd << 1 << 2 << "C+Perl is awesome!" << 3.14159;
	interface.sub("distArg", stackToAdd << interface.sub("squareAll"));



	cout << endl << endl << "Now using another file" << endl;

	PerlInterface interface2("example1Perl2.pl");//Look that we need a different PerlInterface when using a different file

	cout << "Function 1 (no parameters and no return)" << endl;
	interface2.sub("showtime");

	cout << endl << "Function 2 (with parameters and one return)" << endl;
	interface2  << 2 << 3;
	result = interface2.sub("sum").get();
	cout << "Result: " << result << endl << endl;

	cout << "Function 3 (Returning multiple values)" << endl;
	interface2 << 5 << 6 << 9;
	PerlStack ret2 = interface2.sub("squareAll");
	int results2[] = {ret2.get(), ret2.get(), ret2.get()};
	cout << "Results: " << results2[0] << ", " << results2[1] << ", " << results2[2] << endl << endl;

	cout << "Function 4 (Passing the return of a function to another)" << endl;
	interface2 << 2 << 4 << 9;
	interface2.sub("distArg", interface2.sub("squareAll"));

	cout << "The end" << endl;
	return 0;
}
