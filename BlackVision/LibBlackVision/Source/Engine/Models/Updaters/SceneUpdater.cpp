#include "stdafx.h"

#include "SceneUpdater.h"
#include "Engine/Models/SceneModel.h"
#include "Engine/Graphics/SceneGraph/Scene.h"
#include "Engine/Graphics/SceneGraph/LightsLayout.h"
#include "Engine/Graphics/SceneGraph/Lines.h"

#include "Engine/Models/BVProjectTools.h"
#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Graphics/Resources/RenderableArrayDataArrays.h"
#include "Engine/Graphics/Resources/VertexBuffer.h"
#include "Engine/Graphics/Effects/Utils/RenderableEffectFactory.h"

#include "UpdatersHelpers.h"



#include "Memory/MemoryLeaks.h"



namespace bv 
{

// *****************************
//
SceneUpdaterPtr     SceneUpdater::Create            ( Scene * scene, model::SceneModel * modelScene )
{
    struct make_shared_enabler_SceneUpdater : public SceneUpdater
    {
        make_shared_enabler_SceneUpdater( Scene * scene, model::SceneModel * modelScene )
            : SceneUpdater( scene, modelScene )
        {
        }
    };

    return std::make_shared< make_shared_enabler_SceneUpdater >( scene, modelScene );
}

// *****************************
//
                    SceneUpdater::SceneUpdater      ( Scene * scene, model::SceneModel * modelScene )
    : m_scene( scene )
    , m_modelScene( modelScene )
    , m_gridLinesUpdateID( 0 )
    , m_cameraUpdateID( 0 )
{
}

// *****************************
//
                    SceneUpdater::~SceneUpdater     ()
{}

// *****************************
//
void                SceneUpdater::DoUpdate          ()
{
    m_scene->SetOutputChannelIdx( m_modelScene->GetRenderChannelIdx() );
    UpdateCamera();
    UpdateLights();
    UpdateGridLines();
}

// *****************************
//
void                SceneUpdater::UpdateLights      ()
{
    auto buffer = m_scene->GetLightsBuffer();
    if( buffer )
    {
        auto layout = buffer->GetLayout();

        auto lightTypeNum = ( UInt32 )LightType::LT_TOTAL;

        std::vector< UInt32 > lightNum( lightTypeNum, 0 );

        for( UInt32 i = 0; i < m_modelScene->NumLights(); ++i )
        {
            auto light = m_modelScene->GetLight( i );
            auto type = ( UInt32 )light->GetType();
         
            auto offsetsIdx = LightsLayout::Instance().GetLightOffsetIdx( LightType( type ), lightNum[ type ] );
            for( auto & value : light->GetValues() )
            {
                auto offset = layout->uniformDescs[ offsetsIdx ].offset;
                buffer->WriteData( value->GetData(), value->GetSize(), offset );
                offsetsIdx++;
            }
            lightNum[ type ]++;
        }

        for( UInt32 i = 0; i < lightTypeNum; ++i )
        {
            buffer->WriteData( ( char * )&lightNum[ i ], sizeof( Float32 ), layout->uniformDescs[ LightsLayout::Instance().GetLightsNumOffsetIdx( LightType( i ) ) ].offset );
        }
    }
}

// ***********************
//
void                SceneUpdater::UpdateGridLines     ()
{
    auto & gridLinesLogic = m_modelScene->GetGridLinesLogic();

    bool linesVisible = gridLinesLogic.GetGridLinesVisibility();
    m_scene->SetGridLinesVisible( linesVisible );

    if( linesVisible )
    {
        if( m_gridLinesUpdateID == 0 )
        {
            // First update. Create RenderableEntity
            auto component = gridLinesLogic.BuildConnectedComponent();
            auto linesRenderable = Cast< Lines * >( BVProjectTools::BuildRenderableFromComponent( std::static_pointer_cast< model::IConnectedComponent >( component ), PrimitiveType::PT_LINES ) );

            linesRenderable->SetRenderableEffect( RenderableEffectFactory::CreateGridLinesEffect() );
            linesRenderable->SetWidth( 1.0f );

            auto param = Cast< ShaderParamVec4 * >( linesRenderable->GetRenderableEffect()->GetPass( 0 )->GetPixelShader()->GetParameters()->AccessParam( "color" ) );
            param->SetValue( gridLinesLogic.GetColor() );

            m_scene->SetGridLinesRenderable( linesRenderable );
            m_gridLinesUpdateID = gridLinesLogic.GetUpdateID();
        }

        if( m_gridLinesUpdateID < gridLinesLogic.GetUpdateID() )
        {
            auto component = gridLinesLogic.BuildConnectedComponent();
            auto linesRenderable = m_scene->GetGridLines();
            
            // Sometimes buffers are recreated when not necessary. But GridLines are only editor helpers
            // and they don't need to be efficient.
            UpdatersHelpers::RecreateRenderableBuffer( linesRenderable, component );

            m_gridLinesUpdateID = gridLinesLogic.GetUpdateID();
        }
    }
}

// ***********************
//
void                    SceneUpdater::UpdateCamera        ()
{
    auto & camLogic = m_modelScene->GetCamerasLogic();
    
    if( m_cameraUpdateID < camLogic.GetUpdateID() )
    {
        auto camera = m_scene->GetCamera();
        UpdatersHelpers::UpdateCamera( camera, camLogic.GetCurrentCamera() );

        // Set camera position in UBO.
        // FIXME: In future all camera data should be placed in this UBO instead of using
        // uniforms per object.
        auto cameraUBO = m_scene->GetCameraBuffer();
        auto positionValue = camLogic.GetCurrentCamera()->GetValue( "Position" );
        cameraUBO->WriteData( positionValue->GetData(), positionValue->GetSize(), 0 );
    }
}

} //bv
