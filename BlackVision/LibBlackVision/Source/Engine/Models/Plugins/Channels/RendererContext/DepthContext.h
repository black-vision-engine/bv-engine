#pragma once


namespace bv { namespace model {

//FIXME: add compare function
class DepthContext
{
public:

    bool    enabled;        // true
    bool    writable;       // true

public:

    DepthContext();

    DepthContext *			Clone			() const;

    void			        SetContext	    ( const DepthContext * ctx );

};

} //model
} //bv
