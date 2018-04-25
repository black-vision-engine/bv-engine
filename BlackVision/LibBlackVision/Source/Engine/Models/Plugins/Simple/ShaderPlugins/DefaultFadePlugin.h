#pragma once

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/PixelShader/ResourceStateModel.h"

#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"



namespace bv { namespace model {

    // ***************************** DESCRIPTOR **********************************
    class DefaultFadePluginDesc : public BasePluginDescriptor
    {
    public:

        DefaultFadePluginDesc                                       ();

        virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
        virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;

        static  std::string                     UID                 ();
        static  std::string                     TextureName         ();
    };

    // ***************************** PLUGIN ********************************** 
    ///@ingroup PluginsList
    class DefaultFadePlugin : public BasePlugin
    {
    public:

        struct PARAMS
        {
            static const std::string        POINT1;
            static const std::string        POINT2;
            static const std::string        POINT3;
            static const std::string        POINT4;
            static const std::string        POINT1_ALPHA;
            static const std::string        POINT2_ALPHA;
            static const std::string        POINT3_ALPHA;
            static const std::string        POINT4_ALPHA;
        };

    protected:

        DefaultPixelShaderChannelPtr    m_psc;
        DefaultVertexShaderChannelPtr   m_vsc;

        VertexAttributesChannelPtr      m_vaChannel;

    public:

        explicit                                    DefaultFadePlugin           ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
                                                    ~DefaultFadePlugin          ();

        virtual bool                                IsValid						() const override;

        virtual bool                                LoadResource                ( AssetDescConstPtr assetDescr ) override;

        virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;
        virtual IPixelShaderChannelPtr              GetPixelShaderChannel       () const override;
        virtual IVertexShaderChannelConstPtr        GetVertexShaderChannel      () const override;

        virtual void                                Update                      ( TimeType t ) override;

    private:
        virtual bool								SetPrevPlugin               ( IPluginPtr plugin ) override;
        void										InitVertexAttributesChannel ();
    };


} // model
} // bv


