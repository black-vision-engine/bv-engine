#include "stdafx.h"

#include "SceneUpdater.h"
#include "Engine/Models/SceneModel.h"
#include "Engine/Graphics/SceneGraph/Scene.h"
#include "Engine/Graphics/SceneGraph/LightsLayout.h"


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
{
}

// *****************************
//
                    SceneUpdater::~SceneUpdater     ()
{
}

// *****************************
//
void                SceneUpdater::DoUpdate          ()
{
    UpdateLights();
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

} //bv
