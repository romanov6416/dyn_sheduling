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
Task::Task(const string & s, const int p, const int d) : name(s), period(p), duration(d), priority(0)
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
	out << "<start name=" << name << " time=" << time << '>';
}

// ==================== End of class Task ====================


// ==================== class System ====================

bool System::analyze(const string & namefile)
{
	ParserXML parser(namefile);
	if (!parser.analyze(*this)) {
		cerr << "Error: can't read file \"" << namefile << "\"" << endl;
		return false;
	}
	return true;
}

// ==================== End of class System ====================


// ==================== class ParserXML ====================

ParserXML::ParserXML(const string & namefile) : name(namefile), file(name), line(1), pos(1)
{}

void ParserXML::error(const char c)
{
	throw ErrorParser(name, line, pos, c);
	//cerr << "Error: unexepected symbol \'" << c
	//		<< "\' in line " << line << " in position " << pos << endl;
}

bool ParserXML::get(char & c)
{
	bool happy_read = file.get(c);
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
	try
	{
		if (!file.is_open())
		{
			cerr << "Error: can't open file \"" << name << "\"" << endl;
			return nullptr;
		}
		// check first line in input file
		int runtime;
		vector<Task*> vTasks;
		if (!isPhrase("<system runtime="))
			return nullptr;
		if (!getNumber(runtime))
			return nullptr;
		if (!isPhrase(">\n"))
			return nullptr;
		while (true)
			vTasks.push_back(getTask());
	}
	catch(ErrorParser & err)
	{
		if (!(err.symbol == '/' && err.line != 1 && err.pos == 2))
		{
			err.print();
			return false;
		}
	}
	try
	{
		if (!isPhrase("</system>\n"))
			return false;
		if (get(c))
			error(c);
	}
	catch(ErrorParser & err)
	{
		err.print();
		return false;
	}
	return true;
}

bool ParserXML::isPhrase(const string & w)
{
	char c;
	for (unsigned int i = 0; i < w.size(); ++i)
	{
		if (!isLetter(w[i]))
			return false;
	}
	return true;
}

bool ParserXML::isLetter(const char & symbol)
{
	char c;
	if (!file.get(c) || symbol != c)
	{
		error(c);
		return false;
	}
	return true;
}

bool ParserXML::getNumber(int & number)
{
	string s;
	char c;
	while (file.get(c) && isdigit(c))
		s += c;
	if (!file.eof())
		file.unget();
	number = strtol(s.c_str(), nullptr,10);
	return s.empty();
}

bool ParserXML::getChar(char & symbol)
{
	char c;
	if (!file.get(c))
	{
		error(c);
		return false;
	}
	symbol = c;
	return true;
}

Task * ParserXML::getTask()
{
	string name;
	if (!)
}
