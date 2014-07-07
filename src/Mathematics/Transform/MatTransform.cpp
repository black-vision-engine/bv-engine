#include "Mathematics/Transform/MatTransform.h"


namespace bv { namespace model {

// *************************************
//
Transform::Transform()
    : m_mat( glm::mat4x4( 1.f ) )
{
}

// *************************************
//
Transform::Transform( const glm::mat4x4 & m )
    :   m_mat( m )
{
}

// *************************************
//
Transform               Transform::operator *    ( const Transform & m )    const
{
    return Transform( m_mat * m.m_mat );
}

// *************************************
//
void     Transform::SetMatrix   ( const glm::mat4x4 & m )
{
    m_mat = m;
}

// *************************************
//
const glm::mat4x4 &      Transform::GetMatrix   ()                       const
{
    return m_mat;
}

} // model

// *************************************
//
template<typename ParamT>
SimpleTransform<ParamT>::SimpleTransform( TransformKind kind, ParamT p0, ParamT p1, ParamT p2 )
    : kind( kind ), p0( p0 ), p1( p1 ), p2 (p2 )
{
}

// *************************************
//
template<typename ParamT>
SimpleTransform<ParamT>::SimpleTransform( TransformKind kind )
    : kind( kind )
{
}

template<typename ParamT>
Rotation<ParamT>::Rotation    ( ParamT angle, const Vec3Interpolator & rotAxis )
    : SimpleTransform( TransformKind::rotation )
    , angle( angle )
    , m_hasRotAxisInterpolator( true )
    , m_rotationAxis( rotAxis ) 
{
}

// *************************************
//
template<typename ParamT>
Rotation<ParamT>::Rotation( ParamT angle, ParamT p0, ParamT p1, ParamT p2 )
    : SimpleTransform( TransformKind::rotation, p0, p1, p2 )
    , angle( angle )
    , m_hasRotAxisInterpolator( false )
{
}

// *************************************
//
template<typename ParamT>
void            CompositeTransform<ParamT>::InitializeDefaultSRT()
{
    auto t =  TimeType( 0.0 );

    for( auto tr : m_transformations )
    {
        delete tr;
    }

    ParamT ctx, cty, ctz;
    ParamT ictx, icty, ictz;
    ParamT sx, sy, sz;
    ParamT angle;
    ParamT rx, ry, rz;
    ParamT tx, ty, tz;

    ParamT::ValueType   v0  = ParamT::ValueType( 0.0 );
    ParamT::ValueType   v1  = ParamT::ValueType( 1.0 );

    ctx.AddKey( t, v0 ); cty.AddKey( t, v0 ); ctz.AddKey( t, v0 );
    sx.AddKey( t, v1 ); sy.AddKey( t, v1 ); sz.AddKey( t, v1 );
    angle.AddKey( t, v0 );
    rx.AddKey( t, v0 ); ry.AddKey( t, v0 ); rz.AddKey( t, v1 );
    tx.AddKey( t, v0 ); ty.AddKey( t, v0 ); tz.AddKey( t, v0 );
    ictx.AddKey( t, v0 ); icty.AddKey( t, v0 ); ictz.AddKey( t, v0 );

    AddTranslationCFwd( ctx, cty, ctz );
    AddTranslation( tx, ty, tz );
    AddRotation( angle, rx, ry, rz );
    AddScale( sx, sy, sz );
    AddTranslationCInv( ictx, icty, ictz );
}

// *************************************
//
template<typename ParamT>
CompositeTransform<ParamT>::~CompositeTransform()
{
    for( auto t : m_transformations )
    {
        delete t;
    }
}

// *************************************
//
template<typename ParamT>
int CompositeTransform<ParamT>::EvalToCBuffer( typename ParamT::TimeT t, char * buf ) const
{
    glm::mat4x4 val = Evaluate( t );

    memcpy( buf, &val, value_size );

    return value_size;
}

// *************************************
//
template<typename ParamT>
void CompositeTransform<ParamT>::AddTranslation( ParamT x0, ParamT x1, ParamT x2 )
{
    m_transformations.push_back( SimpleTransform<ParamT>::CreateTranslation( x0, x1, x2 ) );
}

// *************************************
//
template<typename ParamT>
void CompositeTransform<ParamT>::AddScale( ParamT s0, ParamT s1, ParamT s2 )
{
    m_transformations.push_back( SimpleTransform<ParamT>::CreateScale( s0, s1, s2 ) );
}

// *************************************
//
template<typename ParamT>
void CompositeTransform<ParamT>::AddRotation( ParamT angle, ParamT r0, ParamT r1, ParamT r2 )
{
    m_transformations.push_back( new Rotation<ParamT>( angle, r0, r1, r2 ) );
}

// *************************************
//
template<typename ParamT>
void            CompositeTransform<ParamT>::AddRotation         ( ParamT angle, const Vec3Interpolator & rotAxis )
{
    m_transformations.push_back( new Rotation<ParamT>( angle, rotAxis ) );    
}

// *************************************
//
template<typename ParamT>
void            CompositeTransform<ParamT>::AddTranslationCFwd  ( ParamT x0, ParamT x1, ParamT x2 )
{
    m_transformations.push_back( SimpleTransform<ParamT>::CreateTranslation( x0, x1, x2, TransformKind::fwd_center ) );
}

// *************************************
//
template<typename ParamT>
void            CompositeTransform<ParamT>::AddTranslationCInv  ( ParamT x0, ParamT x1, ParamT x2 )
{
    m_transformations.push_back( SimpleTransform<ParamT>::CreateTranslation( x0, x1, x2, TransformKind::inv_center ) );
}

// *************************************
//
template<typename ParamT>
void CompositeTransform<ParamT>::AddTransform( SimpleTransform<ParamT> * trans )
{
    m_transformations.push_back( trans );
}

// *************************************
//
template<typename ParamT>
CompositeTransform<ParamT>::CompositeTransform()
{
}

// *************************************
//
template<typename ParamT>
CompositeTransform<ParamT>::CompositeTransform  ( const CompositeTransform & src )
{
    for ( auto t : m_transformations )
    {
        delete t;
    }

    m_transformations.clear();

    for ( auto st : src.m_transformations )
    {
        m_transformations.push_back( st->Clone() );
    }
}

// *************************************
//
template<typename ParamT>
unsigned int CompositeTransform<ParamT>::Size() const
{
    return m_transformations.size();
}

// *************************************
//
template<typename ParamT>
SimpleTransform<ParamT> * CompositeTransform<ParamT>::operator[]( unsigned int i )
{
    return m_transformations[ i ];
}

template<typename ParamT>
const SimpleTransform<ParamT> * CompositeTransform<ParamT>::operator[]( unsigned int i ) const
{
    return m_transformations[ i ];
}

// *************************************
//
template<typename ParamT>
glm::mat4x4 CompositeTransform<ParamT>::Evaluate( typename ParamT::TimeT t ) const
{
    glm::mat4x4 ret(1.0f);

    for( unsigned int i = 0; i < m_transformations.size(); ++i )
    {
        ret *= m_transformations[ i ]->Evaluate( t );
    }

    return ret; 
}

// *************************************
//
template<typename ParamT>
glm::mat4x4 SimpleTransform<ParamT>::Evaluate( typename ParamT::TimeT t ) const
{
    switch( kind )
    {
    case TransformKind::translation:
    case TransformKind::fwd_center:
    case TransformKind::inv_center:
        return glm::translate( glm::mat4( 1.0f ), glm::vec3( p0.Evaluate( t ), p1.Evaluate( t ), p2.Evaluate( t ) ) );
    case TransformKind::scale:
        return glm::scale( glm::mat4( 1.0f ), glm::vec3( p0.Evaluate( t ), p1.Evaluate( t ), p2.Evaluate( t ) ) );
    case TransformKind::rotation:
        assert( false );
        return glm::mat4( 1.0f);
    default:
        assert( false );
        return glm::mat4( 1.0f );
    }
}

// *************************************
//
template<typename ParamT>
SimpleTransform<ParamT> * SimpleTransform<ParamT>::Clone() const
{
    return new SimpleTransform( *this );
}

} //bv

//explicit instantiation - this way class' implementation can be stored in cpp file (like here)
template class bv::CompositeTransform<bv::FloatInterpolator>;
