#pragma once

#include "Tools/IncludeJSON.h"
#include "Serialization/IDeserializer.h"

#include <fstream>
#include <stack>
#include <memory>

namespace bv
{



class JsonDeserializeObject : public IDeserializer
{
private:
	Json::Value				        	m_root;
	mutable Json::Value*				m_currentNode;
	mutable std::stack<Json::Value*>	m_nodeStack;
    mutable std::stack<unsigned int>    m_indexStack;

    std::unique_ptr< DeserializeContext >       m_context;

public:
	JsonDeserializeObject();
    virtual ~JsonDeserializeObject();

    virtual DeserializeContext* GetDeserializeContext() const;

    bool                        LoadFile            ( const std::string& fileName );
	bool						Load                ( const std::string& jsonString );
	bool						Load                ( std::istream& stream );

    std::string                 GetAttribute        ( const std::string& name ) const override;
    std::string                 GetParentAttribute  ( const std::string& parentName, const std::string& attName ) const override;

    bool						EnterChild          ( const std::string& name ) const override;
	bool						ExitChild           () const override;

    bool                        NextChild           () const override;

    std::wstring	        	GetAttribute        ( const std::wstring& name ) const override;
    std::wstring	        	GetParentAttribute  ( const std::wstring& parentName, const std::wstring& attName ) const override;
    bool                        EnterChild          ( const std::wstring& name ) const override;

    virtual IDeserializer*      DetachBranch        ( const std::string & name ) override;

private:
    void                        OnRootInit          ();

    JsonDeserializeObject       ( Json::Value && initValue );
};


} //bv

