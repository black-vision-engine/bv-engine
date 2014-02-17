#pragma once

#include <memory>

#include "Engine/Interfaces/IUpdatable.h"
#include "Engine/Models/Plugins/Interfaces/IParamValModel.h"


namespace bv { namespace model {

class IPluginParamValModel : public IUpdatable
{
public:

    virtual     IParamValModel *    GetPluginModel                  () = 0;

    virtual     IParamValModel *    GetTransformChannelModel        () = 0;
    virtual     IParamValModel *    GetVertexAttributesChannelModel () = 0;
    virtual     IParamValModel *    GetPixelShaderChannelModel      () = 0;
    virtual     IParamValModel *    GetVertexShaderChannelModel     () = 0;
    virtual     IParamValModel *    GetGeometryShaderChannelModel   () = 0;

    virtual     ~IPluginParamValModel   () {} 

};

typedef std::shared_ptr< IPluginParamValModel > IPluginParamValModelPtr;

} //model
} //bv
