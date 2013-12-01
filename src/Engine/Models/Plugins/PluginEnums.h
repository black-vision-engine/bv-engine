#pragma once

namespace bv { namespace model {

// ************************ 
enum class TextureAttachmentMode
{
    MM_ATTACHED,
    MM_FREE,

    MM_TOTAL
};

enum class TextureWrappingMode : int
{
    TWM_CLAMP = 0,
    TWM_REPEAT,
    TWM_MIRROR,
    TWM_CLAMP_BORDER,
    TWM_CLAMP_EDGE, 

    TWM_TOTAL
};

enum class TextureFilteringMode : int
{
    TFM_POINT = 0,
    TFM_LINEAR,

    TFM_TOTAL
};

} //model
} //bv
