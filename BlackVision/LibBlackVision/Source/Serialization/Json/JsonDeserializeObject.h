#pragma once

#include "Tools/IncludeJSON.h"
#include "Serialization/IDeserializer.h"

#include <fstream>
#include <stack>

namespace bv
{



class JsonDeserializeObject : public IDeserializer
{
private:
	Json::Value				        	m_root;
	mutable Json::Value*				m_currentNode;
	mutable std::stack<Json::Value*>	m_nodeStack;
    mutable std::stack<unsigned int>    m_indexStack;
public:
	JsonDeserializeObject();
    virtual ~JsonDeserializeObject();

    bool                        LoadFile            ( const std::string& fileName );
	void						Load                ( const std::string& jsonString );
	void						Load                ( std::istream& stream );

    std::string                 GetAttribute        ( const std::string& name ) const override;
    std::string                 GetParentAttribute  ( const std::string& parentName, const std::string& attName ) const override;

    bool						EnterChild          ( const std::string& name ) const override;
	bool						ExitChild           () const override;

    bool                        NextChild           () const override;

    std::wstring	        	GetAttribute        ( const std::wstring& name ) const override;
    std::wstring	        	GetParentAttribute  ( const std::wstring& parentName, const std::wstring& attName ) const override;
    bool                        EnterChild          ( const std::wstring& name ) const override;
};


} //bv

