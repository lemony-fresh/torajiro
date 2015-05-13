#include <gtest/gtest.h>

// must be included first
#include "SgSystem.h"

#include "SgInit.h"
#include "SgNode.h"
#include "SgPoint.h"
#include "GoGame.h"
#include "GoInit.h"

#include "bitboard.h"
#include "move_normalizer.h"

TEST(NormalizationTest, HorizontalFlip)
{
    /* initialize libs */
    SgInit();
    GoInit();

    const int N = 9;

    std::shared_ptr<Bitboard<N> > board = std::make_shared<Bitboard<N> >();
    GoPlayerMove move(SG_BLACK, SgPointUtil::Pt(2,3));

    auto combo = std::make_pair(board, move);

    flip_horizontally(combo);

    EXPECT_EQ(SG_BLACK, combo.second.Color());
    EXPECT_EQ(SgPointUtil::Pt(8,3), combo.second.Point());

    SgFini();
    GoFini();
}

TEST(NormalizationTest, VerticalFlip)
{
    /* initialize libs */
    SgInit();
    GoInit();

    const int N = 9;

    std::shared_ptr<Bitboard<N> > board = std::make_shared<Bitboard<N> >();
    GoPlayerMove move(SG_BLACK, SgPointUtil::Pt(2,3));

    auto combo = std::make_pair(board, move);

    flip_vertically(combo);

    EXPECT_EQ(SG_BLACK, combo.second.Color());
    EXPECT_EQ(SgPointUtil::Pt(2,7), combo.second.Point());

    SgFini();
    GoFini();
}

TEST(NormalizationTest, DiagonalFlip)
{
    /* initialize libs */
    SgInit();
    GoInit();

    const int N = 9;

    std::shared_ptr<Bitboard<N> > board = std::make_shared<Bitboard<N> >();
    GoPlayerMove move(SG_BLACK, SgPointUtil::Pt(2,3));

    auto combo = std::make_pair(board, move);

    flip_on_secondary_diagonal(combo);

    EXPECT_EQ(SG_BLACK, combo.second.Color());
    EXPECT_EQ(SgPointUtil::Pt(7,8), combo.second.Point());

    SgFini();
    GoFini();
}

TEST(NormalizationTest, Normalization)
{
    /* initialize libs */
    SgInit();
    GoInit();

    const int N = 9;

    std::shared_ptr<Bitboard<N> > board = std::make_shared<Bitboard<N> >();
    GoPlayerMove move(SG_BLACK, SgPointUtil::Pt(3,8));

    auto combo = std::make_pair(board, move);
    std::vector<std::pair<std::shared_ptr<Bitboard<N>>, GoPlayerMove>> vec;
    vec.push_back(combo);

    normalize_all_moves(vec);

    EXPECT_EQ(SG_BLACK, vec[0].second.Color());
    EXPECT_EQ(SgPointUtil::Pt(8,3), vec[0].second.Point());

    SgFini();
    GoFini();
}
