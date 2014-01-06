#pragma once

#include <memory>

#include "Engine/Models/Plugins/Channels/PixelShaderChannelBase.h"
#include "Engine/Models/Plugins/Parameters/TypedParameters.h"
#include "Engine/Models/Plugins/Parameters/TypedValues.h"


namespace bv { namespace model
{

class TexturePixelShaderChannelPD
{
public:

    static std::string          alphaParamName[];
    static std::string          txMatrixParamName[];
    static std::string          borderColorsParamName[];

};


class TexturePixelShaderChannel : public model::PixelShaderChannelBase< TexturePixelShaderChannelPD >
{
    std::vector< ParamFloat >       m_alphaParams;
    std::vector< ValueFloatPtr >    m_alphaValues;

    std::vector< ParamTransform >   m_texTransformParams;
    std::vector< ValueMat4Ptr >     m_texTransformValues;

    std::vector< ParamVec4 >        m_borderColorParams;
    std::vector< ValueVec4Ptr >     m_borderColorValues;

public:

    virtual void                    Update( TimeType t );

    TexturePixelShaderChannel( const std::string & shaderFile, const std::vector< ParamFloat > & alphas, const std::vector< ParamTransform > & texTransforms, const std::vector< ParamVec4 > & borderColors );

};

typedef std::shared_ptr< TexturePixelShaderChannel >    TexturePixelShaderChannelPtr;

} // model
} // bv
