/* 
 * perlInteface.cpp: The main classes 
 *
 * 
 * Copyright (c) 2013 Hugo de Freitas Siqueira Sadok Menna Barreto <sadok.com.br>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <list>
#include <string>
#include <cstring>

#include <EXTERN.h>
#include <perl.h>

#include "perlInterface.h"

using namespace std;

/*
 * PerlObject
 */
PerlObject::PerlObject(SV* scalarValue, PerlInterpreter *interpreter){
	my_perl = interpreter;
	PERL_SET_CONTEXT(my_perl);

	perlScalarValue = scalarValue;
}

PerlObject::~PerlObject(){
	PERL_SET_CONTEXT(my_perl);
	sv_2mortal(perlScalarValue);

	FREETMPS;
}

PerlObject::operator char*() const{
	PERL_SET_CONTEXT(my_perl);
	return SvPV(perlScalarValue, PL_na);//PL_na here is to tell Perl to calculate the string length	
}

PerlObject::operator int() const{
	PERL_SET_CONTEXT(my_perl);
	return SvIV(perlScalarValue);
}

PerlObject::operator unsigned() const{
	PERL_SET_CONTEXT(my_perl);
	return SvUV(perlScalarValue);
}

PerlObject::operator double() const{
	PERL_SET_CONTEXT(my_perl);
	return SvNV(perlScalarValue);
}

PerlObject::operator bool() const{
	PERL_SET_CONTEXT(my_perl);
	return (bool) SvIV(perlScalarValue);
}

/********************************************/

/*
 * PerlStack
 */
 //public
PerlStack::PerlStack(PerlInterpreter *interpreter){
	my_perl = interpreter;
	numberOfElements = 0;
}

PerlStack::~PerlStack(){
	clearStack();
}

	//SV*  newSViv(IV);//int
    //SV*  newSVuv(UV);//unsigned
    //SV*  newSVnv(double);
    //SV*  newSVpv(const char*, STRLEN);
    //SV*  newSVpvn(const char*, STRLEN);
    //SV*  newSVpvf(const char*, ...);
    //SV*  newSVsv(SV*);

bool PerlStack::isEmpty(){
	return numberOfElements == 0;
}

PerlStack& PerlStack::push(const unsigned u){
	PERL_SET_CONTEXT(my_perl);
	innerList.push_back(newSVuv(u));
	++numberOfElements;
	return *this;
}

PerlStack& PerlStack::push(const int i){
	PERL_SET_CONTEXT(my_perl);
	innerList.push_back(newSViv(i));
	++numberOfElements;
	return *this;
}

PerlStack& PerlStack::push(const double d){
	PERL_SET_CONTEXT(my_perl);
	innerList.push_back(newSVnv(d));
	++numberOfElements;
	return *this;
}

PerlStack& PerlStack::push(const char* const s){
	PERL_SET_CONTEXT(my_perl);
	innerList.push_back(newSVpvn(s, strlen(s)));
	++numberOfElements;
	return *this;
}

PerlStack& PerlStack::push(const PerlStack& stack){
	PERL_SET_CONTEXT(my_perl);
	list<sv*> tempList = stack.getSVList();
	innerList.insert(innerList.end(), tempList.begin(), tempList.end());
	numberOfElements+=stack.numberOfElements; 
	return *this;
}

PerlStack& PerlStack::operator<<(const unsigned u){
	return push(u);
}

PerlStack& PerlStack::operator<<(const int i){
	return push(i);
}

PerlStack& PerlStack::operator<<(const double d){
	return push(d);
}

PerlStack& PerlStack::operator<<(const char* const s){
	return push(s);
}

PerlStack& PerlStack::operator<<(const string s){
	return push(s.c_str());
}

PerlStack& PerlStack::operator<<(const PerlStack& stack){
	return push(stack);
}

PerlObject PerlStack::pop(){
	PERL_SET_CONTEXT(my_perl);
	if (numberOfElements == 0){
		throw 1;
	}
	
	PerlObject perlObject(innerList.back(), my_perl);
	innerList.pop_back();

	--numberOfElements;

	return perlObject;
}

