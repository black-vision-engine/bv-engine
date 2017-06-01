#include "stdafxBVApp.h"

#include "PieChartNode.h"

namespace bv {

void PieChartNode::Init( model::ITimeEvaluatorPtr timeEvaluator, const std::vector< PieChartSubnodeDesc > descs )
{
	AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );

	auto simpleTransform = GetPlugin( "transform" )->GetParameter( "simple_transform" );
	SetParameterScale( simpleTransform, 0.0f, glm::vec3( 1.f, .2f, 1.f ) );
	SetParameterTranslation( simpleTransform, 0.0f, glm::vec3( 0, 0, -1.f) );
	SetParameterRotation ( simpleTransform, 0.0f, glm::vec3( 20.f, 0.f, 0.f ) );

	float angle = 0;

	for( auto desc : descs )
	{
		float percent = desc.percent;
		float offset = desc.offset;

		model::BasicNodePtr node = NULL;
		if( desc.type == PieChartSubnodeDesc::SubnodeType::COLORED)
			node = SimpleNodesFactory::CreateCreedColoredPieChartNode( timeEvaluator, 0 );
		else if( desc.type == PieChartSubnodeDesc::SubnodeType::LINEAR_GRADIENTED)
			node = SimpleNodesFactory::CreateCreedGradedPieChartNode( timeEvaluator, 0 );
		assert( node );

		SetParameter( node->GetPlugin( "piechart" )->GetParameter( "angleStart" ), 0.f, angle );
		float angleMid = angle + float( percent * PI / 100 );
		angle += float( percent * 2*PI / 100 );
		SetParameter( node->GetPlugin( "piechart" )->GetParameter( "angleEnd" ), 0.f, angle );
		AddChildToModelOnly( node );

		glm::vec3 vecOffset = glm::vec3( cos( angleMid ), 0, sin( angleMid ) );
		vecOffset *= offset;

		SetParameterTranslation( node->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, vecOffset );
	}
}

} //bv
