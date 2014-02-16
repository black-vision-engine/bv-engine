#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <memory>

#include "Engine/Models/Plugins/Interfaces/IValue.h"
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
typedef std::vector< TextureInfo * > Textures;

class IPlugin : public IUpdatable
{
public:

    virtual const char *                        GetName                     () const = 0;

    virtual IPluginParamValModel *              GetPluginModelParameters    () = 0;

    virtual const IVertexAttributesChannel *    GetVertexAttributesChannel  () const = 0;
    virtual const ITransformChannel *           GetTransformChannel         () const = 0;
    virtual const IPixelShaderChannel *         GetPixelShaderChannel       () const = 0;
    virtual const IVertexShaderChannel *        GetVertexShaderChannel      () const = 0;
    virtual const IGeometryShaderChannel *      GetGeometryShaderChannel    () const = 0;
    virtual const IDefaultTransformChannel *    GetDefaultTransformChannel  () const = 0;

    virtual Textures                            GetTextures                 () const = 0; // FIXME: use resources
    virtual bool                                HasAnimatingTexture         () const = 0;
    virtual ISequenceAnimationSource *          QuerySequenceAnimationSource() = 0; // FIXME: use generic interface for querying resources that can be 

    virtual void                                Print                       ( std::ostream & out, int tabs = 0 )    const = 0;

    virtual                                     ~IPlugin                    (){}

};

typedef std::shared_ptr<IPlugin>        IPluginPtr;

} // model
} // bv
