#pragma once

#include "Engine/Models/Interfaces/ITextureParams.h"


namespace bv {

class ITextureDescriptor : public ITextureParams
{
public:

    virtual const char *            GetBits             () const = 0;

    virtual ~ITextureDescriptor() {}

};

//    std::string             fileName;
//    ParamTransform          transform;

//    ParamFloat              alpha;
//    ParamVec4               borderColor;

} //bv
