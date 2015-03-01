#ifndef __FURRY_CORE_CONFIGFILE_H__
#define __FURRY_CORE_CONFIGFILE_H__

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

#include <string>
#include <map>

FURRY_NS_BEGIN

class FURRY_API ConfigFile {
public:
	class Conversion {
	public:
		Conversion() {};
		explicit Conversion(const std::string&);
		explicit Conversion(double);
		explicit Conversion(int);
		explicit Conversion(bool);
		explicit Conversion(const char*);

		Conversion(const Conversion&);
		Conversion& operator=(const Conversion &);

		Conversion& operator=(double);
		Conversion& operator=(const std::string &);
		Conversion& operator=(bool);
		Conversion& operator=(int);

		operator std::string() const;
		operator double() const;
		operator bool() const;
		operator int() const;
	private:
		std::string value_;
	};

	ConfigFile(const std::string & configFile = "");
	void addConfigFile(const std::string & configFile);

	Conversion const& Value(const std::string & section, const std::string & entry) const;
	Conversion const& Value(const std::string & section, const std::string & entry, double value);
	Conversion const& Value(const std::string & section, const std::string & entry, int value);
	Conversion const& Value(const std::string & section, const std::string & entry, bool value);
	Conversion const& Value(const std::string & section, const std::string & entry, const std::string & value);

private:
	std::map<std::string, Conversion> content_;
};

FURRY_NS_END

#endif