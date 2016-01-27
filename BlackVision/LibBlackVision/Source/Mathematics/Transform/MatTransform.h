#pragma once

#include "Mathematics/glm_inc.h"
#include <glm/gtc/matrix_transform.hpp>

#include "Mathematics/Interpolators/Interpolators.h"


namespace bv { namespace model {

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

//// ******************* TransformInterpolator **************** //
//
enum class TransformKind : int
{
    fwd_center,
    rotation,
    scale,
    translation,

    invalid
};
//
//template<typename ParamT>
//class SimpleTransform : public ISerializable
//{
//protected:
//
//    TransformKind kind;
//
//    ParamT p0; 
//    ParamT p1;
//    ParamT p2;
//
////protected:
//public:
//
//    explicit SimpleTransform( const TransformKind kind, const ParamT p0, const ParamT p1, const ParamT p2 );
//    explicit SimpleTransform( TransformKind kind );
//
//public:
//    virtual void                Serialize       ( ISerializer& sob ) const;
//    static ISerializablePtr     Create          ( const IDeserializer& dob );
//    virtual void                SetCurveType        ( CurveType type );
//    virtual void                SetWrapPostMethod   ( WrapMethod method );
//    virtual void                SetWrapPreMethod    ( WrapMethod method );
//
//    virtual glm::mat4x4         Evaluate            ( typename ParamT::TimeT t ) const;
//    virtual SimpleTransform *   Clone               () const;
//
//    static std::shared_ptr< SimpleTransform >    CreateScale         ( ParamT p0, ParamT p1, ParamT p2 )
//    {
//        return std::make_shared< SimpleTransform >( TransformKind::scale, p0, p1, p2 );
//    }
//
//    static std::shared_ptr< SimpleTransform >    CreateTranslation   ( ParamT p0, ParamT p1, ParamT p2, TransformKind tk = TransformKind::translation )
//    {
//        return std::make_shared< SimpleTransform >( tk, p0, p1, p2 );
//    }
//
//    static std::shared_ptr< SimpleTransform >    CreateDefaultTransform( TransformKind kind );
//
//    void  SetValues ( TimeType t, float v0, float v1, float v2 )
//    {
//        p0.AddKey( t, v0 );
//        p1.AddKey( t, v1 );
//        p2.AddKey( t, v2 );
//    }
//
//    void  SetVecVal ( TimeType t, const glm::vec3 & v )
//    {
//        SetValues( t, v[ 0 ], v[ 1 ], v[ 2 ] );
//    }
//
//    void  RemoveValues ( TimeType t )
//    {
//        p0.RemoveKey( t );
//        p1.RemoveKey( t );
//        p2.RemoveKey( t );
//    }
//
//    TransformKind		KindKurwaMac()
//    {
//        return kind;
//    }
//
//    ParamT &			GetP0MotylaNoga()// const
//    {
//        return p0;
//    }
//
//    ParamT &			GetP1MotylaNoga()// const
//    {
//        return p1;
//    }
//
//    ParamT &			GetP2MotylaNoga()// const
//    {
//        return p2;
//    }
//
//    const ParamT &		GetP0MotylaNoga() const
//    {
//        return p0;
//    }
//
//    const ParamT &      GetP1MotylaNoga() const
//    {
//        return p1;
//    }
//
//    const ParamT &		GetP2MotylaNoga() const
//    {
//        return p2;
//    }
//};
//
//template<typename ParamT>
//class Rotation : public SimpleTransform<ParamT>
//{
//private:
//
//    bool                m_hasRotAxisInterpolator;
//
//    ParamT              m_angle;
//    Vec3Interpolator    m_rotationAxis;
//
//public:
//    virtual void                SetCurveType        ( CurveType type ) override;
//    virtual void                SetWrapPostMethod   ( WrapMethod method ) override;
//    virtual void                SetWrapPreMethod    ( WrapMethod method ) override;
//
//    explicit                    Rotation    ( ParamT angle, const Vec3Interpolator & rotAxis );
//    explicit                    Rotation    ( ParamT angle, ParamT p0, ParamT p1, ParamT p2 );
//
//    virtual glm::mat4x4         Evaluate    ( typename ParamT::TimeT t ) const override;
//    virtual SimpleTransform *   Clone       () const;
//
//    void                        SetRotation ( TimeType t, const glm::vec3 & rotAxis, float angle_ )
//    {
//        m_angle.AddKey( t, angle_ );
//        m_rotationAxis.AddKey( t, rotAxis );
//    }
//
//    void                        RemoveRotation  ( TimeType t )
//    {
//        m_angle.RemoveKey( t );
//        m_rotationAxis.RemoveKey( t );
//    }
//
//	Vec3Interpolator &			AccessRotAxis	()
//	{
//		return m_rotationAxis;
//	}
//
//	ParamT	& 					AccessAngle		()
//	{
//		return m_angle;
//	}
//
//	bool						IsAxisVec3		() const
//	{
//		return m_hasRotAxisInterpolator;
//	}
//};

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

    static const int value_size = sizeof( glm::mat4x4 );

public:

    explicit        CompositeTransform  ();

    static CompositeTransform * Create      ( const IDeserializer & dob );
    virtual void                Serialize   ( ISerializer & doc ) const;

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

    glm::mat4x4     Evaluate            ( TimeType t ) const;

};

//// *************************************
////
//template<typename ParamT>
//glm::mat4x4 Rotation<ParamT>::Evaluate(typename ParamT::TimeT t) const
//{
//    if( m_hasRotAxisInterpolator )
//    {
//        return glm::rotate( glm::mat4( 1.0f ), (float) m_angle.Evaluate( t ), m_rotationAxis.Evaluate( t ) );        
//    }
//    else
//    {
//        return glm::rotate( glm::mat4( 1.0f ), (float) m_angle.Evaluate( t ), glm::vec3( p0.Evaluate ( t ), p1.Evaluate( t ), p2.Evaluate( t ) ) );
//    }
//}
//
//// *************************************
////
//template<typename ParamT>
//SimpleTransform<ParamT> *   Rotation<ParamT>::Clone       () const
//{
//    return new Rotation( *this );
//}

}
