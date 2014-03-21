/* 
 * CPlusPerl.h: Header file from the library. Classes that may be seen from the 
 *              outside of the library
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

/*
 * TODO:
 * port for windows
 * 
 */

#ifndef C_PLUS_PERL_H
#define C_PLUS_PERL_H

/* C++ Standard Lib */
#include <string>
#include <list>
#include <stdexcept>

/* Perl Stuff */
#include <EXTERN.h>
#include <perl.h>

class PerlStack;

/* 
 * The problem is that the PerlInterpreter is a pointer so it is not safe to pass 
 * it to the other objects. We also need a way to tell everybody if a PerlInterpreter
 * is destroyed.
 * 
 * SOLUTION:
 * We will have a parent-child architecture, once a PerlInterpreterManager object is 
 * copied, it is not really copied. In fact a child is created, if a parent is destroyed
 * it will tell its children.  If a child is 
 * going to be removed, it must tell its parent.  If a child who has other children is destroyed,
 * it passes its child to his parent. If a parent has a single child and will be destroyed, 
 * the PerlInterpreter do not need to be destroyed, it may just pass its attributes to its child.
 */
class PerlInterpreterManager{
	friend class PerlInterface;//to be able to get the my_perl pointer
public:
	explicit PerlInterpreterManager(const char* fileName);
	PerlInterpreterManager(const PerlInterpreterManager&);

	~PerlInterpreterManager();

	bool operator==(const PerlInterpreterManager&) const;
	bool operator!=(const PerlInterpreterManager&) const;

	void setContext() const;

	SV* newPerlSV(SV*) const;
	SV* newPerlSV(const unsigned) const;
	SV* newPerlSV(const int) const;
	SV* newPerlSV(const double) const;
	SV* newPerlSV(const char* const s) const;

	std::string getStringFromSV(SV*) const;
	int getIntFromSV(SV*) const;
	unsigned getUnsignedFromSV(SV*) const;
	double getDoubleFromSV(SV*) const;
	bool getBoolFromSV(SV*) const;

	void markSVasMortal(SV*);
	void freeTemps();

	bool isValid() const;

	static unsigned getNumberOfInterfaces();

private:
	PerlInterpreterManager* parent;
	mutable std::list<PerlInterpreterManager*> children;
	PerlInterpreter *my_perl;
	bool valid;
	static unsigned numberOfInstances;

	//should not be implemented
	PerlInterpreterManager& operator=(const PerlInterpreterManager&);//not a good idea to allow assignments to this class
};





class PerlObject{
public:
	PerlObject(SV*, PerlInterpreterManager&);
	PerlObject(const PerlObject&);

	~PerlObject();

	PerlObject& operator=(const PerlObject&);

	/* Converting to standard types */
	operator std::string() const;
	operator int() const;
	operator unsigned() const;
	operator double() const;
	operator bool() const;

	bool isValid() const;//know if it is a valid object or if the perl interpreter was killed

private:
	SV* perlScalarValue;
	PerlInterpreterManager perlManager;
};





class PerlStack{
public:
	/* Perl has 3 types: scalar (SV), array (AV) and hash (HV) */
	explicit PerlStack(const PerlInterpreterManager&);
	PerlStack(const PerlStack&);

	virtual ~PerlStack();

	PerlStack& operator=(const PerlStack&);

	PerlStack& push(const unsigned);
	PerlStack& push(const int);
	PerlStack& push(const double);
	PerlStack& push(const char* const);
	PerlStack& push(const PerlStack&);
	PerlStack& operator<<(const unsigned);
	PerlStack& operator<<(const int);
	PerlStack& operator<<(const double);
	PerlStack& operator<<(const char* const);
	PerlStack& operator<<(const std::string&);
	PerlStack& operator<<(const PerlStack&);

	bool isEmpty();

	PerlObject pop(); //get from the back and remove the element
	PerlObject get(); //get from the front and remove the element

	PerlStack& push(SV*);
	PerlStack& pushFront(SV*);

	std::list<SV*> getSVList() const;

	SV* getSV();

	void clearStack();

	bool isValid() const;//know if it is a valid object or if the perl interpreter was killed
	bool isValid(const PerlInterpreterManager& pim) const;//check if it is in the same interpreter as pim

protected:
	void copyList(std::list<SV*> list);

	PerlInterpreterManager perlManager;
	std::list<SV*> innerList;
	unsigned numberOfElements;
};





/* we have one of this for each perl file */
class PerlInterface : public PerlStack{
public:
	explicit PerlInterface(const char* fileName);

	~PerlInterface();

	PerlStack sub(const char* functionName);//pass parameters that are in this class inner stack
	PerlStack sub(const char* functionName, const PerlStack& param);

	PerlStack newStack();

private:
	PerlInterface(const PerlInterface&);
	PerlInterface& operator=(const PerlInterface&);
	const std::string fileName;
	PerlInterpreter* my_perl;
};





class PerlException : public std::logic_error{
public:
	explicit PerlException(const char* errorDescription);
};

class PerlStackIsEmpty : public PerlException{
public:
	PerlStackIsEmpty();
};

class AccessToDeadInterpreter : public PerlException{
public:
	AccessToDeadInterpreter();
};

class CrossInterpreterOperation : public PerlException{
public:
	CrossInterpreterOperation();
};

#endif
