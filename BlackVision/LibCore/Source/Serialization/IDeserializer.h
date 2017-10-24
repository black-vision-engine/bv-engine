#pragma once

#include "CoreDEF.h"
#include "DeserializeContext.h"

#include <string>


namespace bv
{


// ***********************
//
struct FilePosition
{
    SizeType    Line;
    SizeType    CharPosition;

    // ***********************
    //
    FilePosition()
        : Line( 0 )
        , CharPosition( 0 )
    {}
};



// ***********************
//
class IDeserializer
{
friend class ISerializer;

public:
    virtual ~IDeserializer() {};

    virtual DeserializeContext* GetDeserializeContext() const = 0;

    virtual bool    			HasAttribute        ( const std::string& name ) const = 0;
    virtual bool		        HasAttribute        ( const std::wstring& name ) const = 0;

    virtual std::string			GetAttribute        ( const std::string& name ) const = 0;
    virtual std::wstring		GetAttribute        ( const std::wstring& name ) const = 0;

    virtual std::string			GetParentAttribute  ( const std::string& parentName, const std::string& attName ) const = 0;
    virtual std::wstring		GetParentAttribute  ( const std::wstring& parentName, const std::wstring& attName ) const = 0;

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

    virtual IDeserializer*      DetachBranch        ( const std::string & name ) = 0;
    virtual bool                AttachBranch        ( const std::string & name, ISerializer * ser ) = 0;

    virtual ISerializer *       CreateSerializer    () const = 0;

    virtual FilePosition        CurrentLineNumber   () const = 0;
};

} //bv