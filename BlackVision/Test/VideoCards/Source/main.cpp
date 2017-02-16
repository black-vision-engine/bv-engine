#include "TestVideoCardsMainThread.h"

namespace bv { namespace videocards {

// ****************************
//
TestVideoCardsMainThread *  RunTest ()
{
    TestVideoCardsMainThread * mt = new TestVideoCardsMainThread();
    mt->Start();

    return mt;
}

} // videocards
} // bv

// ****************************
//
int main()
{
    auto th = bv::videocards::RunTest ();
    th->Join();

    delete th;

    return 0;
}



