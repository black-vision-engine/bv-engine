#include "stdafx.h"
#include "TextGeometry.h"

#include "Assets/Font/IFontEngine.h"


namespace bv
{


// ***********************
//
const ContoursList &        TextGeometry::CreateCharacter   ( wchar_t ch, float size ) const
{
    auto iter = m_contours.find( ch );
    if( iter != m_contours.end() )
        return iter->second;
    else
    {
        auto contours = m_fontEngine->Create3dVerticies( ch, size );
        auto result = m_contours.insert( std::make_pair( ch, std::move( contours ) ) );

        assert( result.second );
        return result.first->second;
    }
}


}   // bv
