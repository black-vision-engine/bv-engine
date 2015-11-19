#pragma once


#include <string>

namespace bv {


class IDeserializer
{
private:
protected:
public:
    virtual ~IDeserializer() {};

	virtual std::string			GetAttribute        ( const std::string& name ) const = 0;
    virtual std::wstring		GetAttribute        ( const std::wstring& name ) const = 0;

    /**@brief Push.
    @param[in] name Nazwa wêz³a dziecka.
    @param[in] index Indeks w tablicy dzieci.
    @return If serializing - always true.
    If deserializing, returns false if child does not exist.*/
    virtual bool                EnterChild          ( const std::string& name ) const = 0;
    virtual bool                EnterChild          ( const std::wstring& name ) const = 0;
    virtual bool                NextChild           () const  = 0;

    /**@brief Pop.*/
    virtual bool                ExitChild           () const = 0;
};



};
