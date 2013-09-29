#pragma once

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/PluginParameters.h"
#include "../../app/MockFonts/MockFont.h" //FIXME:

namespace bv { namespace model {

class Resource;

// ***************************** DESCRIPTOR **********************************
class SimpleTextPluginPD : public BaseParametersDescriptor
{
public:

    static const std::string pluginName;

    explicit SimpleTextPluginPD()
        : BaseParametersDescriptor( "SimpleTextPlugin" )
    {}
};


// ***************************** PLUGIN ********************************** 
class SimpleTextPlugin : public BasePlugin< IPlugin, SimpleTextPluginPD >
{
private:
    typedef std::pair< ResourceHandle*, std::string > TexturePair;

    bv::Text*           m_text;
    std::string         m_textureFileName;
    std::string         m_fontFileName;


    TexturePair         LoadAtlas( const std::string& name )   const;

    void                EvalGeometryChannel();

public:

    explicit            SimpleTextPlugin    ( const std::wstring& text, const std::string & fontFileName );
                        ~SimpleTextPlugin   ();


    void                Update                      ( float t ) override;
    void                Print                       ( std::ostream & out, int tabs = 0 ) const override;

};

} // model
} // bv