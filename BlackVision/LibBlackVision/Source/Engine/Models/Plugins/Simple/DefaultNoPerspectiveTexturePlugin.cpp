#include "DefaultNoPerspectiveTexturePlugin.h"


namespace bv { namespace model {

	// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultNoPerspectiveTexturePluginDesc::DefaultNoPerspectiveTexturePluginDesc                          ()
    : BasePluginDescriptor( UID(), "texture", "nptx" )
{
}

// *******************************
//
IPluginPtr              DefaultNoPerspectiveTexturePluginDesc::CreatePlugin              ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultTexturePlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultNoPerspectiveTexturePluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    //Create all models
    DefaultPluginParamValModelPtr model  = std::make_shared< DefaultPluginParamValModel >( timeEvaluator );
    DefaultParamValModelPtr psModel      = std::make_shared< DefaultParamValModel >();
    DefaultParamValModelPtr vsModel      = std::make_shared< DefaultParamValModel >();

    //Create all parameters and evaluators
    SimpleFloatEvaluatorPtr     alphaEvaluator   = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "alpha", timeEvaluator );
    SimpleTransformEvaluatorPtr trTxEvaluator    = ParamValEvaluatorFactory::CreateSimpleTransformEvaluator( "txMat", timeEvaluator );

    //Register all parameters and evaloators in models
    vsModel->RegisterAll( trTxEvaluator );
    psModel->RegisterAll( alphaEvaluator );

    //Set models structure
    model->SetVertexShaderChannelModel( vsModel );
    model->SetPixelShaderChannelModel( psModel );

    //Set default values of all parameters
    alphaEvaluator->Parameter()->SetVal( 1.f, TimeType( 0.0 ) );
    trTxEvaluator->Parameter()->Transform().InitializeDefaultSRT();

    return model;
}

// *******************************
//
std::string             DefaultNoPerspectiveTexturePluginDesc::UID                       ()
{
    return "DEFAULT_NO_PERSPECTIVE_TEXTURE";
}

// *******************************
// 
std::string             DefaultNoPerspectiveTexturePluginDesc::TextureName               ()
{
    return "Tex0";
}



//FIXME: dodawanie kanalow w ten sposob (przez przypisanie na m_<xxx>channel powoduje bledy, trzeba to jakos poprawic, zeby bylo wiadomo, o co chodzi
//FIXME: teraz zle dodanie wychodzi dopiero po odpaleniu silnika, a to jest oczywisty blad

// ************************************************************************* PLUGIN *************************************************************************

// *************************************
// 

DefaultNoPerspectiveTexturePlugin::DefaultNoPerspectiveTexturePlugin( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
	: DefaultTexturePlugin( name, uid, prev, model )
{
}


DefaultNoPerspectiveTexturePlugin::~DefaultNoPerspectiveTexturePlugin()
{
}


}} // model // bv
