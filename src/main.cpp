/*
 * main.cpp
 *
 *  Created on: 16.04.2015
 *      Author: andrey
 */
#include <fstream>

#include "classes.h"

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		cerr << "Error: wrong number of parameters (need to 3 parameters)" << endl;
		return 1;
	}
	ParserXML parser(argv[1]);
	System * system = parser.analyze();
	if (system == nullptr)
	{
		cerr << "Error: can't parse file \"" << argv[1] << '\"' << endl;
		return 2;
	}
	ofstream output(argv[2]);
	if (!output.is_open())
	{
		cerr << "Error: can't open file \"" << argv[2] << '\"' << endl;
		return 3;
	}
	system->printSheduling(output);
	return 0;
}
