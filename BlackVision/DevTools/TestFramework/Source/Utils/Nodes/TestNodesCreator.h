#pragma once

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

namespace bv
{


// ***********************
//
class TestNodesCreator
{
public:

    static const Float32		SPEED;
    static const UInt32			ANIM_NUM;

public:

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
    static model::BasicNodePtr	VideoInputRectangle					( model::ITimeEvaluatorPtr timeEval, const std::string & name, Float32 width, Float32 height, UInt32 videoInputIdx );

    static model::BasicNodePtr	FakeAudioPlugin                     ( model::ITimeEvaluatorPtr timeEval, const std::string & name );

};

// *******************************
//
inline model::BasicNodePtr         QueryTyped      ( model::IModelNodePtr node )
{
    return std::static_pointer_cast< model::BasicNode >( node );
}


}	// bv



