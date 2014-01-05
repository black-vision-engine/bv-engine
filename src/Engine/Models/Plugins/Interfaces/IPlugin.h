#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "Engine/Models/Plugins/Interfaces/IValue.h"
#include "Engine/Interfaces/IUpdatable.h"


namespace bv { namespace model {

class ISequenceAnimationSource;
class IVertexAttributesChannel;
class ITransformChannel;
class IPixelShaderChannel;
class IVertexShaderChannel;
class IGeometryShaderChannel;
class ResourceHandle;

struct TextureInfo;
typedef std::vector< TextureInfo* > Textures;

class IPlugin : public IUpdatable
{
public:

    virtual const std::string &                 GetName                     () const = 0;

    virtual const IVertexAttributesChannel *    GetGeometryChannel          () const = 0;
    virtual const ITransformChannel *           GetTransformChannel         () const = 0;
    virtual const IPixelShaderChannel *         GetPixelShaderChannel       () const = 0;
    virtual const IVertexShaderChannel *        GetVertexShaderChannel      () const = 0;
    virtual const IGeometryShaderChannel *      GetGeometryShaderChannel    () const = 0;

    virtual Textures                            GetTextures                 () const = 0; // FIXME: use resources
    virtual bool                                HasAnimatingTexture         () const = 0;
    virtual ISequenceAnimationSource *          QuerySequenceAnimationSource() = 0;

    virtual void                                Print                       ( std::ostream & out, int tabs = 0 )    const = 0;

    virtual const std::vector<IValue *> &       GetValuesList               () const = 0;

    virtual                                     ~IPlugin                    (){}

};

} // model
} // bv