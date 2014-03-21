
#include <iostream>
#include <string>
#include <cstring>

#include "CPlusPerl.h"

void teste();

int main(int argc, char **argv)
{
	std::cout << "Testing C+Perl..." << std::endl << std::endl;
	PerlInterface interface("test1-1.pl");

	//1
	interface.sub("no_args");

	//2
	interface << 2 << -2 << 3.5 << "C" << "+" << "Perl";
	interface.sub("args_no_return");

	//3
	interface.sub("args_no_return");

	//4
	std::cout << static_cast<std::string>(interface.sub("no_args_return").get()) << std::endl;

	//5
	PerlStack ret = interface.sub("mult_return");
	while(!ret.isEmpty()){
		std::string s = ret.get();
		std::cout << s << " ";
	}
	std::cout << std::endl;

	//6
	PerlStack stackToAdd = interface.newStack();
	stackToAdd << "6:";
	interface << "!";
	interface.sub("disp_arg", stackToAdd << interface.sub("pass_func_ret"));

	//// New Interface here
	PerlInterface interface2("test1-2.pl");
	
	//7
	interface2.sub("no_args");

	//8
	interface2 << 2 << -2 << 3.5 << "C" << "+" << "Perl";
	interface2.sub("args_no_return");

	//9
	interface2.sub("args_no_return");

	//10
	std::cout << static_cast<std::string>(interface2.sub("no_args_return").get()) << std::endl;

	//11
	PerlStack ret2 = interface2.sub("mult_return");
	while(!ret2.isEmpty()){
		std::string s = ret2.get();
		std::cout << s << " ";
	}
	std::cout << std::endl;

	//12
	PerlStack stackToAdd2 = interface2.newStack();
	stackToAdd2 << "12:";
	interface2 << "!";
	interface2.sub("disp_arg", stackToAdd2 << interface2.sub("pass_func_ret"));

	//Test exceptions:
	//13
	try{
		interface2.get();
		std::cout << "---> Problem in 13!!" << std::endl;
		return 13;
	}
	catch (PerlStackIsEmpty& e){
		std::cout << "13: Testing exception: \"" << e.what() << "\" this is OK!!" << std::endl;
	}

	//14
	PerlInterface* interfaceToBeRemoved;
	interfaceToBeRemoved = new PerlInterface("test1-1.pl");
	PerlStack stack1 = interfaceToBeRemoved->newStack();
	PerlStack stack2 = interfaceToBeRemoved->newStack();
	delete interfaceToBeRemoved;
	try{
		stack1 << 1;
		std::cout << "---> Problem in 14!!" << std::endl;
		return 14;
	}
	catch (AccessToDeadInterpreter& e){
		std::cout << "14: Testing exception: \"" << e.what() << "\" this is OK!!" << std::endl;
	}

	//15
	try{
		stackToAdd2 << 1;
		stackToAdd << stackToAdd2;
		std::cout << "---> Problem in 15!!" << std::endl;
		return 15;
	}
	catch (CrossInterpreterOperation& e){
		std::cout << "15: Testing exception: \"" << e.what() << "\" this is OK!!" << std::endl;
	}

	std::cout << std::endl << "Test is over! The C+Perl is probably working fine!" << std::endl;

	return 0;
}
