#pragma once

#include "Engine\Graphics\Effects\DefaultEffect.h"

namespace bv {




class DefaultMultipassEffect	:	public DefaultEffect
{
private:


public:
	DefaultMultipassEffect( const IShaderDataSource * psds, const IShaderDataSource * vsds, const IShaderDataSource * gsds );
	~DefaultMultipassEffect();

	void addPass( const IShaderDataSource * psds, const IShaderDataSource * vsds, const IShaderDataSource * gsds );
};





} //bv