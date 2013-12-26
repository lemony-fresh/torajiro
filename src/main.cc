#include <fstream>
#include <iostream>

// must be included first
#include "SgSystem.h"

#include "SgBlackWhite.h"
#include "SgGameReader.h"
#include "SgInit.h"
#include "SgNode.h"
#include "SgDebug.h"
#include "GoGame.h"
#include "GoInit.h"
#include "GoNodeUtil.h"
#include "SgPoint.h"

#include "basics/bitboard.h"


int main(){
    /* initialize libs */
    SgInit();
    GoInit();

    /* read test file */
    std::string testfile("../sgf/9x9_test.sgf");
    std::ifstream in(testfile.c_str());
    if (! in) { std::cerr << "Could not open file " << testfile << std::endl; return 1; }
    SgGameReader reader(in);
    SgNode* root = reader.ReadGame();
    if (root == 0) { std::cerr << "No games in file " << testfile << std::endl; return 1; }
    if (reader.GetWarnings().any()){
        SgWarning() << testfile << ":\n";
        reader.PrintWarnings(SgDebug());
    }

    /* convert node tree into game */
    GoGame game;
    game.Init(root);
    GoRules rules;
    rules.SetKomi(GoNodeUtil::GetKomi(game.CurrentNode()));
    rules.SetHandicap(GoNodeUtil::GetHandicap(game.CurrentNode()));
    game.SetRulesGlobal(rules);

    std::cout << "board size: " << game.Board().Size() << std::endl;

    std::cout << game.Board() << std::endl;

    std::vector<SgMove> gameline;
    Bitboard<9> test;
    while(true){
        SgMove move = game.CurrentMove();
        if(!SgIsSpecialMove(move))
            gameline.push_back(game.CurrentMove());

        const GoBoard& board = game.Board();
        for(int y = 0; y < 9; ++y)
            for(int x = 0; x < 9; ++x){
                SgPoint p = SgPointUtil::Pt(x+1, y+1);
                test.set(x, y, board.GetColor(p));
            }
        std::cout << test;


        if(!game.CanGoInDirection(SgNode::Direction::NEXT))
            break;
        game.GoInDirection(SgNode::Direction::NEXT);
    }

    SgFini();
    GoFini();

    return 0;
}
