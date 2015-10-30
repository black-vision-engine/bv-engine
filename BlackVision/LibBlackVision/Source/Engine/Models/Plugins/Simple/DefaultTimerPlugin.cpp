#include "DefaultTimerPlugin.h"

#include "Assets/Font/TextHelper.h"
#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"
#include "Engine/Models/Plugins/Channels/Geometry/HelperVertexAttributesChannel.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"
#include "Assets/Font/FontLoader.h"
#include "Assets/Font/Text.h"
#include "Assets/Font/Glyph.h"
#include "Assets/Font/FontAssetDescriptor.h"

#include <algorithm>

#include <Windows.h>

namespace bv { namespace model {

// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultTimerPluginDesc::DefaultTimerPluginDesc                            ()
    : BasePluginDescriptor( UID(), "timer", "txt" ) // FIXME: it does not really look right :/
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
	SimpleFloatEvaluatorPtr     outlineSizeEvaluator    = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "outlineSize", timeEvaluator );

    SimpleFloatEvaluatorPtr     spacingEvaluator        = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "spacing", timeEvaluator );
    SimpleFloatEvaluatorPtr     alignmentEvaluator      = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "alignment", timeEvaluator );

    SimpleFloatEvaluatorPtr     precisionEvaluator      = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "precision", timeEvaluator );

    //Register all parameters and evaloators in models
    psModel->RegisterAll( borderColorEvaluator );
    psModel->RegisterAll( alphaEvaluator );
	plModel->RegisterAll( blurSizeEvaluator );
    plModel->RegisterAll( outlineSizeEvaluator );
    plModel->RegisterAll( spacingEvaluator );
    plModel->RegisterAll( fontSizeEvaluator );
    plModel->RegisterAll( alignmentEvaluator );
    plModel->RegisterAll( precisionEvaluator );

    //Set models structure
    model->SetVertexShaderChannelModel( vsModel );
    model->SetPixelShaderChannelModel( psModel );
    model->SetPluginModel( plModel );

    //Set default values of all parameters
    alphaEvaluator->Parameter()->SetVal( 1.f, TimeType( 0.0 ) );
    blurSizeEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.0 ) );
	outlineSizeEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.0 ) );
    spacingEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.0 ) );
    borderColorEvaluator->Parameter()->SetVal( glm::vec4( 0.f, 0.f, 0.f, 0.f ), TimeType( 0.f ) );
    fontSizeEvaluator->Parameter()->SetVal( 8.f, TimeType( 0.f ) );
    alignmentEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.0 ) );
    precisionEvaluator->Parameter()->SetVal( 1.f, TimeType( 0.0 ) );

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
    return  other.fracOfSecond != this->fracOfSecond
        ||  other.second != this->second
        ||  other.minute != this->minute
        ||  other.hour != this->hour;
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
void		DefaultTimerPlugin::SetPrevPlugin		( IPluginPtr prev )
{
    BasePlugin::SetPrevPlugin( prev );

    if( prev == nullptr )
	{
        return;
	}

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
}

