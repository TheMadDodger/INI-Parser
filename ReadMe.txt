================================================================================
								    INI-Parser
================================================================================

An easy to use INI Parser capable of reading INI files with error detection.

The following classes explained:

- INILoader: This is the class responsible for loading in the INI files
			 Detecting declarations and errors in declarations
			 The loader will always skip lines marked as comment with //

- Settings:  This is the class that holds the actual data of an INI file
			 It is created and returned when INILoader loads in a file
			 And is automatically filled in with all the correct data

Standard library includes needed for the code to work:
string, map, iostream, vector, fstream, algorithm

The files you need to copy over to your own project:
INILoader.h, INILoader.cpp, Settings.cpp, Settings.h, INIParser.h
Make sure all of the copied files are in the same folder.

File to include into stdafx.h:
INIParser.h

Code example:
You can find an example of how to use the loader in INIParser.cpp

How to use the loader:
1. Make sure you have all the files needed copied correctly and included.
2. Create an object of the INILoader class.
   This can either be on stack or on the heap.
3. Use the method INILoader::Load(string path);
   to load in an INI file from the specified path
4. Retreive the data from the loader by using INILoader::GetSettings();
   This returns a pointer to a Settings object, containing all the data.
5. Use the getters in Settings to retreive the data you whish to use.
   Settings::GetFloat(string section, string value, [optional] float default);
   Settings::GetInt(string section, string value, [optional] int default);
   Settings::GetString(string section, string value, [optional] string default);
   Settings::GetBool(string section, string value, [optional] bool default);
   Settings::GetDouble(string section, string value, [optional] double default);
   The optional argument default will be returned if no value can be found.
   You don't have to specify a default value.

Extra functions
INILoader::EnableDebugger(bool enable);
-> When enable is set to true the system will output everything line it analyzes.
   To the console, with the results of analyzing included.
   You can use this to find error lines in your INI file.

INILoader::RemoveCharacter(string &stringRef, const char &toRemove);
-> Extra function you can use to remove characters inside of a string.

Settings::GetAllValuesFromSection(const string &section);
-> Can be used to return all data from a single section in the form of
   map<string, string>
   Beware as this data is formatted as strings.
   The getters convert these to the required data types for you.

Terms of use:
- You are free to use the code in your own projects
- You don't have to mention to author
- You can modify the code to your likings
- This software is provided as-is
  I am NOT responsible to ANY harm caused to your system!

Thank you for using INI Loader by Cordemans Robin!

/////////////////////////////////////////////////////////////////////////////
