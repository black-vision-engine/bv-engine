#pragma once

#include <vector>

#include "Engine/Models/Interfaces/ITimeEvaluator.h"


namespace bv { namespace model {

class TimeEvaluatorBase : public ITimeEvaluator
{
protected:

    std::string                     m_name;

    std::vector< ITimeEvaluatorPtr > m_children;

public:

                                                TimeEvaluatorBase   ( const std::string & name );
    virtual                                     ~TimeEvaluatorBase  ();

    void                                        AddChild            ( ITimeEvaluatorPtr child );

    ITimeEvaluatorPtr                           GetChild            ( const std::string & name );
    const std::vector< ITimeEvaluatorPtr > &    GetChildren         ();

    bool                                        RemoveChild         ( ITimeEvaluatorPtr child );
    bool                                        RemoveChild         ( const std::string & name );

    virtual const std::string &                 GetName             () const override;

};

} //model
} //bv
