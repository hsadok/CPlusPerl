/* 
 * PerlInterface.cpp: 
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
#include <cstdlib>

#include <EXTERN.h>
#include <perl.h>

#include "CPlusPerl.h"


PerlInterface::PerlInterface(const char* fileName)
: PerlStack(PerlInterpreterManager(fileName)), fileName(fileName)
{
	my_perl = perlManager.my_perl;
}

PerlInterface::~PerlInterface()
{}

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

	PerlStack returnStack(perlManager);

	for(unsigned i=0; i < numberOfReturns; ++i){
		returnStack.pushFront(POPs);//get SV returned from the sub and push it to the stack
	}

	PUTBACK;
	FREETMPS; //free the return values
	LEAVE;

	return returnStack;
}

PerlStack PerlInterface::sub(const char* functionName, const PerlStack& param){
	if(!param.isValid(perlManager)){
		throw CrossInterpreterOperation();
	}
	PERL_SET_CONTEXT(my_perl);
	clearStack();

	innerList = param.getSVList();
	numberOfElements = innerList.size();

	return sub(functionName);
}

PerlStack PerlInterface::newStack(){
	return PerlStack(perlManager);
}
