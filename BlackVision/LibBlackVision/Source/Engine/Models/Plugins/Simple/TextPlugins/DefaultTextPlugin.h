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
class DefaultTextPluginDesc : public BasePluginDescriptor
{
public:

    DefaultTextPluginDesc                                       ();

    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
   
    static  std::string                     UID                 ();

    static  std::string                     TextureName         ();
};

// ***************************** PLUGIN ********************************** 
class DefaultTextPlugin : public TextPluginBase
{
public:

    struct PARAM
    {
        static const std::string    TEXT;
    };

private:

    ParamWStringPtr                 m_textParam;
    std::wstring                    m_currentText;
    Float32                         m_currentAligment;
    Float32                         m_currentSpacing;

    Float32                         m_textLength;

    ParamFloatPtr                   m_maxTextLengthParam;
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

public:

    explicit                                    DefaultTextPlugin           ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
                                                ~DefaultTextPlugin          ();

    std::wstring                                GetText                     () const;
};

} // model
} // bv
