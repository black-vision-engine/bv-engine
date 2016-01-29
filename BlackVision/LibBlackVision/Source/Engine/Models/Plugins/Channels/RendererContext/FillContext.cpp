#include "stdafx.h"

#include "FillContext.h"


namespace bv { namespace model {

// ******************************
//
FillContext::FillContext()
{
    fillMode = Mode::M_POLYGONS;
}

// ******************************
//
FillContext *		FillContext::Clone		() const
{
	auto ctx = new FillContext();
    ctx->fillMode = fillMode;
	return ctx;
}

} //model
} //bv
