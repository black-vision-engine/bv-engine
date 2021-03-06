#include "stdafx.h"

#include "CullContext.h"




#include "Memory/MemoryLeaks.h"



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

// ******************************
//
void		        CullContext::SetContext	    ( const CullContext * ctx )
{
    enabled = ctx->enabled;
    isCCWOrdered = ctx->isCCWOrdered;
}

} //model
} //bv