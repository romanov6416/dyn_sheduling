/*
 * classes.cpp
 *
 *  Created on: 16.04.2015
 *      Author: andrey
 */

#include "classes.h"
#include "tinyxml2.h"

#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <random>
#include <time.h>


// ==================== class Task ====================

Task::Task(const string & s, const int p, const int d) : name(s), period(p), duration(d), priority(p)
{}

const string & Task::getName()
{
	return name;
}

const int Task::getPeriod()
{
	return period;
}

const int Task::getDuration()
{
	return duration;
}

const int Task::getPriority()
{
	return priority;
}

void Task::setPriority(const int p)
{
	priority = p;
}

void Task::printXML(ostream & out, const int time)
{
	out << "<start name=\"" << name << "\" time=\"" << time <<"\"/>" << endl;
}

// ==================== End of class Task ====================



// ==================== class System ====================

System::System(const int time, vector<Task *> & v) :
	runtime(time), vTasks(v)
{}

System * System::error(const string & err)
{
	cerr << "Error: " << err << endl;
	return nullptr;
}

void System::printSheduling(ostream & out)
{
	//
	if (vTasks.empty())
		return;
	// last time, when tasks has init
	vector<int> vExecTimes(vTasks.size());
	int curtime = 0;
	// in beginning all tasks must be executing, so init last time as "(-1) * period"
	for (unsigned i = 0; i < vExecTimes.size(); ++i)
		vExecTimes[i] = (-1) * vTasks[i]->getPeriod();
	// random for choosing one of set the most important tasks
	srand(time(NULL));
	out << "<trace runtime=\"2000\">" << endl;
	while (curtime < runtime)
	{
		int mostPrior = -1;
		vector<int> vNumberTask;
		for (unsigned i = 0; i < vTasks.size(); ++i)
		{
			// timeout (period) has expired
			if (vExecTimes[i] + vTasks[i]->getPeriod() <= curtime)
			{
				// it is found more important (priority) task
				if (vTasks[i]->getPriority() > mostPrior)
				{
					vNumberTask.clear();
					vNumberTask.push_back(i);
					mostPrior = vTasks[i]->getPriority();
				} // the same important (priority) task
				else if (vTasks[i]->getPriority() == mostPrior)
				{
					vNumberTask.push_back(i);
				}
			}
		}
		if (vNumberTask.empty())
		{
			++curtime;
			continue;
		}

		int chosenTask = vNumberTask.size() == 1 ? vNumberTask[0] : rand() % vNumberTask.size();
		// remember time of execute starting
		vExecTimes[chosenTask] = curtime;
		// time is moving to end of executing task
		curtime += vTasks[chosenTask]->getDuration();
		// printing executing task
		vTasks[chosenTask]->printXML(out, vExecTimes[chosenTask]);
	}
	out << "</trace>" << endl;
}

System * System::getSystemFromXML(const string & namefile)
{
	tinyxml2::XMLDocument fileXML;
	// if any error during parsing ...
	if (fileXML.LoadFile(namefile.c_str()) != 0)
		return error("can\'t open or parsing file");
	// read system
	auto sysElem = fileXML.FirstChildElement();
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
			if (attr == nullptr || string(attr->Name()) != string("name"))
				throw string("in element \"system\" in child #") + to_string(vTasks.size()+1) +
						string(" with name \"task\" first attribute is not \"name\"");
			string name = attr->Value();

			// read attribute period
			attr = attr->Next();
			if (attr == nullptr || string(attr->Name()) != string("period"))
				throw string("in element \"system\" in child #") + to_string(vTasks.size()+1) +
						string(" with name \"task\" second attribute is not \"period\"");
			int period = attr->IntValue();

			// read attribute duration
			attr = attr->Next();
			if (attr == nullptr || string(attr->Name()) != string("duration"))
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

System::~System()
{
	for (unsigned i = 0; i < vTasks.size(); ++i)
		delete vTasks[i];
}

// ==================== End of class System ====================



// ==================== class ErrorParser ====================

ErrorParser::ErrorParser(const string & namefile,
		const int l, const int p, const char c) :
				name(namefile), line(l), pos(p), symbol(c)

{}

void ErrorParser::print()
{
	cerr << "Error: in file \"" << name <<
			"\" in line " << line <<
			" in position " << pos <<
			" unexepected symbol \'" << symbol << '\'' <<
			endl;
}

// ==================== End of class ErrorParser ====================

// ==================== class ParserXML ====================

ParserXML::ParserXML(const string & namefile) : name(namefile), file(namefile.c_str()), line(1), pos(1)
{}

void ParserXML::error(const char c)
{
	throw ErrorParser(name, line, pos, c);
	//cerr << "Error: unexepected symbol \'" << c
	//		<< "\' in line " << line << " in position " << pos << endl;
}

bool ParserXML::get(char & c)
{
	bool happy_read = file.get(c); //cout << static_cast<int>(c);
	if (c == '\n')
	{
		++line;
		pos = 0;
	}
	else
		++pos;
	return happy_read;
}

System * ParserXML::analyze()
{
	if (!file.is_open())
	{
		cerr << "Error: can't open file \"" << name << '\"' << endl;
		return nullptr;
	}
	int runtime;
	vector<Task*> vTasks;
	try
	{
		// check first line in input file
		isPhrase("<system runtime="); getNumber(runtime); isPhrase(">\n");
		// check task line in input file
		while (true)
			vTasks.push_back(getTask());
	}
	catch(ErrorParser & err)
	{
		if (!(err.symbol == '/' && err.line != 1 && err.pos == 2))
		{
			for (unsigned i = 0; i < vTasks.size(); ++i)
				delete vTasks[i];
			err.print();
			return nullptr;
		}
	}
	try
	{
		// "</" was reading, so we need read remain symbols
		isPhrase("system>\n");
		// check end of file
		char c;
		if (get(c))
			error(c);
	}
	catch(ErrorParser & err)
	{
		err.print();
		return nullptr;
	}
	return new System(runtime, vTasks);
}

void ParserXML::isPhrase(const string & w)
{
	for (unsigned int i = 0; i < w.size(); ++i)
		isLetter(w[i]);
}

void ParserXML::isLetter(const char & symbol)
{
	char c;
	if (!get(c) || symbol != c)
		error(c);
}

void ParserXML::getNumber(int & number)
{
	string s;
	char c;
	while (get(c) && isdigit(c))
		s += c;
	if (!file.eof())
		file.unget();
	number = strtol(s.c_str(), nullptr,10);
	if (s.empty())
		error(c);
}

void ParserXML::getChar(char & symbol)
{
	char c;
	if (!get(c))
		error(c);
	symbol = c;
}

void ParserXML::getName(string & name)
{
	char c;
	getChar(c);
	if (!isalpha(c))
		error(c);
	name = c;
	try
	{
		getChar(c);
		while (isalpha(c) || isdigit(c))
		{
			name += c;
			getChar(c);
		}
	}
	catch(...)
	{}
	if (!file.eof())
		file.unget();
}

Task * ParserXML::getTask()
{
	string name;
	int period, duration;
	isPhrase("<task name=\""); getName(name);
	isPhrase("\" period="); getNumber(period);
	isPhrase(" duration="); getNumber(duration);
	isPhrase(">\n");
	return new Task(name, period, duration);
}
