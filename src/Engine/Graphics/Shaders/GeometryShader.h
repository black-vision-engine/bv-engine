#pragma once

#include "Engine/Graphics/Shaders/Shader.h"
#include "Engine/Models/Plugins/Parameters/TypedValues.h"


namespace bv {

class GeometryShader : public Shader
{
private:

    ValueMat4Ptr         m_MVP;
    ValueMat4Ptr         m_MV;
    ValueMat4Ptr         m_P;

public:

    void        UpdateMVP   ( const glm::mat4 & MVP );
    void        UpdateMV    ( const glm::mat4 & MV );
    void        UpdateP     ( const glm::mat4 & P );

    GeometryShader( const std::string & programSource );

};

} // bv