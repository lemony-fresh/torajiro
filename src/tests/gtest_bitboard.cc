#include <gtest/gtest.h>

#include <random>
#include <chrono>

// must be included first
#include "SgSystem.h"

#include "SgInit.h"
#include "SgPoint.h"
#include "SgBlackWhite.h"
#include "GoInit.h"
#include "GoPlayerMove.h"

#include "bitboard.h"
#include "sgf_reader.h"

TEST(BitBoardTest, Copy)
{
    SgInit();
    GoInit();

    int const N = 9;

    BitboardPtr<N> board = Bitboard<N>::create();

    std::chrono::microseconds ms = std::chrono::duration_cast< std::chrono::microseconds >(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    );
    std::default_random_engine position_generator(ms.count());
    std::uniform_int_distribution<int> position_distribution(1,N);
    auto position_dice = std::bind(position_distribution, position_generator);
    static_assert(SG_BLACK+1==SG_WHITE, "SG_WHITE must be one larger than SG_BLACK");
    std::default_random_engine color_generator;
    std::uniform_int_distribution<int> color_distribution(SG_BLACK, SG_WHITE);
    auto color_dice = std::bind(color_distribution, color_generator);

    for (int i = 0; i < 40; ++i)
        board->set(SgPointUtil::Pt(position_dice(),position_dice()), color_dice());

    BitboardPtr<N> copy = Bitboard<N>::create(*board);

    std::vector<SgPoint> points;
    points.push_back(SgPointUtil::Pt(2, 3));
    points.push_back(SgPointUtil::Pt(3, 2));
    for (SgPoint const & p : points)
        if(copy->get(p) == SG_BLACK)
            copy->set(p, SG_WHITE);
        else
            copy->set(p, SG_BLACK);

    for (std::size_t x = 1; x <= N; ++x)
        for (std::size_t y = 1; y <= N; ++y) {
            SgPoint p = SgPointUtil::Pt(x,y);
            if((x==2 && y==3) || (x==3 && y==2))
                EXPECT_NE(board->get(p), copy->get(p));
            else
                EXPECT_EQ(board->get(p), copy->get(p));
        }

    SgFini();
    GoFini();
}

TEST(BitBoardTest, Invert)
{
    SgInit();
    GoInit();

    int const N = 9;

    BitboardPtr<N> board = Bitboard<N>::create();

    std::chrono::microseconds ms = std::chrono::duration_cast< std::chrono::microseconds >(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    );
    std::default_random_engine position_generator(ms.count());
    std::uniform_int_distribution<int> position_distribution(1,N);
    auto position_dice = std::bind(position_distribution, position_generator);
    static_assert(SG_BLACK+1==SG_WHITE, "SG_WHITE must be one larger than SG_BLACK");
    std::default_random_engine color_generator;
    std::uniform_int_distribution<int> color_distribution(SG_BLACK, SG_WHITE);
    auto color_dice = std::bind(color_distribution, color_generator);

    for (int i = 0; i < 40; ++i)
        board->set(SgPointUtil::Pt(position_dice(),position_dice()), color_dice());

    BitboardPtr<N> copy = Bitboard<N>::create(*board);
    copy->invert();

    for (std::size_t x = 1; x <= N; ++x)
        for (std::size_t y = 1; y <= N; ++y) {
            SgPoint p = SgPointUtil::Pt(x,y);
            if (board->get(p) == SG_EMPTY)
                EXPECT_EQ(board->get(p), copy->get(p));
            else
                EXPECT_EQ(SgOppBW(board->get(p)), copy->get(p));
        }

    SgFini();
    GoFini();
}

TEST(BitBoardTest, Rotations)
{
    SgInit();
    GoInit();

    int const N = 9;

    BitboardPtr<N> board = Bitboard<N>::create();

    std::chrono::microseconds ms = std::chrono::duration_cast< std::chrono::microseconds >(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    );
    std::default_random_engine position_generator(ms.count());
    std::uniform_int_distribution<int> position_distribution(1,N);
    auto position_dice = std::bind(position_distribution, position_generator);
    static_assert(SG_BLACK+1==SG_WHITE, "SG_WHITE must be one larger than SG_BLACK");
    std::default_random_engine color_generator;
    std::uniform_int_distribution<int> color_distribution(SG_BLACK, SG_WHITE);
    auto color_dice = std::bind(color_distribution, color_generator);

    for (int i = 0; i < 40; ++i)
        board->set(SgPointUtil::Pt(position_dice(),position_dice()), color_dice());

    BitboardPtr<N> copy = Bitboard<N>::create(*board);

    // this is the exact same sequence of rotations in
    // which the board is rotated in Pattern.match()
    std::vector<Bitboard<N> > rotations;
    rotations.emplace_back(*copy);
    copy->flip_horizontally();
    rotations.emplace_back(*copy);
    copy->flip_vertically();
    rotations.emplace_back(*copy);
    copy->flip_horizontally();
    rotations.emplace_back(*copy);
    copy->flip_on_secondary_diagonal();
    rotations.emplace_back(*copy);
    copy->flip_vertically();
    rotations.emplace_back(*copy);
    copy->flip_horizontally();
    rotations.emplace_back(*copy);
    copy->flip_vertically();
    rotations.emplace_back(*copy);

    std::map<int, int> my_rotation_to_fuego_rotation_mapping;
    my_rotation_to_fuego_rotation_mapping.insert(std::make_pair(0,0));
    my_rotation_to_fuego_rotation_mapping.insert(std::make_pair(1,1));
    my_rotation_to_fuego_rotation_mapping.insert(std::make_pair(2,6));
    my_rotation_to_fuego_rotation_mapping.insert(std::make_pair(3,2));
    my_rotation_to_fuego_rotation_mapping.insert(std::make_pair(4,5));
    my_rotation_to_fuego_rotation_mapping.insert(std::make_pair(5,3));
    my_rotation_to_fuego_rotation_mapping.insert(std::make_pair(6,4));
    my_rotation_to_fuego_rotation_mapping.insert(std::make_pair(7,7));

    for (std::size_t rotation = 0; rotation < rotations.size(); ++rotation)
        for (int x = 1; x <= N; ++x)
            for (int y = 1; y <= N; ++y) {
                SgPoint p = SgPointUtil::Pt(x,y);
                SgPoint fuego_rotated_point = SgPointUtil::Rotate(my_rotation_to_fuego_rotation_mapping[rotation], p, N);
                EXPECT_EQ(board->get(p), rotations[rotation].get(fuego_rotated_point));
            }
}
