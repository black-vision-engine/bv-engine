#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <memory>

#include "Engine/Interfaces/IValue.h"
#include "Engine/Models/Plugins/Interfaces/IParameter.h"
#include "Engine/Interfaces/IUpdatable.h"
#include "Engine/Models/Plugins/Channels/RendererContext/RendererContext.h"
#include "Engine/Models/Plugins/Interfaces/IPluginParamValModel.h"
#include "Engine/Models/Resources/IPluginResourceDescr.h"
#include "Engine/Models/Plugins/Interfaces/IVertexAttributesChannel.h"
#include "Engine/Models/Plugins/Interfaces/IPixelShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IVertexShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IGeometryShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/ITransformChannel.h"

namespace bv { namespace model {

class IDefaultTransformChannel;
class ITransformChannel;
class IVertexShaderChannel;
class IGeometryShaderChannel;
class ResourceHandle;

struct TextureInfo;
typedef std::vector< TextureInfo * > TextureInfoVec;

class IPlugin : public IUpdatable
{
public:

    virtual const std::string &                 GetName                     () const = 0;
    virtual const std::string &                 GetTypeUid                  () const = 0;

    //TODO: rething the whole query API - and const correctness as well for that matter
    virtual IPluginParamValModelPtr             GetPluginParamValModel      () const = 0; //FIXME: czy tu nie powinno byc tez od razu const przy zwracanym wskazniku?
    virtual IParameterPtr                       GetParameter                ( const std::string & name ) const = 0;
    virtual bv::IValueConstPtr                  GetValue                    ( const std::string & name ) const = 0;

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const = 0;
    virtual ITransformChannelConstPtr           GetTransformChannel         () const = 0;
    virtual IPixelShaderChannelConstPtr         GetPixelShaderChannel       () const = 0;
    virtual IVertexShaderChannelConstPtr        GetVertexShaderChannel      () const = 0;
    virtual IGeometryShaderChannelConstPtr      GetGeometryShaderChannel    () const = 0;

    virtual RendererContextConstPtr             GetRendererContext          () const = 0;

    //FIXME: implement a more generic interface for resource loading
    virtual bool                                LoadResource                ( IPluginResourceDescrConstPtr resDescr ) = 0;

    virtual                                     ~IPlugin                    (){}

};

DEFINE_PTR_TYPE(IPlugin)
DEFINE_CONST_PTR_TYPE(IPlugin)

} // model
} // bv
