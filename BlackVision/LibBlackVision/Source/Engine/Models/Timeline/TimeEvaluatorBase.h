#pragma once

#include <cassert>
#include <vector>

#include "Engine/Models/Interfaces/ITimeEvaluator.h"


namespace bv { namespace model {

template< typename ITimeEvaluatorIface >
class TimeEvaluatorBase : public ITimeEvaluatorIface
{
protected:

    std::string                         m_name;
    std::vector< ITimeEvaluatorPtr >    m_children;

public:

                                                        TimeEvaluatorBase   ( const std::string & name );
    virtual                                             ~TimeEvaluatorBase  ();

	static ISerializablePtr                             Create              ( ISerializer&  );

    virtual void                                        AddChild            ( ITimeEvaluatorPtr child ) override;

    virtual ITimeEvaluatorPtr                           GetChild            ( const std::string & name ) override;
    virtual const std::vector< ITimeEvaluatorPtr > &    GetChildren         () override;

    virtual bool                                        RemoveChild         ( ITimeEvaluatorPtr child ) override;
    virtual bool                                        RemoveChild         ( const std::string & name ) override;

    virtual const std::string &                         GetName             () const override;

    virtual void                                        SetGlobalTime       ( TimeType t ) override;

protected:

    virtual void                                        SetGlobalTimeImpl   ( TimeType t ) = 0;

};

} //model
} //bv

#include "TimeEvaluatorBase.inl"
