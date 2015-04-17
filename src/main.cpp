/*
 * main.cpp
 *
 *  Created on: 16.04.2015
 *      Author: andrey
 */

#include "classes.h"

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		cerr << "Error: wrong number of parameters" << endl;
		return 1;
	}
	ParserXML parser(argv[1]);
	System * system = parser.analyze();
	if (system == nullptr)
	{
		cerr << "Error: can't parse file \"" << argv[1] << '\"' << endl;
		return 2;
	}
	system->printSheduling();
	return 0;
}
