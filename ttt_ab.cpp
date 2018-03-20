// ttt_ab.cpp
// Compile with: g++ -std=c++11 -o ttt_ab ttt_ab.cpp ttt.cpp

#include <iostream>
#include <algorithm>
#include <map>
#include <limits>
#include "ttt.h"

enum class PlayerType { Human, Computer };

/*int getScore(const Player player, const State board, int ply){
    Player opponent = (player == Player::X) ? Player::O : Player::X;

    if(getWinner(board) == player){
        return 10 - ply;
    }
    else if(getWinner(board) == opponent){
        return  ply - 10;
    } else return 0;
}*/

int evaluateLine(const State &board, const Player &player, int row1, int col1, int row2, int col2, int row3, int col3)
{
    int score = 0;

    // First cell
    if (board[row1 + 3 * col1] == Player::O) {
        score = 1;
    } else if (board[row1 + 3 * col1] == Player::X) {
        score = -1;
    }

    // Second cell
    if (board[row2 + 3 * col2] == Player::O) {
        if (score == 1) {   // cell1 is mySeed
            score = 10;
        } else if (score == -1) {  // cell1 is oppSeed
            return 0;
        } else {  // cell1 is empty
            score = 1;
        }
    } else if (board[row2 + 3 * col2] == Player::X) {
        if (score == -1) { // cell1 is oppSeed
            score = -10;
        } else if (score == 1) { // cell1 is mySeed
            return 0;
        } else {  // cell1 is empty
            score = -1;
        }
    }

    // Third cell
    if (board[row3 + 3 * col3] == Player::O) {
        if (score > 0) {  // cell1 and/or cell2 is mySeed
            score *= 10;
        } else if (score < 0) {  // cell1 and/or cell2 is oppSeed
            return 0;
        } else {  // cell1 and cell2 are empty
            score = 1;
        }
    } else if (board[row3 + 3 * col3] == Player::X) {
        if (score < 0) {  // cell1 and/or cell2 is oppSeed
            score *= 10;
        } else if (score > 1) {  // cell1 and/or cell2 is mySeed
            return 0;
        } else {  // cell1 and cell2 are empty
            score = -1;
        }
    }
    return score;
}

int eval(const State &board, const Player &player)
{
    int score = 0;

    score += evaluateLine(board, player, 0, 0, 0, 1, 0, 2);  // row 0
    score += evaluateLine(board, player, 1, 0, 1, 1, 1, 2);  // row 1
    score += evaluateLine(board, player, 2, 0, 2, 1, 2, 2);  // row 2
    score += evaluateLine(board, player, 0, 0, 1, 0, 2, 0);  // col 0
    score += evaluateLine(board, player, 0, 1, 1, 1, 2, 1);  // col 1
    score += evaluateLine(board, player, 0, 2, 1, 2, 2, 2);  // col 2
    score += evaluateLine(board, player, 0, 0, 1, 1, 2, 2);  // diagonal
    score += evaluateLine(board, player, 0, 2, 1, 1, 2, 0);  // alternate diagonal

    return score;
}

std::vector<int> minimax(const State &board, const Player &player, int ply, int alpha, int beta)
{
    std::vector<Move> moves = getMoves(board);

    int score;
    int bestMove = -1;

    if(ply == 0 || moves.size() == 0){
        score = eval(board, player);
        //score = getScore(player, board, ply);
        return {score,bestMove};
    }
    else
    {
        for(Move m: moves)
        {
            State tempboard = doMove(board, m);;

            if(player == Player::O)
            {
                score = minimax(tempboard, Player::X, ply--, alpha, beta)[0];

                if(score > alpha)
                {
                    alpha = score;
                    bestMove = m;
                }
            }
            else
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

