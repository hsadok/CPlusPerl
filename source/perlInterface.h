/* 
 * perlInteface.h: Header file from the main classes 
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

#ifndef PERL_INTERFACE_H
#define PERL_INTERFACE_H

#include <string>
#include <list>

#include <EXTERN.h>
#include <perl.h>

using namespace std;

class PerlStack;

class PerlObject{
public:
	PerlObject(SV*, PerlInterpreter*);
	~PerlObject();

	/* Converting to standard types */
	operator char*() const;
	operator int() const;
	operator unsigned() const;
	operator double() const;
	operator bool() const;

private:
	SV* perlScalarValue;
	PerlInterpreter *my_perl;
};


class PerlStack{
public:
	/*Perl has 3 types: scalar (SV), array (AV) and hash (HV)*/
	explicit PerlStack(PerlInterpreter*);
	~PerlStack();

	bool isEmpty();

	PerlStack& push(const unsigned);
	PerlStack& push(const int);
	PerlStack& push(const double);
	PerlStack& push(const char* const);
	PerlStack& push(const PerlStack&);
	PerlStack& operator<<(const unsigned);
	PerlStack& operator<<(const int);
	PerlStack& operator<<(const double);
	PerlStack& operator<<(const char* const);
	PerlStack& operator<<(const string);
	PerlStack& operator<<(const PerlStack&);

	PerlObject pop(); //get from the back and remove the element
	PerlObject get(); //get from the front and remove the element

	PerlStack& push(SV*);
	PerlStack& pushFront(SV*);

	list<SV*> getSVList() const;

	SV* getSV();

	void clearStack();

protected:
	PerlInterpreter *my_perl;
	list<SV*> innerList;
	unsigned numberOfElements;
};

/* we should have one of this for each perl file */
class PerlInterface : public PerlStack{
public:
	explicit PerlInterface(const char* fileName);
	~PerlInterface();

	PerlInterface(const PerlInterface&);

	PerlStack sub(const char* functionName);//now I can pass parameters that are in this class inner stack
	PerlStack sub(const char* functionName, const PerlStack& param);

	PerlStack newStack();

	static unsigned getNumberOfInterfaces();

private:
	static unsigned numberOfInterfaces;
};

#endif
