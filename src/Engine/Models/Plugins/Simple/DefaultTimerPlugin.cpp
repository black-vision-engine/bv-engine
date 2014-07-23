#include "DefaultTimerPlugin.h"

#include "Engine/Models/Resources/Font/TextHelper.h"
#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"
#include "Engine/Models/Resources/Font/FontLoader.h"
#include "Engine/Models/Resources/Font/Text.h"

#include <algorithm>

namespace bv { namespace model {

// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultTimerPluginDesc::DefaultTimerPluginDesc                            ()
    : BasePluginDescriptor( UID(), "timer" )
{
}

// *******************************
//
IPluginPtr              DefaultTimerPluginDesc::CreatePlugin             ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultTimerPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultTimerPluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    //Create all models
    DefaultPluginParamValModelPtr model  = std::make_shared< DefaultPluginParamValModel >();
    DefaultParamValModelPtr psModel      = std::make_shared< DefaultParamValModel >();
    DefaultParamValModelPtr vsModel      = std::make_shared< DefaultParamValModel >();
    DefaultParamValModelPtr plModel      = std::make_shared< DefaultParamValModel >();


    //Create all parameters and evaluators
    SimpleVec4EvaluatorPtr      borderColorEvaluator    = ParamValEvaluatorFactory::CreateSimpleVec4Evaluator( "borderColor", timeEvaluator );
    SimpleFloatEvaluatorPtr     alphaEvaluator          = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "alpha", timeEvaluator );
    SimpleFloatEvaluatorPtr     fontSizeEvaluator       = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "fontSize", timeEvaluator );

    SimpleFloatEvaluatorPtr     blurSizeEvaluator       = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "blurSize", timeEvaluator );

    SimpleFloatEvaluatorPtr     timerTimeEvaluator      = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "time", timeEvaluator );
    SimpleFloatEvaluatorPtr     spacingEvaluator        = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "spacing", timeEvaluator );

    //Register all parameters and evaloators in models
    psModel->RegisterAll( borderColorEvaluator );
    psModel->RegisterAll( alphaEvaluator );
    plModel->RegisterAll( blurSizeEvaluator );
    plModel->RegisterAll( spacingEvaluator );
    plModel->RegisterAll( timerTimeEvaluator );
    plModel->RegisterAll( fontSizeEvaluator );

    //Set models structure
    model->SetVertexShaderChannelModel( vsModel );
    model->SetPixelShaderChannelModel( psModel );
    model->SetPluginModel( plModel );

    //Set default values of all parameters
    alphaEvaluator->Parameter()->SetVal( 1.f, TimeType( 0.0 ) );
    blurSizeEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.0 ) );
    spacingEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.0 ) );
    timerTimeEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.0 ) );
    borderColorEvaluator->Parameter()->SetVal( glm::vec4( 0.f, 0.f, 0.f, 0.f ), TimeType( 0.f ) );
    fontSizeEvaluator->Parameter()->SetVal( 8.f, TimeType( 0.f ) );

    return model;
}

// *******************************
//
bool                   DefaultTimerPluginDesc::CanBeAttachedTo   ( IPluginConstPtr plugin ) const
{
    if ( plugin == nullptr )
    {
        return true;
    }

    auto  vac = plugin->GetVertexAttributesChannel();
    if ( vac != nullptr )
    {
        return false;
    }

    return true;
}

// *******************************
//
std::string             DefaultTimerPluginDesc::UID                      ()
{
    return "DEFAULT_TIMER";
}

// *******************************
//
std::string             DefaultTimerPluginDesc::VertexShaderSource       ()
{
    return "../dep/media/shaders/dummy.vert";   //FIXME: deprecated
}

// *******************************
//
std::string             DefaultTimerPluginDesc::PixelShaderSource        ()
{
    return "../dep/media/shaders/dummy.frag";   //FIXME: deprecated
}

// *******************************
//
std::string             DefaultTimerPluginDesc::TextureName              ()
{
    return "AtlasTex0";
}

