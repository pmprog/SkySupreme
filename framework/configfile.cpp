
#include "configfile.h"
#include "graphicslib.h"
#include <ctype.h>

ConfigFile::ConfigFile()
{
}

ConfigFile::ConfigFile( std::string Filename )
{
	ALLEGRO_FILE* fileHnd;
	std::string document;
	char buf[1024];

	fileHnd = al_fopen( Filename.c_str(), "r" );
	if( fileHnd != 0 )
	{
		document.clear();
		while( !al_feof( fileHnd ) )
		{
			al_fgets( fileHnd, (char*)&buf, 1024 );
			document.append( (char*)&buf );
			memset( (void*)buf, 0, 1024 );
		}
		ParseFile( document );
		al_fclose( fileHnd );
	}
}

ConfigFile::~ConfigFile()
{
	while( Contents.size() > 0 )
	{
		ConfigData* cd = Contents.back();
		delete cd->Key;
		while( cd->Contents->size() > 0 )
		{
			delete cd->Contents->back();
			cd->Contents->pop_back();
		}
		//free( (void*)Contents.back() );		// <- Should delete cd instead of free?!
		Contents.pop_back();
		delete cd;
	}
}

bool ConfigFile::Save( std::string Filename )
{
	ALLEGRO_FILE* fileHnd;
	std::string document;
	bool dataNum;
	std::string* escstr;

	fileHnd = al_fopen( Filename.c_str(), "w" );
	if( fileHnd == 0 )
		return false;

	document.clear();
	for( std::list<ConfigData*>::iterator i = Contents.begin(); i != Contents.end(); i++ )
	{
		ConfigData* cd = (ConfigData*)(*i);
		document.append( cd->Key->c_str() );
		if( cd->IsArray )
		{
			document.append( " [ " );

			bool isFirst = true;
			for( std::vector<std::string*>::iterator s = cd->Contents->begin(); s != cd->Contents->end(); s++ )
			{
				if( isFirst )
					isFirst = false;
				else
					document.append( ", " );

				std::string* cs = (std::string*)(*s);
				dataNum = IsNumber( *cs );
				if( !dataNum )
				{
					document.append( "\"" );
					escstr = EscapeString(*cs);
					document.append( escstr->c_str() );
					delete escstr;
					document.append( "\"" );
				} else {
					document.append( cs->c_str() );
				}
			}

			document.append( " ]\n" );
		} else {
			document.append( " = " );
			dataNum = IsNumber( *cd->Contents->front() );
			if( !dataNum )
			{
				document.append( "\"" );
				escstr = EscapeString(*cd->Contents->front());
				document.append( escstr->c_str() );
				delete escstr;
				document.append( "\"" );
			} else {
				document.append( cd->Contents->front()->c_str() );
			}
			document.append( "\n" );
		}

	}

	al_fputs( fileHnd, document.c_str() );
	al_fclose( fileHnd );

	return true;
}

bool ConfigFile::KeyExists( std::string Key )
{
	if( GetData( Key ) != 0 )
		return true;
	return false;
}

bool ConfigFile::KeyIsArray( std::string Key )
{
	ConfigData* cd = GetData( Key );
	if( cd == 0 )
		return false;
	return cd->IsArray;
}

int ConfigFile::GetArraySize( std::string Key )
{
	ConfigData* cd = GetData( Key );
	if( cd == 0 )
		return 0;
	return cd->Contents->size();
}

void ConfigFile::RemoveArrayElement( std::string Key, int ArrayIndex )
{
	if( !KeyExists( Key ) )
		return;
	ConfigData* cd = GetData( Key );
	delete cd->Contents->at( ArrayIndex );
	cd->Contents->erase( cd->Contents->begin() + ArrayIndex );
}

void ConfigFile::RemoveKey( std::string Key )
{
	if( !KeyExists( Key ) )
		return;
	ConfigData* cd = GetData( Key );
	delete cd->Key;
	while( cd->Contents->size() > 0 )
	{
		delete cd->Contents->back();
		cd->Contents->pop_back();
	}
	Contents.remove( cd );
	delete cd;
}

