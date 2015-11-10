#pragma once

#include "Engine/Models/Plugins/Channels/DefaultFinalizeShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IPixelShaderChannel.h"

//#include "StaticShaderGenerator.h"

namespace bv { namespace model {

class DefaultFinalizePixelShaderChannel : public DefaultFinalizeShaderChannel< IPixelShaderChannel >
{
private:

    typedef  DefaultFinalizeShaderChannel< IPixelShaderChannel > Parent;

private:
    StaticShaderGenerator m_staticShaderGenerator;

private:

    void     InitializePixelShaderMapping       () const;

public:

        DefaultFinalizePixelShaderChannel       ( IPixelShaderChannelPtr channel, const std::string & shadersDir );
        ~DefaultFinalizePixelShaderChannel      ();

        IPixelShaderChannelPtr				GetChannel					() const;

        virtual RendererContextConstPtr		GetRendererContext			() const override;

		virtual UInt64						GetTexturesDataUpdateID		() const override;
		virtual UInt64						GetRendererContextUpdateID	() const override;

protected:

        virtual std::string     GetShaderSource ( const std::vector< std::string > & uids ) const override;
};

DEFINE_PTR_TYPE(DefaultFinalizePixelShaderChannel)

} //model
} //bv