////////////////////////////
//
namespace
{

bool IsPlaceHolder(wchar_t wch)
{
    return  wch == L'H'
        ||  wch == L'M'
        ||  wch == L'S'
        ||  wch == L's';
}

void TimeFormatError()
{
    throw std::exception("wrong time format");
}

TimeInfo ParseTimePatern(const std::wstring& timePatern)
{
    bool HPHPossible = true;
    bool MPHPossible = true;
    bool SPHPossible = true;
    bool FSPHPossible = true;

    TimeInfo timeInfo = {0,0,0,0};

    int i = 0;
    int whiteSpaces = 0;
    for( auto wch : timePatern )
    {
        if(IsPlaceHolder(wch))
        {
            switch ( wch )
            {
            case L'H':
                if( !HPHPossible ) TimeFormatError();
                if( timeInfo.hoursPlaceholderSize == 0 ) timeInfo.hoursPHStart = i - whiteSpaces;
                timeInfo.hoursPlaceholderSize++;
                break;
            case L'M':
                if(!MPHPossible) TimeFormatError();
                if( timeInfo.minutesPlaceHolderSize == 0 ) timeInfo.minutesPHStart = i - whiteSpaces;
                HPHPossible = false;
                if(timeInfo.minutesPlaceHolderSize < 2)
                    timeInfo.minutesPlaceHolderSize++;
                else
                    TimeFormatError();
                break;
            case L'S':
                if(!SPHPossible) TimeFormatError();
                if( timeInfo.secondsPlaceHolderSize == 0 ) timeInfo.secondsPHStart = i - whiteSpaces;
                HPHPossible = false;
                MPHPossible = false;
                if(timeInfo.secondsPlaceHolderSize < 2)
                    timeInfo.secondsPlaceHolderSize++;
                else
                    TimeFormatError();
                break;
            case L's':
                if( timeInfo.fracOfSecondsPlaceholderSize == 0 ) timeInfo.fosPHStart = i - whiteSpaces;
                HPHPossible = false;
                MPHPossible = false;
                SPHPossible = false;
                if(!FSPHPossible) TimeFormatError();
                timeInfo.fracOfSecondsPlaceholderSize++;
                break;
            }
        }

        if(isspace(wch))
            whiteSpaces++;

        ++i;
    }

    return timeInfo;
}

} // anonymous


////////////////////////////
//
bool    TimeValue::operator!=(const TimeValue& other) const
{
    return  other.fracOfSecond == this->fracOfSecond
        ||  other.second == this->second
        ||  other.minute == this->minute
        ||  other.hour == this->hour;
}

////////////////////////////
//
TimeValue::TimeValue( double time, int accuracy )
{
    this->fracOfSecond = int( ( time - floor( time ) ) * pow( 10, accuracy ) );
    this->second    = int( time ) % 60;
    this->minute    = int( time / 60 ) % 60;
    this->hour      = int( time / (60 * 60 ) );
}

// *************************************
//
DefaultTimerPlugin::DefaultTimerPlugin  ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin< IPlugin >( name, uid, prev, std::static_pointer_cast< IPluginParamValModel >( model ) )
    , m_fontResource()
    , m_paramValModel( model )
    , m_textAtlas()
    , m_timePatern( )
    , m_currentTime( 0.0 )
    , m_defaultSeparator(L':')
{
    auto colorParam = prev->GetParameter( "color" );

    if ( colorParam == nullptr )
    {
        auto bcParam = this->GetParameter( "borderColor" );
        SimpleVec4EvaluatorPtr      colorEvaluator = ParamValEvaluatorFactory::CreateSimpleVec4Evaluator( "color", bcParam->GetTimeEvaluator() );
        std::static_pointer_cast< DefaultParamValModel >( m_paramValModel->GetPixelShaderChannelModel() )->RegisterAll( colorEvaluator );
        colorEvaluator->Parameter()->SetVal( glm::vec4( 1.f, 1.f, 1.f, 1.f ), TimeType( 0.f ) );
    }
    else
    {
        auto evaluators = prev->GetPluginParamValModel()->GetPixelShaderChannelModel()->GetEvaluators();
        for( unsigned int i = 0; i < evaluators.size(); ++i )
        {
            auto colorParam = evaluators[ i ]->GetParameter( "color" );
            if( colorParam != nullptr )
            {
                //FIXME: upewnic sie, ze to nie hack (wszystko sie raczej zwalania, jesli sa ptry, ale jednak)
                std::static_pointer_cast< DefaultParamValModel >( m_paramValModel->GetPixelShaderChannelModel() )->RegisterAll( evaluators[ i ] );
                break;
            }
        }
        
    }


    m_timeParam     = QueryTypedParam< ParamFloatPtr >( GetPluginParamValModel()->GetPluginModel()->GetParameter( "time" ) );
    m_spacingParam  = QueryTypedParam< ParamFloatPtr >( GetPluginParamValModel()->GetPluginModel()->GetParameter( "spacing" ) );

    m_psc = DefaultPixelShaderChannelPtr( DefaultPixelShaderChannel::Create( DefaultTimerPluginDesc::PixelShaderSource(), model->GetPixelShaderChannelModel(), nullptr ) );
    m_vsc = DefaultVertexShaderChannelPtr( DefaultVertexShaderChannel::Create( DefaultTimerPluginDesc::VertexShaderSource(), model->GetVertexShaderChannelModel() ) );

    auto ctx = m_psc->GetRendererContext();
    ctx->cullCtx->enabled = false;
    ctx->alphaCtx->blendEnabled = true;

    ctx->alphaCtx->srcBlendMode = model::AlphaContext::SrcBlendMode::SBM_ONE;
    ctx->alphaCtx->dstBlendMode = model::AlphaContext::DstBlendMode::DBM_ONE_MINUS_SRC_ALPHA;

    m_fontSizeParam = QueryTypedParam< ParamFloatPtr >( GetPluginParamValModel()->GetPluginModel()->GetParameter( "fontSize" ) );
    m_blurSizeParam = QueryTypedParam< ParamFloatPtr >( GetPluginParamValModel()->GetPluginModel()->GetParameter( "blurSize" ) );
    m_spacingParam  = QueryTypedParam< ParamFloatPtr >( GetPluginParamValModel()->GetPluginModel()->GetParameter( "spacing" ) );
}

