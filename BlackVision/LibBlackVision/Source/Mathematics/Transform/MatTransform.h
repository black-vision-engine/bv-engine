#pragma once

#include <vector>
#include <iostream>

#include "Mathematics/glm_inc.h"
#include <glm/gtc/matrix_transform.hpp>

#include "Mathematics/Interpolators/Interpolators.h"


namespace bv
{
namespace model
{

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

// ******************* TransformInterpolator **************** //

enum class TransformKind : int
{
    fwd_center,
    rotation,
    scale,
    translation,
    inv_center
};

template<typename ParamT>
class SimpleTransform : public ISerializable
{
protected:

    TransformKind kind;

    ParamT p0; 
    ParamT p1;
    ParamT p2;

//protected:
public:

    explicit SimpleTransform( const TransformKind kind, const ParamT p0, const ParamT p1, const ParamT p2 );
    explicit SimpleTransform( TransformKind kind );

public:
    virtual void                Serialize       ( SerializeObject & sob ) const;
    static ISerializablePtr     Create          ( DeserializeObject & dob );
    virtual void                SetCurveType        ( CurveType type );

    virtual glm::mat4x4         Evaluate            ( typename ParamT::TimeT t ) const;
    virtual SimpleTransform *   Clone               () const;

    static std::shared_ptr< SimpleTransform >    CreateScale         ( ParamT p0, ParamT p1, ParamT p2 )
    {
        return std::make_shared< SimpleTransform >( TransformKind::scale, p0, p1, p2 );
    }

    static std::shared_ptr< SimpleTransform >    CreateTranslation   ( ParamT p0, ParamT p1, ParamT p2, TransformKind tk = TransformKind::translation )
    {
        return std::make_shared< SimpleTransform >( tk, p0, p1, p2 );
    }

    void  SetValues ( TimeType t, float v0, float v1, float v2 )
    {
        p0.AddKey( t, v0 );
        p1.AddKey( t, v1 );
        p2.AddKey( t, v2 );
    }

    void  SetVecVal ( TimeType t, const glm::vec3 & v )
    {
        SetValues( t, v[ 0 ], v[ 1 ], v[ 2 ] );
    }

    TransformKind		KindKurwaMac()
    {
        return kind;
    }

    ParamT &			GetP0MotylaNoga()// const
    {
        return p0;
    }

    ParamT &			GetP1MotylaNoga()// const
    {
        return p1;
    }

    ParamT &			GetP2MotylaNoga()// const
    {
        return p2;
    }

    const ParamT &		GetP0MotylaNoga() const
    {
        return p0;
    }

    const ParamT &      GetP1MotylaNoga() const
    {
        return p1;
    }

    const ParamT &		GetP2MotylaNoga() const
    {
        return p2;
    }
};

template<typename ParamT>
class Rotation : public SimpleTransform<ParamT>
{
private:

    bool                m_hasRotAxisInterpolator;

    ParamT              m_angle;
    Vec3Interpolator    m_rotationAxis;

public:
    virtual void                SetCurveType        ( CurveType type );

    explicit                    Rotation    ( ParamT angle, const Vec3Interpolator & rotAxis );
    explicit                    Rotation    ( ParamT angle, ParamT p0, ParamT p1, ParamT p2 );

    virtual glm::mat4x4         Evaluate    ( typename ParamT::TimeT t ) const override;
    virtual SimpleTransform *   Clone       () const;

    void                        SetRotation ( TimeType t, const glm::vec3 & rotAxis, float angle_ )
    {
        m_angle.AddKey( t, angle_ );
        m_rotationAxis.AddKey( t, rotAxis );
    }

	Vec3Interpolator &			AccessRotAxis	()
	{
		return m_rotationAxis;
	}

	ParamT	& 					AccessAngle		()
	{
		return m_angle;
	}

	bool						IsAxisVec3		() const
	{
		return m_hasRotAxisInterpolator;
	}
};

template<typename ParamT>
class CompositeTransform : public Interpolator<typename ParamT::TimeT>, public ISerializable
{
private:

    std::vector< std::shared_ptr< SimpleTransform<ParamT> > > m_transformations;

public:

    static const int value_size = sizeof( glm::mat4x4 );

public:

    explicit        CompositeTransform  ();
                    CompositeTransform  ( const CompositeTransform & src );

    static ISerializablePtr                     Create                  ( const DeserializeObject & dob );
    virtual void                                Serialize               ( SerializeObject & doc ) const;

    void            InitializeDefaultSRT();

    virtual         ~CompositeTransform ();

    void            SetCurveType        ( CurveType type );

    void            AddTranslation      ( ParamT x0, ParamT x1, ParamT x2 );
    void            AddScale            ( ParamT s0, ParamT s1, ParamT s2 );
    void            AddRotation         ( ParamT angle, ParamT r0, ParamT r1, ParamT r2 );
    void            AddRotation         ( ParamT angle, const Vec3Interpolator & rotAxis );
    void            AddTranslationCFwd  ( ParamT x0, ParamT x1, ParamT x2 );
    void            AddTranslationCInv  ( ParamT x0, ParamT x1, ParamT x2 );
    void            AddTransform        ( std::shared_ptr< SimpleTransform<ParamT> > trans );
    void            InsertTransform     ( int i, std::shared_ptr< SimpleTransform<ParamT> > trans );

    SizeType        Size                () const;

    SimpleTransform<ParamT> *        operator[](unsigned int i);
    const SimpleTransform<ParamT> *  operator[](unsigned int i) const;

    glm::mat4x4     Evaluate            ( typename ParamT::TimeT t ) const;

    int             EvalToCBuffer       ( typename ParamT::TimeT,char * ) const;

};

// *************************************
//
template<typename ParamT>
glm::mat4x4 Rotation<ParamT>::Evaluate(typename ParamT::TimeT t) const
{
    if( m_hasRotAxisInterpolator )
    {
        return glm::rotate( glm::mat4( 1.0f ), (float) m_angle.Evaluate( t ), m_rotationAxis.Evaluate( t ) );        
    }
    else
    {
        return glm::rotate( glm::mat4( 1.0f ), (float) m_angle.Evaluate( t ), glm::vec3( p0.Evaluate ( t ), p1.Evaluate( t ), p2.Evaluate( t ) ) );
    }
}

// *************************************
//
template<typename ParamT>
SimpleTransform<ParamT> *   Rotation<ParamT>::Clone       () const
{
    return new Rotation( *this );
}

typedef CompositeTransform<FloatInterpolator> TransformF;
typedef SimpleTransform<FloatInterpolator> SimpleTransformF;
typedef Rotation<FloatInterpolator> RotationF;

}
