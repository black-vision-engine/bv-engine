#pragma once

#include "Engine/Interfaces/IUpdatable.h"

#include "Engine/Models/Plugins/Interfaces/IParamValModel.h"


namespace bv { namespace model {

class IPluginParamValModel : public IUpdatable
{
public:

    virtual     IParamValModel *    GetPluginModel          () = 0;

    virtual     IParamValModel *    GetTransformModel       () = 0;
    virtual     IParamValModel *    GetVertexAttributesModel() = 0;
    virtual     IParamValModel *    GetPixelShaderModel     () = 0;
    virtual     IParamValModel *    GetVertexShaderModel    () = 0;
    virtual     IParamValModel *    GetGeometryShaderModel  () = 0;

    virtual     ~IPluginParamValModel   () {} 

};

} //model
} //bv