// *************************************
//
DefaultTimerPlugin::~DefaultTimerPlugin  ( )
{

}

namespace {
// *************************************
// FIXME: implement int parameters and bool parameters
template< typename EnumClassType >
inline EnumClassType EvaluateAsInt( ParamFloatPtr param )
{
    int val = int( param->Evaluate() );

    return EnumClassType( val );
}

} //anonymous


// *************************************
//
void DefaultTimerPlugin::InitAttributesChannel( IPluginPtr prev )
{
}

// *************************************
// 
bool            DefaultTimerPlugin::LoadResource  ( IPluginResourceDescrConstPtr resDescr )
{
    auto txResDescr = QueryFontResourceDescr( resDescr );

    // FIXME: dodac tutaj API pozwalajace tez ustawiac parametry dodawanej tekstury (normalny load z dodatkowymi parametrami)
    if ( txResDescr != nullptr )
    {
        auto txData = m_psc->GetTexturesDataImpl();
        assert( txData->GetTextures().size() <= 1 );

        auto fontResource = TextHelper::LoadFont( txResDescr->GetFontFile(), int( m_fontSizeParam->Evaluate() ), int( m_blurSizeParam->Evaluate() ) );

        m_textAtlas = TextHelper::GetAtlas( fontResource.get(), false, false );

        auto textureResource = TextHelper::GetAtlasTextureInfo( m_textAtlas );

        auto txInfo = new TextureInfo( textureResource.get(), DefaultTimerPluginDesc::TextureName() );

        //FIXME: use some better API to handle resources in general and textures in this specific case
        auto txDesc = new DefaultTextureDescriptor(     textureResource
                                                    ,   DefaultTimerPluginDesc::TextureName()
                                                    ,   TextureWrappingMode::TWM_CLAMP_BORDER
                                                    ,   TextureWrappingMode::TWM_CLAMP_BORDER
                                                    ,   TextureFilteringMode::TFM_LINEAR
                                                    ,   glm::vec4( 0.f, 0.f, 0.f, 0.f )
                                                    ,   DataBuffer::Semantic::S_TEXTURE_STATIC );

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

            m_vaChannel = VertexAttributesChannelPtr( TextHelper::CreateEmptyVACForText() );

            SetTimePatern( L"M:SS:ss" );

            return true;
        }
    }    

    return false;
}

// *************************************
// 
IVertexAttributesChannelConstPtr    DefaultTimerPlugin::GetVertexAttributesChannel  () const
{
    return m_vaChannel;
}

// *************************************
// 
IPixelShaderChannelConstPtr         DefaultTimerPlugin::GetPixelShaderChannel       () const
{
    return m_psc;
}

// *************************************
// 
IVertexShaderChannelConstPtr        DefaultTimerPlugin::GetVertexShaderChannel      () const
{
    return m_vsc;
}

