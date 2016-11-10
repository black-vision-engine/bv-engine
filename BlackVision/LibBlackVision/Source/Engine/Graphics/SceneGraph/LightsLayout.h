#pragma once

#include "Engine/Types/Enums.h"
#include "Engine/Graphics/Resources/UniformBuffer.h"

#include <map>


namespace bv
{

class LightsLayout
{
private:

    static const UInt32         MAX_LIGTHS;
    static std::string          UNIFORM_BLOCK_NAME;
    static std::string          UNIFORM_BLOCK_DEF_PATH;

private:

    UniformBlockLayoutUPtr                          m_blockLayout;

    std::map< LightType, std::vector< UInt32 > >    m_lightOffsetIdx;
    std::map< LightType, UInt32 >                   m_lightsNumOffsetIdx;

private:

                                LightsLayout            ();

public:

    UInt32                      GetLightOffsetIdx      ( LightType type, UInt32 idx ) const;
    UInt32                      GetLightsNumOffsetIdx  ( LightType type ) const;

    const UniformBlockLayout *  GetBlockLayout          () const;

    static LightsLayout &       Instance                ();

};

} // bv
