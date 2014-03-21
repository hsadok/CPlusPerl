
#include <iostream>

#include "CPlusPerl.h"

using namespace std;

void teste();

int main(int argc, char **argv)
{
	PerlInterface interface("example1Perl1.pl");//Iniciando o arquivo perl

	cout << "Funcao 1 (sem parametros e sem valor de retorno)" << endl;
	interface.sub("showtime");

	cout << endl << "Funcao 2 (com parametros e com 1 retorno)" << endl;
	interface  << 2 << 3;
	int resultado = interface.sub("soma").get();
	cout << "Resultado: " << resultado << endl << endl;

	cout << "Funcao 3 (Retornando multiplos parametros)" << endl;
	interface << 5 << 6 << 9;
	PerlStack ret = interface.sub("squareAll");
	int resultados[] = {ret.get(), ret.get(), ret.get()};
	cout << "Resultados: " << resultados[0] << ", " << resultados[1] << ", " << resultados[2] << endl << endl;

	cout << "Funcao 4 (Passando saida de uma funcao como argumento de outra)" << endl;
	interface << 2 << 4 << 9;
	PerlStack stackToAdd = interface.newStack();
	stackToAdd << 1 << 2 << "lalala" << 3.14159;
	interface.sub("distArg", stackToAdd << interface.sub("squareAll"));



	cout << endl << endl << "Agora usando um outro arquivo" << endl;

	PerlInterface interface2("example1Perl2.pl");

	cout << "Funcao 1 (sem parametros e sem valor de retorno)" << endl;
	interface2.sub("showtime");

	cout << endl << "Funcao 2 (com parametros e com 1 retorno)" << endl;
	interface2  << 2 << 3;
	resultado = interface2.sub("soma").get();
	cout << "Resultado: " << resultado << endl << endl;

	cout << "Funcao 3 (Retornando multiplos parametros)" << endl;
	interface2 << 5 << 6 << 9;
	PerlStack ret2 = interface2.sub("squareAll");
	int resultados2[] = {ret2.get(), ret2.get(), ret2.get()};
	cout << "Resultados: " << resultados2[0] << ", " << resultados2[1] << ", " << resultados2[2] << endl << endl;

	cout << "Funcao 4 (Passando saida de uma funcao como argumento de outra)" << endl;
	interface2 << 2 << 4 << 9;
	interface2.sub("distArg", interface2.sub("squareAll"));

	cout << "Fim" << endl;
	return 0;
}
