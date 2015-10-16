#pragma once

#include "Serialization/ISerializer.h"
#include "Tools/IncludeJSON.h"

#include <stack>
#include <fstream>
#include <string>


namespace bv
{


class JsonSerializeObject : public ISerializer
{
private:
	Json::Value					m_root;
	Json::Value*				m_currentNode;
	std::stack<Json::Value*>	m_nodeStack;
public:
	JsonSerializeObject();
	~JsonSerializeObject();

	void						Save                ( const std::string& filename );
	void						Save                ( std::ostream& out );

	void						SetAttribute        ( const std::string& name, const std::string& value ) override;
    std::string                 GetAttribute        ( const std::string& name ) override;

    bool						EnterChild          ( const std::string& name, unsigned int index = 0 ) override;
	bool						ExitChild           () override;
};


} //bv
