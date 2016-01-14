#pragma once
#include "CoreDEF.h"
#include "Engine/Models/Interfaces/INodeLogic.h"
#include "Engine/Types/Values/TypedValues.h"
#include "Engine/Models/Plugins/Parameters/CachedSimpleTypedParameters.h"
#include "Engine/Models/Interfaces/ITimeEvaluator.h"

namespace bv {
namespace model {

class BasicNode;
typedef std::shared_ptr< BasicNode > BasicNodePtr;

} // model
} // bv

namespace bv { namespace widgets {


	class WidgetCounter;

DEFINE_PTR_TYPE( WidgetCounter )
DEFINE_CONST_PTR_TYPE( WidgetCounter )

class WidgetCounter:  public model::INodeLogic, public std::enable_shared_from_this< WidgetCounter >
{
private:
	bv::model::BasicNode *					m_parentNode;
	bool									m_isFinalized;
	bv::model::ParamFloatPtr				m_param;
	bv::ValueFloatPtr						m_value;

public:
	explicit WidgetCounter(bv::model::BasicNode * parent, bv::model:: ITimeEvaluatorPtr timeEvaluator);
	~WidgetCounter(void);


	virtual void	Initialize		()				override {}
	virtual void	Update			( TimeType t )	override;
	virtual void	Deinitialize	()				override {}
	static		WidgetCounterPtr Create	( bv::model::BasicNode * parent,bv::model:: ITimeEvaluatorPtr timeEvaluator);
	bv::model::IParameterPtr       GetValueParam       ();


    virtual void                Serialize       ( ISerializer& ser ) const override;
    static ISerializablePtr     Create          ( const IDeserializer& deser );

    virtual bool                HandleEvent     ( IDeserializer& eventSer, ISerializer& response ) override;
};

}
}
