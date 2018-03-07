// ttt_mc.cpp
// Compile with: g++ -std=c++11 -o ttt_mc ttt_mc.cpp ttt.cpp

#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include "ttt.h"

unsigned const n_trials = 1000;
unsigned const mc_match = 1;
unsigned const mc_other = 1;

enum class PlayerType { Human, Computer };

State mcTrial(const State &board)
{
    State testBoard = board;
    std::vector<Move> moves = getMoves(board);

    while(moves.size() > 0)
    {
        std::vector<int> empty;

        for (int i=0; i<9; i++) {
            if (testBoard[i] == Player::None)
            {
                empty.push_back(i);
            }
        }

        Move m = empty[rand() % empty.size()];
        testBoard = doMove(testBoard, m);
        moves = getMoves(testBoard);
    }
    return testBoard;
}

void mcUpdateScores(std::array<int,9> &scores, const State &board, const Player &player)
{
    Player winner = getWinner(board);

    for(int i = 0; i < 9; i++)
    {
        if(winner != player && board[i] == Player::X) scores[i] += mc_other;
        else if(winner != player && board[i] == Player::O) scores[i] -= mc_other;
        else if(winner == player && board[i] == Player::O) scores[i] += mc_match;
        else if(winner == player && board[i] == Player::X) scores[i] -= mc_match;
    }
}

Move getBestMove(const std::array<int, 9> &scores, const State &board)
{
    int highScore = 0;

    std::vector<int> highestPositions;
    for(int i = 0; i < 9; i++)
    {
        if(scores[i] >= highScore && board[i] == Player::None)
        {
            highScore = scores[i];
            highestPositions.push_back(i);
        }
    }
    Move m = highestPositions[rand() % highestPositions.size()];
    return m;
}

Move mcMove(const State &board, const Player &player)
{
    std::array<int, 9> scores = {0,0,0,0,0,0,0,0,0};
    for(int i = 0; i < n_trials; i++)
    {
        State tempBoard = mcTrial(board);
        mcUpdateScores(scores, tempBoard, player);
    }

    return getBestMove(scores, board);
}

int main()
{
	std::srand(std::time(0));

	std::map<Player,PlayerType> playerType;
	playerType[Player::X] = PlayerType::Human;
	playerType[Player::O] = PlayerType::Computer;

        State board = { 
            Player::None, Player::None, Player::None, 
            Player::None, Player::None, Player::None, 
            Player::None, Player::None, Player::None };
	std::cout << board << std::endl;

	std::vector<Move> moves = getMoves(board);
	while (moves.size() > 0) {
		if (playerType[getCurrentPlayer(board)] == PlayerType::Human)
		{
			std::cout << "+-+-+-+" << std::endl << 
				     "|0|1|2|" << std::endl <<
				     "+-+-+-+" << std::endl <<
				     "|3|4|5|" << std::endl <<
				     "+-+-+-+" << std::endl <<
				     "|6|7|8|" << std::endl <<
				     "+-+-+-+" << std::endl << std::endl;
			std::cout << "Enter a move ( ";
			for (Move m: moves) std::cout << m << " ";
			std::cout << "): ";
			Move m;
			std::cin >> m;
			board = doMove(board, m);
		}
        else {
			board = doMove(board, mcMove(board, getCurrentPlayer(board)));
		}
		std::cout << board << std::endl;
		moves = getMoves(board);
	}

    Player winner = getWinner(board);
    if(winner == Player::X) std::cout << "You won!" << std::endl;
    else if(winner == Player::O) std::cout << "The computer won! " << std::endl;
    else std::cout << "Draw!" << std::endl;

	return 0;
}

