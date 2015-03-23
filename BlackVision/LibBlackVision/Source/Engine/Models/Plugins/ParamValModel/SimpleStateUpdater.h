#include "Engine/Interfaces/IUpdater.h"

#include "Engine/Models/Plugins/Interfaces/IState.h"

namespace bv { namespace model {

template< typename T >
class SimpleStateUpdater : public IUpdater
{
public:
    SimpleStateUpdater( std::shared_ptr< SimpleState< T > > /*state*/, IValueConstPtr /*value*/ ) {}

    virtual void    DoUpdate    () {}
};

typedef SimpleStateUpdater< float > SimpleFloatStateUpdater;

} //model
} //bv
