#include "Engine/Graphics/State/StateEnums.h"

namespace bv {

class CullState
{
public:

    bool enabled;       // true
    bool isCCWOrdered;  // true

public:

    CullState  ();

};

} //bv
