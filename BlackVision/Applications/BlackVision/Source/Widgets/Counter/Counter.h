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

namespace bv { namespace nodelogic {


	class WidgetCounter;

DEFINE_PTR_TYPE( WidgetCounter )
DEFINE_CONST_PTR_TYPE( WidgetCounter )

class WidgetCounter:  public model::INodeLogic, public std::enable_shared_from_this< WidgetCounter >
{
private:
    static const std::string        m_type;

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

    virtual const std::string &     GetType             () const override;
    static const std::string &      Type                ();

	static WidgetCounterPtr         Create              ( bv::model::BasicNode * parent,bv::model:: ITimeEvaluatorPtr timeEvaluator);
	bv::model::IParameterPtr        GetValueParam       ();

    virtual model::IParameterPtr                     GetParameter        ( const std::string & name ) const override;
    virtual const std::vector< model::IParameterPtr > & GetParameters    () const override;


    virtual void                Serialize       ( ISerializer& ser ) const override;
    static WidgetCounterPtr     Create          ( const IDeserializer& deser, bv::model::BasicNode * parent );

    virtual bool                HandleEvent     ( IDeserializer& eventSer, ISerializer& response, BVProjectEditor * editor  ) override;
};

}
}
