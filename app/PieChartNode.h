#include "Mathematics\defines.h"
#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Timeline/TimelineManager.h"
#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"
#include "NewModelTestNodes.h"

namespace bv {

namespace {

struct PieChartSubnodeDesc
{
	float percent;
	float offset;
	enum SubnodeType {
		COLORED,
		LINEAR_GRADIENTED
	} type;

	PieChartSubnodeDesc( float p, float o, SubnodeType t = LINEAR_GRADIENTED )
		: percent(p), offset(o), type(t) {}
};

class PieChartNode : public model::BasicNode // FIXME: separate file
{
public:
    //PieChartNode( const std::string & name, model::ITimeEvaluatorPtr timeEvaluator, const model::PluginsManager * pluginsManager = nullptr );
    PieChartNode( /*const std::string & name, */model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, const std::vector< float > percents, const std::vector< float > offsets )
		: BasicNode( "PieChartNode", timeEvaluator ) 
	{ 
		std::vector< PieChartSubnodeDesc > descs;

		assert( percents.size() == offsets.size() );
		for( int i = 0; i < percents.size(); i++ )
			descs.push_back( PieChartSubnodeDesc( percents[i], offsets[i] ) );

		Init( timelineManager, timeEvaluator, descs );
	}

private:
	void Init( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, const std::vector< PieChartSubnodeDesc > descs )
	{
		AddPlugin( "DEFAULT_TRANSFORM", timeEvaluator );

		auto simpleTransform = GetPlugin( "transform" )->GetParameter( "simple_transform" );
		SetParameterScale( simpleTransform, 0, 0.0f, glm::vec3( 1.f, .2f, 1.f ) );
		SetParameterTranslation( simpleTransform, 0, 0.0f, glm::vec3( 0, 0, -1.f) );
		SetParameterRotation ( simpleTransform, 0, 0.0f, glm::vec3( 1.f, 0.f, 0.f ), 20.f );

		float angle = 0;

		for( auto desc : descs )
		{
			float percent = desc.percent;
			float offset = desc.offset;

			auto node = SimpleNodesFactory::CreateCreedGradedPieChartNode( timelineManager, timeEvaluator, 0 ); // FIXME : flexible subnode type
			SetParameter( node->GetPlugin( "piechart" )->GetParameter( "angleStart" ), 0.f, angle );
			float angleMid = angle + float( percent * PI / 100 );
			angle += float( percent * 2*PI / 100 );
			SetParameter( node->GetPlugin( "piechart" )->GetParameter( "angleEnd" ), 0.f, angle );
			AddChild( node );

			glm::vec3 vecOffset = glm::vec3( cos( angleMid ), 0, sin( angleMid ) );
			vecOffset *= offset;

			SetParameterTranslation( node->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, vecOffset );
		}
	}
};

} }