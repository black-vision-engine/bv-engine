#pragma once

#include "ISerializer.h"


namespace bv {

class BVSerializeContext : public SerializeContext
{
public:
    BVSerializeContext()
    {
        recursive = true;
        detailedInfo = true;
        pluginsInfo = true;
    }

    bool            recursive;
    bool            detailedInfo;
    bool            pluginsInfo;
};

}
