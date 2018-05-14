#pragma once

#include "TextPluginBase.h"

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/Transform/DefaultTransformChannel.h"

#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"

#include "Engine/Events/BaseEvent.h"
#include "Assets/Font/TextHelper.h"
#include "TextArranger.h"

namespace bv { namespace model {

class TextAtlas;

// ***************************** DESCRIPTOR **********************************
class DefaultTextPluginDesc : public TextPluginBaseDesc
{
public:

    DefaultTextPluginDesc                                       ();

    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
   
    static  std::string                     UID                 ();

    static  std::string                     TextureName         ();
};

// ***************************** PLUGIN ********************************** 
/// @ingroup PluginsList
class DefaultTextPlugin : public TextPluginBase
{
public:

    struct PARAM
    {
        static const std::string TEXT;
        static const std::string MAX_TEXT_LENGTH;
        static const std::string TX_MAT;
        static const std::string RCC_BEGIN_COLOR;
        static const std::string RCC_END_COLOR;
        static const std::string COL_TEXT_EFFECT;
        static const std::string TX_TEXT_EFFECT;
        static const std::string TIME;
        static const std::string TX_EFFECT_VAL1;
        static const std::string TX_EFFECT_VAL2;
        static const std::string EXPLOSION_CENTER;
        static const std::string ANIM_SCALE_OFFSET;
        static const std::string ANIM_SCALE;
        static const std::string ANIM_ALPHA_OFFSET;
        static const std::string ANIM_ALPHA;
    };

private:

    ValueParamState< std::wstring > m_text;

    Float32                         m_textLength;
	ValueParamState< Float32 >		m_maxTextLength;

    ParamFloatPtr                   m_timeParam;
    ValueMat4Ptr                    m_scaleValue;
    glm::mat4                       m_scaleMat;

    TextArranger *                  m_arranger;

    //delegates
    void                            OnSetText                   ( IEventPtr evt );

    void                            SetText                     ( const std::wstring & newText );

    virtual bool                    LoadResource                ( AssetDescConstPtr assetDescr ) override;

private:

    virtual mathematics::RectConstPtr           GetAABB                     ( const glm::mat4 & trans ) const override;

    virtual void                                Update                      ( TimeType t ) override;

    void                                        ScaleToMaxTextLength        ();

	bool										NeedsSetText				() const;

public:

    explicit                                    DefaultTextPlugin           ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
                                                ~DefaultTextPlugin          ();

    std::wstring                                GetText                     () const;
};

} // model
} // bv
