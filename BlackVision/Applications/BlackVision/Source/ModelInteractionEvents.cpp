#include "stdafxBVApp.h"

#include "ModelInteractionEvents.h"

namespace bv
{

// ************************************* Static Events' Definitions *************************************

const EventType SetTransformParamsEvent::m_sEventType   = 0x10000001;
std::string SetTransformParamsEvent::m_sEventName       = "Event_SetTransformParams";

const EventType SetColorParamEvent::m_sEventType        = 0x10000002;
std::string SetColorParamEvent::m_sEventName            = "Event_SetColorParamEvent";

// ************************************* SetTransformParamsEvent *************************************

// *************************************
//
SetTransformParamsEvent::SetTransformParamsEvent()
{
}

// *************************************
//
SetTransformParamsEvent::SetTransformParamsEvent( const std::vector< std::string > & paramPath, const glm::vec3 & pos, const glm::vec3 & rotAxis, float rotAng, const glm::vec3 & scale )
    : m_paramPath( paramPath )
    , m_pos( pos )
    , m_rotAxis( rotAxis )
    , m_rotAngle( rotAng )
    , m_scale( scale )
{
    m_posChanged = m_rotChanged = m_sclChanged = true;
}

// *************************************
//
EventType               SetTransformParamsEvent::GetEventType    () const
{
    return Type();
}

// *************************************
//
void                    SetTransformParamsEvent::Serialize       ( std::ostringstream & out ) const
{   
    { out; } // FIXME: suppress unused warning

    assert( false );
}

// *************************************
//
void                    SetTransformParamsEvent::Deserialize     ( std::istringstream & in )
{
    { in; } // FIXME: suppress unused warning

    assert( false );
}

// *************************************
//
IEventPtr               SetTransformParamsEvent::Clone           () const
{
    return IEventPtr( new SetTransformParamsEvent( *this ) );
}

// *************************************
//
const std::string &     SetTransformParamsEvent::GetName         () const
{
    return m_sEventName;
}

// *************************************
//
const std::vector< std::string > &  SetTransformParamsEvent::GetPath     () const
{
    return m_paramPath;
}

// *************************************
//
const glm::vec3 & SetTransformParamsEvent::GetPos      () const
{
    return m_pos;
}

// *************************************
//
const glm::vec3 & SetTransformParamsEvent::GetRotAxis  () const
{
    return m_rotAxis;
}

// *************************************
//
float             SetTransformParamsEvent::GetRotAngle () const
{
    return m_rotAngle;
}

// *************************************
//
const glm::vec3 & SetTransformParamsEvent::GetScale    () const
{
    return m_scale;
}

// *************************************
//
bool                SetTransformParamsEvent::IsPosChanged() const
{
    return m_posChanged;
}

// *************************************
//
bool                SetTransformParamsEvent::IsRotChanged() const
{
    return m_rotChanged;
}

// *************************************
//
bool                SetTransformParamsEvent::IsSclChanged() const
{
    return m_sclChanged;
}

// *************************************
//
void              SetTransformParamsEvent::SetState    ( const std::vector< std::string > & paramPath, const glm::vec3 & pos, const glm::vec3 & rotAxis, float rotAng, const glm::vec3 & scale, bool pc, bool rc, bool sc )
{
    m_paramPath = paramPath;
    m_pos = pos;
    m_rotAxis = rotAxis;
    m_rotAngle = rotAng;
    m_scale = scale;

    m_posChanged = pc;
    m_rotChanged = rc;
    m_sclChanged = sc;
}

// *************************************
//
EventType                SetTransformParamsEvent::Type            ()
{
    return m_sEventType;
}


// ************************************* SetColorParamEvent *************************************

// *************************************
//
SetColorParamEvent::SetColorParamEvent()
{
}

// *************************************
//
SetColorParamEvent::SetColorParamEvent( const std::vector< std::string > & paramPath, const glm::vec4 & col )
    : m_paramPath( paramPath )
    , m_color( col )
{
}

// *************************************
//
EventType               SetColorParamEvent::GetEventType    () const
{
    return Type();
}

// *************************************
//
void                    SetColorParamEvent::Serialize       ( std::ostringstream & out ) const
{
    { out; } // FIXME: suppress unused warning
    assert( false );
}

// *************************************
//
void                    SetColorParamEvent::Deserialize     ( std::istringstream & in )
{
    { in; } // FIXME: suppress unused warning
    assert( false );
}

// *************************************
//
IEventPtr               SetColorParamEvent::Clone           () const
{
    return IEventPtr( new SetColorParamEvent( *this ) );
}

// *************************************
//
const std::string &     SetColorParamEvent::GetName         () const
{
    return m_sEventName;
}

// *************************************
//
const std::vector< std::string > &  SetColorParamEvent::GetPath     () const
{
    return m_paramPath;
}

// *************************************
//
const glm::vec4 &                   SetColorParamEvent::GetColor    () const
{
    return m_color;
}

// *************************************
//
void                                SetColorParamEvent::SetState    ( const std::vector< std::string > & paramPath, const glm::vec4 & col )
{
    m_paramPath = paramPath;
    m_color = col;
}

// *************************************
//
EventType                SetColorParamEvent::Type            ()
{
    return m_sEventType;
}

} //bv
