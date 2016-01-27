#include "Mathematics/Transform/MatTransform.h"

#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"
#include "Serialization/SerializationHelper.h"

#include "UseLoggerLibBlackVision.h"

namespace bv { 

namespace SerializationHelper {

std::pair< TransformKind, const char* > tk2s[] = {
    std::make_pair( TransformKind::fwd_center, "fwd_center" ),
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

//// *************************************
////
//template<typename ParamT>
//SimpleTransform<ParamT>::SimpleTransform( const TransformKind kind, const ParamT p0, const ParamT p1, const ParamT p2 )
//    : kind( kind ), p0( p0 ), p1( p1 ), p2 (p2 )
//{
//}
//
//// *************************************
////
//template<typename ParamT>
//SimpleTransform<ParamT>::SimpleTransform( TransformKind kind )
//    : kind( kind )
//{
//}
//
//// *************************************
////
//template<typename ParamT>
//void                SimpleTransform<ParamT>::Serialize       ( ISerializer& sob ) const
//{ 
//    assert( !"Tell me why I'm not implemented ;)" );
//    sob;
//}
//
//// *************************************
////
//template<typename ParamT>
//ISerializablePtr     SimpleTransform<ParamT>::Create          ( const IDeserializer& dob )
//{
//    //if( dob.GetName() != "transform" )
//    //{
//    //    std::cerr << "SimpleTransform<ParamT>::Create failed" << std::endl;
//    //    return nullptr; // FIXME so much: error handling
//    //}
//
//    auto kindName = dob.GetAttribute( "kind" );
//    auto kind = SerializationHelper::String2T< TransformKind >( kindName );
//
//    if( kind == TransformKind::rotation ) // very special case indeed :)
//    {
//        auto angleArray = SerializationHelper::DeserializeArray< ParamT >( dob, "angle", "interpolator" );
//        auto rotAxisArray = SerializationHelper::DeserializeArray< Vec3Interpolator >( dob, "rotaxis", "interpolator" );
//
//        if( angleArray.size() != 1 )
//        {
//            LOG_MESSAGE( SeverityLevel::error ) << "CompositeTransform<ParamT>::Create failed, incorrect entry for angle in rotation";
//            return SimpleTransform< ParamT >::CreateDefaultTransform( TransformKind::rotation );
//        }
//        if( rotAxisArray.size() != 1 )
//        {
//            LOG_MESSAGE( SeverityLevel::error ) << "CompositeTransform<ParamT>::Create failed, incorrect entry for rotAxis in rotation";
//            return SimpleTransform< ParamT >::CreateDefaultTransform( TransformKind::rotation );
//        }
//
//        auto angle = angleArray[0];
//        auto rotAxis = rotAxisArray[0];
//
//        return std::make_shared< Rotation< ParamT > >( *angle, *rotAxis );
//    }
//
//    auto params = SerializationHelper::DeserializeArray< ParamT >( dob, "interpolators" );
//    
//    if( params.size() != 3 && ( kind != TransformKind::rotation || params.size() != 2 ) ) // de Morgan FTW!
//    {
//        LOG_MESSAGE( SeverityLevel::error ) << "CompositeTransform<ParamT>::Create failed, incorrect number of interpolators in " << kindName;
//        return SimpleTransform< ParamT >::CreateDefaultTransform( kind );
//    }
//
//    if( kind == TransformKind::fwd_center )
//        return std::make_shared< SimpleTransform< ParamT > >( TransformKind::fwd_center, *params[0], *params[1], *params[2] );
//    else if( kind == TransformKind::translation )
//        return std::make_shared< SimpleTransform< ParamT > >( TransformKind::translation, *params[0], *params[1], *params[2] );
//    else if( kind == TransformKind::scale )
//        return std::make_shared< SimpleTransform< ParamT > >( TransformKind::scale, *params[0], *params[1], *params[2] );
//    else if( kind == TransformKind::inv_center )
//        return std::make_shared< SimpleTransform< ParamT > >( TransformKind::inv_center, *params[0], *params[1], *params[2] );
//
//    assert( false );
//    return nullptr;
//}
//
//// *************************************
////
//template<typename ParamT>
//std::shared_ptr< SimpleTransform< ParamT > >    SimpleTransform<ParamT>::CreateDefaultTransform( TransformKind kind )
//{
//    auto t =  TimeType( 0.0 );
//
//    ParamT::ValueType   v0  = ParamT::ValueType( 0.0 );
//    ParamT::ValueType   v1  = ParamT::ValueType( 1.0 );
//
//
//    if( kind == TransformKind::fwd_center )
//    {
//        ParamT ctx, cty, ctz;
//
//        ctx.AddKey( t, v0 ); cty.AddKey( t, v0 ); ctz.AddKey( t, v0 );
//
//        return CreateTranslation( ctx, cty, ctz, kind );
//    } 
//    else if( kind == TransformKind::translation )
//    {
//        ParamT tx, ty, tz;
//
//        tx.AddKey( t, v0 ); ty.AddKey( t, v0 ); tz.AddKey( t, v0 );
//
//        return CreateTranslation( tx, ty, tz, kind );
//    }
//    else if( kind == TransformKind::rotation )
//    {
//        ParamT angle;
//        Vec3Interpolator rotAxis;
//
//        angle.AddKey( t, v0 );
//        rotAxis.AddKey( t, glm::vec3( v0, v0, v1 ) );
//
//        return std::make_shared< Rotation< ParamT > >( angle, rotAxis );
//    }
//    else if( kind == TransformKind::scale )
//    {
//        ParamT sx, sy, sz;
//
//        sx.AddKey( t, v1 ); sy.AddKey( t, v1 ); sz.AddKey( t, v1 );
//
//        return CreateScale( sx, sy, sz );
//    }
//    else if( kind == TransformKind::inv_center )
//    {
//        ParamT ictx, icty, ictz;
//
//        ictx.AddKey( t, v0 ); icty.AddKey( t, v0 ); ictz.AddKey( t, v0 );
//
//        return CreateTranslation( ictx, icty, ictz, kind );
//    }
//
//    assert( false );
//    return nullptr;
//}
//
//template<typename ParamT>
//Rotation<ParamT>::Rotation    ( ParamT angle, const Vec3Interpolator & rotAxis )
//    : SimpleTransform( TransformKind::rotation )
//    , m_angle( angle )
//    , m_hasRotAxisInterpolator( true )
//    , m_rotationAxis( rotAxis ) 
//{
//}
//
//// *************************************
////
//template<typename ParamT>
//Rotation<ParamT>::Rotation( ParamT angle, ParamT p0, ParamT p1, ParamT p2 )
//    : SimpleTransform( TransformKind::rotation, p0, p1, p2 )
//    , m_angle( angle )
//    , m_hasRotAxisInterpolator( false )
//{
//}
//
//// *************************************
////
//template<typename ParamT>
//void Rotation<ParamT>::SetCurveType( CurveType type )
//{
//    m_angle.SetCurveType( type );
//    m_rotationAxis.SetCurveType( type );
//}
//
//// *************************************
////
//template<typename ParamT>
//void                Rotation<ParamT>::SetWrapPostMethod   ( WrapMethod method )
//{
//    m_angle.SetWrapPostMethod( method );
//    m_rotationAxis.SetWrapPostMethod( method );
//}
//
//// *************************************
////
//template<typename ParamT>
//void                Rotation<ParamT>::SetWrapPreMethod    ( WrapMethod method )
//{
//    m_angle.SetWrapPreMethod( method );
//    m_rotationAxis.SetWrapPreMethod( method );
//}

// *************************************
//
void            CompositeTransform::InitializeDefaultSRT()
{
    auto t =  TimeType( 0.0 );

    auto v0 = 0.0f;
    auto v1 = 1.0f;

    SetTranslation( glm::vec3( v0, v0, v0 ), t );
    SetRotation( glm::vec3( v0, v0, v1 ), v0, t );
    SetScale( glm::vec3( v1, v1, v1 ), t );
    SetCenter( glm::vec3( v0, v0, v0 ), t );
}

// *************************************
//
CompositeTransform::~CompositeTransform()
{
}

// *************************************
//
void            CompositeTransform::SetCurveType        ( CurveType type )  
{ 
    m_translationX.SetCurveType( type );  m_translationY.SetCurveType( type );  m_translationZ.SetCurveType( type ); 
    m_rotationAxis.SetCurveType( type );  m_rotationAngle.SetCurveType( type ); 
    m_scaleX.SetCurveType( type );  m_scaleY.SetCurveType( type );  m_scaleZ.SetCurveType( type ); 
    m_centerX.SetCurveType( type );  m_centerY.SetCurveType( type );  m_centerZ.SetCurveType( type ); 
}

// *************************************
//
void            CompositeTransform::SetWrapPostMethod   ( WrapMethod method )
{
    m_translationX.SetWrapPostMethod( method );  m_translationY.SetWrapPostMethod( method );  m_translationZ.SetWrapPostMethod( method ); 
    m_rotationAxis.SetWrapPostMethod( method );  m_rotationAngle.SetWrapPostMethod( method ); 
    m_scaleX.SetWrapPostMethod( method );  m_scaleY.SetWrapPostMethod( method );  m_scaleZ.SetWrapPostMethod( method ); 
    m_centerX.SetWrapPostMethod( method );  m_centerY.SetWrapPostMethod( method );  m_centerZ.SetWrapPostMethod( method ); 
}

// *************************************
//
void            CompositeTransform::SetWrapPreMethod    ( WrapMethod method )
{
    m_translationX.SetWrapPreMethod( method );  m_translationY.SetWrapPreMethod( method );  m_translationZ.SetWrapPreMethod( method ); 
    m_rotationAxis.SetWrapPreMethod( method );  m_rotationAngle.SetWrapPreMethod( method ); 
    m_scaleX.SetWrapPreMethod( method );  m_scaleY.SetWrapPreMethod( method );  m_scaleZ.SetWrapPreMethod( method ); 
    m_centerX.SetWrapPreMethod( method );  m_centerY.SetWrapPreMethod( method );  m_centerZ.SetWrapPreMethod( method ); 
}

// *************************************
//
glm::vec3       CompositeTransform::GetTranslation      ( TimeType time ) const
{
    return glm::vec3( m_translationX.Evaluate( time ), m_translationY.Evaluate( time ), m_translationZ.Evaluate( time ) );
}

// *************************************
//
Float32         CompositeTransform::GetRotationAngle    ( TimeType time ) const
{
    return m_rotationAngle.Evaluate( time );
}

// *************************************
//
glm::vec3       CompositeTransform::GetRotationAxis     ( TimeType time ) const
{
    return m_rotationAxis.Evaluate( time );
}

// *************************************
//
glm::vec3       CompositeTransform::GetScale            ( TimeType time ) const
{
    return glm::vec3( m_scaleX.Evaluate( time ), m_scaleY.Evaluate( time ), m_scaleZ.Evaluate( time ) );
}

// *************************************
//
glm::vec3       CompositeTransform::GetCenter           ( TimeType time ) const
{
    return glm::vec3( m_centerX.Evaluate( time ), m_centerY.Evaluate( time ), m_centerZ.Evaluate( time ) );
}

// *************************************
//
void            CompositeTransform::SetTranslation      ( const glm::vec3 & vec, TimeType time )
{
    m_translationX.AddKey( time, vec.x );
    m_translationY.AddKey( time, vec.y );
    m_translationZ.AddKey( time, vec.z );
}

// *************************************
//
void            CompositeTransform::SetRotation         ( const glm::vec3 & axis, Float32 angle, TimeType time )
{
    m_rotationAxis.AddKey( time, axis );
    m_rotationAngle.AddKey( time, angle );
}

// *************************************
//
void            CompositeTransform::SetScale            ( const glm::vec3 & vec, TimeType time )
{
    m_scaleX.AddKey( time, vec.x );
    m_scaleY.AddKey( time, vec.y );
    m_scaleZ.AddKey( time, vec.z );
}

// *************************************
//
void            CompositeTransform::SetCenter            ( const glm::vec3 & vec, TimeType time )
{
    m_centerX.AddKey( time, vec.x );
    m_centerY.AddKey( time, vec.y );
    m_centerZ.AddKey( time, vec.z );
}

// *************************************
//
void            CompositeTransform::RemoveTranslation   ( TimeType time )
{
    m_translationX.RemoveKey( time );
    m_translationY.RemoveKey( time );
    m_translationZ.RemoveKey( time );
}

// *************************************
//
void            CompositeTransform::RemoveRotation      ( TimeType time )
{
    m_rotationAxis.RemoveKey( time );
    m_rotationAngle.RemoveKey( time );
}

// *************************************
//
void            CompositeTransform::RemoveScale         ( TimeType time )
{
    m_scaleX.RemoveKey( time );
    m_scaleY.RemoveKey( time );
    m_scaleZ.RemoveKey( time );
}

// *************************************
//
void            CompositeTransform::RemoveCenter         ( TimeType time )
{
    m_centerX.RemoveKey( time );
    m_centerY.RemoveKey( time );
    m_centerZ.RemoveKey( time );
}

// *************************************
//
CompositeTransform::CompositeTransform      ()
{
}

// *************************************
//
CompositeTransform *    CompositeTransform::Create      ( const IDeserializer & dob )
{
    { dob; }

    auto transform = new CompositeTransform();

    //auto transes = SerializationHelper::DeserializeProperties< SimpleTransform< ParamT > >( dob, "transform" );

    //int i = 0;
    //for( auto trans : transes )
    //{
    //    transform->InsertTransform( i, trans );
    //    i++;
    //}

    return transform;
}

// *************************************
//
void                                CompositeTransform::Serialize               ( ISerializer & ser ) const
{
    { ser; }

    ser.EnterArray( "composite_transform" );

    //for( auto trans : m_transformations )
    //{
    //    ser.EnterChild( "transform" );
    //    SerializationHelper::SerializeAttribute( ser, trans->KindKurwaMac(), "kind" );

    //    if( trans->KindKurwaMac() == TransformKind::rotation ) // FIXME: this really should be virtualized
    //    {
    //        auto rotation = std::static_pointer_cast< Rotation< ParamT > >( trans );
    //        if( rotation->IsAxisVec3() )
    //        {
    //            ser.SetAttribute( "isaxisvec3", "true" );

    //            ser.EnterChild( "angle" );
    //                rotation->AccessAngle().Serialize( ser );
    //            ser.ExitChild();

    //            ser.EnterChild( "rotaxis" );
    //                rotation->AccessRotAxis().Serialize( ser );
    //            ser.ExitChild();
    //        }
    //        else
    //            assert( false );
    //    }
    //    else
    //    {
    //        ser.EnterArray( "interpolators" );
    //        
    //        trans->GetP0MotylaNoga().Serialize( ser );
    //        trans->GetP1MotylaNoga().Serialize( ser );
    //        trans->GetP2MotylaNoga().Serialize( ser );

    //        ser.ExitChild();
    //    }

    //    ser.ExitChild(); // transform
    //}

    ser.ExitChild(); // composite_transform
}

// *************************************
//
glm::mat4x4         CompositeTransform::Evaluate        ( TimeType t ) const
{
    glm::mat4x4 ret( 1.0f );

    ret *= glm::translate( glm::mat4( 1.0f ), GetCenter( t ) );
    ret *= glm::translate( glm::mat4( 1.0f ), GetTranslation( t ) );
    ret *= glm::rotate( glm::mat4( 1.0f ), GetRotationAngle( t ), GetRotationAxis( t ) );
    ret *= glm::scale( glm::mat4( 1.0f ), GetScale( t ) );
    ret *= glm::translate( glm::mat4( 1.0f ), -GetCenter( t ) );

    return ret; 
}

//// *************************************
////
//template<typename ParamT>
//glm::mat4x4 SimpleTransform<ParamT>::Evaluate( typename ParamT::TimeT t ) const
//{
//    switch( kind )
//    {
//    case TransformKind::translation:
//    case TransformKind::fwd_center:
//    case TransformKind::inv_center:
//        return glm::translate( glm::mat4( 1.0f ), glm::vec3( p0.Evaluate( t ), p1.Evaluate( t ), p2.Evaluate( t ) ) );
//    case TransformKind::scale:
//        return glm::scale( glm::mat4( 1.0f ), glm::vec3( p0.Evaluate( t ), p1.Evaluate( t ), p2.Evaluate( t ) ) );
//    case TransformKind::rotation:
//        assert( false );
//        return glm::mat4( 1.0f);
//    default:
//        assert( false );
//        return glm::mat4( 1.0f );
//    }
//}
//
//// *************************************
////
//template<typename ParamT>
//SimpleTransform<ParamT> * SimpleTransform<ParamT>::Clone() const
//{
//    //return new SimpleTransform( *this );
//    return new SimpleTransform( kind, p0, p1, p2 );
//}

} //bv