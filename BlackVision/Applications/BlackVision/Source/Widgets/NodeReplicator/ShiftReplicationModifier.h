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

DEFINE_PTR_TYPE( ShiftReplicationModifier )
DEFINE_CONST_PTR_TYPE( ShiftReplicationModifier )

class ShiftReplicationModifier : public IReplicationModifier
{
public:

    virtual void                            Apply               ( const BasicNodeConstPtr & prev, const BasicNodePtr & next ) const override;

    void                                    AddParamShift       ( const std::string & pluginName, const std::string & paramName, const ParamValDelta & shift );

    static ShiftReplicationModifierPtr      Create  ();

    virtual void                            Serialize           ( ISerializer& ser ) const override;
    static ShiftReplicationModifierPtr      Create              ( const IDeserializer& deser );

private:

    void                                    ApplyTranslationDelta ( const ParamValDelta & delta, const BasicNodePtr & node ) const;
    void                                    ApplyScaleDelta       ( const ParamValDelta & delta, const BasicNodePtr & node ) const;
    void                                    ApplyRotationDelta    ( const ParamValDelta & delta, const BasicNodePtr & node ) const;

    explicit                ShiftReplicationModifier ();

    typedef std::map< std::pair< std::string, std::string >, ParamValDelta > ParamsShiftsMapType;
    ParamsShiftsMapType     m_paramsShifts;

};

} // model
} // bv