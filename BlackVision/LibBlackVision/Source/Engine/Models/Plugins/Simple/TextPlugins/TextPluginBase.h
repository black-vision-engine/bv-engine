#pragma once

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Assets/Asset.h"
#include "Assets/Font/2D/TextAtlas.h"

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTextureDescriptor.h"

#include "Assets/Font/TextHelper.h"

#include "Engine/Types/TypeTraits.h"

namespace bv { namespace model {

typedef ParamEnum< TextAlignmentType > ParamEnumTAT;
DEFINE_PTR_TYPE( ParamEnumTAT );

// ***************************** DESCRIPTOR **********************************
class TextPluginBaseDesc : public BasePluginDescriptor
{
public:

    TextPluginBaseDesc                                          ( const std::string & uid, const std::string & defaultName, const std::string & abbrv );

    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
};

template< typename T >
struct ValueParamState
{
	const static ParamType PT = Value2ParamTypeTrait< T >::ParamT;

	typedef typename ValueT< T >::Type																			VType;
	typedef SimpleParameterImpl< CompositeInterpolator< TimeType,
								typename Type2InterpolatorType< T >::Type >, 
								typename Type2InterpolatorType< T >::Type,
		ParamType2ModelParamType< PT >::MPT >	PType;
	typedef SimpleState< T >																					SType;

	const VType *			valuePtr;
	const PType *			paramPtr;
	const IStatedValue *	statePtr;

	ValueParamState()
		: valuePtr( nullptr )
		, paramPtr( nullptr )
		, statePtr( nullptr )
	{}

	ValueParamState( const VType * value, const PType * param, const IStatedValue * state )
		: valuePtr( value )
		, paramPtr( param )
		, statePtr( state )
	{}

	const T &		GetValue() const
	{
		assert( valuePtr != nullptr );
		return valuePtr->GetValue();
	}

	const PType &	GetParameter() const
	{
		assert( paramPtr != nullptr );
		return *paramPtr;
	}

	//PType &			GetParameter()
	//{
	//	assert( paramPtr != nullptr )
	//	return *paramPtr;
	//}

	bool			Changed() const
	{
		assert( statePtr != nullptr );
		return statePtr->StateChanged();
	}
};

template< typename T >
ValueParamState< T >		GetValueParamState( IParamValModel * paramValModel, const std::string & name )
{
	typedef typename ValueParamState< T >::VType	VType;
	typedef typename ValueParamState< T >::PType	PType;
	
	auto v = paramValModel->GetValue( name );
	auto p = paramValModel->GetParameter( name );

	auto vT = v ? QueryTypedValue< VType >( v.get() ) : nullptr;
	auto pT = p ? QueryTypedParam< PType >( p.get() ) : nullptr;

	return ValueParamState< T >( vT, pT, paramValModel->GetState( name ).get() );
}

class TextPluginBase : public BasePlugin
{
public:
    struct PARAM
    {
        static const std::string        ALPHA;
        static const std::string        FONT_SIZE;
        static const std::string        NEW_LINE_SIZE;
        static const std::string        TEXT_BOX;
        static const std::string        USE_TEXT_BOX;

        static const std::string        SHADOW_ENABLED;
        static const std::string        SHADOW_COLOR;
        static const std::string        SHADOW_TX;

        static const std::string        OUTLINE_ENABLED;
        static const std::string        OUTLINE_COLOR;
        static const std::string        OUTLINE_TX;

        static const std::string        GLOW_ENABLED;
        static const std::string        GLOW_STRENGTH;
        static const std::string        GLOW_COLOR;

        static const std::string        SPACING;
        static const std::string        ALIGNEMENT;
        static const std::string        ALIGN_CHARACTER;
        static const std::string        FIRST_TEXT_CC;
        static const std::string        FIRST_TEXT_OUT_CC;
        static const std::string        FIRST_TEXT_SH_CC;
        static const std::string        FIRST_TEXT_GLOW_CC;
    };

protected:
    DefaultPixelShaderChannelPtr    m_psc;
    DefaultVertexShaderChannelPtr   m_vsc;

    VertexAttributesChannelPtr      m_vaChannel;

    SizeType                        m_fontSize;
    SizeType                        m_blurSize;
    SizeType                        m_glowBlurSize;
    SizeType                        m_outlineSize;

    TextAtlasConstPtr               m_atlas;

	ValueParamState< TextAlignmentType > m_alignment;

	ValueParamState< Float32 >      m_newLineSize;
	ValueParamState< Float32 >		m_spacing;

	ValueParamState< glm::vec2 >    m_textBox;
	ValueParamState< bool >			m_useTextBox;

	ValueParamState< bool >			m_shadowEnabled;
	ValueParamState< bool >         m_outlineEnabled;
	ValueParamState< bool >         m_glowEnabled;

	ValueParamState< Int32 >        m_alignCharacter;

    ParamTransformPtr               m_outlineTxParam;
    ParamTransformPtr               m_glowTxParam;


    ValueIntPtr                     m_firstTextCC;
    ValueIntPtr                     m_firstTextOutCC;
    ValueIntPtr                     m_firstTextGlowCC;
    ValueIntPtr                     m_firstTextShCC;

    bool                                        LoadResource                ( AssetDescConstPtr assetDescr, const std::string & name );
    bool                                        LoadAtlas                   ( const FontAssetDescConstPtr & fontAssetDesc, const std::string & name );

public:

    virtual void                                Update                      ( TimeType t ) override;

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;
    virtual IPixelShaderChannelPtr              GetPixelShaderChannel       () const override;
    virtual IVertexShaderChannelConstPtr        GetVertexShaderChannel      () const override;

    virtual void                                SetPrevPlugin               ( IPluginPtr plugin ) override;

    virtual Float32                             BuildVACForText             ( const std::wstring & text, bool useKernin, bool useBoxg );

    explicit                                    TextPluginBase              (   const std::string & name,
                                                                                const std::string & uid,
                                                                                IPluginPtr prev,
                                                                                DefaultPluginParamValModelPtr model );
    virtual                                     ~TextPluginBase             ();

private:

    DefaultTextureDescriptorPtr                 LoadTexture                 (   DefaultTexturesDataPtr txData,
                                                                                TextureAssetConstPtr res,
                                                                                const std::string & name,
                                                                                TextureWrappingMode hWrappingMode,
                                                                                TextureWrappingMode vWrappingMode,
                                                                                TextureFilteringMode txFilteringMode,
                                                                                const glm::vec4 & bColor,
                                                                                DataBuffer::Semantic semantic );

};

} // model
} // bv