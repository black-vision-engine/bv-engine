#pragma once

#include <vector>
#include "Engine/Events/BaseEvent.h"
#include  "Mathematics/glm_inc.h"

namespace bv
{

class SetTransformParamsEvent : public BaseEvent
{
private:

    std::vector< std::string >  m_paramPath;
   
    glm::vec3                   m_pos;
    bool                        m_posChanged;

    glm::vec3                   m_rotAxis;
    float                       m_rotAngle;
    bool                        m_rotChanged;

    glm::vec3                   m_scale;
    bool                        m_sclChanged;

public:

    static const EventType      m_sEventType;
    static std::string          m_sEventName;

public:

    explicit                        SetTransformParamsEvent();
    explicit                        SetTransformParamsEvent( const std::vector< std::string > & paramPath, const glm::vec3 & pos, const glm::vec3 & rotAxis, float rotAng, const glm::vec3 & scale );

    virtual EventType               GetEventType    () const;

    virtual void                    Serialize       ( std::ostringstream & out ) const;
    virtual void                    Deserialize     ( std::istringstream & in );

    virtual IEventPtr               Clone           () const;

    virtual const std::string &     GetName         () const;

    const std::vector< std::string > &  GetPath     () const;
    const glm::vec3 &                   GetPos      () const;
    const glm::vec3 &                   GetRotAxis  () const;
    float                               GetRotAngle () const;
    const glm::vec3 &                   GetScale    () const;

    bool                                IsPosChanged() const;
    bool                                IsRotChanged() const;
    bool                                IsSclChanged() const;

    void                                SetState    ( const std::vector< std::string > & paramPath, const glm::vec3 & pos, const glm::vec3 & rotAxis, float rotAng, const glm::vec3 & scale, bool pc, bool rc, bool sc );

    static EventType                Type            ();

};

DEFINE_PTR_TYPE(SetTransformParamsEvent)


class SetColorParamEvent : public BaseEvent
{
private:

    std::vector< std::string >  m_paramPath;
   
    glm::vec4                   m_color;

public:

    static const EventType      m_sEventType;
    static std::string          m_sEventName;

public:

    explicit                        SetColorParamEvent();
    explicit                        SetColorParamEvent( const std::vector< std::string > & paramPath, const glm::vec4 & col );

    virtual EventType               GetEventType    () const;

    virtual void                    Serialize       ( std::ostringstream & out ) const;
    virtual void                    Deserialize     ( std::istringstream & in );

    virtual IEventPtr               Clone           () const;

    virtual const std::string &     GetName         () const;

    const std::vector< std::string > &  GetPath     () const;
    const glm::vec4 &                   GetColor    () const;
    void                                SetState    ( const std::vector< std::string > & paramPath, const glm::vec4 & col );

    static EventType                Type            ();

};

DEFINE_PTR_TYPE(SetColorParamEvent)

} //bv