// *************************************
// 
void                                DefaultTimerPlugin::Update                      ( TimeType t )
{
    //FIXME: UPDATER TO FIX
    float time = m_timeParam->Evaluate();

    SetTime( time );

    m_paramValModel->Update();

    m_vsc->PostUpdate();
    m_psc->PostUpdate();    
}


////////////////////////////
//
namespace
{

////////////////////////////
//
std::wstring        toTextPatern( const std::wstring& timePatern )
{
    auto timePaternCopy = timePatern;
    std::replace( timePaternCopy.begin(), timePaternCopy.end(), L'H', L'#');
    std::replace( timePaternCopy.begin(), timePaternCopy.end(), L'M', L'#');
    std::replace( timePaternCopy.begin(), timePaternCopy.end(), L'S', L'#');
    std::replace( timePaternCopy.begin(), timePaternCopy.end(), L's', L'#');

    return timePaternCopy;
}

////////////////////////////
//
std::wstring        toTimerInit( const std::wstring& timePatern )
{
    auto timePaternCopy = timePatern;
    std::replace( timePaternCopy.begin(), timePaternCopy.end(), L'H', L'0');
    std::replace( timePaternCopy.begin(), timePaternCopy.end(), L'M', L'0');
    std::replace( timePaternCopy.begin(), timePaternCopy.end(), L'S', L'0');
    std::replace( timePaternCopy.begin(), timePaternCopy.end(), L's', L'0');

    return timePaternCopy;
}

} // anonymous

////////////////////////////
//
void                                DefaultTimerPlugin::SetTimePatern  ( const std::wstring& patern )
{
    m_timePatern = patern;
    m_timePaternInfo = ParseTimePatern( m_timePatern );

    auto textPatern = toTextPatern( m_timePatern );
    auto timerInit = toTimerInit( m_timePatern );

    m_vaChannel->ClearConnectedComponent();

    TextHelper::BuildVACForText( m_vaChannel.get(), m_textAtlas, timerInit, unsigned int( m_blurSizeParam->Evaluate() ), m_spacingParam->Evaluate(), TextAlignmentType::Left, false );

    //m_vaChannel->SetNeedsTopologyUpdate( true );
}

////////////////////////////
//
const GlyphCoords&                  DefaultTimerPlugin::GetGlyphCoords  ( wchar_t wch ) const
{
    auto glyphCoords = m_textAtlas->GetGlyphCoords( wch );
    if( glyphCoords )
        return *glyphCoords;
    else
    {
        assert( !( "Cannot find glyph for char " + wch) );
        throw;
    }
}

////////////////////////////
//
int TimeInfo::GetSize() const
{
    return  hoursPlaceholderSize
        +   minutesPlaceHolderSize
        +   secondsPlaceHolderSize
        +   fracOfSecondsPlaceholderSize;
}

////////////////////////////
//
void                                DefaultTimerPlugin::Refresh         ()
{
    int hPHSize = m_timePaternInfo.hoursPlaceholderSize;
    int shift = m_timePaternInfo.hoursPHStart;
    if( hPHSize > 0 )
    {
        int hour = m_currentTime.hour;
        auto hourStr = std::to_wstring( hour );

        int zerosBefore = hPHSize > (int)hourStr.size() ? ( hPHSize - (int)hourStr.size() ) : 0;

        int i = 0;
        for(; i < zerosBefore ; ++i )
        {
            SetValue( shift + i, L'0' );
        }

        for(; i < hPHSize; ++i )
        {
            SetValue( shift + i, hourStr[i - zerosBefore] );
        }       
    }

    shift = m_timePaternInfo.minutesPHStart;
    int mPHSize = m_timePaternInfo.minutesPlaceHolderSize;
    if( mPHSize > 0 )
    {
        int minute = m_currentTime.minute;
        auto minuteStr = std::to_wstring( minute );

        int zerosBefore = mPHSize > (int)minuteStr.size() ? ( mPHSize - (int)minuteStr.size() ) : 0;

        int i = 0;
        for(; i < zerosBefore ; ++i )
        {
            SetValue( shift + i, L'0' );
        }

        for(; i < mPHSize; ++i )
        {
            SetValue( shift + i, minuteStr[i - zerosBefore] );
        }       
    }

    shift = m_timePaternInfo.secondsPHStart;
    int sPHSize = m_timePaternInfo.secondsPlaceHolderSize;
    if( sPHSize > 0 )
    {
        int second = m_currentTime.second;
        auto secondStr = std::to_wstring( second );

        int zerosBefore = sPHSize > (int)secondStr.size() ? ( sPHSize - (int)secondStr.size() ) : 0;

        int i = 0;
        for(; i < zerosBefore ; ++i )
        {
            SetValue( shift + i, L'0' );
        }

        for(; i < sPHSize; ++i )
        {
            SetValue( shift + i, secondStr[i - zerosBefore] );
        }       
    }

    shift = m_timePaternInfo.fosPHStart;
    int fosPHSize = m_timePaternInfo.fracOfSecondsPlaceholderSize;
    if( fosPHSize > 0 )
    {
        int fos = m_currentTime.fracOfSecond;
        auto fosStr = std::to_wstring( fos );

        int zerosBefore = fosPHSize > (int)fosStr.size() ? ( fosPHSize - (int)fosStr.size() ) : 0;

        int i = 0;
        for(; i < zerosBefore ; ++i )
        {
            SetValue( shift + i, L'0' );
        }

        for(; i < fosPHSize; ++i )
        {
            SetValue( shift + i, fosStr[i - zerosBefore] );
        }       
    }
}

