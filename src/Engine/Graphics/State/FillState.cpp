#include "FillState.h"

namespace bv {

// *******************************
//
FillState::FillState  ()
    : fillMode( FillStateMode::FSM_POLYGONS )
{
}


class A
{
public:

    int b() { return 0; };

private:

    int b( int a ) { return a + b(); }

public:

    int (A::*get_pptr())(int)
    {
        return &A::b;
    }
};

int w( A * a )
{
    //&A::b;
    //int k = (char*) (&A::b);
    int (A::*memberPtr0)(int) = a->get_pptr();
    int (A::*memberPtr1)() = &A::b;

    return (a->*memberPtr0)( 10 );
    return (a->*memberPtr1)();
};

}