void ConfigFile::ParseFile( std::string TextContents )
{
	ConfigData* cd;
	unsigned int charPos = 0;
	std::string token;
	bool charQuoted = false;
	bool wasQuoted = false;
	int TokenStep = 0;

	cd = (ConfigData*)malloc( sizeof( ConfigData ) );
	cd->Contents = new std::vector<std::string*>();
	cd->IsArray = false;

	while( charPos < TextContents.size() )
	{
		switch( TextContents.at( charPos ) )
		{
			case '#':
				while( charPos < TextContents.size() && TextContents.at( charPos ) != '\n' && TextContents.at( charPos ) != '\r' )
					charPos++;
				break;

			case ' ':
			case '\t':
			case ',':
			case '\n':
			case '\r':
			// case ']':
				if( charQuoted )
				{
					token.append( TextContents.substr( charPos, 1 ) );
				} else if( token.size() > 0 || wasQuoted ) {
					switch( TokenStep )
					{
						case 0:
							cd->Key = new std::string( token );
							TokenStep++;
							break;
						case 1:
							if( token == "[" )
								cd->IsArray = true;
							else
								cd->IsArray = false;
							TokenStep++;
							break;
						case 2:
							if( token != "]" )
								cd->Contents->push_back( new std::string(token) );
							break;
					}
					if( (!cd->IsArray && (TextContents.at( charPos ) == '\n' || TextContents.at( charPos ) == '\r')) || token == "]" )
					{
						Contents.push_back( cd );
						cd = (ConfigData*)malloc( sizeof( ConfigData ) );
						cd->Contents = new std::vector<std::string*>();
						cd->IsArray = false;
						TokenStep = 0;
					}
					token.clear();
					wasQuoted = false;
				}
				break;

			case '\\':
				if( charQuoted )
				{
					token.append( TextContents.substr( charPos + 1, 1 ) );
					charPos++;
				}
				break;

			case '"':
				if( TextContents.at( charPos - 1 ) != '\\' )
				{
					charQuoted = !charQuoted;
					wasQuoted = true;
				}
				break;

			default:
				token.append( TextContents.substr( charPos, 1 ) );
				break;
		}
		charPos++;
	}
	free( cd );

}

bool ConfigFile::IsNumber(std::string s)
{
	if( s.empty() )
		return false;
	for( std::string::iterator i = s.begin(); i != s.end(); i++ )
	{
		if( !isdigit(*i) && (*i) != '.' && (*i) != '+' && (*i) != '-' )
			return false;
	}
	return true;
}

std::string* ConfigFile::EscapeString( std::string s )
{
	std::string* out = new std::string();
	if( !s.empty() )
	{
		for( unsigned int i = 0; i < s.size(); i++ )
		{
			if( s.at(i) == '\\' || s.at(i) == '\"' )
				out->append( "\\" );
			out->append( s.substr( i, 1 ) );
		}
	}
	return out;
}


ConfigData* ConfigFile::GetData( std::string Key )
{
	for( std::list<ConfigData*>::iterator i = Contents.begin(); i != Contents.end(); i++ )
	{
		ConfigData* cd = (ConfigData*)(*i);
		if( cd->Key->compare( Key ) == 0 )
			return cd;
	}
	return 0;
}

bool ConfigFile::GetBooleanValue( std::string Key, bool* Value )
{
	return GetBooleanValue( Key, 0, Value );
}

bool ConfigFile::GetBooleanValue( std::string Key, int ArrayIndex, bool* Value )
{
	ConfigData* cd = GetData( Key );
	if( cd == 0 )
		return false;
	if( Value == 0 )
		return false;
	if( ArrayIndex < 0 || (int)cd->Contents->size() < ArrayIndex )
		return false;

	switch( ((std::string*)cd->Contents->at( ArrayIndex ))->at( 0 ) )
	{
		case 'T':
		case 't':
		case 'Y':
		case 'y':
			*Value = true;
			break;
		case 'F':
		case 'f':
		case 'N':
		case 'n':
			*Value = false;
			break;
		default:
			return false;
			break;
	}
	return true;
}

