#include "Mathematics\defines.h"
#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Timeline/TimelineManager.h"
#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"
#include "NewModelTestNodes.h"

namespace bv {

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
		for( SizeType i = 0; i < percents.size(); i++ )
			descs.push_back( PieChartSubnodeDesc( percents[i], offsets[i] ) );

		Init( timelineManager, timeEvaluator, descs );
	}

	PieChartNode( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, const std::vector< PieChartSubnodeDesc > descs )
		: BasicNode( "PieChartNode", timeEvaluator ) 
	{ 
		Init( timelineManager, timeEvaluator, descs );
	}

private:

	void Init( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, const std::vector< PieChartSubnodeDesc > descs );

};

} //bv
