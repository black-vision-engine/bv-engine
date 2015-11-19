#pragma once

#include "Serialization/IDeserializer.h"
#include "Serialization/JsonSpirit/JsonSpiritSource/json_spirit.h"

#include <stack>


namespace bv {

class JsonSpiritDeserilizeObject : public IDeserializer
{
private:
    json_spirit::wValue                         m_root;
	mutable json_spirit::wValue*				m_currentNode;
	mutable std::stack<json_spirit::wValue*>	m_nodeStack;
    mutable std::stack<unsigned int>            m_indexStack;
public:
    JsonSpiritDeserilizeObject();
    ~JsonSpiritDeserilizeObject();

    bool                LoadFile            ( const std::string& fileName );
    bool                LoadWString         ( const std::wstring& jsonString );

	std::string			GetAttribute        ( const std::string& name ) const override;
    std::wstring		GetAttribute        ( const std::wstring& name ) const override;

    bool                EnterChild          ( const std::string& name ) const override;
    bool                EnterChild          ( const std::wstring& name ) const override;
    bool                NextChild           () const override;

    bool                ExitChild           () const override;

private:
    json_spirit::wObject::value_type::Value_type*     FindValue           ( const json_spirit::wObject& obj, const std::wstring& name ) const;
};

} //bv
