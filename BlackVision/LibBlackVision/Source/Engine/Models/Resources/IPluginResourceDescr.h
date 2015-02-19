#pragma once

#include <memory>
#include <vector>
#include <string>

#include "Engine/Types/Enums.h"
#include "System/BasicTypes.h"
#include "Engine/Models/Resources/IPluginResourceDescr.h"
#include "../Interfaces/ITextureDescriptor.h"
#include "../Plugins/Channels/PixelShader/DefaultTextureDescriptor.h"

namespace bv { namespace model {

class IPluginResourceDescr
{
public:

    virtual PluginResourceType      GetResourceType () const = 0;

    virtual ~IPluginResourceDescr() {}

};

DEFINE_PTR_TYPE(IPluginResourceDescr)
DEFINE_CONST_PTR_TYPE(IPluginResourceDescr)


//FIXME: move to a separate file
class ITextureResourceDescr : public IPluginResourceDescr
{
public:

    virtual const std::string &     GetTextureFile  () const = 0;

    virtual ~ITextureResourceDescr () {}

};

DEFINE_PTR_TYPE(ITextureResourceDescr)
DEFINE_CONST_PTR_TYPE(ITextureResourceDescr)

ITextureResourceDescrConstPtr QueryTextureResourceDescr( IPluginResourceDescrConstPtr resDescr );


//FIXME: move to a separate file
class IVideoInputResourceDescr : public IPluginResourceDescr
{
public:

	virtual DefaultTextureDescriptor*		GetITextureDescriptor  () const = 0;
	virtual void							Update () const = 0; // const Update? WTF? ;)

	virtual ~IVideoInputResourceDescr () {}

};

DEFINE_PTR_TYPE(IVideoInputResourceDescr)
DEFINE_CONST_PTR_TYPE(IVideoInputResourceDescr)

IVideoInputResourceDescrConstPtr QueryVideoInputResourceDescr( IPluginResourceDescrConstPtr resDescr );


//FIXME: move to a separate file
class IAnimationResourceDescr : public IPluginResourceDescr
{
public:

    virtual const std::vector< std::string > & GetFrames   () const = 0;

    virtual ~IAnimationResourceDescr () {}

};

DEFINE_PTR_TYPE(IAnimationResourceDescr)
DEFINE_CONST_PTR_TYPE(IAnimationResourceDescr)

IAnimationResourceDescrConstPtr QueryAnimationResourceDescr( IPluginResourceDescrConstPtr resDescr );

//FIXME: move to a separate file
class IFontResourceDescr : public IPluginResourceDescr
{
public:

    virtual const std::string &     GetFontFile  () const = 0;

    virtual ~IFontResourceDescr () {}

};

DEFINE_PTR_TYPE(IFontResourceDescr)
DEFINE_CONST_PTR_TYPE(IFontResourceDescr)

IFontResourceDescrConstPtr QueryFontResourceDescr( IPluginResourceDescrConstPtr resDescr );



} //model
} //bv
