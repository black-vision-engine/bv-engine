#include "stdafx.h"

#include "DefaultVideoInputPlugin.h"
//#include "DefaultVideoInput.h"
#include "Engine\Models\Plugins\ParamValModel\ParamValEvaluatorFactory.h"
#include "Engine/Models/Plugins/Channels/HelperVertexShaderChannel.h"

#include "VideoInput/DefaultVideoInputResourceDescr.h"




#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {

    // ************************************************************************* DESCRIPTOR *************************************************************************

    // *******************************
    //
    DefaultVideoInputPluginDesc::DefaultVideoInputPluginDesc                          ()
        : BasePluginDescriptor( UID(), "video input", "tx" )
    {
    }

    // *******************************
    //
    IPluginPtr              DefaultVideoInputPluginDesc::CreatePlugin              ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
    {
        return CreatePluginTyped< DefaultVideoInputPlugin >( name, prev, timeEvaluator );
    }

    // *******************************
    //
    DefaultPluginParamValModelPtr   DefaultVideoInputPluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
    {
        //Create all models
        DefaultPluginParamValModelPtr model  = std::make_shared< DefaultPluginParamValModel >( timeEvaluator );
        DefaultParamValModelPtr psModel      = std::make_shared< DefaultParamValModel >();
        DefaultParamValModelPtr vsModel      = std::make_shared< DefaultParamValModel >();

        //Create all parameters and evaluators
        SimpleFloatEvaluatorPtr     alphaEvaluator   = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "alpha", timeEvaluator );
        SimpleTransformEvaluatorPtr trTxEvaluator    = ParamValEvaluatorFactory::CreateSimpleTransformEvaluator( "txMat", timeEvaluator );

        ParamFloatPtr  paramVideoInputSource = ParametersFactory::CreateParameterFloat( "source", timeEvaluator );

        //Register all parameters and evaloators in models
        vsModel->RegisterAll( trTxEvaluator );
        psModel->RegisterAll( alphaEvaluator );
       
        psModel->AddParameter( paramVideoInputSource );

        //Set models structure
        model->SetVertexShaderChannelModel( vsModel );
        model->SetPixelShaderChannelModel( psModel );

        //Set default values of all parameters
        alphaEvaluator->Parameter()->SetVal( 1.f, TimeType( 0.0 ) );
        trTxEvaluator->Parameter()->Transform().InitializeDefaultSRT();
        trTxEvaluator->Parameter()->Transform().SetCenter( glm::vec3( 0.5, 0.5, 0.0 ), 0.0f );

        //FIXME: integer parmeters should be used here
        paramVideoInputSource->SetVal( (float) 0.f, TimeType( 0.f ) );

        return model;
    }

    // *******************************
    //
    std::string             DefaultVideoInputPluginDesc::UID                       ()
    {
        return "DEFAULT_VIDEOINPUT";
    }

    // *******************************
    //
    std::string             DefaultVideoInputPluginDesc::VertexShaderSource        ()
    {
        //return "../dep/media/shaders/DefaultVideoInput.vert";
        return NULL;
    }

    // *******************************
    //
    std::string             DefaultVideoInputPluginDesc::PixelShaderSource         ()
    {
        //return "../dep/media/shaders/DefaultVideoInput.frag";
        return NULL;
    }

    // *******************************
    //
    std::string             DefaultVideoInputPluginDesc::TextureName               ()
    {
        return "Tex0";
    }

    // ************************************************************************* PLUGIN *************************************************************************

    // *************************************
    // 
    DefaultVideoInputPlugin::DefaultVideoInputPlugin( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
        : DefaultTexturePlugin( name, uid, prev, model )
    {
        //auto txData = m_psc->GetTexturesDataImpl();

        //auto vi = new DefaultVideoInput( 10, 10, 1.f ); // FIXME
        //txData->AddTexture( vi->GetTexture() );
        //vis.push_back( vi );
    }


    DefaultVideoInputPlugin::~DefaultVideoInputPlugin(void)
    {
    }


    int                                     DefaultVideoInputPlugin::GetSourceNumber()
    {
        auto param = GetParameter( "source" );
        float fSource = QueryTypedParam< ParamFloatPtr > ( param )->Evaluate();
        return (int) fSource;
    }


    void                                DefaultVideoInputPlugin::Update                      ( TimeType t )
    {
		BasePlugin::Update( t );

        HelperVertexShaderChannel::InverseTextureMatrix( m_pluginParamValModel, "txMat" );

        //desc.Update();

        auto txData = m_psc->GetTexturesDataImpl();
        assert( txData->GetTextures().size() == 1 );

        //int source = GetSourceNumber();
        //desc.SetCurrentInput( source );
    }

// *************************************
// 
bool                                DefaultVideoInputPlugin::LoadResource                ( AssetDescConstPtr assetDescr )
{
	auto txAssetDescr = QueryTypedDesc< TextureAssetDescConstPtr >( assetDescr );

    // FIXME: dodac tutaj API pozwalajace tez ustawiac parametry dodawanej tekstury (normalny load z dodatkowymi parametrami)
    if ( txAssetDescr != nullptr )
    {
        //FIXME: use some better API to handle resources in general and textures in this specific case
        auto txDesc = DefaultTextureDescriptor::LoadTexture( txAssetDescr, DefaultTexturePluginDesc::TextureName() );
        if( txDesc != nullptr )
        {
            auto txData = m_psc->GetTexturesDataImpl();
            auto replacedTex = txData->GetTexture( 0 );

            SamplerStateModelPtr newSamplerStateModel = replacedTex != nullptr ? replacedTex->GetSamplerState() : SamplerStateModel::Create( m_pluginParamValModel->GetTimeEvaluator() );

            txDesc->SetSamplerState( newSamplerStateModel );
			txDesc->SetSemantic( DataBuffer::Semantic::S_TEXTURE_STATIC );

            txData->SetTexture( 0, txDesc );

            return true;
        }
    }

    auto viResDescr = QueryTypedDesc< DefaultVideoInputResourceDescrConstPtr >( assetDescr );

    if( viResDescr != nullptr )
    {
        auto txData = m_psc->GetTexturesDataImpl();
        assert( txData->GetTextures().size() <= 1 ); // to be safe for now
        
        if( txData->GetTextures().size() == 0 )
            txData->AddTexture( viResDescr->GetITextureDescriptor() );
        //else
        //    txData->SetTexture( viResDescr->GetITextureDescriptor() );

        //desc.AddAndSetInputAsCurrent( viResDescr );

        return true;
    }

    return false;
}

} }