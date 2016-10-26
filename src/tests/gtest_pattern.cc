#include <gtest/gtest.h>

// must be included first
#include "SgSystem.h"

#include "SgInit.h"
#include "GoInit.h"

#include "pattern.h"

TEST(BitBoardTest, DISABLED_PatternsUnmodifiedAfterMatching)
{
    SgInit();
    GoInit();

    int const N = 9;

    // TODO: write test that checks that patterns are unmodified (not translated, etc.) after the matching process

//    Pattern<N> pat;

    SgFini();
    GoFini();
}
