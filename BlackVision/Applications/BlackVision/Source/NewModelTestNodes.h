#pragma once

#include "Engine/Models/Interfaces/ITimeEvaluator.h"
#include "Engine/Models/BasicNode.h"
#include "Mathematics/glm_inc.h"

namespace bv {

namespace model 
{
    class BasicNode;
    class PluginsManager;
    class TimelineManager;
}

// ********************************
class SimpleNodesFactory
{
public:

    static model::BasicNodePtr  CreateSolidRectNode         ( const std::string & name, float w, float h, const glm::vec3 & pos, const glm::vec4 col, model::ITimeEvaluatorPtr timeEvaluator );
    static model::BasicNodePtr  CreateTexturedRectNode      ( const std::string & name, float w, float h, const glm::vec3 & pos, const std::string & txFileName, model::ITimeEvaluatorPtr timeEvaluator );
                           
    static model::BasicNodePtr  CreateGlobalEffectTest      ( model::ITimeEvaluatorPtr timeEvaluator );
    static model::BasicNodePtr  CreateLightScatteringTest   ( model::ITimeEvaluatorPtr timeEvaluator );
    static model::BasicNodePtr  CreateShadowTest            ( model::ITimeEvaluatorPtr timeEvaluator );
    static model::BasicNodePtr  CreateImageMaskTest         ( model::ITimeEvaluatorPtr timeEvaluator );
    static model::BasicNodePtr  CreateBlurTest              ( model::ITimeEvaluatorPtr timeEvaluator );
    static model::BasicNodePtr  CreateMultiShadowTest       ( model::ITimeEvaluatorPtr timeEvaluator );
    static model::BasicNodePtr  CreateOverrideAlphaTest     ( model::ITimeEvaluatorPtr timeEvaluator );
    static model::BasicNodePtr  CreateOverrideNodeMaskTest  ( model::ITimeEvaluatorPtr timeEvaluator );
    static model::BasicNodePtr  CreateOverrideNodeMaskTest1 ( model::ITimeEvaluatorPtr timeEvaluator );
                           
    static model::BasicNodePtr  CreateGreenRectNode         ( model::ITimeEvaluatorPtr timeEvaluator, const std::string & nodeName );
    static model::BasicNodePtr  CreateGreenRectNodeNoAssert ( model::ITimeEvaluatorPtr timeEvaluator, bool useAlphaMask );

    static model::BasicNodePtr  CreateOlafRectNode          ( model::ITimeEvaluatorPtr timeEvaulator);
    static model::BasicNodePtr  CreateCreedRectNode         ( model::ITimeEvaluatorPtr timeEvaulator);
    static model::BasicNodePtr  CreateCreedTextNode         ( model::ITimeEvaluatorPtr timeEvaulator);
    static model::BasicNodePtr  CreateCreedTimerNode        ( model::ITimeEvaluatorPtr timeEvaluator, unsigned int blurSize, bool useAlphaMask  );
	static model::BasicNodePtr  CreateCreedPrismNode        ( model::ITimeEvaluatorPtr timeEvaluator, float offset);
	static model::BasicNodePtr  CreateCreedColoredPrismNode ( model::ITimeEvaluatorPtr timeEvaluator, float offset);
	static model::BasicNodePtr  CreateCreedTexturedPrismNode( model::ITimeEvaluatorPtr timeEvaluator, float offset);
	static model::BasicNodePtr  CreateCreedGradedPrismNode  ( model::ITimeEvaluatorPtr timeEvaluator, float offset);
	
	static model::BasicNodePtr	CreateCreedColoredPieChartNode( model::ITimeEvaluatorPtr timeEvaluator, float offset );
	static model::BasicNodePtr	CreateCreedGradedPieChartNode( model::ITimeEvaluatorPtr timeEvaluator, float offset );

    static model::BasicNodePtr  CreateTexturedRectNode      ( model::ITimeEvaluatorPtr timeEvaluator, bool useAlphaMask );
    static model::BasicNodePtr  CreateTexturedTextNode      ( model::ITimeEvaluatorPtr timeEvaluator, bool useAlphaMask );
    static model::BasicNodePtr  CreateTextureAnimationRectNode( model::ITimeEvaluatorPtr timeEvaluator, bool useAlphaMask );
    static model::BasicNodePtr  CreateVideoStreamDecoderRectNode( model::ITimeEvaluatorPtr timeEvaluator, bool useAlphaMask );
                           
    static model::BasicNodePtr  CreateTextNode              ( model::ITimeEvaluatorPtr timeEvaluator, unsigned int blurSize, bool useAlphaMask );
    static model::BasicNodePtr  CreateTextWithShadowNode    ( model::ITimeEvaluatorPtr timeEvaluator, unsigned int blurSize, const glm::vec3 shadowTranslation );

    static model::BasicNodePtr  CreateTimerNode             ( model::ITimeEvaluatorPtr timeEvaluator, unsigned int blurSize, bool useAlphaMask  );

    static model::BasicNodePtr  CreateHeightMapNode         ( model::ITimeEvaluatorPtr timeEvaluator );

	static model::BasicNodePtr	ScrollerNodeTestScene        ( model::ITimeEvaluatorPtr timeEvaluator );

	static model::BasicNodePtr	CreateBasicShapesTestNode   ( model::ITimeEvaluatorPtr timeEvaluator );
	static model::BasicNodePtr	CreateBasicShapeShow        ( model::ITimeEvaluatorPtr timeEvaluator, const std::string& uid, glm::vec3 translation, std::string = "textures/sand.jpg" );
    static model::BasicNodePtr  CreateTextCacheTest         ( model::ITimeEvaluatorPtr timeEvaluator, const std::string& nodeName, glm::vec3 translation, glm::vec4 color, const std::wstring text, const std::string& fontName );
    static model::BasicNodePtr  CreateNodeReplicatorTest    ( model::ITimeEvaluatorPtr timeEvaluator );
    static model::BasicNodePtr  CreateNodeMultiGlobalEffectTest( model::ITimeEvaluatorPtr timeEvaluator );
    static model::BasicNodePtr  CreateScrollerTestNode      ( model::ITimeEvaluatorPtr timeEvaluator );

	static void					TestTypedParamQueries	    ( model::ITimeEvaluatorPtr timeEvaluator );

};

} //bv
