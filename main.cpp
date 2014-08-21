/* #include "maze.h" */

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
#include <stdexcept>
#include <climits>

#include "depthbuilder.h"
#include "maze.h"

const std::string GENERATE_FLAG = "-g";
const std::string LOAD_BIN_FLAG = "--lb";
const std::string SAVE_BIN_FLAG = "--sb"; 
const std::string SAVE_SVG_FLAG = "--sv"; 

void argument_error(std::string program_name);

int main(int argc, char * argv[])
{
	bool input_specified = false;
	bool generating_maze = false; /* Otherwise loading binary */
	std::string seed;
	std::string load_path;
	bool output_specified = false;
	bool saving_binary = false; /* Otherwise saving svg */
	std::string save_path;
	unsigned long height, width;

	maze::Maze * maze;
	
	std::string program_name = argv[0];
	
	
	/* Your program should be able to run from the command line, with arguments to specify how the 
	program should run, ie:
	./exe --lb filename.maze --sv filename.svg # load binary file and save svg file
	./exe -g seed --sb filename.maze # generate with seed value, save binary file
	./exe -g seed --sv filename.svg # generate with seed value, save svg file
	./exe -g seed --sb filename.maze --sv filename.svg # gen with seed, save binary, save svg

	You should also be able to specify the width/height of the maze when generating (but not when 
	loading), ie:

	./exe -g seed width height �

	Note that command line args can be given in any order, and an appropriate error message 
	should be given if inappropriate args are entered. */
	
	/* First check number of arguments */
	/* Minimum is 5 - maximum is 7 (including program name) */
	if (argc < 5 || argc > 7)
	{
		std::cerr << "Missing/too many args.\n\n";
		argument_error(program_name);
		return 1;
	}
	
	/* Now go through the arguments */
	for(int i = 1; i < argc; i++) /* Skip program name */
	{
		std::string current_argument = argv[i];
		std::string next_argument;
		if(i+1 < argc) /* Making sure we have a next argument */
		{
			next_argument = argv[i+1];
		}
		else
		{
			next_argument = "";
		}


		if(current_argument == GENERATE_FLAG)
		{
			generating_maze = true;
			input_specified = true;
			/* now try grab seed */
			if(next_argument == "") /* No more arguments! */
			{
				std::cerr << "Found generate flag without subsequent data.\n\n";
				argument_error(program_name);
				return 1;
			}
			else
			{
					seed = next_argument;
					i+=2; /* we no longer need to read the flag + seed */


					/* Now test for height / width */
					if(i+1 < argc)
					{
						/* There are two more arguments after the seed */
						try 
						{
							height = std::stol(argv[i]);
							width = std::stol(argv[i+1]);
						}
						catch (const std::invalid_argument& e)
						{
							std::cerr << "Height and/or width have to be numbers.\n\n";
							argument_error(program_name);
							return 1;
						}
						catch (const std::out_of_range& e)
						{
							std::cerr << "Maximum height/width: " << ULONG_MAX << ".\n\n";
							argument_error(program_name);
							return 1;
						}
						
						i++; /* for loop will skip width also */

					}
					else
					{
						std::cerr << "Found generate flag without height and width.\n\n";
						argument_error(program_name);
						return 1;
					}
			}
		}
		else if(current_argument == LOAD_BIN_FLAG)
		{
			generating_maze = false;
			input_specified = true;
			/* Now try grab filepath */
			if(next_argument == "") /* No more arguments! */
			{
				std::cerr << "Found load flag without subsequent data.\n\n";
				argument_error(program_name);
				return 1;
			}
			else
			{
					load_path = next_argument;
					i++; /* we no longer need to read the argument */
			}
		}
		else if(current_argument == SAVE_BIN_FLAG)
		{
			output_specified = true;
			saving_binary = true;
			/* Now try grab filepath */
			if(next_argument == "") /* No more arguments! */
			{
				std::cerr << "Found save bin flag without subsequent data.\n\n";
				argument_error(program_name);
				return 1;
			}
			else
			{
					save_path = next_argument;
					i++; /* we no longer need to read the argument */
			}
		}
		else if(current_argument == SAVE_SVG_FLAG)
		{
			output_specified = true;
			saving_binary = false;
			/* Now try grab filepath */
			if(next_argument == "") /* No more arguments! */
			{
				std::cerr << "Found save svg flag without subsequent data.\n\n";
				argument_error(program_name);
				return 1;
			}
			else
			{
					save_path = next_argument;
					i++; /* we no longer need to read the argument */
			}
		}
		else
		{
			/* Found extra info */
			std::cerr << "Ignoring argument \"" << current_argument << "\".\n\n";
		}		
		
	}


	/* Now that we have gone through all the arguments, lets see the validity */

	if(!input_specified || !output_specified)
	{
		/* We need a type of input and output */
		std::cerr << "Missing input or output.\n\n";
		argument_error(program_name);
		return 1;
	}

	/* Now we have our configuration */
	if(generating_maze)
	{
		std::cout << "Generating maze.\n";
		std::cout << "Height: " << height << ". \n";
		std::cout << "Width: " << width << ". \n";
		maze = new maze::Maze(height, width);
		const std::string &seed_ref = seed;
		const unsigned long &height_ref = height;
		const unsigned long &width_ref = width;
		maze::build(maze, height_ref, width_ref, seed_ref);
	}
	else
	{
		std::cout << "Loading maze binary from: " << load_path << ". \n";
		maze = new maze::Maze(load_path);
	}

	if(saving_binary)
	{
		std::cout << "Saving maze to binary: " << save_path << ". \n";
		maze->save_maze_to_binary(save_path);
	}
	else
	{
		std::cout << "Saving maze to SVG: " << save_path << ". \n";
		maze->save_maze_to_svg(save_path);
	}


	delete maze;
	
	
} 

void argument_error(std::string program_name)
{
	std::cout << "Incorrect use of arguments. Please use:\n";
	std::cout << program_name << " <[-g seed height width] OR [--lb filename.maze]> ";
	std::cout << "<[--sb filename.maze] OR [--sv filename.svg]>\n";
}