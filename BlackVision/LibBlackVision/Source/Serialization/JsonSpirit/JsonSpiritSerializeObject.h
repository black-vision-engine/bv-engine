#pragma once

#include "Serialization/ISerializer.h"
#include "Serialization/JsonSpirit/JsonSpiritSource/json_spirit.h"

#include <stack>
#include <fstream>
#include <string>

namespace bv {

enum FormatStyle
{
    FORMATSTYLE_SPARING,
    FORMATSTYLE_READABLE
};

class JsonSpiritSerializeObject : public ISerializer
{
private:
    json_spirit::wValue                  m_root;
    json_spirit::wValue*                 m_currentNode;
    std::stack<json_spirit::wValue*>     m_nodeStack;
public:
    JsonSpiritSerializeObject();
    ~JsonSpiritSerializeObject();


    void						Save                ( const std::string& filename, FormatStyle style = FormatStyle::FORMATSTYLE_SPARING );
    std::wstring				Save                ( FormatStyle style = FormatStyle::FORMATSTYLE_SPARING );

    // Serializer on string
	virtual void				SetAttribute        ( const std::string& name, const std::string& value ) override;
	virtual std::string			GetAttribute        ( const std::string& name ) override;
    // Serializer on wstring
	virtual void				SetAttribute        ( const std::wstring& name, const std::wstring& value ) override;
	virtual std::wstring		GetAttribute        ( const std::wstring& name ) override;

    /**@brief Push.
    @param[in] name Nazwa w�z�a dziecka.
    @param[in] index Indeks w tablicy dzieci.
    @return If serializing - always true.
    If deserializing, returns false if child does not exist.*/
    virtual void                EnterChild          ( const std::string& name ) override;
    virtual void                EnterChild          ( const std::wstring& name ) override;
    
    /**@brief Pop.*/
    virtual bool                ExitChild           () override;

private:
    json_spirit::wObject::value_type::Value_type*     FindValue           ( const json_spirit::wObject& obj, const std::wstring& name );
};


} //bv
