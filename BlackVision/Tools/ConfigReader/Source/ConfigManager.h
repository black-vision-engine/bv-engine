#pragma once

#include <string>
#include <map>

#include "Serialization/XML/XMLDeserializer.h"

#include "helpers/auxiliary.hpp"


namespace bv { namespace config {

class ConfigManager
{
private:

    static XMLDeserializer  m_deserializer;

    typedef std::map< std::string, std::string >    PropertyMap;

	static std::string      MediaFolder;
	static std::string      ConfigFile;
	
public:
    
    static PropertyMap      Properties; 

                            ConfigManager       ();
	                        ~ConfigManager      ();

	//static bool             LoadConfig          ();

    static bool             LoadXMLConfig       ();
    static bool             SaveXMLConfig       ();

    static bool             GetBool             ( const std::string & name );
    static int              GetInt              ( const std::string & name );
    static std::string      GetString           ( const std::string & name );
    static float            GetFloat            ( const std::string & name );

    static void             SetBool             ( const std::string & name, bool value );
    static void             SetInt              ( const std::string & name, int value );
    static void             SetString           ( const std::string & name, const std::string & value );
    static void             SetFloat            ( const std::string & name, float value );

	static std::string      GetPath             ();

private:

    static void             SetProperty         ( const std::string & name, const std::string & value );
    static std::string      GetProperty         ( const std::string & name );
   
};

} //config
} //bv
