#pragma once

#include <vector>

#include "Engine/Editors/BVProjectEditor.h"
#include "Engine/Editors/ModelNodeEditor.h"

#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"
#include "Engine/Models/Timeline/TimelineManager.h"
#include "Engine/Models/Plugins/PluginUtils.h"

#include "Engine/Models/Plugins/Simple/Shapes/DefaultRectPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultTransformPlugin.h"
#include "Engine/Models/Plugins/Simple/ShaderPlugins/DefaultColorPlugin.h"
#include "Engine/Models/Plugins/Simple/ShaderPlugins/DefaultTexturePlugin.h"
#include "Engine/Models/Plugins/Simple/ShaderPlugins/DefaultAnimationPlugin.h"
#include "Engine/Models/Plugins/Simple/ShaderPlugins/DefaultAlphaMaskPlugin.h"
#include "Engine/Models/Plugins/Simple/ShaderPlugins/DefaultGradientPlugin.h"
#include "Engine/Models/Plugins/Simple/TextPlugins/DefaultTextPlugin.h"
#include "Engine/Models/Plugins/Simple/TextPlugins/DefaultTimerPlugin.h"

#include "Engine/Models/Plugins/Simple/Shapes/DefaultTrianglePlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultCirclePlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultEllipsePlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultRoundedRectPlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultCubePlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultSimpleCubePlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultSpherePlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultGeospherePlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultCylinderPlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultConePlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultTorusPlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultSpringPlugin.h"
#include "Engine/Models/Plugins/Simple/Shapes/DefaultCogwheelPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultPiechartPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultPrismPlugin.h"

#include "Engine/Models/Plugins/Simple/AVStreamDecoder/DefaultAVDecoderPlugin.h"

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

	static void	                AddColoredRectangleScene			( BVProjectEditor * editor, const std::string & name, glm::vec4 color, glm::vec3 transform );

	static model::BasicNodePtr	ColoredRectangle					( model::ITimeEvaluatorPtr timeEval, const std::string & name, Float32 width, Float32 height, glm::vec4 color, const std::string & alphaMask = "" );
	static model::BasicNodePtr	ColoredGeometry						( model::ITimeEvaluatorPtr timeEval, const std::string & name, const std::string & plugin, glm::vec4 color, const std::string & alphaMask = "" );
	
	static model::BasicNodePtr	TexturedRectangle					( model::ITimeEvaluatorPtr timeEval, const std::string & name, Float32 width, Float32 height, const std::string & path, const std::string & alphaMask = "" );
	static model::BasicNodePtr	TexturedGeometry					( model::ITimeEvaluatorPtr timeEval, const std::string & name, const std::string & plugin, const std::string & path, const std::string & alphaMask = "" );
	
	static model::BasicNodePtr	AnimatedRectangle					( model::ITimeEvaluatorPtr timeEval, const std::string & name, Float32 width, Float32 height, const std::string & path, const std::string & alphaMask = "" );
	static model::BasicNodePtr	AnimatedGeometry					( model::ITimeEvaluatorPtr timeEval, const std::string & name, const std::string & plugin, const std::string & path, const std::string & alphaMask = "" );
	
	static model::BasicNodePtr	GradientRectangle					( model::ITimeEvaluatorPtr timeEval, const std::string & name, Float32 width, Float32 height, glm::vec4 c1, glm::vec4 c2, const std::string & alphaMask = "" );
	static model::BasicNodePtr	GradientGeometry					( model::ITimeEvaluatorPtr timeEval, const std::string & name, const std::string & plugin, glm::vec4 c1, glm::vec4 c2, const std::string & alphaMask = "" );
	
	static model::BasicNodePtr	ColoredText							( model::ITimeEvaluatorPtr timeEval, const std::string & name, glm::vec4 color, UInt32 fontSize, const std::string & alphaMask = "" );
	static model::BasicNodePtr	GradientText						( model::ITimeEvaluatorPtr timeEval, const std::string & name, glm::vec4 c1, glm::vec4 c2, UInt32 fontSize, const std::string & alphaMask = "" );
	
	static model::BasicNodePtr	ColoredTimer						( model::ITimeEvaluatorPtr timeEval, const std::string & name, glm::vec4 color, UInt32 fontSize, const std::string & alphaMask = "" );
	static model::BasicNodePtr	GradientTimer						( model::ITimeEvaluatorPtr timeEval, const std::string & name, glm::vec4 c1, glm::vec4 c2, UInt32 fontSize, const std::string & alphaMask = "" );

	static model::BasicNodePtr	VideoStreamDecoder					( model::ITimeEvaluatorPtr timeEval, const std::string & name, const std::string & plugin, const std::string & videoPath, const std::string & alphaMask = "" );

	static void					GenerateCheckboardTex				( const std::string & name, UInt32 width, UInt32 height, glm::uvec3 color );
	static void					GenerateCheckboardAlphaMaskTex		( const std::string & name, UInt32 width, UInt32 height, UInt32 size = 32 );
	static void					GenerateCheckboardAnim				( const std::string & path, UInt32 width, UInt32 height, UInt32 animNum );

	static std::vector< model::IPluginDescriptor * >  DefaultBVPluginDescriptors  ();

    static const IConfig*       GetConfig                           ();
};


class FakeConfig : public IConfig
{
    friend class TestSceneUtils;
private:
    
    static const std::string    EmptyString;
    static const FakeConfig     Config;

public:

    const std::string &     FakeConfig::PropertyValue       ( const std::string & /*key*/ ) const
    {
        return EmptyString;
    }
};


} // bv

