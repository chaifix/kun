#ifndef __UTIL_H
#define __UTIL_H
#include <string>
#include <string>
#include <fstream>
#include <streambuf>

using std::string;
using std::istreambuf_iterator;
using std::ifstream;

string loadSource(const string& file);

string floatToStr(float f);
float strToFloat(std::string s);

bool isFloat(string myString);

string textToCString(string text);

#endif