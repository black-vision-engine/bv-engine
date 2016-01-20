#pragma once

#include "Serialization/IDeserializer.h"
#include "JsonSpiritSource/json_spirit.h"

#include <stack>


namespace bv {

class JsonSpiritDeserializeObject : public IDeserializer
{
private:
    json_spirit::wValue                         m_root;
	mutable json_spirit::wValue*				m_currentNode;
	mutable std::stack<json_spirit::wValue*>	m_nodeStack;
    mutable std::stack<unsigned int>            m_indexStack;

    std::unique_ptr< DeserializeContext >       m_context;

public:
    JsonSpiritDeserializeObject();
    ~JsonSpiritDeserializeObject();

    virtual DeserializeContext* GetDeserializeContext() const;

    bool                LoadFile            ( const std::string& fileName );
    bool                LoadWString         ( const std::wstring& jsonString );

	std::string			GetAttribute        ( const std::string& name ) const override;
    std::wstring		GetAttribute        ( const std::wstring& name ) const override;

	std::string			GetParentAttribute  ( const std::string& parentName, const std::string& attName ) const override;
    std::wstring		GetParentAttribute  ( const std::wstring& parentName, const std::wstring& attName ) const override;

    bool                EnterChild          ( const std::string& name ) const override;
    bool                EnterChild          ( const std::wstring& name ) const override;
    bool                NextChild           () const override;

    bool                ExitChild           () const override;

private:
    json_spirit::wObject::value_type::Value_type*   FindValue           ( const json_spirit::wObject& obj, const std::wstring& name ) const;
    bool                                            OnRootInit          ();
};

} //bv
