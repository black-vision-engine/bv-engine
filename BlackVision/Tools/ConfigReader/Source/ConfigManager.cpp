#include "ConfigManager.h"

#include "rapidxml.hpp"

#pragma warning( push )
#pragma warning( disable : 4100 )

#include "rapidxml_print.hpp"

#pragma warning( pop )

#include "win_sock.h"
#include "INIReader.h"

#include "Tools/StringHeplers.h"
#include "IO/FileIO.h"


#define GetCurrentDir _getcwd

using namespace rapidxml;


namespace bv { namespace config {


std::string                     ConfigManager::MediaFolder = "";
std::string                     ConfigManager::ConfigFile  = "config.xml";
ConfigManager::PropertyMap      ConfigManager::Properties;

namespace 
{

// ********************************
//
void ParseProperties( xml_node<> * node, std::string current_name )
{
    xml_node<> * CurrentProperty = node;
        
    while( CurrentProperty != nullptr )
	{
        std::string node_name = CurrentProperty->name();

        if( node_name == "property" )
        {
			xml_attribute<> * name   = CurrentProperty->first_attribute( "name" );
            xml_attribute<> * value  = CurrentProperty->first_attribute( "value" );
            string name_s            = name->value();

            if( current_name != "" )
            {
                name_s = current_name + "/" + name->value();
            }
                
                
            if( value == nullptr )
            {
                ParseProperties( CurrentProperty->first_node( "property" ), name_s );
            }
            else
            {
                string value_s = value->value();

                ConfigManager::Properties[ name_s ] = value_s;
				ParseProperties( CurrentProperty->first_node( "property" ), name_s );
            }
        }

        CurrentProperty = CurrentProperty->next_sibling();
	}
}

} // anonymous

// ********************************
//
ConfigManager::ConfigManager(void)
{
}


// ********************************
//
ConfigManager::~ConfigManager(void)
{
}

// ********************************
//
wstring ExePath()
{
	WCHAR szFilePath	[_MAX_PATH];
	GetModuleFileName	(NULL, szFilePath, _MAX_PATH);
	wstring::size_type pos = wstring( szFilePath ).find_last_of( L"\\/" );
	wstring temp = wstring( szFilePath ).substr(0,pos);
	//printf("Executable directory is : %s\n",string(temp.begin(),temp.end()).c_str());
	return temp;
}

//// ***********************
////
//bool            ConfigManager::LoadConfig               ()
//{
//	wstring wExePath = ExePath();
//	std::string sExePath( wExePath.begin(), wExePath.end() );
//	INIReader ini( sExePath + "/config.ini");
//	std::string path=ini.Get("assets","MediaFolder","-");
//	if(path=="-")
//	{
//		cout<<"error parsing config file"<<endl;
//		return false;
//	}else{
//		cout<<"config: "<<path<<endl;
//	}
//	MediaFolder = path;
//	return true;
//};

// ***********************
//
bool ConfigManager::LoadXMLConfig()
{
    wstring wExePath = ExePath();
	std::string sExePath( wExePath.begin(), wExePath.end() );

	char * buffer = nullptr;
    SizeType size = 0;

    auto fileName = ( sExePath + "/" + ConfigFile );
    m_deserializer.LoadFile( fileName );

	xml_document<> doc;    // character type defaults to char
	doc.parse< 0 >( buffer );    
	    
    // parse config file
    xml_node<> * config = doc.first_node( "config" );
    if( config )
    {
        ParseProperties( config->first_node( "property" ), "" );
    }

    delete[] buffer;

    return true;
}

// ***********************
//
void AppendConfigNode( xml_document<>* document, xml_node<>* parentNode, const std::string & key, StrVec& configPathVec )
{
    std::string& pathElement = configPathVec[ 0 ];


    xml_node<>* childNode = nullptr;

    //Find node with attribute name set to pathElement.c_str()
    xml_node<>* tmpNode = parentNode->first_node();
    while( tmpNode )
    {
        if( auto nameAttrib = tmpNode->first_attribute( "name" ) )
        {
            if( nameAttrib->value() == pathElement )
            {
                childNode = tmpNode;
                break;
            }
        }

        tmpNode = tmpNode->next_sibling();
    }


    if( !childNode )
    {
        childNode = document->allocate_node( node_element, "property" );
        childNode->append_attribute( document->allocate_attribute( "name", document->allocate_string( pathElement.c_str() ) ) );
        parentNode->append_node( childNode );
    }

    configPathVec.erase( configPathVec.begin() );
    if( configPathVec.empty() )
    {
        childNode->append_attribute( document->allocate_attribute( "value", ConfigManager::Properties[ key ].c_str() ) );
    }
    else
    {
        AppendConfigNode( document, childNode, key, configPathVec );
    }
}

// ***********************
//
bool ConfigManager::SaveXMLConfig()
{
    xml_document<> document;
    xml_node<>* config = document.allocate_node( node_element, "config", "" );

    document.append_node( config );

    for( auto & kv : Properties )
    {
        auto configPathVec = Split( kv.first, "/" );
        AppendConfigNode( &document, config, kv.first, configPathVec );
    }

    std::string configContent;
    print( std::back_inserter( configContent ), document, 0);

    wstring wExePath = ExePath();
	std::string configFilePath = std::string( wExePath.begin(), wExePath.end() ) + "/" + ConfigFile;

    File configFile = File::Open( configFilePath, File::FOMReadWrite );
    configFile.Write( configContent );
    configFile.Close();

    return true;
}

// ***********************
//
std::string     ConfigManager::GetString                ( const std::string & name )
{
    return GetProperty( name );
}

// ***********************
//
int             ConfigManager::GetInt                   ( const std::string & name )
{
    auto value = GetProperty( name );

    char * end = nullptr;
    auto ret = strtol( value.c_str(), &end, 10 );

    if( !*end && end != value.c_str() )
    {
        if ( ret < std::numeric_limits< int >::lowest() )
            return std::numeric_limits< int >::lowest();

        if ( ret > ( std::numeric_limits< int >::max )() ) 
            return ( std::numeric_limits< int >::max )();

        return ( int )ret;
    }

    return 0;
}

// ***********************
//
float           ConfigManager::GetFloat                 ( const std::string & name )
{
    auto value = GetProperty( name );

    char * end = nullptr;
    auto ret = strtod( value.c_str(), &end );

    if( !*end && end != value.c_str() )
    {
        if ( ret < std::numeric_limits< float >::lowest() )
            return std::numeric_limits< float >::lowest();

        if ( ret > ( std::numeric_limits< float >::max )() ) 
            return ( std::numeric_limits< float >::max )();

        return ( float )ret;
    }

    return 0.f;
}

// ***********************
//
bool            ConfigManager::GetBool                  ( const std::string & name )
{
    auto value = GetProperty( name );

    if( value == "true" || value == "1" )
        return true;

    return false;
}

// ***********************
//
void             ConfigManager::SetBool     ( const std::string & name, bool value )
{
    SetProperty( name, std::to_string( value ) );
}

// ***********************
//
void             ConfigManager::SetInt      ( const std::string & name, int value )
{
    SetProperty( name, std::to_string( value ) );
}

// ***********************
//
void             ConfigManager::SetString   ( const std::string & name, const std::string & value )
{
    SetProperty( name, value );
}

// ***********************
//
void             ConfigManager::SetFloat    ( const std::string & name, float value )
{
    SetProperty( name, std::to_string( value ) );
}
    
// ***********************
//
std::string     ConfigManager::GetPath      ()
{
    return MediaFolder;
}

// ***********************
//
void            ConfigManager::SetProperty              ( const std::string & name, const std::string & value )
{
    Properties[ name ] = value;
}

// ***********************
//
std::string     ConfigManager::GetProperty              ( const std::string & name )
{
    if( Properties.count( name ) )
    {
        return Properties[ name ];
    }

    return "";
}

} //config
} //bv
