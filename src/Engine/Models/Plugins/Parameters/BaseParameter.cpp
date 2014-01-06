#include "BaseParameter.h"

namespace bv { namespace model {

namespace
{

class DefaultTimeEvaluator : public ITimeEvaluator
{
public:

    virtual TimeType    Evaluate        ( TimeType t ) const
    {
        return t;
    }

    static const ITimeEvaluator * GetDefaultEvaluator   ()
    {
        static DefaultTimeEvaluator instance;

        return &instance;
    }

};

}

// *******************************
//
BaseParameter::BaseParameter  ( const std::string & name, const ITimeEvaluator * evaluator )
    : m_lastEvaluatedTime( -1.f ) // TODO: Is it a good idea to set it to -1 ?
    , m_timeEvaluator( evaluator )
    , m_name( name )
{
    if( evaluator == nullptr )
    {
        m_timeEvaluator = DefaultTimeEvaluator::GetDefaultEvaluator();
    }
}

// *******************************
//
BaseParameter::~BaseParameter ()
{
}

} //model
} //bv
