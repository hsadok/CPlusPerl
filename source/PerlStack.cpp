/* 
 * PerlStack.cpp: Class that simulates a stack with PerlObjects, this stack is 
 *                used as function parameters and to get returns.
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
#include <cstdlib>

#include <EXTERN.h>
#include <perl.h>

#include "CPlusPerl.h"

PerlStack::PerlStack(const PerlInterpreterManager& interpreter)
: perlManager(interpreter), numberOfElements(0)
{}

PerlStack::PerlStack(const PerlStack& rhs)
: perlManager(rhs.perlManager), numberOfElements(rhs.numberOfElements)
{
	copyList(rhs.innerList);
}

PerlStack::~PerlStack(){
	clearStack();
}


PerlStack& PerlStack::operator=(const PerlStack& rhs){
	if(rhs.perlManager != perlManager){
		throw CrossInterpreterOperation();
	}
	copyList(rhs.innerList);
	numberOfElements = rhs.numberOfElements;

	return *this;
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
	perlManager.setContext();
	innerList.push_back(perlManager.newPerlSV(u));
	++numberOfElements;
	return *this;
}

PerlStack& PerlStack::push(const int i){
	perlManager.setContext();
	innerList.push_back(perlManager.newPerlSV(i));
	++numberOfElements;
	return *this;
}

PerlStack& PerlStack::push(const double d){
	perlManager.setContext();
	innerList.push_back(perlManager.newPerlSV(d));
	++numberOfElements;
	return *this;
}

PerlStack& PerlStack::push(const char* const s){
	perlManager.setContext();
	innerList.push_back(perlManager.newPerlSV(s));
	++numberOfElements;
	return *this;
}

PerlStack& PerlStack::push(const PerlStack& stack){
	if(stack.perlManager != perlManager){
		throw CrossInterpreterOperation();
	}
	perlManager.setContext();
	std::list<sv*> tempList = stack.getSVList();
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

PerlStack& PerlStack::operator<<(const std::string& s){
	return push(s.c_str());
}

PerlStack& PerlStack::operator<<(const PerlStack& stack){
	return push(stack);
}

PerlObject PerlStack::pop(){
	perlManager.setContext();
	if (numberOfElements == 0){
		throw PerlStackIsEmpty();
	}
	
	PerlObject perlObject(innerList.back(), perlManager);
	innerList.pop_back();

	--numberOfElements;

	return perlObject;
}

PerlObject PerlStack::get(){
	perlManager.setContext();
	if (numberOfElements == 0){
		throw PerlStackIsEmpty();
	}

	PerlObject perlObject(innerList.front(), perlManager);
	innerList.pop_front();

	--numberOfElements;

	return perlObject;
}

PerlStack& PerlStack::push(SV* sv){
	perlManager.setContext();
	innerList.push_back(perlManager.newPerlSV(sv));
	++numberOfElements;
	return *this;
}

PerlStack& PerlStack::pushFront(SV* sv){
	perlManager.setContext();
	innerList.push_front(perlManager.newPerlSV(sv));
	++numberOfElements;

	return *this;
}

SV* PerlStack::getSV(){
	if (numberOfElements == 0){
		throw PerlStackIsEmpty();
	}

	SV* returnSV = innerList.front();
	innerList.pop_front();
	
	--numberOfElements;

	return returnSV;
}

std::list<SV*> PerlStack::getSVList() const{
	perlManager.setContext();
	std::list<SV*> returnList;
	for(std::list<SV*>::const_iterator i = innerList.begin(); i != innerList.end(); ++i){
		returnList.push_back(perlManager.newPerlSV(*i));
	}

	return returnList;
}

void PerlStack::clearStack(){
	try{
		perlManager.setContext();
		for (std::list<SV*>::const_iterator i = innerList.begin(); i != innerList.end(); ++i){
	    	perlManager.markSVasMortal(*i);//mark the SV elements in the innerList as mortal
	    }
		perlManager.freeTemps();
	}
	catch (AccessToDeadInterpreter& e)
	{}
	innerList.clear();
}

bool PerlStack::isValid() const{
	return perlManager.isValid();
}

bool PerlStack::isValid(const PerlInterpreterManager& pim) const{
	return pim == perlManager;
}

void PerlStack::copyList(std::list<SV*> list){
	perlManager.setContext();
	for (std::list<SV*>::iterator i = list.begin(); i != list.end(); ++i){
		innerList.push_back(perlManager.newPerlSV(*i));
	}
}
