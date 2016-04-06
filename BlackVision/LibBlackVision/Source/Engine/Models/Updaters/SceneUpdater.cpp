#include "stdafx.h"

#include "SceneUpdater.h"
#include "Engine/Models/SceneModel.h"
#include "Engine/Graphics/SceneGraph/Scene.h"


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

        static auto directionalLightsNumIdx = ( UInt32 )std::distance( layout->uniformDescs.begin(), std::find_if( layout->uniformDescs.begin(), layout->uniformDescs.end(), []( const UniformDesc & desc ) { return desc.name == "directionalLightNum"; } ) );
        static auto pointLightsNumIdx = ( UInt32 )std::distance( layout->uniformDescs.begin(), std::find_if( layout->uniformDescs.begin(), layout->uniformDescs.end(), []( const UniformDesc & desc ) { return desc.name == "pointLightNum"; } ) );
        static auto spotLightsNumIdx = ( UInt32 )std::distance( layout->uniformDescs.begin(), std::find_if( layout->uniformDescs.begin(), layout->uniformDescs.end(), []( const UniformDesc & desc ) { return desc.name == "spotLightNum"; } ) );

        auto lightTypeNum = ( UInt32 )LightType::LT_TOTAL;

        std::vector< UInt32 > offsetsIdx( lightTypeNum );
        std::vector< UInt32 > lightNum( lightTypeNum, 0 );

        offsetsIdx[ 0 ] = 0;                            //LightType::LT_DIRECTIONAL
        offsetsIdx[ 1 ] = directionalLightsNumIdx + 1;  //LightType::LT_POINT
        offsetsIdx[ 2 ] = pointLightsNumIdx + 1;        //LightType::LT_SPOT
        
        UInt32 maxLightsNum = Scene::GetMaxLightsNum();
        for( UInt32 i = 0; i < m_modelScene->NumLights(); ++i )
        {
            auto light = m_modelScene->GetLight( i );
            auto type = ( UInt32 )light->GetType();
         
            if( lightNum[ type ] < maxLightsNum )
            {
                for( auto & value : light->GetValues() )
                {
                    auto offset = layout->uniformDescs[ offsetsIdx[ type ] ].offset;
                    buffer->WriteData( value->GetData(), value->GetSize(), offset );
                    offsetsIdx[ type ]++;
                }

                lightNum[ type ] += 1;
            }
        }

        buffer->WriteData( ( char * )&lightNum[ 0 ], sizeof( Float32 ), layout->uniformDescs[ directionalLightsNumIdx ].offset );
        buffer->WriteData( ( char * )&lightNum[ 1 ], sizeof( Float32 ), layout->uniformDescs[ pointLightsNumIdx ].offset );
        buffer->WriteData( ( char * )&lightNum[ 2 ], sizeof( Float32 ), layout->uniformDescs[ spotLightsNumIdx ].offset );
    }
}

} //bv
