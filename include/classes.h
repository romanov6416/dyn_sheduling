/*
 * classes.h
 *
 *  Created on: 16.04.2015
 *      Author: andrey
 */

#ifndef CLASSES_H_
#define CLASSES_H_

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;


class Task
{
	string name;
	int period;
	int duration;
	int priority;
public:
	Task(const string & s, const int p, const int d);
	const string & getName();
	const int getPeriod();
	const int getDuration();
	const int getPriority();
	void setPriority(const int p);
	void printXML(ostream & out, const int time);
};

class System
{
	int runtime;
	vector<Task *> vTasks;
	static System * error(const string & err = "unknown error");
public:
	System(const int time, vector<Task *> & v);
	void printSheduling(ostream & out);
	static System * getSystemFromXML(const string & namefile);
	~System();
};

struct ErrorParser
{
	string name;
	int line;
	int pos;
	char symbol;
public:
	ErrorParser(const string & namefile,
			const int l,
			const int p,
			const char c);
	void print();
};

class ParserXML
{
	string name;
	ifstream file;
	int line;
	int pos;

	bool get(char & c);
	void error(const char c);
	void isPhrase(const string & w);
	void isLetter(const char & symbol);
	void getNumber(int & number);
	void getChar(char & symbol);
	void getName(string & name);
	Task * getTask();
public:
	ParserXML(const string & namefile);
	System * analyze();
};


#endif /* CLASSES_H_ */
