#pragma once
class Settings
{
public:
	Settings(const map<string, map<string, string>> &settings);
	~Settings();

	// Gets a single value from a section and converts to the needed type
	string GetString(const string &section, const string &key, const string &defaultValue = "");
	int GetInt(const string &section, const string &key, const int defaultValue = 0);
	float GetFloat(const string &section, const string &key, const float defaultValue = 0.0f);
	bool GetBool(const string &section, const string &key, const bool defaultValue = true);
	double GetDouble(const string &section, const string &key, const double defaultValue = 0.0);

	// Gets all the values from a section
	map<string, string> GetAllValuesFromSection(const string &section);

private:
	// Map holding all the data
	map<string, map<string, string>> m_Settings;
};

