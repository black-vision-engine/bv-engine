#include "IReplicationModifier.h"

namespace bv { namespace model
{

struct ParamValDelta
{
    IValueConstPtr  delta;

    TimeType        deltaTime;

    TimeType        startTime;
};

class ShiftReplicationModifier;

DEFINE_CONST_PTR_TYPE( ShiftReplicationModifier )

class ShiftReplicationModifier : public IReplicationModifier
{
public:

    virtual void                            Apply   ( const BasicNodeConstPtr & prev, const BasicNodePtr & next ) const override;

    void                                    AddParamShift( const std::string & pluginName, const std::string & paramName, ParamValDelta & shift );

    static ShiftReplicationModifierConstPtr Create  ();

private:

    explicit                ShiftReplicationModifier ();

    typedef std::map< std::pair< std::string, std::string >, ParamValDelta > ParamsShiftsMapType;
    ParamsShiftsMapType     m_paramsShifts;
};

} // model
} // bv