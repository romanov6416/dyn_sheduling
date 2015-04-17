/*
 * classes.cpp
 *
 *  Created on: 16.04.2015
 *      Author: andrey
 */

#include "classes.h"
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <stdlib.h>


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
	out << "<start name=" << name << " time=" << time << '>' << endl;
}

// ==================== End of class Task ====================


// ==================== class System ====================

System::System(const int time, vector<Task *> & v) :
	runtime(time), vTasks(v)
{}

void System::printSheduling()
{
	if (vTasks.size() == 0)
		return;
	vector<int> vExecTimes(vTasks.size());
	int curtime = 0;
	for (unsigned i = 0; i < vExecTimes.size(); ++i)
		vExecTimes[i] = -1;
	while (curtime < runtime)
	{
		int mostPrior = -1;
		int nTask = -1;
		for (unsigned i = 0; i < vTasks.size(); ++i)
			if (vExecTimes[i] < 0 || vTasks[i]->getPeriod() + vExecTimes[i] <= curtime)
				if ((mostPrior < 0 || vTasks[i]->getPriority() < mostPrior))
				{
					nTask = i;
					mostPrior = vTasks[i]->getPriority();
				}
		if (nTask >= 0)
		{
			vExecTimes[nTask] = curtime;
			curtime += vTasks[nTask]->getDuration();
			vTasks[nTask]->printXML(cout, vExecTimes[nTask]);
			continue;
		}
		++curtime;
	}
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
