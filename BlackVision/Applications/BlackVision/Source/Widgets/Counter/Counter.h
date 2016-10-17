#pragma once

#include "Widgets/NodeLogicBase.h"

#include "Engine/Types/Values/TypedValues.h"


namespace bv { namespace model {

class BasicNode;
DEFINE_WEAK_PTR_TYPE( BasicNode )

} // model
} // bv


namespace bv { namespace nodelogic {


class WidgetCounter;

DEFINE_PTR_TYPE( WidgetCounter )
DEFINE_CONST_PTR_TYPE( WidgetCounter )


class WidgetCounter:  public model::NodeLogicBase, public std::enable_shared_from_this< WidgetCounter >
{
private:

    static const std::string        m_type;

    struct PARAMETERS
    {
        static const std::string    PRECISION;
        static const std::string    VALUE;
    };

private:

	model::BasicNodeWeakPtr     m_parentNode;

    ValueIntPtr                 m_precision;
    ValueFloatPtr               m_value;

public:

	explicit        WidgetCounter   ( model::BasicNodeWeakPtr parent, model::ITimeEvaluatorPtr timeEvaluator );
	                ~WidgetCounter  ();


	virtual void	Initialize		()				override {}
	virtual void	Update			( TimeType t )	override;
	virtual void	Deinitialize	()				override {}

    virtual const std::string &     GetType             () const override;
    static const std::string &      Type                ();

	static WidgetCounterPtr         Create              ( model::BasicNodeWeakPtr parent, model:: ITimeEvaluatorPtr timeEvaluator);


    virtual void                Serialize       ( ISerializer & ser ) const override;
    static WidgetCounterPtr     Create          ( const IDeserializer & deser, model::BasicNodeWeakPtr parent );

    virtual bool                HandleEvent     ( IDeserializer & eventSer, ISerializer & response, BVProjectEditor * editor  ) override;

};

}
}
