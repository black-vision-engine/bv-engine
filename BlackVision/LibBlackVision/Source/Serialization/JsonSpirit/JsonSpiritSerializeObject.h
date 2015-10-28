#pragma once

#include "Serialization/ISerializer.h"

namespace bv {



class JsonSpiritSerializeObject : public ISerializer
{

public:
    JsonSpiritSerializeObject();
    ~JsonSpiritSerializeObject();


    // Serializer on string
	virtual void				SetAttribute        ( const std::string& name, const std::string& value ) override;
	virtual std::string			GetAttribute        ( const std::string& name ) override;
    // Serializer on wstring
	virtual void				SetAttribute        ( const std::wstring& name, const std::wstring& value ) override;
	virtual std::wstring		GetAttribute        ( const std::wstring& name ) override;

    /**@brief Push.
    @param[in] name Nazwa wêz³a dziecka.
    @param[in] index Indeks w tablicy dzieci.
    @return If serializing - always true.
    If deserializing, returns false if child does not exist.*/
    virtual void                EnterChild          ( const std::string& name ) override;
    virtual void                EnterChild          ( const std::wstring& name ) override;
    
    /**@brief Pop.*/
    virtual bool                ExitChild           () override;
};


} //bv
