#pragma once

#include "Mathematics/Transform/SQTTransform.h"
#include "Mathematics/Interpolators/Interpolators.h"


namespace bv { namespace model {

// ******************* Transform **************** 
class Transform
{
private:

    glm::mat4x4 m_mat;

public:

    explicit                Transform   ( const glm::mat4x4 & m );
                            Transform   ();

    Transform               operator *  ( const Transform & m )     const;

    void                    SetMatrix   ( const glm::mat4x4 & m );
    const glm::mat4x4 &     GetMatrix   ()                          const;

};

}

//maintain serialization backward compatibility
enum class TransformKind : int
{
    center,
    rotation,
    scale,
    translation,

    invalid
};

class CompositeTransform;
DEFINE_PTR_TYPE(CompositeTransform)

// ******************* CompositeTransform **************** 
class CompositeTransform : public ISerializable
{
    FRIEND_TEST_ACCESSOR( CompositeTransform )
private:

    FloatInterpolator   m_translationX;
    FloatInterpolator   m_translationY;
    FloatInterpolator   m_translationZ;

    /* stored in degrees */
    FloatInterpolator   m_eulerPitch;
    FloatInterpolator   m_eulerYaw;
    FloatInterpolator   m_eulerRoll;

    FloatInterpolator   m_scaleX;
    FloatInterpolator   m_scaleY;
    FloatInterpolator   m_scaleZ;

    FloatInterpolator   m_centerX;
    FloatInterpolator   m_centerY;
    FloatInterpolator   m_centerZ;

    
public:
    static CompositeTransformPtr Create     ();

    static CompositeTransformPtr Create      ( const IDeserializer & dob );
    virtual void                 Serialize   ( ISerializer & doc ) const;
    void                         Serialize   ( ISerializer & doc, TransformKind kind ) const;

    explicit        CompositeTransform   ();
    void            InitializeDefaultSRT();

    virtual         ~CompositeTransform ();

    void            SetGlobalCurveType  ( CurveType type );
    void            SetAddedKeyCurveType( CurveType type );
    void            SetWrapPostMethod   ( WrapMethod method );
    void            SetWrapPreMethod    ( WrapMethod method );

    glm::vec3       GetTranslation      ( TimeType time ) const;
    glm::vec3       GetRotation         ( TimeType time ) const;
    glm::vec3       GetScale            ( TimeType time ) const;
    glm::vec3       GetCenter           ( TimeType time ) const;

    void            SetTranslation      ( const glm::vec3 & vec, TimeType time );
    void            SetRotation         ( const glm::vec3 & vec, TimeType time );
    void            SetScale            ( const glm::vec3 & vec, TimeType time );
    void            SetCenter           ( const glm::vec3 & vec, TimeType time );

    void            RemoveTranslation   ( TimeType time );
    void            RemoveRotation      ( TimeType time );
    void            RemoveScale         ( TimeType time );
    void            RemoveCenter        ( TimeType time );

    bool            MoveTranslation     ( TimeType time, TimeType newTime );
    bool            MoveRotation        ( TimeType time, TimeType newTime );
    bool            MoveScale           ( TimeType time, TimeType newTime );
    bool            MoveCenter          ( TimeType time, TimeType newTime );

    glm::mat4x4     Evaluate            ( TimeType t ) const;

};

}
