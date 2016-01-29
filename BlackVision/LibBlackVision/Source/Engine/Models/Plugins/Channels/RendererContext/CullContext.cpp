#include "stdafx.h"

#include "CullContext.h"


namespace bv { namespace model {

// ******************************
//
CullContext::CullContext()
{
    enabled = true;
    isCCWOrdered = true;
}

// ******************************
//
CullContext *		CullContext::Clone			() const
{
	auto ctx = new CullContext();
    ctx->enabled = enabled;
    ctx->isCCWOrdered = isCCWOrdered;
	return ctx;
}

} //model
} //bv