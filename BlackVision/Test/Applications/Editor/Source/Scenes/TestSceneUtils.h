#pragma once

#include <vector>

#include "Engine/Models/BVSceneEditor.h"
#include "Engine/Models/ModelNodeEditor.h"

#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"
#include "Engine/Models/Plugins/PluginUtils.h"

#include "Tools/Utils.h"

#include "BVGL.h"

namespace bv {

class TestSceneUtils
{
public:
	static const std::string	TEXTURE_PATH;
	static const std::string	ALPHA_MASK_PATH;
	static const std::string	ANIM_PATH;

	static const UInt32			SPEED;
	static const UInt32			ANIM_NUM;
	static const UInt32			IMG_SIZE;
	static const UInt32			AM_SIZE;

public:
	static model::BasicNodePtr	ColoredRectangle					( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, const std::string & name, Float32 width, Float32 height, glm::vec4 color, const std::string & alphaMask = "" );
	static model::BasicNodePtr	TexturedRectangle					( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, const std::string & name, Float32 width, Float32 height, const std::string & path, const std::string & alphaMask = "" );
	static model::BasicNodePtr	AnimatedRectangle					( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, const std::string & name, Float32 width, Float32 height, const std::string & path, const std::string & alphaMask = "" );
	static model::BasicNodePtr	GradientRectangle					( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, const std::string & name, Float32 width, Float32 height, glm::vec4 c1, glm::vec4 c2, const std::string & alphaMask = "" );

	static void					GenerateCheckboardTex				( const std::string & name, UInt32 width, UInt32 height, glm::uvec3 color );
	static void					GenerateCheckboardAlphaMaskTex		( const std::string & name, UInt32 width, UInt32 height );
	static void					GenerateCheckboardAnim				( const std::string & path, UInt32 width, UInt32 height, UInt32 animNum );
};

} // bv