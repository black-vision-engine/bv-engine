#include "ModelInteractionLayer.h"

#include "Engine/Events/Interfaces/IEventManager.h"


namespace bv
{

SetTransformParamsEventPtr  ModelInteractionLayer::m_SetTransformParamEvent = SetTransformParamsEventPtr( new SetTransformParamsEvent() );
SetColorParamEventPtr       ModelInteractionLayer::m_SetColorParamEvent = SetColorParamEventPtr( new SetColorParamEvent() );

// *************************************
//
void    ModelInteractionLayer::SetTransform( const std::string & name, const glm::vec3 & pos, const glm::vec3 & rotAxis, float rotAng, const glm::vec3 & scale, bool pc, bool rc, bool sc )
{
    m_SetTransformParamEvent->SetState( ParseParamName( name ), pos, rotAxis, rotAng, scale, pc, rc, sc );

    GetDefaultEventManager().ConcurrentQueueEvent( m_SetTransformParamEvent->Clone() );
}

// *************************************
//
void    ModelInteractionLayer::SetColor    ( const std::string & name, const glm::vec4 & color )
{
    m_SetColorParamEvent->SetState( ParseParamName( name ), color );

    GetDefaultEventManager().ConcurrentQueueEvent( m_SetColorParamEvent->Clone() );
}

// *************************************
//
void    ModelInteractionLayer::SetPos      ( const std::string & name, const glm::vec3 & pos )
{
    SetTransform( name, pos, pos, 0.0f, pos, true, false, false );
}

// *************************************
//
void    ModelInteractionLayer::SetRot      ( const std::string & name, const glm::vec3 & rotAxis, float angle )
{
    SetTransform( name, rotAxis, rotAxis, angle, rotAxis, false, true, false );
}

// *************************************
//
void    ModelInteractionLayer::SetScl      ( const std::string & name, const glm::vec3 & scale )
{
    SetTransform( name, scale, scale, 0.0f, scale, false, false, true );
}

// *************************************
// TODO: implement and implement delegate in bvAppLogic to service these events (this is going to be mentioned public access API in some ridimentary, stub form)
std::vector< std::string > ModelInteractionLayer::ParseParamName( const std::string & name )
{
    std::vector< std::string > ret;

    return ret;
}

}
