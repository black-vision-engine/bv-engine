#pragma once


#include <string>

namespace bv {


enum FormatStyle
{
    FORMATSTYLE_SPARING,
    FORMATSTYLE_READABLE
};

class ISerializer
{
private:
protected:
public:
    virtual ~ISerializer() {};

    // Serializer on string
	virtual void				SetAttribute        ( const std::string& name, const std::string& value ) = 0;
	virtual std::string			GetAttribute        ( const std::string& name ) = 0;
    // Serializer on wstring
	virtual void				SetAttribute        ( const std::wstring& name, const std::wstring& value ) = 0;
	virtual std::wstring		GetAttribute        ( const std::wstring& name ) = 0;

    /**@brief Push.
    @param[in] name Nazwa wêz³a dziecka.
    @param[in] index Indeks w tablicy dzieci.
    @return If serializing - always true.
    If deserializing, returns false if child does not exist.*/
    virtual void                EnterChild          ( const std::string& name ) = 0;
    virtual void                EnterChild          ( const std::wstring& name ) = 0;

    virtual void                EnterArray          ( const std::string& name ) = 0;
    virtual void                EnterArray          ( const std::wstring& name ) = 0;
    
    /**@brief Pop.*/
    virtual bool                ExitChild           () = 0;
};



};

