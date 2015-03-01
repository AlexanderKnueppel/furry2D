/*
* ****************************************
*
* This file is part of Furry2D, a simple gameframework for 2D desktop games.
*
* Copyright (c) 2015 Furry2D. All rights reserved.
*
* For the full copyright and license information, please view the LICENSE.txt
* file that was distributed with this source code.
*
* \Author Alexander Knueppel
*
* ****************************************
*/

#include <algorithm>
#include <string> 
#include <sstream>
#include <iostream>
#include <fstream>

#include <furry2d/furry2d.h>

FURRY_NS_BEGIN

ConfigFile::Conversion::Conversion(const std::string & value) {
	value_ = value;
}

ConfigFile::Conversion::Conversion(const char* c) {
	value_ = c;
}

ConfigFile::Conversion::Conversion(double d) {
	std::stringstream s;
	s << d;
	value_ = s.str();
}

ConfigFile::Conversion::Conversion(bool b) {
	value_ = (b) ? "true" : "false";
}

ConfigFile::Conversion::Conversion(int i) {
	std::stringstream s;
	s << i;
	value_ = s.str();
}

ConfigFile::Conversion::Conversion(const Conversion & other) {
	value_ = other.value_;
}

ConfigFile::Conversion& ConfigFile::Conversion::operator=(const Conversion & other) {
	value_ = other.value_;
	return *this;
}

ConfigFile::Conversion& ConfigFile::Conversion::operator=(double d) {
	std::stringstream s;
	s << d;
	value_ = s.str();
	return *this;
}

ConfigFile::Conversion& ConfigFile::Conversion::operator=(bool b) {
	value_ = (b) ? "true" : "false";
	return *this;
}

ConfigFile::Conversion& ConfigFile::Conversion::operator=(int i) {
	std::stringstream s;
	s << i;
	value_ = s.str();
	return *this;
}

ConfigFile::Conversion& ConfigFile::Conversion::operator=(const std::string & s) {
	value_ = s;
	std::transform(value_.begin(), value_.end(), value_.begin(), ::tolower);
	return *this;
}

ConfigFile::Conversion::operator std::string() const {
	return value_;
}

ConfigFile::Conversion::operator double() const {
	std::stringstream s(value_.c_str());
	double d;
	s >> d;
	return d;
}

ConfigFile::Conversion::operator int() const {
	std::stringstream s(value_.c_str());
	int i;
	s >> i;
	return i;
}

ConfigFile::Conversion::operator bool() const {
	return !(value_ == std::string("false") || value_ == std::string("0"));
}

std::string trim(const std::string & source, const char* delims = " \t\r\n") {
	std::string result(source);
	std::string::size_type index = result.find_last_not_of(delims);
	if (index != std::string::npos)
		result.erase(++index);

	index = result.find_first_not_of(delims);
	if (index != std::string::npos)
		result.erase(0, index);
	else
		result.erase();
	return result;
}

void ConfigFile::addConfigFile(const std::string & configFile) {
	std::ifstream file(configFile.c_str());

	if (!file.good()) {
		throw std::string("Can't open config file: ") + configFile.c_str();
	}

	std::string line;
	std::string name;
	std::string value;
	std::string inSection;
	size_t posEqual;
	while (std::getline(file, line)) {

		if (!line.length()) continue;

		if (line[0] == '#') continue;
		if (line[0] == ';') continue;

		if (line[0] == '[') {
			inSection = trim(line.substr(1, line.find(']') - 1));
			continue;
		}

		posEqual = line.find('=');
		name = trim(line.substr(0, posEqual));
		value = trim(line.substr(posEqual + 1));

		content_[inSection + '/' + name] = Conversion(value);
	}

	file.close();
}

ConfigFile::ConfigFile(const std::string & configFile) {
	if (configFile != "") {
		try {
			addConfigFile(configFile);
		}
		catch (const std::string str) {
			throw str;
		}
	}
}

ConfigFile::Conversion const& ConfigFile::Value(const std::string & section, const std::string & entry) const {

	std::map<std::string, Conversion>::const_iterator ci = content_.find(section + '/' + entry);

	if (ci == content_.end()) throw "does not exist";

	return ci->second;
}

ConfigFile::Conversion const& ConfigFile::Value(const std::string & section, const std::string & entry, double value) {
	try {
		return Value(section, entry);
	}
	catch (const char *) {
		return content_.insert(std::make_pair(section + '/' + entry, Conversion(value))).first->second;
	}
}

ConfigFile::Conversion const& ConfigFile::Value(const std::string & section, const std::string & entry, int value) {
	try {
		return Value(section, entry);
	}
	catch (const char *) {
		return content_.insert(std::make_pair(section + '/' + entry, Conversion(value))).first->second;
	}
}

ConfigFile::Conversion const& ConfigFile::Value(const std::string & section, const std::string & entry, bool value) {
	try {
		return Value(section, entry);
	}
	catch (const char *) {
		return content_.insert(std::make_pair(section + '/' + entry, Conversion(value))).first->second;
	}
}

ConfigFile::Conversion const& ConfigFile::Value(const std::string & section, const std::string & entry, const std::string & value) {
	try {
		return Value(section, entry);
	}
	catch (const char *) {
		return content_.insert(std::make_pair(section + '/' + entry, Conversion(value))).first->second;
	}
}

FURRY_NS_END