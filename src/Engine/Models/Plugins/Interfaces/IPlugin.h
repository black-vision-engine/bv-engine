#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <memory>

#include "Engine/Interfaces/IValue.h"
#include "Engine/Interfaces/IUpdatable.h"


namespace bv { namespace model {

class ISequenceAnimationSource;
class IVertexAttributesChannel;
class IDefaultTransformChannel;
class ITransformChannel;
class IPixelShaderChannel;
class IVertexShaderChannel;
class IGeometryShaderChannel;
class ResourceHandle;
class IParameter;
class IPluginParamValModel;

struct TextureInfo;
typedef std::vector< TextureInfo * > TextureInfoVec;

class IPlugin : public IUpdatable
{
public:

    virtual const std::string &                 GetName                     () const = 0;
    virtual const std::string &                 GetTypeUid                  () const = 0;

    virtual IPluginParamValModel *              GetPluginParamValModel      () = 0;

    virtual const IVertexAttributesChannel *    GetVertexAttributesChannel  () const = 0;
    virtual const ITransformChannel *           GetTransformChannel         () const = 0;
    virtual const IPixelShaderChannel *         GetPixelShaderChannel       () const = 0;
    virtual const IVertexShaderChannel *        GetVertexShaderChannel      () const = 0;
    virtual const IGeometryShaderChannel *      GetGeometryShaderChannel    () const = 0;

    virtual TextureInfoVec                      GetTextures                 () const = 0; // FIXME: use resources
    virtual bool                                HasAnimatingTexture         () const = 0;
    virtual const ISequenceAnimationSource *    QuerySequenceAnimationSource() const = 0; // FIXME: use generic interface for querying resources that can be 

    virtual                                     ~IPlugin                    (){}

};

typedef std::shared_ptr<IPlugin>        IPluginPtr;

} // model
} // bv