////////////////////////////
//
void                                DefaultTimerPlugin::SetValue       ( unsigned int connComp, wchar_t wch )
{
    auto comps = m_vaChannel->GetComponents();

    auto& coords = GetGlyphCoords( wch );
    auto& zeroCoords = GetGlyphCoords( L'0' );

    auto textureXNorm    = ((float)coords.textureX + (float)zeroCoords.glyphX )  / m_textAtlas->GetWidth();
    auto textureYNorm    = ((float)coords.textureY + (float)zeroCoords.glyphY )  / m_textAtlas->GetHeight();
    auto widthNorm       = ((float)zeroCoords.glyphWidth )     / m_textAtlas->GetWidth();
    auto heightNorm      = ((float)zeroCoords.glyphHeight )    / m_textAtlas->GetHeight();

    //auto textureXNorm = ( float )coords.textureX / m_textAtlas->GetWidth();
    //auto textureYNorm = ( float )coords.textureY / m_textAtlas->GetHeight();

    //auto widthNorm  = ( float )coords.width / m_textAtlas->GetWidth();
    //auto heightNorm = ( float )coords.height / m_textAtlas->GetHeight();

    if( IsPlaceHolder( m_timePatern[ connComp ] ) )
        if( connComp < comps.size() )
        {
            if( comps[ connComp ]->GetNumVertices() == 4 )
            {
                auto channels = comps[ connComp ]->GetAttributeChannels();

                auto uvChannel = std::static_pointer_cast< Float2AttributeChannel >( AttributeChannel::GetUVChannel( channels, 1 ) );

                auto& verts = uvChannel->GetVertices();

                verts[ 0 ] = glm::vec2( textureXNorm, textureYNorm + heightNorm );
                verts[ 1 ] = glm::vec2( textureXNorm  + widthNorm, textureYNorm + heightNorm );
                verts[ 2 ] = glm::vec2( textureXNorm ,textureYNorm );
                verts[ 3 ] = glm::vec2( textureXNorm + widthNorm, textureYNorm );
            }
        }
}

////////////////////////////
//
bool                                DefaultTimerPlugin::CheckTimeConsistency ( const std::wstring& time ) const
{
    if( m_timePatern.size() != time.size() )
        return false;

    for( unsigned int i = 0; i < m_timePatern.size(); ++i )
    {
        if( m_timePatern[ i ] == '#' )
        {
            if( time[ i ] >= '0' && time[ i ] <= '9' )
            {
                continue;
            }
        }
        else
        {
            if( m_timePatern[ i ] == time[ i ] )
                continue;
        }

        return false;
    }

    return true;
}

////////////////////////////
//
void                                DefaultTimerPlugin::SetTime        ( const std::wstring& time )
{
    if( !CheckTimeConsistency( time ) )
    {
        //std::cerr << time << L" doesn't match patern " << m_timePatern. << std::endl;
        return;
    }

    for( unsigned int i = 0; i < time.size(); ++i )
    {
        SetValue( i, time[ i ] );
    }
}

////////////////////////////
//
void                                DefaultTimerPlugin::SetTime        ( double time )
{
    TimeValue   newTime( time, m_timePaternInfo.fracOfSecondsPlaceholderSize );

    if( m_currentTime  != newTime )
    {
        m_currentTime = newTime;
        Refresh();
        m_vaChannel->SetNeedsAttributesUpdate( true );
    }
}

} // model
} // bv
