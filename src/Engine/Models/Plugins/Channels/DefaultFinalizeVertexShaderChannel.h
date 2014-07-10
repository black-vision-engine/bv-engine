#pragma once

#include "Engine/Models/Plugins/Channels/DefaultFinalizeShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IVertexShaderChannel.h"


namespace bv { namespace model {

class DefaultFinalizeVertexShaderChannel : DefaultFinalizeShaderChannel< IVertexShaderChannel >
{
private:

    typedef  DefaultFinalizeShaderChannel< IVertexShaderChannel > Parent;

private:

    static PluginUIDHashMap    ms_vertexShaderMapping;

private:

    static void     InitializeVertexShaderMapping       ();

public:

        DefaultFinalizeVertexShaderChannel              ( IVertexShaderChannelPtr channel );
        ~DefaultFinalizeVertexShaderChannel             ();

protected:

        virtual std::string     GetShaderSource         ( const std::vector< std::string > & uids ) const override;

};

DEFINE_PTR_TYPE(DefaultFinalizeVertexShaderChannel)

} //model
} //bv
