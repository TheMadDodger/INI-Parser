#include "stdafx.h"
#include "Settings.h"
#include "INILoader.h"

Settings::Settings(const map<string, map<string, string>> &settings) : m_Settings(settings)
{
}

Settings::~Settings()
{
}

string Settings::GetString(const string &section, const string &key, const string &defaultValue)
{
	if (m_Settings.find(section) == m_Settings.end()) return defaultValue;
	if (m_Settings[section].find(key) == m_Settings[section].end()) return defaultValue;
	string result = m_Settings[section][key];
	return result;
}

int Settings::GetInt(const string &section, const string &key, const int defaultValue)
{
	string value = GetString(section, key);
	if (value == "") return defaultValue;
	return stoi(value);
}

float Settings::GetFloat(const string &section, const string &key, const float defaultValue)
{
	string value = GetString(section, key);
	if (value == "") return defaultValue;
	return stof(value);
}

bool Settings::GetBool(const string &section, const string &key, const bool defaultValue)
{
	string value = GetString(section, key);
	INILoader::RemoveCharacter(value, ' ');
	if (value == "") return defaultValue;
	if (value == "true") return true;
	else if (value == "false") return false;
	else return defaultValue;
}

map<string, string> Settings::GetAllValuesFromSection(const string &section)
{
	if (m_Settings.find(section) == m_Settings.end())
	{
		map<string, string> error;
		error["ERROR"] = "Section not found!";
		return error;
	}
	return m_Settings[section];
}

double Settings::GetDouble(const string &section, const string &key, const double defaultValue)
{
	string value = GetString(section, key);
	if (value == "") return defaultValue;
	return stod(value);
}
