#pragma once
#ifndef _FIELD_H
#define _FIELD_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>"

using namespace std;

class Field
{
public:
	Field();
	Field(string nameMap);
    void InitializationMap(vector < vector < pair<char,int> > > *graph, int &n_out, int &m_out);
	void SetLocation(string nameMap);
	void Print();
private:
	int height;
	int width;
	string nameLocation;
	void GetSize(string txt_Location, int &n_out, int &m_out);
	string GetTextFromFile();
};
#endif
