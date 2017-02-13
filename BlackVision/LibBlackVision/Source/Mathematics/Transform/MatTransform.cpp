#include "stdafx.h"

#include "UseLoggerLibBlackVision.h"

#include "Mathematics/Transform/MatTransform.h"

#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"

#include "Mathematics/Interpolators/CompositeInterpolator.inl"

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"




#include "Memory/MemoryLeaks.h"



namespace bv { 

namespace SerializationHelper {

std::pair< TransformKind, const char* > tk2s[] = {
    std::make_pair( TransformKind::center, "center" ),
    std::make_pair( TransformKind::rotation, "rotation" ),
    std::make_pair( TransformKind::scale, "scale" ),
    std::make_pair( TransformKind::translation, "translation" ),
    std::make_pair( TransformKind::invalid, "" ) };


template<> std::string T2String( const TransformKind& tk ) { return Enum2String( tk2s, tk ); }

// *************************************
//
template<> TransformKind String2T( const std::string & s, const TransformKind & defaultVal )
{
    auto transform = String2Enum( tk2s, s );
    if( transform == TransformKind::invalid )
    {
        return defaultVal;
    }
    return transform;
}

template<> Expected< TransformKind > String2T( const std::string & s ) { return String2Enum( tk2s, s ); }

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
    SetRotation( glm::vec3( v0, v0, v0 ), t );
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
void            CompositeTransform::SetGlobalCurveType        ( CurveType type )  
{ 
    m_translationX.SetGlobalCurveType( type );  m_translationY.SetGlobalCurveType( type );  m_translationZ.SetGlobalCurveType( type ); 
    m_eulerPitch.SetGlobalCurveType( type );  m_eulerYaw.SetGlobalCurveType( type ); m_eulerRoll.SetGlobalCurveType( type ); 
    m_scaleX.SetGlobalCurveType( type );  m_scaleY.SetGlobalCurveType( type );  m_scaleZ.SetGlobalCurveType( type ); 
    m_centerX.SetGlobalCurveType( type );  m_centerY.SetGlobalCurveType( type );  m_centerZ.SetGlobalCurveType( type ); 
}

// *************************************
//
void        CompositeTransform::SetAddedKeyCurveType    ( CurveType type )
{
    m_translationX.SetAddedKeyCurveType( type );  m_translationY.SetAddedKeyCurveType( type );  m_translationZ.SetAddedKeyCurveType( type ); 
    m_eulerPitch.SetAddedKeyCurveType( type ); m_eulerYaw.SetAddedKeyCurveType( type ); m_eulerRoll.SetAddedKeyCurveType( type ); 
    m_scaleX.SetAddedKeyCurveType( type );  m_scaleY.SetAddedKeyCurveType( type );  m_scaleZ.SetAddedKeyCurveType( type ); 
    m_centerX.SetAddedKeyCurveType( type );  m_centerY.SetAddedKeyCurveType( type );  m_centerZ.SetAddedKeyCurveType( type ); 
}

// *************************************
//
void            CompositeTransform::SetWrapPostMethod   ( WrapMethod method )
{
    m_translationX.SetWrapPostMethod( method );  m_translationY.SetWrapPostMethod( method );  m_translationZ.SetWrapPostMethod( method ); 
    m_eulerPitch.SetWrapPostMethod( method );  m_eulerYaw.SetWrapPostMethod( method ); m_eulerRoll.SetWrapPostMethod( method );
    m_scaleX.SetWrapPostMethod( method );  m_scaleY.SetWrapPostMethod( method );  m_scaleZ.SetWrapPostMethod( method ); 
    m_centerX.SetWrapPostMethod( method );  m_centerY.SetWrapPostMethod( method );  m_centerZ.SetWrapPostMethod( method ); 
}

// *************************************
//
void            CompositeTransform::SetWrapPreMethod    ( WrapMethod method )
{
    m_translationX.SetWrapPreMethod( method ); m_translationY.SetWrapPreMethod( method ); m_translationZ.SetWrapPreMethod( method ); 
    m_eulerPitch.SetWrapPreMethod( method ); m_eulerYaw.SetWrapPreMethod( method ); m_eulerRoll.SetWrapPreMethod( method );
    m_scaleX.SetWrapPreMethod( method ); m_scaleY.SetWrapPreMethod( method ); m_scaleZ.SetWrapPreMethod( method ); 
    m_centerX.SetWrapPreMethod( method ); m_centerY.SetWrapPreMethod( method ); m_centerZ.SetWrapPreMethod( method ); 
}

// *************************************
//
glm::vec3       CompositeTransform::GetTranslation      ( TimeType time ) const
{
    return glm::vec3( m_translationX.FloatInterpolator::Evaluate( time ), m_translationY.FloatInterpolator::Evaluate( time ), m_translationZ.FloatInterpolator::Evaluate( time ) );
}

// *************************************
//
glm::vec3       CompositeTransform::GetRotation         ( TimeType time ) const
{
    return glm::vec3( m_eulerPitch.FloatInterpolator::Evaluate( time ), m_eulerYaw.FloatInterpolator::Evaluate( time ), m_eulerRoll.FloatInterpolator::Evaluate( time ) );
}

// *************************************
//
glm::vec3       CompositeTransform::GetScale            ( TimeType time ) const
{
    return glm::vec3( m_scaleX.FloatInterpolator::Evaluate( time ), m_scaleY.FloatInterpolator::Evaluate( time ), m_scaleZ.FloatInterpolator::Evaluate( time ) );
}

// *************************************
//
glm::vec3       CompositeTransform::GetCenter           ( TimeType time ) const
{
    return glm::vec3( m_centerX.FloatInterpolator::Evaluate( time ), m_centerY.FloatInterpolator::Evaluate( time ), m_centerZ.FloatInterpolator::Evaluate( time ) );
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
void            CompositeTransform::SetRotation         ( const glm::vec3 & vec, TimeType time )
{
    m_eulerPitch.AddKey( time, vec.x );
    m_eulerYaw.AddKey( time, vec.y );
    m_eulerRoll.AddKey( time, vec.z );
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
    m_eulerPitch.RemoveKey( time );
    m_eulerYaw.RemoveKey( time );
    m_eulerRoll.RemoveKey( time );
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

    result &= m_eulerPitch.MoveKey( time, newTime );
    result &= m_eulerYaw.MoveKey( time, newTime );
    result &= m_eulerRoll.MoveKey( time, newTime );

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
CompositeTransformPtr   CompositeTransform::Create      ()

{
    return CompositeTransformPtr( new CompositeTransform() );
}

// *************************************
//
CompositeTransformPtr    CompositeTransform::Create      ( const IDeserializer & deser )
{
    auto transform = CompositeTransform::Create();

    if( deser.EnterChild( "transform" ) )
    {
        do
        {
            auto kindName = deser.GetAttribute( "kind" );
            auto kind = SerializationHelper::String2T< TransformKind >( kindName );

            auto params = SerializationHelper::DeserializeArray< FloatInterpolator >( deser, "interpolators" );
            if( params.size() == 3 )
            {
                if( kind == TransformKind::center )
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
                else if( kind == TransformKind::rotation )
                {
                    auto iseuler = deser.GetAttribute( "iseuler" );
                    if( iseuler == "true" )
                    {
                        transform->m_eulerPitch = *params[ 0 ];
                        transform->m_eulerYaw = *params[ 1 ];
                        transform->m_eulerRoll = *params[ 2 ];
                    }
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
        } while( deser.NextChild() );

        deser.ExitChild();
    }

    return transform;
}

// *************************************
//
void                CompositeTransform::Serialize       ( ISerializer & ser ) const
{
    ser.EnterArray( "composite_transform" );

    Serialize( ser, TransformKind::center );
    Serialize( ser, TransformKind::translation );
    Serialize( ser, TransformKind::rotation );
    Serialize( ser, TransformKind::scale );

    ser.ExitChild(); // composite_transform
}

// *************************************
//
void                CompositeTransform::Serialize       ( ISerializer & ser, TransformKind kind ) const
{
    switch( kind )
    {
        case TransformKind::center:
        {
            ser.EnterChild( "transform" );
            SerializationHelper::SerializeAttribute( ser, TransformKind::center, "kind" );
            ser.EnterArray( "interpolators" );

            m_centerX.Serialize( ser );
            m_centerY.Serialize( ser );
            m_centerZ.Serialize( ser );

            ser.ExitChild(); // interpolators
            ser.ExitChild(); // transform
        }
        break;

        case TransformKind::translation:
        {
            ser.EnterChild( "transform" );
            SerializationHelper::SerializeAttribute( ser, TransformKind::translation, "kind" );
            ser.EnterArray( "interpolators" );

            m_translationX.Serialize( ser );
            m_translationY.Serialize( ser );
            m_translationZ.Serialize( ser );

            ser.ExitChild(); // interpolators
            ser.ExitChild(); // transform
        }
        break;

        case TransformKind::rotation:
        {
            ser.EnterChild( "transform" );
            SerializationHelper::SerializeAttribute( ser, TransformKind::rotation, "kind" );
            ser.SetAttribute( "iseuler", "true" ); //?
            ser.EnterArray( "interpolators" );

            m_eulerPitch.Serialize( ser );
            m_eulerYaw.Serialize( ser );
            m_eulerRoll.Serialize( ser );

            ser.ExitChild(); // interpolators
            ser.ExitChild(); // transform
        }
        break;

        case TransformKind::scale:
        {
            ser.EnterChild( "transform" );
            SerializationHelper::SerializeAttribute( ser, TransformKind::scale, "kind" );
            ser.EnterArray( "interpolators" );

            m_scaleX.Serialize( ser );
            m_scaleY.Serialize( ser );
            m_scaleZ.Serialize( ser );

            ser.ExitChild(); // interpolators
            ser.ExitChild(); // transform
        }
        break;

        default:
            break;
    }
}

// *************************************
//
glm::mat4x4         CompositeTransform::Evaluate        ( TimeType t ) const
{
    auto rotQuat = glm::quat( glm::radians( GetRotation( t ) ) );
    return SQTTransform::Evaluate( GetTranslation( t ), rotQuat, GetScale( t ), GetCenter( t ) );
}

} //bv