#include "stdafx.h"
#include "CamerasLogic.h"

#include "Application/ApplicationContext.h"
#include "Engine/Models/ModelHelper.h"




#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {

// ***********************
//
CamerasLogic::CamerasLogic( ITimeEvaluatorPtr timeEvaluator )
    :   m_currentCamera( nullptr )
    ,   m_defaultTimeline( timeEvaluator )
{}

// ***********************
//
CamerasLogic::~CamerasLogic()
{}

// ========================================================================= //
// Serialization
// ========================================================================= //

// ***********************
//
void            CamerasLogic::Deserialize             ( const IDeserializer & deser )
{
    ClearAll();

    m_defaultTimeline = SerializationHelper::GetDefaultTimeline( deser );


    auto cameras = SerializationHelper::DeserializeArray< CameraModel >( deser, "cameras" );

    for( SizeType i = 0; i < cameras.size(); ++i )
    {
        if( cameras[ i ] != nullptr )
            m_cameras.push_back( cameras[ i ] );
    }


    int currCameraIdx = Convert::String2T( deser.GetAttribute( "CurrentCamera" ), 0 );

    // Add default camera in case we deserialized nothing.
    if( m_cameras.empty() )
        m_cameras.push_back( std::make_shared< CameraModel >( m_defaultTimeline ) );

    // Set current camera. Add first camera, when camera at currCameraIdx doesn't exists.
    if( currCameraIdx < ( Int32 )m_cameras.size() )
        m_currentCamera = m_cameras[ currCameraIdx ];
    else
        m_currentCamera = m_cameras[ 0 ];

    UpdateID();
}


// ***********************
//
void            CamerasLogic::Serialize               ( ISerializer & ser ) const
{
    ser.EnterArray( "cameras" );

    int currCameraIdx = -1;

    for( Int32 i = 0; i < ( Int32 )m_cameras.size(); ++i )
    {
        m_cameras[ i ]->Serialize( ser );
        if( m_cameras[ i ] == m_currentCamera )
            currCameraIdx = i;
    }

    ser.ExitChild();    // cameras
        
    if( currCameraIdx > -1 )
        ser.SetAttribute( "CurrentCamera", Convert::T2String( currCameraIdx ) );
}

// ========================================================================= //
// Interaction
// ========================================================================= //

void                    CamerasLogic::Update                  ( TimeType t )
{
    for( auto & camera : m_cameras )
    {
        camera->Update( t );
    }

    if( m_currentCamera->StateChanged() )
        UpdateID();
}

// ***********************
//
bool                    CamerasLogic::AddCamera               ()
{
    m_cameras.push_back( std::make_shared< CameraModel >( m_defaultTimeline ) );
    return true;
}

// ***********************
//
bool                    CamerasLogic::RemoveCamera            ( unsigned int index )
{
    if( index < m_cameras.size() && m_cameras.size() > 1 )
    {
        bool currentRemoved = false;
        if( m_cameras[ index ] == m_currentCamera )
        {
            currentRemoved = true;
        }

        m_cameras.erase( m_cameras.begin() + index );

        if( currentRemoved )
        {
            m_currentCamera = m_cameras[ 0 ];
            UpdateID();
        }

        return true;
    }
    else
        return false;
}

// ***********************
//
CameraModelPtr          CamerasLogic::GetCamera               ( unsigned int index )
{
    if( index < m_cameras.size() )
    {
        return m_cameras[ index ];
    }
    return nullptr;
}

// ***********************
//
SizeType                CamerasLogic::GetNumCameras           ()
{
    return m_cameras.size();
}

// ***********************
//
CameraModelPtr &        CamerasLogic::GetCurrentCamera        ()
{
    return m_currentCamera;
}

// ***********************
//
bool                    CamerasLogic::SetCurrentCamera        ( unsigned int index )
{
    if( index < m_cameras.size() )
    {
        m_currentCamera = m_cameras[ index ];
        UpdateID();
        return true;
    }
    else
        return false;
}

// ========================================================================= //
// Private Helpers
// ========================================================================= //

// ***********************
//
void            CamerasLogic::SetDefaultTimeline      ( ITimeEvaluatorPtr timeEvaluator )
{
    if( timeEvaluator )
        m_defaultTimeline = timeEvaluator;
}

// ***********************
//
void            CamerasLogic::ClearAll                ()
{
    m_cameras.clear();
    m_currentCamera = nullptr;
}

// ***********************
//
void            CamerasLogic::UpdateID                ()
{
    m_cameraUpdateID = ApplicationContext::Instance().GetUpdateCounter() + 1;
}

} // model
} //bv

