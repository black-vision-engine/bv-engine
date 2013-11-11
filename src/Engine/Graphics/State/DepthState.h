#include "Engine/Graphics/State/StateEnums.h"


namespace bv {

class DepthState
{
public:

    bool                enabled;        // true
    bool                writable;       // true
    DepthCompareMode    compareMode;    // DCM_LEQUAL

public:

    DepthState  ();

};

} //bv
