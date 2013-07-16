
#include "languagemanager.h"

LanguageManager::LanguageManager()
{
  std::string firstLanguage;

  activeLanguage = 0;

  LanguageOptions = new ConfigFile( "resource/language.txt" );
  LanguageOptions->GetStringValue( "Languages", 0, &firstLanguage );
  SetActiveLanguage( firstLanguage );
}

LanguageManager::~LanguageManager()
{
  delete LanguageOptions;
  delete activeLanguage;
}

void LanguageManager::SetActiveLanguage( std::string LanguageCode )
{
  std::string languageFilename = "resource/language_";
  if( activeLanguage != 0 )
  {
    delete activeLanguage;
  }

  languageFilename.append( LanguageCode );
  languageFilename.append( ".txt" );

  activeLanguageCode = LanguageCode;
  activeLanguage = new ConfigFile( languageFilename );
}

std::string* LanguageManager::GetLanguageString( std::string Identifier )
{
#ifdef WRITE_LOG
  printf( "LanguageManager: Getting String %s:%s\n", activeLanguageCode.c_str(), Identifier.c_str() );
#endif

  if( activeLanguage == 0 )
  {
    return new std::string( Identifier );
  }
  std::string* translatedText = new std::string();
  if( activeLanguage->KeyExists( Identifier ) )
  {
    activeLanguage->GetStringValue( Identifier, translatedText );
  } else {
    translatedText->append( Identifier );
  }
  return translatedText;
}
