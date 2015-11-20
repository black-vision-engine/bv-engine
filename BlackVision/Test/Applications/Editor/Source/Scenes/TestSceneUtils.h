#pragma once

#include <vector>

#include "Engine/Models/BVSceneEditor.h"
#include "Engine/Models/ModelNodeEditor.h"

#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"
#include "Engine/Models/Timeline/TimelineManager.h"
#include "Engine/Models/Plugins/PluginUtils.h"

#include "Engine/Models/Plugins/Simple/DefaultRectPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultTransformPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultColorPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultTexturePlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultAnimationPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultAlphaMaskPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultGradientPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultTextPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultTimerPlugin.h"

#include "Engine/Models/Plugins/Simple/DefaultTrianglePlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultCirclePlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultEllipsePlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultRoundedRectPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultCubePlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultSimpleCubePlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultSpherePlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultGeospherePlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultCylinderPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultConePlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultTorusPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultSpringPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultCogwheelPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultPiechartPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultPrismPlugin.h"

#include "Engine/Models/Plugins/Simple/DefaultVideoStreamDecoderPlugin.h"

#include "Tools/Utils.h"

#include "BVGL.h"

namespace bv {

class TestSceneUtils
{
public:
	static const std::string	TEXTURE_PATH;
	static const std::string	ALPHA_MASK_PATH;
	static const std::string	ALPHA_MASK0_PATH;
	static const std::string	ANIM_PATH;

	static const std::string	VIDEO_PATH0;
	static const std::string	VIDEO_PATH1;

	static const Float32		SPEED;
	static const UInt32			ANIM_NUM;
	static const UInt32			IMG_SIZE;
	static const UInt32			AM_SIZE;

public:
	static const UInt32				GEOM_PLUGINS_NUM;
	static const std::string		PluginsArr[];
	static const std::string		PluginsNameArr[];

public:
	static model::BasicNodePtr	ColoredRectangle					( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, const std::string & name, Float32 width, Float32 height, glm::vec4 color, const std::string & alphaMask = "" );
	static model::BasicNodePtr	ColoredGeometry						( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, const std::string & name, const std::string & plugin, glm::vec4 color, const std::string & alphaMask = "" );
	
	static model::BasicNodePtr	TexturedRectangle					( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, const std::string & name, Float32 width, Float32 height, const std::string & path, const std::string & alphaMask = "" );
	static model::BasicNodePtr	TexturedGeometry					( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, const std::string & name, const std::string & plugin, const std::string & path, const std::string & alphaMask = "" );
	
	static model::BasicNodePtr	AnimatedRectangle					( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, const std::string & name, Float32 width, Float32 height, const std::string & path, const std::string & alphaMask = "" );
	static model::BasicNodePtr	AnimatedGeometry					( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, const std::string & name, const std::string & plugin, const std::string & path, const std::string & alphaMask = "" );
	
	static model::BasicNodePtr	GradientRectangle					( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, const std::string & name, Float32 width, Float32 height, glm::vec4 c1, glm::vec4 c2, const std::string & alphaMask = "" );
	static model::BasicNodePtr	GradientGeometry					( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, const std::string & name, const std::string & plugin, glm::vec4 c1, glm::vec4 c2, const std::string & alphaMask = "" );
	
	static model::BasicNodePtr	ColoredText							( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, const std::string & name, glm::vec4 color, UInt32 fontSize, const std::string & alphaMask = "" );
	static model::BasicNodePtr	GradientText						( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, const std::string & name, glm::vec4 c1, glm::vec4 c2, UInt32 fontSize, const std::string & alphaMask = "" );
	
	static model::BasicNodePtr	ColoredTimer						( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, const std::string & name, glm::vec4 color, UInt32 fontSize, const std::string & alphaMask = "" );
	static model::BasicNodePtr	GradientTimer						( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, const std::string & name, glm::vec4 c1, glm::vec4 c2, UInt32 fontSize, const std::string & alphaMask = "" );

	static model::BasicNodePtr	VideoStreamDecoder					( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, const std::string & name, const std::string & plugin, const std::string & videoPath, const std::string & alphaMask = "" );

	static void					GenerateCheckboardTex				( const std::string & name, UInt32 width, UInt32 height, glm::uvec3 color );
	static void					GenerateCheckboardAlphaMaskTex		( const std::string & name, UInt32 width, UInt32 height, UInt32 size = 32 );
	static void					GenerateCheckboardAnim				( const std::string & path, UInt32 width, UInt32 height, UInt32 animNum );

	static std::vector< model::IPluginDescriptor * >  DefaultBVPluginDescriptors  ();
};

} // bv