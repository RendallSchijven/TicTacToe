// ttt_ab.cpp
// Compile with: g++ -std=c++11 -o ttt_ab ttt_ab.cpp ttt.cpp

#include <iostream>
#include <algorithm>
#include <map>
#include <limits>
#include "ttt.h"

enum class PlayerType { Human, Computer };

int eval(const Player player, const State board, int ply){
    Player opponent = (player == Player::X) ? Player::O : Player::X;

    if(getWinner(board) == player){
        return 10 - ply;
    }
    else if(getWinner(board) == opponent){
        return  ply - 10;
    } else return 0;
}

std::vector<int> minimax(const State &board, const Player &player, int ply, int alpha, int beta)
{
    std::vector<Move> moves = getMoves(board);

    int score;
    int bestMove = -1;

    if(ply == 0 || moves.size() == 0){
        score = eval(player, board, ply);
        return {score,bestMove};
    }
    else
    {
        for(Move m: moves)
        {
            State tempboard = doMove(board, m);;

            if(player == Player::O) //Computer
            {
                score = minimax(tempboard, Player::X, ply--, alpha, beta)[0];

                if(score > alpha)
                {
                    alpha = score;
                    bestMove = m;
                }
            }
            else //Human
            {
                score = minimax(tempboard, Player::O, ply--, alpha, beta)[0];

                if(score < beta)
                {
                    beta = score;
                    bestMove = m;
                }
            }
            //Alpha beta pruning
            if(alpha >= beta) break;
        }
        return {(player == Player::O) ? alpha : beta, bestMove};
    }
}

Move alphaBeta(const State &board, int ply)
{
    return minimax(board, getCurrentPlayer(board), ply, std::numeric_limits<int>::min(), std::numeric_limits<int>::max())[1];
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
		if (playerType[getCurrentPlayer(board)] == PlayerType::Human) {
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
		} else {
			board = doMove(board, alphaBeta(board, 5));
		}
		std::cout << board << std::endl;
		moves = getMoves(board);
	}

	return 0;
}

