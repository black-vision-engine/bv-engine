#pragma once

#include <memory>

#include "Engine/Models/Plugins/Interfaces/IParamValModel.h"


namespace bv { namespace model {

class IPluginParamValModel;
DEFINE_PTR_TYPE(IPluginParamValModel)

class IPluginParamValModel
{
public:

    virtual     void                Update                          () = 0;

    virtual     IParamValModel *    GetPluginModel                  () = 0;

    virtual     IParamValModel *    GetTransformChannelModel        () = 0;
    virtual     IParamValModel *    GetVertexAttributesChannelModel () = 0;
    virtual     IParamValModel *    GetPixelShaderChannelModel      () = 0;
    virtual     IParamValModel *    GetVertexShaderChannelModel     () = 0;
    virtual     IParamValModel *    GetGeometryShaderChannelModel   () = 0;

    virtual     ~IPluginParamValModel   () {} 

};

} //model
} //bv
