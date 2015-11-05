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
    Json::Value                 GetJson             ();

	void						SetAttribute        ( const std::string& name, const std::string& value ) override;
    std::string                 GetAttribute        ( const std::string& name ) override;

    void				        SetAttribute        ( const std::wstring& /*name*/, const std::wstring& /*value*/ ) override;
	std::wstring	        	GetAttribute        ( const std::wstring& /*name*/ ) override;

    void						EnterChild          ( const std::string& name ) override;
    void                        EnterChild          ( const std::wstring& /*name*/ ) override;
	bool						ExitChild           () override;
};


} //bv
