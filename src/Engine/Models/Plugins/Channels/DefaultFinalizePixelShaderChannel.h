#pragma once

#include "Engine/Models/Plugins/Channels/DefaultFinalizeShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IPixelShaderChannel.h"


namespace bv { namespace model {

class DefaultFinalizePixelShaderChannel : DefaultFinalizeShaderChannel< IPixelShaderChannel >
{
private:

    typedef  DefaultFinalizeShaderChannel< IPixelShaderChannel > Parent;

private:

    static PluginUIDHashMap    ms_pixelShaderMapping;

private:

    static void     InitializePixelShaderMapping();

public:

        DefaultFinalizePixelShaderChannel       ( IPixelShaderChannelPtr channel );
        ~DefaultFinalizePixelShaderChannel      ();

        virtual RendererContextConstPtr     GetRendererContext  () const override;

protected:

        virtual std::string     GetShaderSource ( const std::vector< std::string > & uids ) const override;
};

} //model
} //bv
