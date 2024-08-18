#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <math.h>
#include <cmath>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <memory>
#include <sstream>
#include <stdexcept>

#include <json/json.h>

#include <inicpp.hpp>

class FileSys
{
public:
	const char* ReadINIstring(const char* file, const char* section, const char* key);
	int ReadINIint(const char* file, const char* section, const char* key);
	float ReadINIfloat(const char* file, const char* section, const char* key);

	void WriteINIstring(const char* file, const char* section, const char* key, const char* value);
	void WriteINIint(const char* file, const char* section, const char* key, int value);
	void WriteINIfloat(const char* file, const char* section, const char* key, float value);

	const char* ReadJSONstring(const char* file, const char* key);
	int ReadJSONint(const char* file, const char* key);
	float ReadJSONfloat(const char* file, const char* key);

	void WriteJSONstring(const char* file, const char* key, const char* value);
	void WriteJSONint(const char* file, const char* key, int value);
	void WriteJSONfloat(const char* file, const char* key, float value);

};



#endif // !FILESYSTEM_H
