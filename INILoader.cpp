// Custom INI parser made by Cordemans Robin

#include "stdafx.h"
#include "INILoader.h"
#include "Settings.h"

INILoader::INILoader() : m_pSettings(nullptr)
{
}

INILoader::~INILoader()
{
	delete m_pSettings;
}

void INILoader::Load(const string &path)
{
	const string commentLineIdentifier = "//";
	const string commentTextStartIdentifier = "/*";
	const string commentTextEndIdentifier = "*/";
	const char sectionStartIdentifier = '[';
	const char sectionEndIdentifier = ']';
	const char equalIdentifier = '=';
	const char stringStartAndEndIdentifier = '"';

	string currentSection = "";
	string varName = "";
	string value = "";

	ifstream iniFileStream;
	iniFileStream.open(path);

	bool startComment = false;

	if (!iniFileStream.is_open()) { cout << "File " << path << " could not be opened!" << endl; return; }

	while (!iniFileStream.eof())
	{
		string line;
		getline(iniFileStream, line);

		if (startComment)
		{
			if (line.find(commentTextEndIdentifier) != string::npos)
			{
				// Comment has ended
				startComment = false;
				line = line.substr(line.find(commentTextEndIdentifier) + commentTextEndIdentifier.length(), line.length() - line.find(commentTextEndIdentifier) - commentTextEndIdentifier.length());
			}
			else
			{
				// Ignore this line
				line = "";
			}
		}

		if (line.find(commentTextStartIdentifier) != string::npos)
		{
			line = line.substr(0, line.find(commentTextStartIdentifier));
			startComment = true;
		}

		if (line.find(commentLineIdentifier) != string::npos)
		{
			int pos = line.find(commentLineIdentifier);
			if (pos != 0)
			{
				// Comment is not at the start meaning there is a piece of code in front of it
				line = line.substr(0, pos);
			}
			else
			{
				// Comment is at the start so this line should be ignored
				line = "";
			}
		}
		if (line != "")
		{
			// Before we add this line we have to check what it is and if it is valid
			line += ' ';
			LineType lineType = GetLineType(line);
			switch (lineType)
			{
			case VariableDeclaration:
				if (m_Debug) cout << "Detected line is a variable declaration" << endl;
				varName = line.substr(0, line.find(equalIdentifier));
				if (m_Debug) cout << "Variable name is: " << varName << endl;
				value = line.substr(line.find(equalIdentifier) + 1);
				if (m_Debug) cout << "Value is: " << value << endl;
				m_Settings[currentSection][varName] = value;
				break;
			case StringDeclaration:
				if (m_Debug) cout << "Detected line is a string variable declaration" << endl;
				varName = line.substr(0, line.find(equalIdentifier));
				if (m_Debug) cout << "Variable name is: " << varName << endl;
				value = line.substr(line.find(stringStartAndEndIdentifier) + 1, line.rfind(stringStartAndEndIdentifier) - line.find(stringStartAndEndIdentifier) - 1);
				if (m_Debug) cout << "Value is: " << value << endl;
				m_Settings[currentSection][varName] = value;
				break;
			case SectionDeclaration:
				if (m_Debug) cout << "Detected line is a section declaration" << endl;
				currentSection = line.substr(line.find(sectionStartIdentifier) + 1, line.find(sectionEndIdentifier) - line.find(sectionStartIdentifier) - 1);
				break;
			case IDEqualsID:
				if (m_Debug) cout << "Detected line is an ID EQUALS another ID" << endl;
				varName = line.substr(0, line.find(equalIdentifier));
				if (m_Debug) cout << "Variable name is: " << varName << endl;
				value = line.substr(line.find(equalIdentifier) + 1);
				if (m_Debug) cout << "Other name is: " << value << endl;
				m_Settings[currentSection][varName] = value;
				break;
			case INVALID:
				if (m_Debug) cout << "Detected line is invalid!" << endl;
				// Ignore this line
				break;
			default:
				break;
			}
		}
		if (m_Debug) cout << "Original: " << line << endl;
	}

	iniFileStream.close();

	m_pSettings = new Settings(m_Settings);
}

