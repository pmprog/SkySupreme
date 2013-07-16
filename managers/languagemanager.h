
#pragma once

#include "../framework/configfile.h"

class LanguageManager
{

	private:
	  std::string activeLanguageCode;
    ConfigFile* activeLanguage;

	public:
	  ConfigFile* LanguageOptions;

		LanguageManager();
		~LanguageManager();

    void SetActiveLanguage( std::string LanguageCode );
		std::string* GetLanguageString( std::string Identifier );

};
