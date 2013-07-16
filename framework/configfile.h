
#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <list>

typedef struct ConfigData
{
	std::string* Key;
	bool IsArray;
	std::vector<std::string*>* Contents;
} ConfigData;


class ConfigFile
{
	private:
		std::list<ConfigData*> Contents;

		void ParseFile( std::string TextContents );
		bool IsNumber( std::string s );
		std::string* EscapeString( std::string s );

		ConfigData* GetData( std::string Key );

	public:
		ConfigFile();
		ConfigFile( std::string Filename );
		~ConfigFile();
		bool Save( std::string Filename );

		bool KeyExists( std::string Key );
		bool KeyIsArray( std::string Key );

		int GetArraySize( std::string Key );
		void RemoveArrayElement( std::string Key, int ArrayIndex );
		void RemoveKey( std::string Key );

		bool GetBooleanValue( std::string Key, bool* Value );
		bool GetBooleanValue( std::string Key, int ArrayIndex, bool* Value );
		bool GetIntegerValue( std::string Key, int* Value );
		bool GetIntegerValue( std::string Key, int ArrayIndex, int* Value );
		bool GetInteger64Value( std::string Key, long* Value );
		bool GetInteger64Value( std::string Key, int ArrayIndex, long* Value );
		bool GetFloatValue( std::string Key, float* Value );
		bool GetFloatValue( std::string Key, int ArrayIndex, float* Value );
		bool GetStringValue( std::string Key, std::string* Value );
		bool GetStringValue( std::string Key, int ArrayIndex, std::string* Value );

		bool GetQuickBooleanValue( std::string Key );
		bool GetQuickBooleanValue( std::string Key, int ArrayIndex );
		int GetQuickIntegerValue( std::string Key );
		int GetQuickIntegerValue( std::string Key, int ArrayIndex );
		long GetQuickInteger64Value( std::string Key );
		long GetQuickInteger64Value( std::string Key, int ArrayIndex );
		float GetQuickFloatValue( std::string Key );
		float GetQuickFloatValue( std::string Key, int ArrayIndex );
		std::string* GetQuickStringValue( std::string Key );
		std::string* GetQuickStringValue( std::string Key, int ArrayIndex );

		bool SetBooleanValue( std::string Key, bool Value );
		bool SetBooleanValue( std::string Key, int ArrayIndex, bool Value );
		bool SetIntegerValue( std::string Key, int Value );
		bool SetIntegerValue( std::string Key, int ArrayIndex, int Value );
		bool SetInteger64Value( std::string Key, long Value );
		bool SetInteger64Value( std::string Key, int ArrayIndex, long Value );
		bool SetFloatValue( std::string Key, float Value );
		bool SetFloatValue( std::string Key, int ArrayIndex, float Value );
		bool SetStringValue( std::string Key, std::string* Value );
		bool SetStringValue( std::string Key, int ArrayIndex, std::string* Value );


};
