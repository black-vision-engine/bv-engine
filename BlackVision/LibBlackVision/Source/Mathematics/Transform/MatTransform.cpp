#include "Mathematics/Transform/MatTransform.h"

#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"
#include "Serialization/SerializationHelper.h"

#include "UseLoggerLibBlackVision.h"

namespace bv { 

namespace SerializationHelper {

std::pair< TransformKind, const char* > tk2s[] = {
    std::make_pair( TransformKind::fwd_center, "fwd_center" ),
    std::make_pair( TransformKind::inv_center, "inv_center" ),
    std::make_pair( TransformKind::rotation, "rotation" ),
    std::make_pair( TransformKind::scale, "scale" ),
    std::make_pair( TransformKind::translation, "translation" ),
    std::make_pair( TransformKind::invalid, "" ) };


template<> std::string T2String( const TransformKind& tk ) { return Enum2String( tk2s, tk ); }

// *************************************
//
template<> TransformKind String2T( const std::string & s, const TransformKind & defaultVal )
{
    auto transform = String2T( tk2s, s );
    if( transform == TransformKind::invalid )
    {
        return defaultVal;
    }
    return transform;
}

template<> Expected< TransformKind > String2T( const std::string & s ) { return String2T( tk2s, s ); }

} // SerializationHelper

namespace model {

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
SimpleTransform<ParamT>::SimpleTransform( const TransformKind kind, const ParamT p0, const ParamT p1, const ParamT p2 )
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

// *************************************
//
template<typename ParamT>
void                SimpleTransform<ParamT>::Serialize       ( ISerializer& sob ) const
{ 
    assert( !"Tell me why I'm not implemented ;)" );
    sob;
}

// *************************************
//
template<typename ParamT>
ISerializablePtr     SimpleTransform<ParamT>::Create          ( const IDeserializer& dob )
{
    //if( dob.GetName() != "transform" )
    //{
    //    std::cerr << "SimpleTransform<ParamT>::Create failed" << std::endl;
    //    return nullptr; // FIXME so much: error handling
    //}

    auto kindName = dob.GetAttribute( "kind" );
    auto kind = SerializationHelper::String2T< TransformKind >( kindName );

    if( kind == TransformKind::rotation ) // very special case indeed :)
    {
        auto angleArray = SerializationHelper::DeserializeArray< ParamT >( dob, "angle", "interpolator" );
        auto rotAxisArray = SerializationHelper::DeserializeArray< Vec3Interpolator >( dob, "rotaxis", "interpolator" );

        if( angleArray.size() != 1 )
        {
            LOG_MESSAGE( SeverityLevel::error ) << "CompositeTransform<ParamT>::Create failed, incorrect entry for angle in rotation";
            return SimpleTransform< ParamT >::CreateDefaultTransform( TransformKind::rotation );
        }
        if( rotAxisArray.size() != 1 )
        {
            LOG_MESSAGE( SeverityLevel::error ) << "CompositeTransform<ParamT>::Create failed, incorrect entry for rotAxis in rotation";
            return SimpleTransform< ParamT >::CreateDefaultTransform( TransformKind::rotation );
        }

        auto angle = angleArray[0];
        auto rotAxis = rotAxisArray[0];

        return std::make_shared< Rotation< ParamT > >( *angle, *rotAxis );
    }

    auto params = SerializationHelper::DeserializeArray< ParamT >( dob, "interpolators" );
    
    if( params.size() != 3 && ( kind != TransformKind::rotation || params.size() != 2 ) ) // de Morgan FTW!
    {
        LOG_MESSAGE( SeverityLevel::error ) << "CompositeTransform<ParamT>::Create failed, incorrect number of interpolators in " << kindName;
        return SimpleTransform< ParamT >::CreateDefaultTransform( kind );
    }

    if( kind == TransformKind::fwd_center )
        return std::make_shared< SimpleTransform< ParamT > >( TransformKind::fwd_center, *params[0], *params[1], *params[2] );
    else if( kind == TransformKind::translation )
        return std::make_shared< SimpleTransform< ParamT > >( TransformKind::translation, *params[0], *params[1], *params[2] );
    else if( kind == TransformKind::scale )
        return std::make_shared< SimpleTransform< ParamT > >( TransformKind::scale, *params[0], *params[1], *params[2] );
    else if( kind == TransformKind::inv_center )
        return std::make_shared< SimpleTransform< ParamT > >( TransformKind::inv_center, *params[0], *params[1], *params[2] );

    assert( false );
    return nullptr;
}

// *************************************
//
template<typename ParamT>
std::shared_ptr< SimpleTransform< ParamT > >    SimpleTransform<ParamT>::CreateDefaultTransform( TransformKind kind )
{
    auto t =  TimeType( 0.0 );

    ParamT::ValueType   v0  = ParamT::ValueType( 0.0 );
    ParamT::ValueType   v1  = ParamT::ValueType( 1.0 );


    if( kind == TransformKind::fwd_center )
    {
        ParamT ctx, cty, ctz;

        ctx.AddKey( t, v0 ); cty.AddKey( t, v0 ); ctz.AddKey( t, v0 );

        return CreateTranslation( ctx, cty, ctz, kind );
    } 
    else if( kind == TransformKind::translation )
    {
        ParamT tx, ty, tz;

        tx.AddKey( t, v0 ); ty.AddKey( t, v0 ); tz.AddKey( t, v0 );

        return CreateTranslation( tx, ty, tz, kind );
    }
    else if( kind == TransformKind::rotation )
    {
        ParamT angle;
        Vec3Interpolator rotAxis;

        angle.AddKey( t, v0 );
        rotAxis.AddKey( t, glm::vec3( v0, v0, v1 ) );

        return std::make_shared< Rotation< ParamT > >( angle, rotAxis );
    }
    else if( kind == TransformKind::scale )
    {
        ParamT sx, sy, sz;

        sx.AddKey( t, v1 ); sy.AddKey( t, v1 ); sz.AddKey( t, v1 );

        return CreateScale( sx, sy, sz );
    }
    else if( kind == TransformKind::inv_center )
    {
        ParamT ictx, icty, ictz;

        ictx.AddKey( t, v0 ); icty.AddKey( t, v0 ); ictz.AddKey( t, v0 );

        return CreateTranslation( ictx, icty, ictz, kind );
    }

    assert( false );
    return nullptr;
}

// *************************************
//
template<typename ParamT>
void SimpleTransform<ParamT>::SetCurveType    ( CurveType type ) 
{ 
    p0.SetCurveType( type ); 
    p1.SetCurveType( type ); 
    p2.SetCurveType( type ); 
}

// *************************************
//
template<typename ParamT>
void                SimpleTransform<ParamT>::SetWrapPostMethod   ( WrapMethod method )
{
    p0.SetWrapPostMethod( method );
    p1.SetWrapPostMethod( method );
    p1.SetWrapPostMethod( method );
}

// *************************************
//
template<typename ParamT>
void                SimpleTransform<ParamT>::SetWrapPreMethod    ( WrapMethod method )
{
    p0.SetWrapPreMethod( method );
    p1.SetWrapPreMethod( method );
    p1.SetWrapPreMethod( method );
}


template<typename ParamT>
Rotation<ParamT>::Rotation    ( ParamT angle, const Vec3Interpolator & rotAxis )
    : SimpleTransform( TransformKind::rotation )
    , m_angle( angle )
    , m_hasRotAxisInterpolator( true )
    , m_rotationAxis( rotAxis ) 
{
}

// *************************************
//
template<typename ParamT>
Rotation<ParamT>::Rotation( ParamT angle, ParamT p0, ParamT p1, ParamT p2 )
    : SimpleTransform( TransformKind::rotation, p0, p1, p2 )
    , m_angle( angle )
    , m_hasRotAxisInterpolator( false )
{
}

// *************************************
//
template<typename ParamT>
void Rotation<ParamT>::SetCurveType( CurveType type )
{
    m_angle.SetCurveType( type );
    m_rotationAxis.SetCurveType( type );
}

// *************************************
//
template<typename ParamT>
void                Rotation<ParamT>::SetWrapPostMethod   ( WrapMethod method )
{
    m_angle.SetWrapPostMethod( method );
    m_rotationAxis.SetWrapPostMethod( method );
}

// *************************************
//
template<typename ParamT>
void                Rotation<ParamT>::SetWrapPreMethod    ( WrapMethod method )
{
    m_angle.SetWrapPreMethod( method );
    m_rotationAxis.SetWrapPreMethod( method );
}

// *************************************
//
template<typename ParamT>
void            CompositeTransform<ParamT>::InitializeDefaultSRT()
{
    auto t =  TimeType( 0.0 );

    ParamT ctx, cty, ctz;
    ParamT ictx, icty, ictz;
    ParamT sx, sy, sz;
    ParamT angle;
    ParamT tx, ty, tz;
    Vec3Interpolator rotAxis;

    ParamT::ValueType   v0  = ParamT::ValueType( 0.0 );
    ParamT::ValueType   v1  = ParamT::ValueType( 1.0 );

    ctx.AddKey( t, v0 ); cty.AddKey( t, v0 ); ctz.AddKey( t, v0 );
    sx.AddKey( t, v1 ); sy.AddKey( t, v1 ); sz.AddKey( t, v1 );
    angle.AddKey( t, v0 );
    rotAxis.AddKey( t, glm::vec3( v0, v0, v1 ) );
    tx.AddKey( t, v0 ); ty.AddKey( t, v0 ); tz.AddKey( t, v0 );
    ictx.AddKey( t, v0 ); icty.AddKey( t, v0 ); ictz.AddKey( t, v0 );

    AddTranslationCFwd( ctx, cty, ctz );
    AddTranslation( tx, ty, tz );
    AddRotation( angle, rotAxis );
    AddScale( sx, sy, sz );
    AddTranslationCInv( ictx, icty, ictz );
}

// *************************************
//
template<typename ParamT>
CompositeTransform<ParamT>::~CompositeTransform()
{
}

// *************************************
//
template<typename ParamT>
void CompositeTransform<ParamT>::SetCurveType        ( CurveType type )  
{ 
    for( auto& trans : m_transformations ) 
        trans->SetCurveType( type ); 
}

// *************************************
//
template<typename ParamT>
void            CompositeTransform<ParamT>::SetWrapPostMethod   ( WrapMethod method )
{
    for( auto& trans : m_transformations ) 
        trans->SetWrapPostMethod( method );
}

// *************************************
//
template<typename ParamT>
void            CompositeTransform<ParamT>::SetWrapPreMethod    ( WrapMethod method )
{
    for( auto& trans : m_transformations ) 
        trans->SetWrapPreMethod( method );
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
    m_transformations.push_back( std::make_shared< Rotation<ParamT> >( angle, r0, r1, r2 ) );
}

// *************************************
//
template<typename ParamT>
void            CompositeTransform<ParamT>::AddRotation         ( ParamT angle, const Vec3Interpolator & rotAxis )
{
    m_transformations.push_back( std::make_shared< Rotation< ParamT > >( angle, rotAxis ) );    
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
void CompositeTransform<ParamT>::AddTransform( std::shared_ptr< SimpleTransform<ParamT> > trans )
{
    m_transformations.push_back( trans );
}

// *************************************
//
template<typename ParamT>
void CompositeTransform<ParamT>::InsertTransform     ( int i, std::shared_ptr< SimpleTransform<ParamT> > trans )
{
    if( i < (int)m_transformations.size() )
    {
        auto it = m_transformations.begin();
        m_transformations.insert( it + i - 1, trans );
    }
    else if( i == (int)m_transformations.size() )
        AddTransform( trans );
    else
        assert( false );
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
    m_transformations.clear();

    for ( auto st : src.m_transformations )
    {
        m_transformations.push_back( st );
    }
}

// *************************************
//
template<typename ParamT>
CompositeTransform< ParamT >*                     CompositeTransform<ParamT>::Create                  ( const IDeserializer& dob )
{
    auto transform = new CompositeTransform< ParamT >();

    auto transes = SerializationHelper::DeserializeProperties< SimpleTransform< ParamT > >( dob, "transform" );

    int i = 0;
    for( auto trans : transes )
    {
        transform->InsertTransform( i, trans );
        i++;
    }

    return transform;
}

// *************************************
//
template<typename ParamT>
void                                CompositeTransform<ParamT>::Serialize               ( ISerializer& ser ) const
{
    ser.EnterArray( "composite_transform" );

    for( auto trans : m_transformations )
    {
        ser.EnterChild( "transform" );
        SerializationHelper::SerializeAttribute( ser, trans->KindKurwaMac(), "kind" );

        if( trans->KindKurwaMac() == TransformKind::rotation ) // FIXME: this really should be virtualized
        {
            auto rotation = std::static_pointer_cast< Rotation< ParamT > >( trans );
            if( rotation->IsAxisVec3() )
            {
                ser.SetAttribute( "isaxisvec3", "true" );

                ser.EnterChild( "angle" );
                    rotation->AccessAngle().Serialize( ser );
                ser.ExitChild();

                ser.EnterChild( "rotaxis" );
                    rotation->AccessRotAxis().Serialize( ser );
                ser.ExitChild();
            }
            else
                assert( false );
        }
        else
        {
            ser.EnterArray( "interpolators" );
            
            trans->GetP0MotylaNoga().Serialize( ser );
            trans->GetP1MotylaNoga().Serialize( ser );
            trans->GetP2MotylaNoga().Serialize( ser );

            ser.ExitChild();
        }

        ser.ExitChild(); // transform
    }

    ser.ExitChild(); // composite_transform
}

// *************************************
//
template<typename ParamT>
SizeType    CompositeTransform<ParamT>::Size() const
{
    return m_transformations.size();
}

// *************************************
//
template<typename ParamT>
SimpleTransform<ParamT> * CompositeTransform<ParamT>::operator[]( unsigned int i )
{
    return m_transformations[ i ].get();
}

template<typename ParamT>
const SimpleTransform<ParamT> * CompositeTransform<ParamT>::operator[]( unsigned int i ) const
{
    return m_transformations[ i ].get();
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
    //return new SimpleTransform( *this );
    return new SimpleTransform( kind, p0, p1, p2 );
}

} //bv

//explicit instantiation - this way class' implementation can be stored in cpp file (like here)
template class bv::CompositeTransform<bv::FloatInterpolator>;
