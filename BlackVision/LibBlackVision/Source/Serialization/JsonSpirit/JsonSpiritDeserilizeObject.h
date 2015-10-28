#pragma once

#include "Serialization/IDeserializer.h"

namespace bv {

class JsonSpiritDeserilizeObject : public IDeserializer
{
public:
    JsonSpiritDeserilizeObject();
    ~JsonSpiritDeserilizeObject();

	std::string			GetAttribute        ( const std::string& name ) const override;
    std::wstring		GetAttribute        ( const std::wstring& name ) const override;

    /**@brief Push.
    @param[in] name Nazwa wêz³a dziecka.
    @param[in] index Indeks w tablicy dzieci.
    @return If serializing - always true.
    If deserializing, returns false if child does not exist.*/
    bool                EnterChild          ( const std::string& name ) const override;
    bool                EnterChild          ( const std::wstring& name ) const override;
    bool                NextChild           () const  = 0;

    /**@brief Pop.*/
    bool                ExitChild           () const override;
};

} //bv
