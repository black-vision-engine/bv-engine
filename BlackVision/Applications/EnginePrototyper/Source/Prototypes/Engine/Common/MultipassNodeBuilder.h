#pragma once

#include "SimpleNodeBuilder.h"

namespace bv {


class MultipassNodeBuilder	:	public SimpleNodeBuilder
{
private:

public:
	MultipassNodeBuilder(void);
	~MultipassNodeBuilder(void);


	static SceneNode *		CreateMultipassRectNodeTexture( float w, float h, float z, const std::string & textureFile, unsigned short passes );

	static RenderableEffectPtr CreateMultipassRenderableEffect( ShaderDataSourceType sdst, unsigned short passes, const std::string & textureFile = "", const std::string & textureName = "" );
};

}

