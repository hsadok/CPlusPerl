/* 
 * PerlInterpreterManager.cpp: This is the class that manages the PerlInterpreter
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

#include <iostream>
#include <list>
#include <cstring>

#include <EXTERN.h>
#include <perl.h>

//Used to get the environment in Mac OS X
#ifdef __APPLE__
#include <crt_externs.h>
#endif

#include "CPlusPerl.h"

unsigned PerlInterpreterManager::numberOfInstances = 0;

PerlInterpreterManager::PerlInterpreterManager(const char* fileName)
: parent(NULL), children(), valid(true)
{
	if(numberOfInstances == 0){
		int falseArgc = 0;
		char **falseArgv = {NULL};

		#ifdef __APPLE__ //in Mac OS X we need to use the _NSGetEnviron() to get the environment
			PERL_SYS_INIT3(&falseArgc, &falseArgv, _NSGetEnviron());
		#else // This is most like Linux works
			PERL_SYS_INIT3(&falseArgc, &falseArgv, &environ);
		#endif
	}

	++numberOfInstances;

	/*Creating the interpreter*/
	my_perl = perl_alloc();
	perl_construct(my_perl);
	PL_exit_flags |= PERL_EXIT_DESTRUCT_END;

	char fileNameCopy[strlen(fileName) + 1];

	if (strcpy(fileNameCopy, fileName) == NULL){
		throw PerlException("Error copying");
	}

	char *perlArgs[] = {"", fileNameCopy};
	perl_parse(my_perl, NULL, 2, perlArgs, NULL);
}

PerlInterpreterManager::PerlInterpreterManager(const PerlInterpreterManager& parentManager)
: children()
{
	++numberOfInstances;
	parent = const_cast<PerlInterpreterManager*>(&parentManager);
	valid = parentManager.isValid();
	my_perl = parentManager.my_perl;
	parentManager.children.push_back(this);
}

PerlInterpreterManager::~PerlInterpreterManager(){
	try{
		if (parent == NULL){
			if (children.size() == 1){//has a single child
				(*children.begin())->parent = NULL;
				(*children.begin())->valid = true;
			}
			else{
				for(std::list<PerlInterpreterManager*>::iterator i = children.begin(); i != children.end(); ++i){
					(*i)->parent = NULL;
					(*i)->valid = false;
				}
				if (isValid()){
					/* Killing the Perl Interpreter */
					setContext();
					perl_destruct(my_perl);
					perl_free(my_perl);
				}
			}
		}
		else{
			for(std::list<PerlInterpreterManager*>::iterator i = children.begin(); i != children.end(); ++i){
				(*i)->parent = parent;
			}
			parent->children.splice(parent->children.end(), children);
			parent->children.remove(this);
		}

		if(--numberOfInstances == 0){
			setContext();
			PERL_SYS_TERM();
		}
	}
	catch (AccessToDeadInterpreter& e){
		std::cerr << "Perl exception caught: " << e.what() << '\n';
	}
}

bool PerlInterpreterManager::operator==(const PerlInterpreterManager& rhs) const{
	return my_perl == rhs.my_perl;
}

bool PerlInterpreterManager::operator!=(const PerlInterpreterManager& rhs) const{
	return !operator==(rhs);
}

void PerlInterpreterManager::setContext() const{
	if (isValid()){
		PERL_SET_CONTEXT(my_perl);
	}
	else{
		throw AccessToDeadInterpreter();
	}
}

SV* PerlInterpreterManager::newPerlSV(SV* old) const{
	if(isValid()){
		return newSVsv(old);
	}
	throw AccessToDeadInterpreter();
}

SV* PerlInterpreterManager::newPerlSV(const unsigned u) const{
	if(isValid()){
		return newSVuv(u);
	}
	throw AccessToDeadInterpreter();
}

SV* PerlInterpreterManager::newPerlSV(const int i) const{
	if(isValid()){
		return newSViv(i);
	}
	throw AccessToDeadInterpreter();
}

SV* PerlInterpreterManager::newPerlSV(const double d) const{
	if(isValid()){
		return newSVnv(d);
	}
	throw AccessToDeadInterpreter();
}

SV* PerlInterpreterManager::newPerlSV(const char* const s) const{
	if(isValid()){
		return newSVpvn(s, strlen(s));
	}
	throw AccessToDeadInterpreter();
}

std::string PerlInterpreterManager::getStringFromSV(SV* perlScalarValue) const{
	if(isValid()){
		return std::string(SvPV_nolen(perlScalarValue));
	}
	throw AccessToDeadInterpreter();
}

int PerlInterpreterManager::getIntFromSV(SV* perlScalarValue) const{
	if(isValid()){
		return SvIV(perlScalarValue);
	}
	throw AccessToDeadInterpreter();
}

unsigned PerlInterpreterManager::getUnsignedFromSV(SV* perlScalarValue) const{
	if(isValid()){
		return SvUV(perlScalarValue);
	}
	throw AccessToDeadInterpreter();
}

double PerlInterpreterManager::getDoubleFromSV(SV* perlScalarValue) const{
	if(isValid()){
		return SvNV(perlScalarValue);
	}
	throw AccessToDeadInterpreter();
}

bool PerlInterpreterManager::getBoolFromSV(SV* perlScalarValue) const{
	if(isValid()){
		return static_cast<bool>(SvIV(perlScalarValue));
	}
	throw AccessToDeadInterpreter();
}

void PerlInterpreterManager::markSVasMortal(SV* old){
	if(isValid()){
		sv_2mortal(old);
	}
	else{
		throw AccessToDeadInterpreter();
	}
}

void PerlInterpreterManager::freeTemps(){
	if(isValid()){
		FREETMPS;
	}
	else{
		throw AccessToDeadInterpreter();
	}
}

bool PerlInterpreterManager::isValid() const{
	if (parent == NULL){
		return valid;
	}
	return parent->isValid();
}

unsigned PerlInterpreterManager::getNumberOfInterfaces(){
	return numberOfInstances;
}
