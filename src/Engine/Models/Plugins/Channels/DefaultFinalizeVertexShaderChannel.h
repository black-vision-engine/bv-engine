#pragma once

#include "Engine/Models/Plugins/Channels/DefaultFinalizeShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IVertexShaderChannel.h"


namespace bv { namespace model {

class DefaultFinalizeVertexShaderChannel : public DefaultFinalizeShaderChannel< IVertexShaderChannel >
{
private:

    typedef  DefaultFinalizeShaderChannel< IVertexShaderChannel > Parent;

private:

    static PluginUIDHashMap    ms_vertexShaderMapping;

private:

    void     InitializeVertexShaderMapping              () const;

public:

        DefaultFinalizeVertexShaderChannel              ( IVertexShaderChannelPtr channel, const std::string & shadersDir );
        ~DefaultFinalizeVertexShaderChannel             ();

protected:

        virtual std::string     GetShaderSource         ( const std::vector< std::string > & uids ) const override;

};

DEFINE_PTR_TYPE(DefaultFinalizeVertexShaderChannel)

} //model
} //bv