PerlObject PerlStack::get(){
	PERL_SET_CONTEXT(my_perl);
	if (numberOfElements == 0){
		throw 2;
	}

	PerlObject perlObject(innerList.front(), my_perl);
	innerList.pop_front();

	--numberOfElements;

	return perlObject;
}

PerlStack& PerlStack::push(SV* sv){
	PERL_SET_CONTEXT(my_perl);
	innerList.push_back(newSVsv(sv));
	++numberOfElements;
	return *this;
}

PerlStack& PerlStack::pushFront(SV* sv){
	PERL_SET_CONTEXT(my_perl);
	innerList.push_front(newSVsv(sv));
	++numberOfElements;

	return *this;
}

SV* PerlStack::getSV(){
	if (numberOfElements == 0){
		throw 2;
	}

	SV* returnSV = innerList.front();
	innerList.pop_front();
	
	--numberOfElements;

	return returnSV;
}

list<SV*> PerlStack::getSVList() const{
	PERL_SET_CONTEXT(my_perl);
	list<SV*> returnList;
	for(list<SV*>::const_iterator i = innerList.begin(); i != innerList.end(); ++i){
		returnList.push_back(newSVsv(*i));
	}

	return returnList;
}

void PerlStack::clearStack(){
	PERL_SET_CONTEXT(my_perl);
	for (list<SV*>::const_iterator i = innerList.begin(); i != innerList.end(); ++i){
    	sv_2mortal(*i);//mark the SV elements in the innerList as mortal
    }
	FREETMPS;
}

/********************************************/

/*
 * PerlInterface
 */
unsigned PerlInterface::numberOfInterfaces = 0;// this is actually the default value, it is maintained here for clarity

//public
PerlInterface::PerlInterface(const char* fileName) : PerlStack((PerlInterpreter*) NULL){
	if(numberOfInterfaces == 0){
		int falseArgc = 1;
		char **falseArgv = {NULL};
		PERL_SYS_INIT3(&falseArgc, &falseArgv, &environ);
	}
	++numberOfInterfaces;

	/*Creating the interpreter*/
	my_perl = perl_alloc();
	perl_construct(my_perl);
	PL_exit_flags |= PERL_EXIT_DESTRUCT_END;

	char fileNameCopy[strlen(fileName) + 1];

	if (strcpy(fileNameCopy, fileName) == NULL){
		throw 1;
	}

	char *perlArgs[] = {"", fileNameCopy};
	perl_parse(my_perl, NULL, 2, perlArgs, NULL);
}

PerlInterface::~PerlInterface(){
	PERL_SET_CONTEXT(my_perl);
	/*cleaning the interpreter*/
	perl_destruct(my_perl);
	perl_free(my_perl);

	if(numberOfInterfaces == 1){
		PERL_SYS_TERM();
	}

	--numberOfInterfaces;
}

PerlStack PerlInterface::sub(const char* functionName){
	PERL_SET_CONTEXT(my_perl);

	dSP;

	ENTER;
    SAVETMPS;

    PUSHMARK(SP);//remember the stack pointer

    while(!isEmpty()){
    	XPUSHs(sv_2mortal(getSV()));
    }

	PUTBACK;

	unsigned numberOfReturns = call_pv(functionName, G_ARRAY);
	SPAGAIN;

	PerlStack returnStack(my_perl);

	for(unsigned i=0; i < numberOfReturns; ++i){
		returnStack.pushFront(POPs);//get SV returned from the sub and push it to the stack
	}

	PUTBACK;
	FREETMPS; //free the return values
	LEAVE;

	return returnStack;
}

PerlStack PerlInterface::sub(const char* functionName, const PerlStack& param){
	clearStack();

	innerList = param.getSVList();
	numberOfElements = innerList.size();

	return sub(functionName);
}

PerlStack PerlInterface::newStack(){
	return PerlStack(my_perl);
}

unsigned PerlInterface::getNumberOfInterfaces(){
	return numberOfInterfaces;
}

/********************************************/
