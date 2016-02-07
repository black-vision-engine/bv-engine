#include "stdafx.h"

#include "DepthContext.h"


namespace bv { namespace model {

// ******************************
//
DepthContext::DepthContext()
{
    enabled = true;
    writable = true;
}

// ******************************
//
DepthContext *		DepthContext::Clone		() const
{
	auto ctx = new DepthContext();
    ctx->enabled = enabled;
    ctx->writable = writable;
	return ctx;
}

// ******************************
//
void		        DepthContext::SetContext ( const DepthContext * ctx )
{
    enabled = ctx->enabled;
    writable = ctx->writable;
}

} //model
} //bv
