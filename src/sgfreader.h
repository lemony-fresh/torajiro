#pragma once

#include <vector>
#include <string>

//#include "GoGame.h"

/**
 * @brief read_games reads all go games below the given path
 * @param path if path is a file, only this file is read, otherwise all sgf files below this path are read.
 * @return
 */
std::vector<GoGame*> read_games(const std::string& path);
