#pragma once

#include "Shader.h"

namespace bv {

namespace model
{
    class ValueMat4;
}

class GeometryShader : public Shader
{
    model::ValueMat4*          m_MVP;
    model::ValueMat4*          m_MV;
    model::ValueMat4*          m_P;

public:

    void        UpdateMVP   ( const glm::mat4& MVP );
    void        UpdateMV    ( const glm::mat4& MV );
    void        UpdateP     ( const glm::mat4& P );

    GeometryShader( const std::string & programSource );

};

} // bv