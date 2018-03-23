#pragma once
class Settings;

enum Type
{
	EQUALS,
	ID,
	NUMBER,
	STRING,
	UNDIFINED,
};

enum LineType
{
	VariableDeclaration,
	StringDeclaration,
	SectionDeclaration,
	IDEqualsID,
	INVALID,
};

class INILoader
{
public:
	INILoader();
	~INILoader();

	void Load(const string &path);

	void EnableDebugger(bool enable);

	Settings *GetSettings() const;

	static void RemoveCharacter(string &stringRef, const char &toRemove);

private:
	map<string, map<string, string>> m_Settings;

	LineType GetLineType(const string &line);

	bool IsID(const char character, bool atStart = true);

	bool IsNumber(const char character);

	LineType GetType(const vector<Type> &typeList);

	bool m_Debug = false;

	Settings *m_pSettings = nullptr;
};

