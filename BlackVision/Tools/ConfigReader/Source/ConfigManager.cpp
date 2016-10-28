#include "ConfigManager.h"

#include "rapidxml.hpp"

#pragma warning( push )
#pragma warning( disable : 4100 )

#include "rapidxml_print.hpp"

#pragma warning( pop )

#include "win_sock.h"
#include "INIReader.h"
#include <iostream>
#include <direct.h>

#include "Tools/StringHeplers.h"
#include "IO/FileIO.h"

#define GetCurrentDir _getcwd

//using namespace std;
using namespace rapidxml;

namespace bv
{

std::string             ConfigManager::MediaFolder = "";
std::vector<KeyValue>   ConfigManager::Properties;

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
                ParseProperties(CurrentProperty->first_node("property"),name_s);
            }
            else
            {
                string value_s = value->value();

                ConfigManager::Properties.push_back( KeyValue( name_s, value_s ) );
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

	bool ConfigManager::LoadXMLConfig()
    {
        wstring wExePath = ExePath();
		std::string sExePath( wExePath.begin(), wExePath.end() );
        string temp = get_file_contents(std::string(sExePath + "/config.xml").c_str());
	
	    xml_document<>          doc;    // character type defaults to char
	    char *buffer            =   new char[temp.size()+1];
	    buffer[temp.size()]     =   0;

	    memcpy(buffer,temp.c_str(),temp.size());
	    doc.parse<0>(buffer);    

        

	    
        // parse config file
         xml_node<> *Config = doc.first_node("config");
         if(Config!=NULL)
         ParseProperties(Config->first_node("property"),"");

         delete[] buffer;
         return true;
    }


    void AppendConfigNode( xml_document<>* document, xml_node<>* parentNode, const KeyValue& keyValue, StrVec& configPathVec )
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
            childNode->append_attribute( document->allocate_attribute( "value", keyValue.value.c_str() ) );
        }
        else
        {
            AppendConfigNode( document, childNode, keyValue, configPathVec );
        }
    }

    bool ConfigManager::SaveXMLConfig()
    {
        xml_document<> document;
        xml_node<>* config = document.allocate_node( node_element, "config", "" );

        document.append_node( config );

        for( auto& keyValue : Properties )
        {
            auto configPathVec = Split( keyValue.key, "/" );
            AppendConfigNode( &document, config, keyValue, configPathVec );
        }

        std::string configContent;
        print( std::back_inserter( configContent ), document, 0);

        wstring wExePath = ExePath();
		std::string configFilePath = std::string( wExePath.begin(), wExePath.end() ) + "/config.xml";

        File configFile = File::Open( configFilePath, File::FOMReadWrite );
        configFile.Write( configContent );
        configFile.Close();

        return true;
    }

    KeyValue ConfigManager::GetValue(std::string key)
    {
        for(unsigned int i=0;i<Properties.size();i++)
        {
            if(Properties[i].key == key)return Properties[i];
        }
        return KeyValue(key,"");
    }

    std::string ConfigManager::GetString(std::string key)
    {
        KeyValue value = GetValue(key);
        return value.value;
    }

    int ConfigManager::GetInt(std::string key)
    {
        KeyValue value = GetValue(key);
        if(value.value=="")return 0;
        return atoi(value.value.c_str());
    }

    float ConfigManager::GetFloat(std::string key)
    {
        KeyValue value = GetValue(key);
        if(value.value=="")return 0;
        return (float)atof(value.value.c_str());
    }
    bool ConfigManager::GetBool(std::string key)
    {
        KeyValue value = GetValue(key);
        if(value.value=="1" || value.value=="true")
            return true;
        return false;
    }

    bool ConfigManager::LoadConfig()
	{
		wstring wExePath = ExePath();
		std::string sExePath( wExePath.begin(), wExePath.end() );
		INIReader ini(sExePath + "/config.ini");
		std::string path=ini.Get("assets","MediaFolder","-");
		if(path=="-")
		{
			cout<<"error parsing config file"<<endl;
			return false;
		}else{
			cout<<"config: "<<path<<endl;
		}
		MediaFolder = path;
		return true;
	};

    // ***********************
    //
    KeyValue&       ConfigManager::GetOrCreateValue            ( const std::string& key )
    {
        for( unsigned int i = 0; i < Properties.size(); i++ )
        {
            if( Properties[i].key == key )
                return Properties[i];
        }

        Properties.push_back( KeyValue( key, "" ) );
        return Properties.back();
    }

    // ***********************
    //
    void             ConfigManager::SetBool     ( const std::string& key, bool value )
    {
        auto& keyValue = GetOrCreateValue( key );
        keyValue.value = std::to_string( value );
    }

    // ***********************
    //
    void             ConfigManager::SetInt      ( const std::string& key, int value )
    {
        auto& keyValue = GetOrCreateValue( key );
        keyValue.value = std::to_string( value );
    }

    // ***********************
    //
    void             ConfigManager::SetString   ( const std::string& key, const std::string& value )
    {
        auto& keyValue = GetOrCreateValue( key );
        keyValue.value = value;
    }

    // ***********************
    //
    void             ConfigManager::SetFloat    ( const std::string& key, float value )
    {
        auto& keyValue = GetOrCreateValue( key );
        keyValue.value = std::to_string( value );
    }
    
}
