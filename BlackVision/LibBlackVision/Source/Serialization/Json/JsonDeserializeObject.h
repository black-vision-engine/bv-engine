#pragma once

#include "Tools/IncludeJSON.h"
#include "Serialization/ISerializer.h"

#include <fstream>
#include <stack>

namespace bv
{



class JsonDeserializeObject : public ISerializer
{
private:
	Json::Value					m_root;
	Json::Value*				m_currentNode;
	std::stack<Json::Value*>	m_nodeStack;
public:
	JsonDeserializeObject();
    virtual ~JsonDeserializeObject();

	void						Load                ( const std::string& jsonString );
	void						Load                ( std::istream& stream );

	void						SetAttribute        ( const std::string& name, const std::string& value ) override;
    std::string                 GetAttribute        ( const std::string& name ) override;

    bool						EnterChild          ( const std::string& name ) override;

	bool						ExitChild           () override;
};


} //bv

