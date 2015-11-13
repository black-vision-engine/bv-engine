#pragma once

#include "Serialization/ISerializer.h"

#include "CoreDEF.h"
#include "rapidxml/RapidXml.hpp"
#include <stack>

namespace bv
{

class SerializeObject : public ISerializer
{
    rapidxml::xml_document<>                                m_doc;
    std::stack< rapidxml::xml_node<>* >                     m_roots;

public:
    SerializeObject();
    ~SerializeObject();
    void										            Save( const std::string & filename );
	void										            Save( std::ostream & out );

    void                                                    SetName( const std::string & name );
    void                                                    SetValue( const std::string & name, const std::string & value );
	void                                                    SetContent( const std::string & value );
    void                                                    Pop();

    virtual void				SetAttribute        ( const std::string& name, const std::string& value ) override;
    virtual std::string			GetAttribute        ( const std::string& name ) override;
    virtual void                EnterChild          ( const std::string& name ) override;
    virtual bool                ExitChild           () override;
    void                        EnterArray          ( const std::string& name ) override;

	virtual void				SetAttribute        ( const std::wstring& name, const std::wstring& value ) override;
	virtual std::wstring		GetAttribute        ( const std::wstring& name ) override;
    virtual void                EnterChild          ( const std::wstring& name ) override;
    void                        EnterArray          ( const std::wstring& /*name*/ ) override;
};

}
