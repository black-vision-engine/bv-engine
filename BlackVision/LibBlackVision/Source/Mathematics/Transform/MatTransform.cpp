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


// ******************* Transform **************** 

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


// ******************* CompositeTransform **************** 

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
void        CompositeTransform::SetAddedKeyCurveType    ( CurveType type )
{
    m_translationX.SetAddedKeyCurveType( type );  m_translationY.SetAddedKeyCurveType( type );  m_translationZ.SetAddedKeyCurveType( type ); 
    m_rotationAxis.SetAddedKeyCurveType( type );  m_rotationAngle.SetAddedKeyCurveType( type ); 
    m_scaleX.SetAddedKeyCurveType( type );  m_scaleY.SetAddedKeyCurveType( type );  m_scaleZ.SetAddedKeyCurveType( type ); 
    m_centerX.SetAddedKeyCurveType( type );  m_centerY.SetAddedKeyCurveType( type );  m_centerZ.SetAddedKeyCurveType( type ); 
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
bool            CompositeTransform::MoveTranslation     ( TimeType time, TimeType newTime )
{
    bool result = true;

    result &= m_translationX.MoveKey( time, newTime );
    result &= m_translationY.MoveKey( time, newTime );
    result &= m_translationZ.MoveKey( time, newTime );
        
    return result;
}

// *************************************
//
bool            CompositeTransform::MoveRotation        ( TimeType time, TimeType newTime )
{
    bool result = true;

    result &= m_rotationAxis.MoveKey( time, newTime );
    result &= m_rotationAngle.MoveKey( time, newTime );
        
    return result;
}

// *************************************
//
bool            CompositeTransform::MoveScale           ( TimeType time, TimeType newTime )
{
    bool result = true;

    result &= m_scaleX.MoveKey( time, newTime );
    result &= m_scaleY.MoveKey( time, newTime );
    result &= m_scaleZ.MoveKey( time, newTime );
        
    return result;
}

// *************************************
//
bool            CompositeTransform::MoveCenter          ( TimeType time, TimeType newTime )
{
    bool result = true;

    result &= m_centerX.MoveKey( time, newTime );
    result &= m_centerY.MoveKey( time, newTime );
    result &= m_centerZ.MoveKey( time, newTime );
        
    return result;
}

// *************************************
//
CompositeTransform::CompositeTransform      ()
{
    InitializeDefaultSRT();
}

// *************************************
//
CompositeTransform *    CompositeTransform::Create      ( const IDeserializer & deser )
{
    //maintain serialization backward compatibility

    auto transform = new CompositeTransform();

    if( deser.EnterChild( "transform" ) )
    {
        do
        {
            auto kindName = deser.GetAttribute( "kind" );
            auto kind = SerializationHelper::String2T< TransformKind >( kindName );

            if( kind == TransformKind::rotation )
            {
                auto angleArray = SerializationHelper::DeserializeArray< FloatInterpolator >( deser, "angle", "interpolator" );
                auto rotAxisArray = SerializationHelper::DeserializeArray< Vec3Interpolator >( deser, "rotaxis", "interpolator" );

                if( angleArray.size() != 1 )
                {
                    //LOG_MESSAGE( SeverityLevel::error ) << "CompositeTransform::Create failed, incorrect entry for angle in rotation";
                }
                else if( rotAxisArray.size() != 1 )
                {
                    //LOG_MESSAGE( SeverityLevel::error ) << "CompositeTransform::Create failed, incorrect entry for rotAxis in rotation";
                }
                else
                {
                    transform->m_rotationAngle = *angleArray[ 0 ];
                    transform->m_rotationAxis = *rotAxisArray[ 0 ];
                }
            }
            else
            {
                auto params = SerializationHelper::DeserializeArray< FloatInterpolator >( deser, "interpolators" );
                if( params.size() == 3 )
                {
                    if( kind == TransformKind::fwd_center )
                    {
                        transform->m_centerX = *params[ 0 ];
                        transform->m_centerY = *params[ 1 ];
                        transform->m_centerZ = *params[ 2 ];
                    }
                    else if( kind == TransformKind::translation )
                    {
                        transform->m_translationX = *params[ 0 ];
                        transform->m_translationY = *params[ 1 ];
                        transform->m_translationZ = *params[ 2 ];
                    }
                    else if( kind == TransformKind::scale )
                    {
                        transform->m_scaleX = *params[ 0 ];
                        transform->m_scaleY = *params[ 1 ];
                        transform->m_scaleZ = *params[ 2 ];
                    }
                }
                else
                {
                    //LOG_MESSAGE( SeverityLevel::error ) << "CompositeTransform::Create failed, incorrect number of interpolators in " << kindName;
                }
            }
        } while( deser.NextChild() );

        deser.ExitChild();
    }

    return transform;
}

// *************************************
//
void                                CompositeTransform::Serialize               ( ISerializer & ser ) const
{
    //maintain serialization backward compatibility

    ser.EnterArray( "composite_transform" );


    //center
    ser.EnterChild( "transform" );
    SerializationHelper::SerializeAttribute( ser, TransformKind::fwd_center, "kind" );
    ser.EnterArray( "interpolators" );

    m_centerX.Serialize( ser );
    m_centerY.Serialize( ser );
    m_centerZ.Serialize( ser );

    ser.ExitChild(); // interpolators
    ser.ExitChild(); // transform
    

    //translation
    ser.EnterChild( "transform" );
    SerializationHelper::SerializeAttribute( ser, TransformKind::translation, "kind" );
    ser.EnterArray( "interpolators" );

    m_translationX.Serialize( ser );
    m_translationY.Serialize( ser );
    m_translationZ.Serialize( ser );

    ser.ExitChild(); // interpolators
    ser.ExitChild(); // transform


    //rotation
    ser.EnterChild( "transform" );
    SerializationHelper::SerializeAttribute( ser, TransformKind::rotation, "kind" );
    ser.SetAttribute( "isaxisvec3", "true" ); //?

    ser.EnterChild( "angle" );
    m_rotationAngle.Serialize( ser );
    ser.ExitChild();

    ser.EnterChild( "rotaxis" );
    m_rotationAxis.Serialize( ser );
    ser.ExitChild();

    ser.ExitChild(); // transform


    //scale
    ser.EnterChild( "transform" );
    SerializationHelper::SerializeAttribute( ser, TransformKind::scale, "kind" );
    ser.EnterArray( "interpolators" );

    m_scaleX.Serialize( ser );
    m_scaleY.Serialize( ser );
    m_scaleZ.Serialize( ser );

    ser.ExitChild(); // interpolators
    ser.ExitChild(); // transform


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

} //bv