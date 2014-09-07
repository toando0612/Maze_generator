#pragma once


#include "maze.h"
#include <stdexcept>
#include <string>

namespace maze
{
	class SolvingStrategy
	{
		public:
			virtual void solve_maze(maze::Maze & maze) = 0;
            SolvingStrategy() {};
            ~SolvingStrategy() {};
	};

    class CannotSolveMaze : public std::runtime_error
    {
        public:
            CannotGenerateMaze(const std::string& message)
                : std::runtime_error("Cannot solve maze: " +
                    message) {};
    };
}