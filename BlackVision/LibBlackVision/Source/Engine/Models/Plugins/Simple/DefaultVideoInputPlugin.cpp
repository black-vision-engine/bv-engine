#include "DefaultVideoInputPlugin.h"
//#include "DefaultVideoInput.h"
#include "Engine\Models\Plugins\ParamValModel\ParamValEvaluatorFactory.h"

namespace bv { namespace model {

	// ************************************************************************* DESCRIPTOR *************************************************************************

	// *******************************
	//
	DefaultVideoInputPluginDesc::DefaultVideoInputPluginDesc                          ()
		: BasePluginDescriptor( UID(), "video input" )
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
		DefaultPluginParamValModelPtr model  = std::make_shared< DefaultPluginParamValModel >();
		DefaultParamValModelPtr psModel      = std::make_shared< DefaultParamValModel >();
		DefaultParamValModelPtr vsModel      = std::make_shared< DefaultParamValModel >();

		//Create all parameters and evaluators
		SimpleVec4EvaluatorPtr      borderColorEvaluator = ParamValEvaluatorFactory::CreateSimpleVec4Evaluator( "borderColor", timeEvaluator );
		SimpleFloatEvaluatorPtr     alphaEvaluator   = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "alpha", timeEvaluator );
		SimpleTransformEvaluatorPtr trTxEvaluator    = ParamValEvaluatorFactory::CreateSimpleTransformEvaluator( "txMat", timeEvaluator );

		ParamFloatPtr  paramWrapModeX     = ParametersFactory::CreateParameterFloat( "wrapModeX", timeEvaluator );
		ParamFloatPtr  paramWrapModeY     = ParametersFactory::CreateParameterFloat( "wrapModeY", timeEvaluator );
		ParamFloatPtr  paramFilteringMode = ParametersFactory::CreateParameterFloat( "filteringMode", timeEvaluator );
		ParamFloatPtr  paramAttachMode    = ParametersFactory::CreateParameterFloat( "attachmentMode", timeEvaluator );

        ParamFloatPtr  paramVideoInputSource = ParametersFactory::CreateParameterFloat( "source", timeEvaluator );

		//Register all parameters and evaloators in models
		vsModel->RegisterAll( trTxEvaluator );
		psModel->RegisterAll( borderColorEvaluator );
		psModel->RegisterAll( alphaEvaluator );
		psModel->AddParameter( paramWrapModeX );
		psModel->AddParameter( paramWrapModeY );
		psModel->AddParameter( paramFilteringMode );
		psModel->AddParameter( paramAttachMode );

        psModel->AddParameter( paramVideoInputSource );

		//Set models structure
		model->SetVertexShaderChannelModel( vsModel );
		model->SetPixelShaderChannelModel( psModel );

		//Set default values of all parameters
		alphaEvaluator->Parameter()->SetVal( 1.f, TimeType( 0.0 ) );
		borderColorEvaluator->Parameter()->SetVal( glm::vec4( 0.f, 0.f, 0.f, 0.f ), TimeType( 0.f ) );
		trTxEvaluator->Parameter()->Transform().InitializeDefaultSRT();

		//FIXME: integer parmeters should be used here
		paramWrapModeX->SetVal( (float) TextureWrappingMode::TWM_REPEAT, TimeType( 0.f ) );
		paramWrapModeY->SetVal( (float) TextureWrappingMode::TWM_REPEAT, TimeType( 0.f ) );
		paramFilteringMode->SetVal( (float) TextureFilteringMode::TFM_LINEAR, TimeType( 0.f ) );
		paramAttachMode->SetVal( (float) TextureAttachmentMode::MM_ATTACHED, TimeType( 0.f ) );
        paramVideoInputSource->SetVal( (float) 0.f, TimeType( 0.f ) );

		return model;
	}

	// *******************************
	//
	bool                   DefaultVideoInputPluginDesc::CanBeAttachedTo     ( IPluginConstPtr plugin ) const
	{
		if ( plugin == nullptr )
		{
			return false;
		}

		auto  vac = plugin->GetVertexAttributesChannel();
		if ( vac == nullptr )
		{
			return false;
		}

		//auto numChannels = vac->GetDescriptor()->GetNumVertexChannels();
		//if ( numChannels != 1 ) //only vertex attribute data allowed here
		//{
		//    return false;
		//}

		return true;
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
		t=t; // just to reference t ;)

		for( auto vi : vis )
			vi->Update();

        auto txData = m_psc->GetTexturesDataImpl();
        assert( txData->GetTextures().size() == 1 );
        int source = GetSourceNumber();
        assert( source < vis.size() );
        //txData->SetTexture( 0,  vis[ source ]->GetITextureDescriptor() );
	}

// *************************************
// 
bool                            DefaultVideoInputPlugin::LoadResource  ( IPluginResourceDescrConstPtr resDescr )
{
    auto txResDescr = QueryTextureResourceDescr( resDescr );

    // FIXME: dodac tutaj API pozwalajace tez ustawiac parametry dodawanej tekstury (normalny load z dodatkowymi parametrami)
    if ( txResDescr != nullptr )
    {
        auto txData = m_psc->GetTexturesDataImpl();
        assert( txData->GetTextures().size() <= 2 ); //FIXME: Second one may be added by a mask

        //FIXME: use some better API to handle resources in general and textures in this specific case
        auto txDesc = DefaultTextureDescriptor::LoadTexture( txResDescr->GetTextureFile(), DefaultTexturePluginDesc::TextureName() );
        txDesc->SetSemantic( DataBuffer::Semantic::S_TEXTURE_STATIC );

        if( txDesc != nullptr )
        {
            if( txData->GetTextures().size() == 0 )
            {
                txData->AddTexture( txDesc );
            }
            else
            {
                txData->SetTexture( 0, txDesc );
            }

            m_textureWidth = txDesc->GetWidth();
            m_textureHeight = txDesc->GetHeight();

            return true;
        }
    }

	auto viResDescr = QueryVideoInputResourceDescr( resDescr );

	if( viResDescr != nullptr )
	{
		auto txData = m_psc->GetTexturesDataImpl();
		assert( txData->GetTextures().size() <= 1 ); // to be safe for now
		
        if( txData->GetTextures().size() == 0 )
    		txData->AddTexture( viResDescr->GetITextureDescriptor() );
        //else
        //    txData->SetTexture( viResDescr->GetITextureDescriptor() );

		vis.push_back( viResDescr );

		return true;
	}

    return false;
}

} }