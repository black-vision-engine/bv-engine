#pragma once


#include <string>

namespace bv {


class ISerializer
{
private:
protected:
public:
    virtual ~ISerializer() {};

	virtual void				SetAttribute        ( const std::string& name, const std::string& value ) = 0;
	virtual std::string			GetAttribute        ( const std::string& name ) = 0;

    /**@brief Push.
    @param[in] name Nazwa wêz³a dziecka.
    @param[in] index Indeks w tablicy dzieci.
    @return If serializing - always true.
    If deserializing, returns false if child does not exist.*/
    virtual void                EnterChild          ( const std::string& name ) = 0;
    
    /**@brief Pop.*/
    virtual bool                ExitChild           () = 0;
};



};

