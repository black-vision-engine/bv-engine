#pragma once

#include <string>
#include <vector>

#include "rapidxml.hpp"
#include "helpers/auxiliary.hpp"
#include "helpers/KeyValue.h"

using namespace rapidxml;

namespace bv
{
class ConfigManager
{
private:
	static std::string MediaFolder;
	
public:
    
    static vector<KeyValue> Properties; 
	static bool LoadConfig();

    static bool LoadXMLConfig();

    static KeyValue GetValue(std::string key);
    static bool GetBool(std::string key);
    static int GetInt(std::string key);
    static std::string GetString(std::string key);
    static float GetFloat(std::string key);

    static void ParseProperties(xml_node<> * node,std::string name);

	static std::string GetPath(){return MediaFolder;}
    

	ConfigManager(void);
	~ConfigManager(void);
};

}
