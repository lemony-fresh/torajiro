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
    SgInit();
    GoInit();

    const int N = 9;

    BitboardPtr<N> board = Bitboard<N>::create();
    for (int y = 1; y <= N; ++y)
        for (int x = 1; x <= N; ++x)
            if (x % 3 == 1)
                board->set(SgPointUtil::Pt(x, y), SG_BLACK);
            else if (x % 3 == 0)
                board->set(SgPointUtil::Pt(x, y), SG_WHITE);

    GoPlayerMove move(SG_BLACK, SgPointUtil::Pt(2,3));
    auto combo = std::make_pair(board, move);

    flip_horizontally(combo);

    EXPECT_EQ(SG_BLACK, combo.second.Color());
    EXPECT_EQ(SgPointUtil::Pt(8,3), combo.second.Point());

    for (int y = 1; y < N; ++y)
        for (int x = 1; x < N; ++x)
            if (x % 3 == 1)
                EXPECT_EQ(SG_WHITE, combo.first->get(SgPointUtil::Pt(x, y)));
            else if (x % 3 == 2)
                EXPECT_EQ(SG_EMPTY, combo.first->get(SgPointUtil::Pt(x, y)));
            else if (x % 3 == 0)
                EXPECT_EQ(SG_BLACK, combo.first->get(SgPointUtil::Pt(x, y)));

    SgFini();
    GoFini();
}

TEST(NormalizationTest, VerticalFlip)
{
    SgInit();
    GoInit();

    const int N = 9;

    BitboardPtr<N> board = Bitboard<N>::create();
    for (int y = 1; y <= N; ++y)
        for (int x = 1; x <= N; ++x)
            if (y % 3 == 1)
                board->set(SgPointUtil::Pt(x, y), SG_BLACK);
            else if (y % 3 == 0)
                board->set(SgPointUtil::Pt(x, y), SG_WHITE);

    GoPlayerMove move(SG_BLACK, SgPointUtil::Pt(2,3));
    auto combo = std::make_pair(board, move);

    flip_vertically(combo);

    EXPECT_EQ(SG_BLACK, combo.second.Color());
    EXPECT_EQ(SgPointUtil::Pt(2,7), combo.second.Point());

    for (int y = 1; y < N; ++y)
        for (int x = 1; x < N; ++x)
            if (y % 3 == 1)
                EXPECT_EQ(SG_WHITE, combo.first->get(SgPointUtil::Pt(x, y)));
            else if (y % 3 == 2)
                EXPECT_EQ(SG_EMPTY, combo.first->get(SgPointUtil::Pt(x, y)));
            else if (y % 3 == 0)
                EXPECT_EQ(SG_BLACK, combo.first->get(SgPointUtil::Pt(x, y)));

    SgFini();
    GoFini();
}

TEST(NormalizationTest, DiagonalFlip)
{
    SgInit();
    GoInit();

    const int N = 9;

    BitboardPtr<N> board = Bitboard<N>::create();
    for (int y = 1; y <= N; ++y)
        for (int x = 1; x <= N; ++x)
            if (x % 3 == 1)
                board->set(SgPointUtil::Pt(x, y), SG_BLACK);
            else if (x % 3 == 0)
                board->set(SgPointUtil::Pt(x, y), SG_WHITE);

    GoPlayerMove move(SG_BLACK, SgPointUtil::Pt(2,3));
    auto combo = std::make_pair(board, move);

    flip_on_secondary_diagonal(combo);

    EXPECT_EQ(SG_BLACK, combo.second.Color());
    EXPECT_EQ(SgPointUtil::Pt(7,8), combo.second.Point());

    for (int y = 1; y < N; ++y)
        for (int x = 1; x < N; ++x)
            if (y % 3 == 1)
                EXPECT_EQ(SG_WHITE, combo.first->get(SgPointUtil::Pt(x, y)));
            else if (y % 3 == 2)
                EXPECT_EQ(SG_EMPTY, combo.first->get(SgPointUtil::Pt(x, y)));
            else if (y % 3 == 0)
                EXPECT_EQ(SG_BLACK, combo.first->get(SgPointUtil::Pt(x, y)));

    SgFini();
    GoFini();
}

TEST(NormalizationTest, Normalization)
{
    SgInit();
    GoInit();

    const int N = 9;

    BitboardPtr<N> board = Bitboard<N>::create();
    for (int y = 1; y <= N; ++y)
        for (int x = 1; x <= N; ++x)
            if (x % 3 == 1)
                board->set(SgPointUtil::Pt(x, y), SG_BLACK);
            else if (x % 3 == 0)
                board->set(SgPointUtil::Pt(x, y), SG_WHITE);

    GoPlayerMove move(SG_BLACK, SgPointUtil::Pt(3,8));
    auto combo = std::make_pair(board, move);
    std::vector<std::pair<BitboardPtr<N>, GoPlayerMove>> vec;
    vec.push_back(combo);
\
    normalize_all_moves(vec);

    EXPECT_EQ(SG_BLACK, vec[0].second.Color());
    EXPECT_EQ(SgPointUtil::Pt(8,3), vec[0].second.Point());

    for (int y = 1; y < N; ++y)
        for (int x = 1; x < N; ++x)
            if (y % 3 == 1)
                EXPECT_EQ(SG_BLACK, vec[0].first->get(SgPointUtil::Pt(x, y)));
            else if (y % 3 == 2)
                EXPECT_EQ(SG_EMPTY, vec[0].first->get(SgPointUtil::Pt(x, y)));
            else if (y % 3 == 0)
                EXPECT_EQ(SG_WHITE, vec[0].first->get(SgPointUtil::Pt(x, y)));

    SgFini();
    GoFini();
}
