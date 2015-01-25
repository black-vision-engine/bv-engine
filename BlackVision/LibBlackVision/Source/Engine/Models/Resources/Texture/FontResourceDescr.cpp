#include "FontResourceDescr.h"


namespace bv { namespace model {

// *******************************
//
FontResourceDescr::FontResourceDescr                        ( const std::string & fontFile )
    : m_fontFile( fontFile )
{
}

// *******************************
//
FontResourceDescr::~FontResourceDescr                       ()
{
}

// *******************************
//
PluginResourceType  FontResourceDescr::GetResourceType   () const
{
    return PluginResourceType::PRT_FONT;
}

// *******************************
//
const std::string & FontResourceDescr::GetFontFile    () const
{
    return m_fontFile;
}

// *******************************
//
IFontResourceDescrConstPtr          QueryFontResourceDescr      ( IPluginResourceDescrConstPtr resDescr )
{
    if ( resDescr->GetResourceType() == PluginResourceType::PRT_FONT )
    {
        return std::static_pointer_cast< const IFontResourceDescr >( resDescr );
    }

    return nullptr;
}

} //model
} //bv
