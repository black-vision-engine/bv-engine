#include "gtest/gtest.h"

#include "VideoCardsUtils/TestVideoCardsMainThread.h"


namespace bv {
namespace videocards
{


static std::shared_ptr< TestVideoCardsMainThread > mt = nullptr;

// ****************************
//
TestVideoCardsMainThread *  RunTest ()
{
    mt = std::make_shared< TestVideoCardsMainThread >();
    mt->Start();

    return mt.get();
}

} // videocards
} // bv




// ****************************
//
TEST( Tools_VideoCards, InitAndRun )
{
    auto th = bv::videocards::RunTest ();
    th->Join();

    bv::videocards::mt = nullptr;
}