bool ConfigFile::GetIntegerValue( std::string Key, int* Value )
{
	return GetIntegerValue( Key, 0, Value );
}
bool ConfigFile::GetIntegerValue( std::string Key, int ArrayIndex, int* Value )
{
	ConfigData* cd = GetData( Key );
	if( cd == 0 )
		return false;
	if( Value == 0 )
		return false;
	if( ArrayIndex < 0 || (int)cd->Contents->size() < ArrayIndex )
		return false;

	*Value = atoi( cd->Contents->at( ArrayIndex )->c_str() );
	return true;
}

bool ConfigFile::GetInteger64Value( std::string Key, long* Value )
{
	return GetInteger64Value( Key, 0, Value );
}
bool ConfigFile::GetInteger64Value( std::string Key, int ArrayIndex, long* Value )
{
	ConfigData* cd = GetData( Key );
	if( cd == 0 )
		return false;
	if( Value == 0 )
		return false;
	if( ArrayIndex < 0 || (int)cd->Contents->size() < ArrayIndex )
		return false;

	*Value = atol( cd->Contents->at( ArrayIndex )->c_str() );
	return true;
}

bool ConfigFile::GetFloatValue( std::string Key, float* Value )
{
	return GetFloatValue( Key, 0, Value );
}

bool ConfigFile::GetFloatValue( std::string Key, int ArrayIndex, float* Value )
{
	ConfigData* cd = GetData( Key );
	if( cd == 0 )
		return false;
	if( Value == 0 )
		return false;
	if( ArrayIndex < 0 || (int)cd->Contents->size() < ArrayIndex )
		return false;

	*Value = (float)atof( cd->Contents->at( ArrayIndex )->c_str() );
	return true;
}

bool ConfigFile::GetStringValue( std::string Key, std::string* Value )
{
	return GetStringValue( Key, 0, Value );
}

bool ConfigFile::GetStringValue( std::string Key, int ArrayIndex, std::string* Value )
{
	ConfigData* cd = GetData( Key );
	if( cd == 0 )
		return false;
	if( Value == 0 )
		return false;
	if( ArrayIndex < 0 || (int)cd->Contents->size() < ArrayIndex )
		return false;

	Value->clear();
	Value->append( *(cd->Contents->at( ArrayIndex )) );
	return true;
}

bool ConfigFile::SetBooleanValue( std::string Key, bool Value )
{
	std::string* s = new std::string(( Value ? "True" : "False" ));
	bool r = SetStringValue( Key, s );
	return r;
}

bool ConfigFile::SetBooleanValue( std::string Key, int ArrayIndex, bool Value )
{
	std::string* s = new std::string(( Value ? "True" : "False" ));
	bool r = SetStringValue( Key, ArrayIndex, s );
	return r;
}

bool ConfigFile::SetIntegerValue( std::string Key, int Value )
{
	char val[200];
	sprintf( (char*)&val, "%d", Value );
	std::string* s = new std::string( val );
	bool r = SetStringValue( Key, s );
	return r;
}

bool ConfigFile::SetIntegerValue( std::string Key, int ArrayIndex, int Value )
{
	char val[200];
	sprintf( (char*)&val, "%d", Value );
	std::string* s = new std::string( val );
	bool r = SetStringValue( Key, ArrayIndex, s );
	return r;
}

bool ConfigFile::SetInteger64Value( std::string Key, long Value )
{
	char val[200];
	sprintf( (char*)&val, "%d", Value );
	std::string* s = new std::string( val );
	bool r = SetStringValue( Key, s );
	return r;
}

