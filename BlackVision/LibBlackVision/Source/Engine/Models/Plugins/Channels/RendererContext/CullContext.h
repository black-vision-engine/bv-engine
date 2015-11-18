#pragma once


namespace bv { namespace model {

class CullContext
{
public:

    bool enabled;       // true
    bool isCCWOrdered;  // true

public:

							CullContext		();

    CullContext *			Clone			() const;
};

} //model
} //bv
