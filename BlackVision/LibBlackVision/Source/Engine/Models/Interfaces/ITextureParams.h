#pragma once

#include <string>
#include "Mathematics/glm_inc.h"

#include "Engine/Types/Enums.h"
#include "CoreDEF.h"


namespace bv {

class ITextureParams
{
public:

    virtual const std::string       GetName         () const = 0;

    virtual SizeType				GetWidth        ( UInt32 level = 0  ) const = 0;
    virtual SizeType				GetHeight       ( UInt32 level = 0  ) const = 0;
    virtual TextureFormat           GetFormat       () const = 0;
    virtual TextureWrappingMode     GetWrappingModeX() const = 0;
    virtual TextureWrappingMode     GetWrappingModeY() const = 0;
    virtual TextureFilteringMode    GetFilteringMode() const = 0;
    virtual glm::vec4               BorderColor     () const = 0;

    virtual ~ITextureParams () {}

};

DEFINE_PTR_TYPE(ITextureParams)
DEFINE_CONST_PTR_TYPE(ITextureParams)

//    std::string             fileName;
//    ParamTransform          transform;

//    ParamFloat              alpha;
//    ParamVec4               borderColor;

} //bv
