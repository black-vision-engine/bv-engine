#pragma once

#include <string>
#include <glm/glm.hpp>

#include "Engine/Types/Enums.h"


namespace bv {

class ITextureParams
{
public:

    virtual const std::string       GetName             () const = 0;

    virtual unsigned int            GetWidth            () const = 0;
    virtual unsigned int            GetHeight           () const = 0;
    virtual TextureFormat           GetFormat           () const = 0;
    virtual TextureWrappingMode     GetWrappingModeX    () const = 0;
    virtual TextureWrappingMode     GetWrappingModeY    () const = 0;
    virtual TextureFilteringMode    GetFilteringMode    () const = 0;
    virtual glm::vec4               DefaultBorderColor  () const = 0;

    virtual ~ITextureParams () {}

};

//    std::string             fileName;
//    ParamTransform          transform;

//    ParamFloat              alpha;
//    ParamVec4               borderColor;

} //bv
