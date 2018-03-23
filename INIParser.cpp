// INIParser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "INILoader.h"
#include "Settings.h"

int main()
{
	INILoader *pLoader = new INILoader();
	pLoader->EnableDebugger(true);
	pLoader->Load("Settings.ini");

	Settings *pSettings = pLoader->GetSettings();

	int myInt = pSettings->GetInt("RandomValues", "MyInt", 0);
	cout << "MyInt in section [RandomValues] is " << myInt << endl;
	float myFloat = pSettings->GetFloat("RandomValues", "MyFloat");
	cout << "MyFloat in section [RandomValues] is " << myFloat << endl;
	double myDouble = pSettings->GetDouble("RandomValues", "MyDouble");
	cout << "MyDouble in section [RandomValues] is " << myDouble << endl;
	string myString = pSettings->GetString("RandomValues", "MyString");
	cout << "MyString in section [RandomValues] is " << myString << endl;
	bool myBool = pSettings->GetBool("RandomValues", "MyBool");
	if (myBool)
	{
		cout << "MyBool in section [RandomValues] is enabled!" << endl;
	}
	else
	{
		cout << "MyBool in section [RandomValues] is disabled!" << endl;
	}

	cin.get();

	delete pLoader;

    return 0;
}
