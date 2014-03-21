/* 
 * PerlObject.cpp: Class to handle Perl Objects
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

//#include <list>
//#include <string>
#include <cstring>
#include <cstdlib>

#include <EXTERN.h>
#include <perl.h>

#include "CPlusPerl.h"

PerlObject::PerlObject(SV* scalarValue, PerlInterpreterManager& interpreter)
: perlManager(interpreter)
{
	perlManager.setContext();

	perlScalarValue = scalarValue;
}

PerlObject::PerlObject(const PerlObject& rhs)
: perlManager(rhs.perlManager)
{
	perlManager.setContext();

	perlScalarValue = perlManager.newPerlSV(rhs.perlScalarValue);
}

PerlObject& PerlObject::operator=(const PerlObject& rhs){
	if(rhs.perlManager != perlManager){
		throw CrossInterpreterOperation();
	}
	perlManager.setContext();
	perlManager.markSVasMortal(perlScalarValue);
	perlScalarValue = perlManager.newPerlSV(rhs.perlScalarValue);
	perlManager.freeTemps();//Should free temps only after creating the new one in order to avoid problems with self assignments
	return *this;
}

PerlObject::~PerlObject(){
	perlManager.setContext();
	perlManager.markSVasMortal(perlScalarValue);

	perlManager.freeTemps();
}

PerlObject::operator std::string() const{
	perlManager.setContext();
	return perlManager.getStringFromSV(perlScalarValue);
}

PerlObject::operator int() const{
	perlManager.setContext();
	return perlManager.getIntFromSV(perlScalarValue);
}

PerlObject::operator unsigned() const{
	perlManager.setContext();
	return perlManager.getUnsignedFromSV(perlScalarValue);
}

PerlObject::operator double() const{
	perlManager.setContext();
	return perlManager.getDoubleFromSV(perlScalarValue);
}

PerlObject::operator bool() const{
	perlManager.setContext();
	return perlManager.getBoolFromSV(perlScalarValue);
}

bool PerlObject::isValid() const{
	return perlManager.isValid();
}