bool ConfigFile::SetInteger64Value( std::string Key, int ArrayIndex, long Value )
{
	char val[200];
	sprintf( (char*)&val, "%d", Value );
	std::string* s = new std::string( val );
	bool r = SetStringValue( Key, ArrayIndex, s );
	return r;
}

bool ConfigFile::SetFloatValue( std::string Key, float Value )
{
	char val[200];
	sprintf( (char*)&val, "%f", Value );
	std::string* s = new std::string( val );
	bool r = SetStringValue( Key, s );
	return r;
}

bool ConfigFile::SetFloatValue( std::string Key, int ArrayIndex, float Value )
{
	char val[200];
	sprintf( (char*)&val, "%f", Value );
	std::string* s = new std::string( val );
	bool r = SetStringValue( Key, ArrayIndex, s );
	return r;
}

bool ConfigFile::SetStringValue( std::string Key, std::string* Value )
{
	ConfigData* cd = GetData( Key );
	if( cd == 0 )
	{
		cd = (ConfigData*)malloc( sizeof(ConfigData) );
		cd->Key = new std::string( Key );
		cd->IsArray = false;
		cd->Contents = new std::vector<std::string*>();
		Contents.push_back( cd );
	}
	if( cd->Contents->empty() )
	{
		cd->Contents->push_back( Value );
	} else {
		std::string* s = cd->Contents->at( 0 );
		s->clear();
		s->append( *Value );
	}
	return true;
}

bool ConfigFile::SetStringValue( std::string Key, int ArrayIndex, std::string* Value )
{
	ConfigData* cd = GetData( Key );
	if( cd == 0 )
	{
		cd = (ConfigData*)malloc( sizeof(ConfigData) );
		cd->Key = new std::string( Key );
		cd->IsArray = true;
		cd->Contents = new std::vector<std::string*>();
		Contents.push_back( cd );
	}
	if( (int)cd->Contents->size() <= ArrayIndex )
	{
		while( (long)cd->Contents->size() <= (long)ArrayIndex - 1 )
		{
			cd->Contents->push_back( new std::string() );
		}
		cd->Contents->push_back( Value );
	} else {
		std::string* s = cd->Contents->at( ArrayIndex );
		s->clear();
		s->append( *Value );
	}
	return true;
}

bool ConfigFile::GetQuickBooleanValue( std::string Key )
{
  bool res;
  GetBooleanValue( Key, &res );
  return res;
}

bool ConfigFile::GetQuickBooleanValue( std::string Key, int ArrayIndex )
{
  bool res;
  GetBooleanValue( Key, ArrayIndex, &res );
  return res;
}

int ConfigFile::GetQuickIntegerValue( std::string Key )
{
  int res;
  GetIntegerValue( Key, &res );
  return res;
}

int ConfigFile::GetQuickIntegerValue( std::string Key, int ArrayIndex )
{
  int res;
  GetIntegerValue( Key, ArrayIndex, &res );
  return res;
}

long ConfigFile::GetQuickInteger64Value( std::string Key )
{
  long res;
  GetInteger64Value( Key, &res );
  return res;
}

long ConfigFile::GetQuickInteger64Value( std::string Key, int ArrayIndex )
{
  long res;
  GetInteger64Value( Key, ArrayIndex, &res );
  return res;
}

float ConfigFile::GetQuickFloatValue( std::string Key )
{
  float res;
  GetFloatValue( Key, &res );
  return res;
}

float ConfigFile::GetQuickFloatValue( std::string Key, int ArrayIndex )
{
  float res;
  GetFloatValue( Key, ArrayIndex, &res );
  return res;
}

std::string* ConfigFile::GetQuickStringValue( std::string Key )
{
  std::string* res = new std::string();
  GetStringValue( Key, res );
  return res;
}

std::string* ConfigFile::GetQuickStringValue( std::string Key, int ArrayIndex )
{
  std::string* res = new std::string();
  GetStringValue( Key, ArrayIndex, res );
  return res;
}