// *************************************
//
DefaultTimerPlugin::DefaultTimerPlugin  ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin< IPlugin >( name, uid, prev, std::static_pointer_cast< IPluginParamValModel >( model ) )
    , m_paramValModel( model )
    , m_textAtlas()
    , m_timePatern( )
    , m_globalStartTime( 0 )
    , m_localStartTime( 0 )
    , m_currentTimeValue( 0.0 )
    , m_defaultSeparator(L':')
    , m_secSeparator(L'.')
    , m_widestGlyph( L'0' ) 
    , m_started(false)
{
    m_spacingParam  = QueryTypedParam< ParamFloatPtr >( GetPluginParamValModel()->GetPluginModel()->GetParameter( "spacing" ) );

    m_psc = DefaultPixelShaderChannelPtr( DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel(), nullptr ) );
    m_vsc = DefaultVertexShaderChannelPtr( DefaultVertexShaderChannel::Create( model->GetVertexShaderChannelModel() ) );
	
	SetPrevPlugin( prev );

    auto ctx = m_psc->GetRendererContext();
    ctx->cullCtx->enabled = false;
    ctx->alphaCtx->blendEnabled = true;

    ctx->alphaCtx->srcBlendMode = model::AlphaContext::SrcBlendMode::SBM_ONE;
    ctx->alphaCtx->dstBlendMode = model::AlphaContext::DstBlendMode::DBM_ONE_MINUS_SRC_ALPHA;

    m_fontSizeParam     = QueryTypedParam< ParamFloatPtr >( GetPluginParamValModel()->GetPluginModel()->GetParameter( "fontSize" ) );
    m_blurSizeParam     = QueryTypedParam< ParamFloatPtr >( GetPluginParamValModel()->GetPluginModel()->GetParameter( "blurSize" ) );
	m_outlineSizeParam  = QueryTypedParam< ParamFloatPtr >( GetPluginParamValModel()->GetPluginModel()->GetParameter( "outlineSize" ) );
    m_spacingParam      = QueryTypedParam< ParamFloatPtr >( GetPluginParamValModel()->GetPluginModel()->GetParameter( "spacing" ) );
    m_alignmentParam    = QueryTypedParam< ParamFloatPtr >( GetPluginParamValModel()->GetPluginModel()->GetParameter( "alignment" ) );
    m_precisionParam    = QueryTypedParam< ParamFloatPtr >( GetPluginParamValModel()->GetPluginModel()->GetParameter( "precision" ) );
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
bool            DefaultTimerPlugin::LoadResource  ( AssetDescConstPtr assetDescr )
{
	auto txAssetDescr = QueryTypedDesc< FontAssetDescConstPtr >( assetDescr );

    // FIXME: dodac tutaj API pozwalajace tez ustawiac parametry dodawanej tekstury (normalny load z dodatkowymi parametrami)
    if ( txAssetDescr != nullptr )
    {
        auto txData = m_psc->GetTexturesDataImpl();
        assert( txData->GetTextures().size() <= 1 );

		auto fontResource = LoadTypedAsset<FontAsset>( txAssetDescr );

        m_textAtlas = TextHelper::GetAtlas( fontResource );

        InitBigestGlyph();

		auto textureResource = m_textAtlas->GetAsset();

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
			if( !txData->SetTexture( 0, txDesc ) )
			{
				txData->AddTexture( txDesc );
			}

            m_vaChannel = VertexAttributesChannelPtr( TextHelper::CreateEmptyVACForText() );

            SetTimePatern( GenerateTimePatern( 0.f ) );

            SetTime(0.);

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
void                                DefaultTimerPlugin::Update                      ( TimeType )
{
    //FIXME: UPDATER TO FIX
    if( m_started )
    {
        m_currentLocalTime = m_localStartTime + (GetTickCount() -  m_globalStartTime);
    }

    SetTime( m_currentLocalTime / 1000.f );

    m_paramValModel->Update();

    m_vsc->PostUpdate();
    m_psc->PostUpdate();    
}

// *************************************
// 
void                                DefaultTimerPlugin::Start                       ()
{
    if(!m_started)
    {
        m_started = true;
        m_globalStartTime = GetTickCount();
    }
}

// *************************************
// 
void                                DefaultTimerPlugin::Stop                        ()
{
    if(m_started)
    {
        m_started = false;
        m_localStartTime = m_currentLocalTime;
        Refresh( true );
    }
}

// *************************************
// 
void                                DefaultTimerPlugin::Reset                       ( float localTime )
{
    m_currentLocalTime = unsigned long ( localTime * 1000.f );
    m_localStartTime = m_currentLocalTime;
    m_globalStartTime = GetTickCount();
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
std::wstring        toTimerInit( const std::wstring& timePatern, wchar_t wch )
{
    auto timePaternCopy = timePatern;
    std::replace( timePaternCopy.begin(), timePaternCopy.end(), L'H', wch);
    std::replace( timePaternCopy.begin(), timePaternCopy.end(), L'M', wch);
    std::replace( timePaternCopy.begin(), timePaternCopy.end(), L'S', wch);
    std::replace( timePaternCopy.begin(), timePaternCopy.end(), L's', wch);

    return timePaternCopy;
}

} // anonymous

////////////////////////////
//
void                                DefaultTimerPlugin::SetTimePatern  ( const std::wstring& patern )
{
    if( m_timePatern == patern )
        return;

    m_timePatern = patern;
    m_timePaternInfo = ParseTimePatern( m_timePatern );

    auto timerInit = toTimerInit( m_timePatern, m_widestGlyph );

    m_vaChannel->ClearAll();

    auto alignType =  EvaluateAsInt< TextAlignmentType >( m_alignmentParam );

    TextHelper::BuildVACForText( m_vaChannel.get(), m_textAtlas, timerInit, unsigned int( m_blurSizeParam->Evaluate() ), m_spacingParam->Evaluate(), alignType, false );
 
	HelperVertexAttributesChannel::SetTopologyUpdate( m_vaChannel, true );
    //m_vaChannel->SetNeedsTopologyUpdate( true );
}

////////////////////////////
//
const Glyph *						DefaultTimerPlugin::GetGlyph	( wchar_t wch ) const
{
    auto glyph = m_textAtlas->GetGlyph( wch );
    if( glyph )
        return glyph;
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
void                                DefaultTimerPlugin::Refresh         ( bool isPaused )
{
    int hPHSize = m_timePaternInfo.hoursPlaceholderSize;
    int shift = m_timePaternInfo.hoursPHStart;
    if( hPHSize > 0 )
    {
        int hour = m_currentTimeValue.hour;
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
        int minute = m_currentTimeValue.minute;
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
        int second = m_currentTimeValue.second;
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

    int prec =  EvaluateAsInt< int >( m_precisionParam );

    prec = prec < 0 ? 0 : prec;

    if( fosPHSize > 0 )
    {
        int fos = m_currentTimeValue.fracOfSecond;
        auto fosStr = std::to_wstring( fos );

        int zerosBefore = fosPHSize > (int)fosStr.size() ? ( fosPHSize - (int)fosStr.size() ) : 0;

        int i = 0;
        for(; i < zerosBefore ; ++i )
        {
            if( i > prec - 1 && ! isPaused )
                SetValue( shift + i, L' ' );
            else
                SetValue( shift + i, L'0' );
        }

        for(; i < fosPHSize; ++i )
        {
            if( i > prec - 1 && ! isPaused )
                SetValue( shift + i, L' ' );
            else
                SetValue( shift + i, fosStr[i - zerosBefore] );
        }       
    }
}

////////////////////////////
//
void                                DefaultTimerPlugin::SetValue       ( unsigned int connComp, wchar_t wch )
{
    auto comps = m_vaChannel->GetComponents();

    auto textureXNorm    = 0.f;
    auto textureYNorm    = 0.f;
    auto widthNorm       = 0.f;
    auto heightNorm      = 0.f;

    if( wch != L' ' )
    {
        auto glyph = GetGlyph( wch );
        auto zeroGlyph = GetGlyph( m_widestGlyph );

        textureXNorm    = ((float)glyph->textureX /*+ (float)zeroGlyph->glyphX - 1.f*/ )  / m_textAtlas->GetWidth();
        textureYNorm    = ((float)glyph->textureY /*+ (float)zeroGlyph->glyphY - 1.f*/ )  / m_textAtlas->GetHeight();
        widthNorm       = ((float)zeroGlyph->width + 2.f )     / m_textAtlas->GetWidth();
        heightNorm      = ((float)zeroGlyph->height + 2.f )    / m_textAtlas->GetHeight();
    }

    if( IsPlaceHolder( m_timePatern[ connComp ] ) )
        if( connComp < comps.size() )
        {
            if( comps[ connComp ]->GetNumVertices() == 4 )
            {
                auto channels = comps[ connComp ]->GetAttributeChannels();

                auto uvChannel = std::static_pointer_cast< Float2AttributeChannel >( AttributeChannel::GetAttrChannel( channels, AttributeSemantic::AS_TEXCOORD ) );

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
void                              DefaultTimerPlugin::InitBigestGlyph ()
{
    SizeType width   = 0;
    wchar_t widest       = L'0';

    static const std::wstring numbers = L"0123456789";

    for( auto wch : numbers )
    {
        auto w = m_textAtlas->GetGlyph( wch )->width;
        if( w > width )
        {
            width = w;
            widest = wch;
        }
    }
    
    m_widestGlyph = widest;
}

////////////////////////////
//
std::wstring                        DefaultTimerPlugin::GenerateTimePatern( double time )
{
    double seconds  = std::floor( time );

    double hour     = std::floor( seconds / 3600. );
    double minute   = std::floor( ( time - hour ) / 60. );
    double second   = std::floor( time - hour - minute );
    // double hos      = std::floor( time - seconds * 100. );

    std::wstring ret;

    int ihour       = int( hour );

    if( ihour > 0 )
    {
        int places = int( std::floor( std::log10( ihour ) ) );
        auto hp = std::wstring( places + 1, L'H' );
        ret.append( hp );
        ret.push_back( m_defaultSeparator );
        ret.append( L"MM" );
        ret.push_back( m_defaultSeparator );
        ret.append( L"SS" );
        ret.push_back( m_secSeparator );
        ret.append( L"ss" );
    }
    else
    {
        auto iminute = int( minute );
        if( iminute > 0 )
        {
            if( iminute >= 10 )
                ret.append( L"MM" );
            else
                ret.push_back( L'M' );

            ret.push_back( m_defaultSeparator );
            ret.append( L"SS" );
            ret.push_back( m_secSeparator );
            ret.append( L"ss" );
        }
        else
        {
            auto isecond = int( second );

            if( isecond >= 10 )
                ret.append( L"SS" );
            else
                ret.push_back( L'S' );

            ret.push_back( m_secSeparator );
            ret.append( L"ss" );
        }
    }

    return ret;
}


////////////////////////////
//
void                                DefaultTimerPlugin::SetTime        ( TimeType time )
{
    TimeValue   newTime( time, m_timePaternInfo.fracOfSecondsPlaceholderSize );

    if( m_currentTimeValue  != newTime )
    {
        SetTimePatern( GenerateTimePatern( time ) );
        m_currentTimeValue = newTime;
        Refresh( !m_started );
        //if( ! m_vaChannel->NeedsTopologyUpdate() )
        //{
        //    m_vaChannel->SetNeedsAttributesUpdate( true );
        //}
    }
}

////////////////////////////
//
void                                DefaultTimerPlugin::SetTime         ( int h, int m, int s, int hoSec )
{
    assert( hoSec < 100 );
    SetTime( TimeType( h * 3600. + m * 60 + s + hoSec / 100.f ) );
}

// *************************************
//
bool            SetTimeTimerPlugin( IPluginPtr timerPlugin, TimeType time )
{
    if( timerPlugin->GetTypeUid() == DefaultTimerPluginDesc::UID() )
    {
        std::static_pointer_cast< DefaultTimerPlugin >( timerPlugin )->Reset( time );
        return true;
    }
    else
        return false;
}

// *************************************
//
bool            StartTimerPlugin( IPluginPtr timerPlugin )
{
    if( timerPlugin->GetTypeUid() == DefaultTimerPluginDesc::UID() )
    {
        std::static_pointer_cast< DefaultTimerPlugin >( timerPlugin )->Start();
        return true;
    }
    else
        return false;
}

// *************************************
//
bool            StopTimerPlugin( IPluginPtr timerPlugin )
{
    if( timerPlugin->GetTypeUid() == DefaultTimerPluginDesc::UID() )
    {
        std::static_pointer_cast< DefaultTimerPlugin >( timerPlugin )->Stop();
        return true;
    }
    else
        return false;
}

// *************************************
//
bool            ResetTimerPlugin( IPluginPtr timerPlugin )
{
    if( timerPlugin->GetTypeUid() == DefaultTimerPluginDesc::UID() )
    {
        std::static_pointer_cast< DefaultTimerPlugin >( timerPlugin )->Reset( 0.f );
        return true;
    }
    else
        return false;
}

} // model
} // bv
