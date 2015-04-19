/*
 * main.cpp
 *
 *  Created on: 16.04.2015
 *      Author: andrey
 */
#include <fstream>

#include "classes.h"
#include "tinyxml2.h"

#include <iostream>

inline System * error(const string & err = "unknown error")
{
	cerr << "Error: " << err << endl;
	return nullptr;
}

System * getSystemFromXML(const string & namefile)
{
	tinyxml2::XMLDocument fileXML;
	// if any error during parsing ...
	if (fileXML.LoadFile(namefile.c_str()) != 0)
		return error("can\'t open or parsing file");
	// read system
	auto sysElem = fileXML.FirstChildElement( "system" );
	if (sysElem == nullptr || string(sysElem->Name()) != string("system"))
		return error("can\'t first element is not \"system\"");
	// read runtime
	auto attr = sysElem->FirstAttribute();
	if (attr == nullptr || string(attr->Name()) != string("runtime"))
		return error("in element \"system\" first attribute is not \"runtime\"");
	int runtime = attr->IntValue();

	// read all tasks
	vector<Task*> vTasks;
	auto task = sysElem->FirstChildElement();
	try
	{
		while (task != nullptr)
		{
			// check task
			if (string(task->Name()) != string("task"))
				throw string("in element \"system\" child #") + to_string(vTasks.size()+1) + string(" is not \"task\"");

			// read attribute name
			auto attr = task->FirstAttribute();
			if (string(attr->Name()) != string("name"))
				throw string("in element \"system\" in child #") + to_string(vTasks.size()+1) +
						string(" with name \"task\" first attribute is not \"name\"");
			string name = attr->Value();

			// read attribute period
			attr = attr->Next();
			if (string(attr->Name()) != string("period"))
				throw string("in element \"system\" in child #") + to_string(vTasks.size()+1) +
						string(" with name \"task\" second attribute is not \"period\"");
			int period = attr->IntValue();

			// read attribute duration
			attr = attr->Next();
			if (string(attr->Name()) != string("duration"))
				throw string("in element \"system\" in child #") + to_string(vTasks.size()+1) +
						string(" with name \"task\" third attribute is not \"duration\"");
			int duration = attr->IntValue();

			// if there are more than 3 attributes
			if (attr->Next() != nullptr)
				throw string("in element \"system\" child #") + to_string(vTasks.size()+1) +
						string(" with name \"task\" has more than three attributes");

			// add new task
			vTasks.push_back(new Task(name, period, duration));

			// move to next task
			task = task->NextSiblingElement();
		}
	}
	catch(const string & err)
	{
		for(unsigned i = 0; i < vTasks.size(); ++i)
			delete vTasks[i];
		return error(err);
	}
	if (sysElem->NextSiblingElement() != nullptr)
		return error("there are more than one root element");
	return new System(runtime, vTasks);
}



int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		cerr << "Error: wrong number of parameters (need to 3 parameters)" << endl;
		return 1;
	}

	System * system = getSystemFromXML(argv[1]);
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
