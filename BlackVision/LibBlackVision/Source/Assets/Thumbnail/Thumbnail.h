#pragma once

#include "CoreDEF.h"

namespace bv
{

class Thumbnail;
DEFINE_CONST_PTR_TYPE( Thumbnail )

class Thumbnail
{
public:
    explicit    Thumbnail   () {}
    virtual     ~Thumbnail  () {}

    virtual const char *    Data        () const = 0;
    virtual const char *    DataBase64  () const = 0;
};

} // bv