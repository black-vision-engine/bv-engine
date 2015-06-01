#pragma once

#include "Engine\Graphics\Effects\DefaultEffect.h"

namespace bv {




class DefaultMultipassEffect	:	public DefaultEffect
{
private:


public:
	DefaultMultipassEffect( const IShaderDataSource * psds, const IShaderDataSource * vsds, const IShaderDataSource * gsds );
	~DefaultMultipassEffect();

	RenderablePass* addPass( const IShaderDataSource * psds, const IShaderDataSource * vsds, const IShaderDataSource * gsds );
};


DEFINE_PTR_TYPE(DefaultMultipassEffect)
DEFINE_CONST_PTR_TYPE(DefaultMultipassEffect)


} //bv