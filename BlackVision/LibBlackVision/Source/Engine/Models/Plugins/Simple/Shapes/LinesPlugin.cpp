#include "stdafx.h"
#include "LinesPlugin.h"

#include "Assets/DefaultAssets.h"

#include "Assets/DataArray/DataArrayAssetDescriptor.h"
#include "Assets/DataArray/DataArrayAsset.h"


namespace bv {
namespace model {



const std::string           LinesPlugin::AssetName = "LinesData";


// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
LinesPluginDesc::LinesPluginDesc                          ()
    : DefaultGeometryPluginDescBase( UID(), "lines" )
{}

// *******************************
//
IPluginPtr              LinesPluginDesc::CreatePlugin              ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< LinesPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   LinesPluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper h( timeEvaluator );
    h.SetOrCreateVacModel();

    h.SetOrCreatePSModel();

    return h.GetModel();
}

// *******************************
//
std::string             LinesPluginDesc::UID                       ()
{
    return "LINES_PLUGIN";
}


// ========================================================================= //
// Generators
// ========================================================================= //


#include "Mathematics/Defines.h"
namespace LinesPluginGenerator
{

class MainGenerator : public IGeometryOnlyGenerator
{
protected:

    DataArrayAssetConstPtr  m_data;

public:
    MainGenerator( DataArrayAssetConstPtr data )
        : m_data( data )
    {}

    virtual ~MainGenerator()
    {
        m_data = nullptr;
    }

    virtual void GenerateGeometry( Float3AttributeChannelPtr verts ) override
    {
        if( !m_data->IsEmpty() )
        {
            auto plot = m_data->GetRow( "Lines" );
            if( plot->GetType() == ModelParamType::MPT_VEC3 )
            {
                auto typedPlot = static_cast< DataArrayRow< glm::vec3 >* >( plot );
                auto & plotPoints = typedPlot->GetArray();

                for( UInt32 i = 0; i < ( UInt32 )plotPoints.size() - 1; i += 2 )
                {
                    verts->AddAttribute( plotPoints[ i ] );
                    verts->AddAttribute( plotPoints[ i + 1 ] );
                }

                return;
            }
        }
        else
        {
            // Engine demands non empty channel.
            verts->AddAttribute( glm::vec3( 0.0f, 0.0f, 0.0f ) );
            verts->AddAttribute( glm::vec3( 0.0f, 0.0f, 0.0f ) );
        }
    }

};
}



// ************************************************************************* PLUGIN *************************************************************************


// *************************************
// 
LinesPlugin::LinesPlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    :   DefaultGeometryPluginBase( name, uid, prev, model )
    ,   m_assetUpdated( false )
{
    m_pluginParamValModel->Update();
    LoadResource( DefaultAssets::Instance().GetDefaultDesc< DataArrayAssetDesc >() );

    InitGeometry( PrimitiveType::PT_LINES );
}

// *************************************
// 
LinesPlugin::~LinesPlugin         ()
{}


std::vector<IGeometryGeneratorPtr>	LinesPlugin::GetGenerators()
{
    std::vector<IGeometryGeneratorPtr> gens;
    gens.push_back( std::make_shared< LinesPluginGenerator::MainGenerator >( m_asset ) );

    return gens;
}


// ***********************
//
bool                                LinesPlugin::NeedsTopologyUpdate()
{
    auto & states = m_pluginParamValModel->GetVertexAttributesChannelModel()->GetStates();
    for( auto iter = states.begin(); iter != states.end(); ++iter )
    {
        if( iter->second->StateChanged() )
        {
            return true;
        }
    }

    if( m_assetUpdated )
    {
        m_assetUpdated = false;
        return true;
    }

    return false;
}

// *************************************
// 
bool                                LinesPlugin::LoadResource  ( AssetDescConstPtr assetDescr )
{
    auto daAssetDescr = QueryTypedDesc< DataArrayAssetBaseDescConstPtr >( assetDescr );

    if( daAssetDescr )
    {
        m_asset = LoadTypedAsset< DataArrayAsset >( daAssetDescr );

        SetAsset( 0, LAsset( AssetName, assetDescr, nullptr ) );
        m_assetUpdated = true;

        return true;
    }

    return false;
}


} // model
} // bv



