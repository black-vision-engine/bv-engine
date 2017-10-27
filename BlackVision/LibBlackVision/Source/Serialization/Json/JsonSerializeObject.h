#pragma once

#include "Serialization/ISerializer.h"
#include "Tools/IncludeJSON.h"

#include <stack>
#include <fstream>
#include <string>
#include <memory>


namespace bv
{

class JsonSerializeObject;
class JsonDeserializeObject;


// ***********************
//
class JsonSerializeObject : public ISerializer
{
private:
	Json::Value					m_root;
	Json::Value*				m_currentNode;
	
    std::stack<Json::Value*>	m_nodeStack;

    std::unique_ptr< SerializeContext >     m_context;

public:
	JsonSerializeObject( SerializeContext * context );
    JsonSerializeObject( Json::Value && initValue );
	~JsonSerializeObject();

    SerializeContext*           GetSerializeContext () const override;

	void						Save                ( const std::string& filename, FormatStyle style = FormatStyle::FORMATSTYLE_SPARING );
	void						Save                ( std::ostream& out );
    Json::Value                 GetJson             () const;
    Json::Value&                StealJson           ();
    std::string                 GetString           ();

	void						SetAttribute        ( const std::string& name, const std::string& value ) override;
    std::string                 GetAttribute        ( const std::string& name ) override;

    void				        SetAttribute        ( const std::wstring& /*name*/, const std::wstring& /*value*/ ) override;
	std::wstring	        	GetAttribute        ( const std::wstring& /*name*/ ) override;

    void						EnterChild          ( const std::string& name ) override;
    void                        EnterChild          ( const std::wstring& /*name*/ ) override;
    void                        EnterArray          ( const std::string& name ) override;
    void                        EnterArray          ( const std::wstring& /*name*/ ) override;

	bool						ExitChild           () override;

    bool                        AttachBranch        ( const std::string & name, const ISerializer * ser ) override;
    bool                        AttachBranch        ( const std::string & name, const IDeserializer * ser ) override;

};


// ***********************
// This class is a "stupid" Serializer, with a primitive context and not sufficient to handle BV objects serialization
class SimpleJsonSerializeObject : public JsonSerializeObject
{
public:
    SimpleJsonSerializeObject()
        : JsonSerializeObject( new SerializeContext() )
    {}
};

} //bv
