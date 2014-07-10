#pragma once

#include "Engine/Models/Plugins/Channels/DefaultFinalizeShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IGeometryShaderChannel.h"

namespace bv { namespace model {

class DefaultFinalizeGeometryShaderChannel : public DefaultFinalizeShaderChannel< IGeometryShaderChannel >
{
private:

    typedef  DefaultFinalizeShaderChannel< IGeometryShaderChannel > Parent;

public:

        DefaultFinalizeGeometryShaderChannel            ( IGeometryShaderChannelPtr channel );
        ~DefaultFinalizeGeometryShaderChannel           ();

protected:

        virtual std::string     GetShaderSource         ( const std::vector< std::string > & uids ) const override;

};

DEFINE_PTR_TYPE(DefaultFinalizeGeometryShaderChannel)

} //model
} //bv
