#include "stdafx.h"

#include "RendererContext.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"



#include "Memory/MemoryLeaks.h"



namespace bv {     
namespace Convert
{

template< typename T >
T String2Enum( std::string s );

// ***********************
//
std::pair< model::AlphaContext::SrcBlendMode, const char* > sbm2s[] =
{
    std::make_pair( model::AlphaContext::SrcBlendMode::SBM_ZERO, "SBM_ZERO" )
    , std::make_pair( model::AlphaContext::SrcBlendMode::SBM_ONE, "SBM_ONE" )
    , std::make_pair( model::AlphaContext::SrcBlendMode::SBM_DST_COLOR, "SBM_DST_COLOR" )
    , std::make_pair( model::AlphaContext::SrcBlendMode::SBM_ONE_MINUS_DST_COLOR, "SBM_ONE_MINUS_DST_COLOR" )
    , std::make_pair( model::AlphaContext::SrcBlendMode::SBM_SRC_ALPHA, "SBM_SRC_ALPHA" )
    , std::make_pair( model::AlphaContext::SrcBlendMode::SBM_ONE_MINUS_SRC_ALPHA, "SBM_ONE_MINUS_SRC_ALPHA" )
    , std::make_pair( model::AlphaContext::SrcBlendMode::SBM_DST_ALPHA, "SBM_DST_ALPHA" )
    , std::make_pair( model::AlphaContext::SrcBlendMode::SBM_ONE_MINUS_DST_ALPHA, "SBM_ONE_MINUS_DST_ALPHA" )
    , std::make_pair( model::AlphaContext::SrcBlendMode::SBM_SRC_ALPHA_SATURATE, "SBM_SRC_ALPHA_SATURATE" )
    , std::make_pair( model::AlphaContext::SrcBlendMode::SBM_CONSTANT_COLOR, "SBM_CONSTANT_COLOR" )
    , std::make_pair( model::AlphaContext::SrcBlendMode::SBM_ONE_MINUS_CONSTANT_COLOR, "SBM_ONE_MINUS_CONSTANT_COLOR" )
    , std::make_pair( model::AlphaContext::SrcBlendMode::SBM_CONSTANT_ALPHA, "SBM_CONSTANT_ALPHA" )
    , std::make_pair( model::AlphaContext::SrcBlendMode::SBM_ONE_MINUS_CONSTANT_ALPHA, "SBM_ONE_MINUS_CONSTANT_ALPHA" )
    , std::make_pair( model::AlphaContext::SrcBlendMode::SBM_TOTAL, "" )
};

template<> std::string T2String( const model::AlphaContext::SrcBlendMode& sbm ) { return Enum2String( sbm2s, sbm ); }
template<> model::AlphaContext::SrcBlendMode String2Enum( std::string s ) { return String2Enum( sbm2s, s ); }

// ***********************
//
std::pair< model::AlphaContext::DstBlendMode, const char* > dbm2s[] =
{
    std::make_pair( model::AlphaContext::DstBlendMode::DBM_ZERO, "DBM_ZERO" )
    , std::make_pair( model::AlphaContext::DstBlendMode::DBM_ONE, "DBM_ONE" )
    , std::make_pair( model::AlphaContext::DstBlendMode::DBM_SRC_COLOR, "DBM_SRC_COLOR" )
    , std::make_pair( model::AlphaContext::DstBlendMode::DBM_ONE_MINUS_SRC_COLOR, "DBM_ONE_MINUS_SRC_COLOR" )
    , std::make_pair( model::AlphaContext::DstBlendMode::DBM_SRC_ALPHA, "DBM_SRC_ALPHA" )
    , std::make_pair( model::AlphaContext::DstBlendMode::DBM_ONE_MINUS_SRC_ALPHA, "DBM_ONE_MINUS_SRC_ALPHA" )
    , std::make_pair( model::AlphaContext::DstBlendMode::DBM_DST_ALPHA, "DBM_DST_ALPHA" )
    , std::make_pair( model::AlphaContext::DstBlendMode::DBM_ONE_MINUS_DST_ALPHA, "DBM_ONE_MINUS_DST_ALPHA" )
    , std::make_pair( model::AlphaContext::DstBlendMode::DBM_CONSTANT_COLOR, "DBM_CONSTANT_COLOR" )
    , std::make_pair( model::AlphaContext::DstBlendMode::DBM_ONE_MINUS_CONSTANT_COLOR, "DBM_ONE_MINUS_CONSTANT_COLOR" )
    , std::make_pair( model::AlphaContext::DstBlendMode::DBM_CONSTANT_ALPHA, "DBM_CONSTANT_ALPHA" )
    , std::make_pair( model::AlphaContext::DstBlendMode::DBM_ONE_MINUS_CONSTANT_ALPHA, "DBM_ONE_MINUS_CONSTANT_ALPHA" )
    , std::make_pair( model::AlphaContext::DstBlendMode::DBM_TOTAL, "" )
};

template<> std::string T2String( const model::AlphaContext::DstBlendMode& dbm ) { return Enum2String( dbm2s, dbm ); }
template<> model::AlphaContext::DstBlendMode String2Enum( std::string s ) { return String2Enum( dbm2s, s ); }


std::pair< model::FillContext::Mode, const char* > fcm2s[] =
{
    std::make_pair( model::FillContext::Mode::M_LINES, "M_LINES" )
    , std::make_pair( model::FillContext::Mode::M_POINTS, "M_POINTS" )
    , std::make_pair( model::FillContext::Mode::M_POLYGONS, "M_POLYGONS" )
    , std::make_pair( model::FillContext::Mode::M_TOTAL, "" )
};
template<> std::string T2String( const model::FillContext::Mode& fcm ) { return Enum2String( fcm2s, fcm ); }
template<> model::FillContext::Mode String2Enum( std::string s ) { return String2Enum( fcm2s, s ); }

}
   
namespace model
{

// *****************************
//
RendererContext::RendererContext     ()
    : m_stateChanged( true )
    , alphaCtx( nullptr )
    , cullCtx( nullptr )
    , depthCtx( nullptr )
    , fillCtx( nullptr )
{
}

// *****************************
//
RendererContext::~RendererContext    ()
{
    delete alphaCtx;
    delete cullCtx;
    delete depthCtx;
    delete fillCtx;
}

// *****************************
//
bool                RendererContext::StateChanged       () const
{
    return m_stateChanged;
}

// *****************************
//
void                RendererContext::SetStateChanged    ( bool stateChanged ) const
{
    m_stateChanged = stateChanged;
}

// *****************************
//
RendererContextPtr   RendererContext::Create             ()
{
    return std::make_shared< model::RendererContext >();
}

// *****************************
//
RendererContextPtr   RendererContext::CreateDefault		()
{
    auto ctx        = Create();
    ctx->alphaCtx   = new model::AlphaContext();
    ctx->cullCtx    = new model::CullContext();
    ctx->depthCtx   = new model::DepthContext();
    ctx->fillCtx    = new model::FillContext();

    return ctx;
}

// *****************************
//
RendererContextPtr   RendererContext::Clone				() const
{
    auto ctx        = Create();
    ctx->alphaCtx   = alphaCtx->Clone();
    ctx->cullCtx    = cullCtx->Clone();
    ctx->depthCtx   = depthCtx->Clone();
    ctx->fillCtx    = fillCtx->Clone();
    return ctx;
}

// *****************************
//
void                    RendererContext::SetContext	    ( RendererContextConstPtr & ctx ) 
{
    alphaCtx->SetContext( ctx->alphaCtx );
    cullCtx->SetContext( ctx->cullCtx );
    depthCtx->SetContext( ctx->depthCtx );
    fillCtx->SetContext( ctx->fillCtx );
}

// *****************************
//
void                RendererContext::Serialize       ( ISerializer& ser ) const
{
ser.EnterChild( "renderer_context" );
    
    ser.EnterChild( "alpha_context" );
    SerializationHelper::SerializeAttribute( ser, alphaCtx->blendEnabled, "blendEnabled" );
    SerializationHelper::SerializeAttribute( ser, alphaCtx->srcRGBBlendMode, "srcRGBBlendMode" );
    SerializationHelper::SerializeAttribute( ser, alphaCtx->dstRGBBlendMode, "dstRGBBlendMode" );
	SerializationHelper::SerializeAttribute( ser, alphaCtx->srcAlphaBlendMode, "srcAlphaBlendMode" );
	SerializationHelper::SerializeAttribute( ser, alphaCtx->dstAlphaBlendMode, "dstAlphaBlendMode" );
    SerializationHelper::SerializeAttribute( ser, alphaCtx->blendColor, "blendColor" );
    ser.ExitChild(); // alpha_context

    ser.EnterChild( "cull_context" );
    SerializationHelper::SerializeAttribute( ser, cullCtx->enabled, "enabled" );
    SerializationHelper::SerializeAttribute( ser, cullCtx->isCCWOrdered, "isCCWOrdered" );
    ser.ExitChild(); // cull_context

    ser.EnterChild( "depth_context" );
    SerializationHelper::SerializeAttribute( ser, depthCtx->enabled, "enabled" );
    SerializationHelper::SerializeAttribute( ser, depthCtx->writable, "writable" );
    ser.ExitChild(); // depth_context

    ser.EnterChild( "fill_context" );
    SerializationHelper::SerializeAttribute( ser, fillCtx->fillMode, "fillMode" );
    ser.ExitChild(); // fill_context

ser.ExitChild();
}


// *****************************
//
void                        RendererContext::Deserialize     ( const IDeserializer& deser )
{
    deser.EnterChild( "alpha_context" );
    alphaCtx->blendEnabled = Convert::String2T< bool >( deser.GetAttribute( "blendEnabled" ), false );
    alphaCtx->srcRGBBlendMode = Convert::String2Enum< AlphaContext::SrcBlendMode >( deser.GetAttribute( "srcRGBBlendMode" ) );
    alphaCtx->dstRGBBlendMode = Convert::String2Enum< AlphaContext::DstBlendMode >( deser.GetAttribute( "dstRGBBlendMode" ) );
    alphaCtx->srcAlphaBlendMode = Convert::String2Enum< AlphaContext::SrcBlendMode >( deser.GetAttribute( "srcAlphaBlendMode" ) );
    alphaCtx->dstAlphaBlendMode = Convert::String2Enum< AlphaContext::DstBlendMode >( deser.GetAttribute( "dstAlphaBlendMode" ) );
    alphaCtx->blendColor = Convert::String2T< glm::vec4 >( deser.GetAttribute( "blendColor" ), glm::vec4( 0, 0, 0, 0 ) );
    deser.ExitChild(); // alpha_context

    deser.EnterChild( "cull_context" );
    cullCtx->enabled = Convert::String2T< bool >( deser.GetAttribute( "enabled" ), true );
    cullCtx->isCCWOrdered = Convert::String2T< bool >( deser.GetAttribute( "enabled" ), true );
    deser.ExitChild(); // cull_context

    deser.EnterChild( "depth_context" );
    depthCtx->enabled = Convert::String2T< bool >( deser.GetAttribute( "enabled" ), true );
    depthCtx->writable = Convert::String2T< bool >( deser.GetAttribute( "writable" ), true );
    deser.ExitChild(); // depth_context

    deser.EnterChild( "fill_context" );
    fillCtx->fillMode = Convert::String2Enum< FillContext::Mode >( deser.GetAttribute( "fillMode" ) );
    deser.ExitChild(); // fill_context

}

// *****************************
//
RendererContextPtr   RendererContext::Create          ( const IDeserializer& deser )
{
    auto context = CreateDefault();

    context->Deserialize( deser );

    return context;
}

} //model
} //bv
