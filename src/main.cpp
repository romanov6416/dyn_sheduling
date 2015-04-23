/*
 * main.cpp
 *
 *  Created on: 16.04.2015
 *      Author: andrey
 */

#include "classes.h"
#include <string>
#include <iostream>
#include <stdlib.h>

string to_string(const int i)
{
	char str[20];
	sprintf(str, "%d", i);
	return string(str);
}



int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		cerr << "Error: wrong number of parameters (need to 3 parameters)" << endl;
		return 1;
	}
	System * system = System::getSystemFromXML(argv[1]);
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
	delete system;
	return 0;
}
