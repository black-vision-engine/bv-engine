#pragma once

#include <vector>
#include <iostream>

#include <glm/glm.hpp>
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
    Transform               operator *  (const Transform& m)        const;

    void                    SetMatrix   ( const glm::mat4x4& m );
    const glm::mat4x4&      GetMatrix   ()                          const;

    explicit                Transform   (const glm::mat4x4& m);
    Transform();
};

}

// ******************* TransformInterpolator **************** //

enum class TransformKind : int
{
    rotation,
    scale,
    translation
};

template<typename ParamT>
class SimpleTransform
{
protected:

    TransformKind kind;

    ParamT p0; 
    ParamT p1;
    ParamT p2;

protected:

    explicit SimpleTransform( TransformKind kind, ParamT p0, ParamT p1, ParamT p2 );
    explicit SimpleTransform( TransformKind kind );

public:

    virtual glm::mat4x4         Evaluate            ( typename ParamT::TimeT t ) const;
    virtual SimpleTransform *   Clone               () const;

    static SimpleTransform *    CreateScale         ( ParamT p0, ParamT p1, ParamT p2 )
    {
        return new SimpleTransform( TransformKind::scale, p0, p1, p2 );
    }

    static SimpleTransform *    CreateTranslation   ( ParamT p0, ParamT p1, ParamT p2 )
    {
        return new SimpleTransform( TransformKind::translation, p0, p1, p2 );
    }

};

template<typename ParamT>
class Rotation : public SimpleTransform<ParamT>
{
private:

    bool                m_hasRotAxisInterpolator;

    ParamT              angle;
    Vec3Interpolator    m_rotationAxis;

public:

    explicit                    Rotation    ( ParamT angle, const Vec3Interpolator & rotAxis );
    explicit                    Rotation    ( ParamT angle, ParamT p0, ParamT p1, ParamT p2 );

    virtual glm::mat4x4         Evaluate    ( typename ParamT::TimeT t ) const override;
    virtual SimpleTransform *   Clone       () const;

};

template<typename ParamT>
class CompositeTransform : public Interpolator<typename ParamT::TimeT>
{
private:

    std::vector< SimpleTransform<ParamT> * > m_transformations;

public:

    static const int value_size = sizeof( glm::mat4x4 );

public:

    explicit        CompositeTransform  ();
                    CompositeTransform  ( const CompositeTransform & src );

    virtual         ~CompositeTransform ();

    void            AddTranslation      ( ParamT x0, ParamT x1, ParamT x2 );
    void            AddScale            ( ParamT s0, ParamT s1, ParamT s2 );
    void            AddRotation         ( ParamT angle, ParamT r0, ParamT r1, ParamT r2 );
    void            AddRotation         ( ParamT angle, const Vec3Interpolator & rotAxis );
    void            AddTransform        ( SimpleTransform<ParamT> * trans );

    unsigned int    Size                () const;

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
        return glm::rotate( glm::mat4( 1.0f ), (float) angle.evaluate( t ), m_rotationAxis.evaluate( t ) );        
    }
    else
    {
        return glm::rotate( glm::mat4( 1.0f ), (float) angle.evaluate( t ), glm::vec3( p0.evaluate ( t ), p1.evaluate( t ), p2.evaluate( t ) ) );
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
