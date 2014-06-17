#pragma once

#include <memory>

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"
#include "Engine/Models/Plugins/Parameters/CompositeTypedParameters.h"
#include "Engine/Types/Values/TypedValues.h"


namespace bv { namespace model
{

class TexturePixelShaderChannelPD
{
public:

    static std::string          alphaParamName[];
    static std::string          txMatrixParamName[];
    static std::string          borderColorsParamName[];

};


class TexturePixelShaderChannel : public DefaultPixelShaderChannel
{
    std::vector< ParamFloat >       m_alphaParams;
    std::vector< ValueFloatPtr >    m_alphaValues;

    ParamTransformVec               m_texTransformParams;
    std::vector< ValueMat4Ptr >     m_texTransformValues;

    std::vector< ParamVec4 >        m_borderColorParams;
    std::vector< ValueVec4Ptr >     m_borderColorValues;

public:

    TexturePixelShaderChannel   ( const std::string & shaderFile, const std::vector< ParamFloat > & alphas, const ParamTransformVec & texTransforms, const std::vector< ParamVec4 > & borderColors );
    ~TexturePixelShaderChannel  ();

};

DEFINE_PTR_TYPE(TexturePixelShaderChannel)

} // model
} // bv
