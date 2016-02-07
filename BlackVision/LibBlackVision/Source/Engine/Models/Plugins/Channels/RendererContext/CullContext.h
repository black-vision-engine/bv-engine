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

    void			        SetContext	    ( const CullContext * ctx );

};

} //model
} //bv