Settings *INILoader::GetSettings() const
{
	return m_pSettings;
}

LineType INILoader::GetLineType(const string &line)
{
	const char equalIdentifier = '=';
	const char sectionStartIdentifier = '[';
	const char sectionEndIdentifier = ']';
	const char stringStartAndEndIdentifier = '"';

	vector<Type> types;

	Type currentFoundType = Type::UNDIFINED;

	string toCheck;

	for (size_t i = 0; i < line.length(); ++i)
	{
		if (i == 0 && line.at(i) == sectionStartIdentifier)
		{
			string section = line;
			RemoveCharacter(section, ' ');
			if (section.at(section.length() - 1) != sectionEndIdentifier || section.length() <= 2) return LineType::INVALID;
			return LineType::SectionDeclaration;
		}

		if (currentFoundType == Type::UNDIFINED)
		{
			if (line.at(i) == equalIdentifier)
			{
				if (m_Debug) cout << "A EQUALS was found" << endl;
				types.push_back(Type::EQUALS);
			}
			else if (IsID(line.at(i)))
			{
				currentFoundType = Type::ID;
			}
			else if (line.at(i) == stringStartAndEndIdentifier)
			{
				currentFoundType = Type::STRING;
			}
			else if(IsNumber(line.at(i)))
			{
				currentFoundType = Type::NUMBER;
			}
		}
		else if(currentFoundType == Type::ID)
		{
			if (!IsID(line.at(i), false))
			{
				if (m_Debug) cout << "An ID was found" << endl;
				types.push_back(Type::ID);
				currentFoundType = Type::UNDIFINED;
				--i;
			}
		}
		else if (currentFoundType == Type::STRING)
		{
			if (line.at(i) == stringStartAndEndIdentifier)
			{
				if (m_Debug) cout << "A STRING was found" << endl;
				types.push_back(Type::STRING);
				currentFoundType = Type::UNDIFINED;
			}
		}
		else if (currentFoundType == Type::NUMBER)
		{
			if (!IsNumber(line.at(i)))
			{
				if (m_Debug) cout << "A NUMBER was found" << endl;
				types.push_back(Type::NUMBER);
				currentFoundType = Type::UNDIFINED;
				--i;
			}
		}
	}

	// Check if sequence of types are correct (ID EQUALS NUMBER | ID EQUALS STRING)
	return GetType(types);
}

bool INILoader::IsID(const char character, bool atStart)
{
	string varNameContainables;
	if (atStart)
	{
		varNameContainables = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	}
	else
	{
		varNameContainables = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	}

	auto it = find_if(varNameContainables.begin(), varNameContainables.end(), [&character](const char &c)
	{
		return (character == c);
	});
	return (it != varNameContainables.end());
}

bool INILoader::IsNumber(const char character)
{
	const string numbers = "0123456789.";

	auto it = find_if(numbers.begin(), numbers.end(), [&character](const char &c)
	{
		return (character == c);
	});
	return (it != numbers.end());
}

LineType INILoader::GetType(const vector<Type> &typeList)
{
	if (typeList.size() == 3)
	{
		if (typeList[0] == Type::ID)
		{
			if (typeList[1] == Type::EQUALS)
			{
				switch (typeList[2])
				{
				case NUMBER:
					return LineType::VariableDeclaration;
				case STRING:
					return LineType::StringDeclaration;
				case ID:
					return LineType::IDEqualsID;
				}
			}
		}
	}
	return LineType::INVALID;
}

void INILoader::RemoveCharacter(string &stringRef, const char &toRemove)
{
	string newString = "";
	for_each(stringRef.begin(), stringRef.end(), [toRemove, &newString](const char &c)
	{
		if (c != toRemove)
		{
			newString += c;
		}
	});
	stringRef = newString;
}

void INILoader::EnableDebugger(bool enable)
{
	m_Debug = enable;
}