#pragma once

#include "ModelInteractionEvents.h"


namespace bv
{

class ModelInteractionLayer
{
private:

    static SetTransformParamsEventPtr  m_SetTransformParamEvent;
    static SetColorParamEventPtr       m_SetColorParamEvent;

public:
    
    void    static SetTransform ( const std::string & name, const glm::vec3 & pos, const glm::vec3 & rotAxis, float rotAng, const glm::vec3 & scale, bool pc, bool rc, bool sc );
    void    static SetColor     ( const std::string & name, const glm::vec4 & color );

    void    static SetPos       ( const std::string & name, const glm::vec3 & pos );
    void    static SetRot       ( const std::string & name, const glm::vec3 & rotAxis, float angle );
    void    static SetScl       ( const std::string & name, const glm::vec3 & scale );

    static std::vector< std::string > ParseParamName( const std::string & name, const std::string & delimiter = "/" );
};

} // bv
