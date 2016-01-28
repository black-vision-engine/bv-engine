#pragma once

#include "Mathematics/glm_inc.h"
#include <glm/gtc/matrix_transform.hpp>

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
    fwd_center,
    rotation,
    scale,
    translation,

    invalid
};
//bool  MoveValues    ( TimeType t, TimeType newTime )
//    {
//        bool result = true;
//
//        result = result && p0.MoveKey( t, newTime );
//        result = result && p1.MoveKey( t, newTime );
//        result = result && p2.MoveKey( t, newTime );
//        
//        return result;
//    }
// ******************* CompositeTransform **************** 
class CompositeTransform : public ISerializable
{
private:

    FloatInterpolator   m_translationX;
    FloatInterpolator   m_translationY;
    FloatInterpolator   m_translationZ;

    Vec3Interpolator    m_rotationAxis;
    FloatInterpolator   m_rotationAngle;

    FloatInterpolator   m_scaleX;
    FloatInterpolator   m_scaleY;
    FloatInterpolator   m_scaleZ;

    FloatInterpolator   m_centerX;
    FloatInterpolator   m_centerY;
    FloatInterpolator   m_centerZ;

public:

    explicit        CompositeTransform  ();

    static CompositeTransform *  Create      ( const IDeserializer & dob );
    virtual void                 Serialize   ( ISerializer & doc ) const;

    void            InitializeDefaultSRT();

    virtual         ~CompositeTransform ();

    void            SetCurveType        ( CurveType type );
    void            SetWrapPostMethod   ( WrapMethod method );
    void            SetWrapPreMethod    ( WrapMethod method );

    glm::vec3       GetTranslation      ( TimeType time ) const;
    glm::vec3       GetRotationAxis     ( TimeType time ) const;
    Float32         GetRotationAngle    ( TimeType time ) const;
    glm::vec3       GetScale            ( TimeType time ) const;
    glm::vec3       GetCenter           ( TimeType time ) const;

    void            SetTranslation      ( const glm::vec3 & vec, TimeType time );
    void            SetRotation         ( const glm::vec3 & axis, Float32 angle, TimeType time );
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
