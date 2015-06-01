#include "DefaultMultipassEffect.h"


namespace bv
{

DefaultMultipassEffect::DefaultMultipassEffect( const IShaderDataSource * psds, const IShaderDataSource * vsds, const IShaderDataSource * gsds )
	:	DefaultEffect( psds, vsds, gsds )
{
}


DefaultMultipassEffect::~DefaultMultipassEffect(void)
{
}


void DefaultMultipassEffect::addPass( const IShaderDataSource * psds, const IShaderDataSource * vsds, const IShaderDataSource * gsds )
{
    auto pass = CreateRenderablePass( psds, vsds, gsds );

    AddPass( pass );
}

} //bv
