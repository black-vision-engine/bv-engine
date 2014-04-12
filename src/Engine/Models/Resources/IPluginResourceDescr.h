#pragma once

#include <memory>
#include <vector>
#include <string>

#include "Engine/Types/Enums.h"


namespace bv { namespace model {

class IPluginResourceDescr
{
public:

    virtual const std::string &     GetName         () const = 0;
    virtual PluginResourceType      GetResourceType () const = 0;

    virtual ~IPluginResourceDescr() {}

};

typedef std::shared_ptr< IPluginResourceDescr > IPluginResourceDescrPtr;


//FIXME: move to a separate file
class ITextureResourceDescr : public IPluginResourceDescr
{
public:

    virtual const std::string &     GetTextureFile  () const = 0;

    virtual ~ITextureResourceDescr () {}

};

typedef std::shared_ptr< ITextureResourceDescr > ITextureResourceDescrPtr;

const ITextureResourceDescr * QueryTextureResourceDescr( const IPluginResourceDescr * resDescr );


//FIXME: move to a separate file
class IAnimationResourceDescr : public IPluginResourceDescr
{
public:

    virtual const std::vector< std::string > & GetFrames   () const = 0;

    virtual ~IAnimationResourceDescr () {}

};

typedef std::shared_ptr< IAnimationResourceDescr > IAnimationResourceDescrPtr;

const IAnimationResourceDescr * QueryAnimationResourceDescr( const IPluginResourceDescr * resDescr );

} //model
} //bv
