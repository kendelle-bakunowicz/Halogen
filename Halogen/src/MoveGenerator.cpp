#include "MoveGenerator.h"

void OrderMoves(std::vector<Move>& moves, Position& position, int distanceFromRoot, Move& hashMove, std::vector<Killer>& KillerMoves, unsigned int(&HistoryMatrix)[N_PLAYERS][N_SQUARES][N_SQUARES]);

MoveGenerator::MoveGenerator(Position& Position, int DistanceFromRoot, Move HashMove, std::vector<Killer>& KillerMoves, unsigned int(&HistoryMatrix)[N_PLAYERS][N_SQUARES][N_SQUARES]) : position(Position), distanceFromRoot(DistanceFromRoot), hashMove(HashMove), killerMoves(KillerMoves), historyMatrix(HistoryMatrix)
{
	stage = Stage::GEN_TT_MOVE;
	index = 0;
}

bool MoveGenerator::GetNext(Move& move)
{
	if (stage == Stage::GEN_TT_MOVE)
	{
		stage = Stage::GIVE_TT_MOVE;
		TTmove = hashMove;
	}

	if (stage == Stage::GIVE_TT_MOVE)
	{
		stage = Stage::GEN_ALL_OTHERS;
		if (!TTmove.IsUninitialized() && MoveIsLegal(position, TTmove))
		{
			move = TTmove;
			return true;
		}
	}

	if (stage == Stage::GEN_ALL_OTHERS)
	{
		stage = Stage::GIVE_ALL_OTHERS;
		LegalMoves(position, moveList);
		OrderMoves(moveList, position, distanceFromRoot, hashMove, killerMoves, historyMatrix);
		index = 0;
	}

	if (stage == Stage::GIVE_ALL_OTHERS)
	{
		if (index < moveList.size())
		{
			move = moveList[index++];
			return true;
		}
	}

	return false;
}

void OrderMoves(std::vector<Move>& moves, Position& position, int distanceFromRoot, Move& hashMove, std::vector<Killer>& KillerMoves, unsigned int(&HistoryMatrix)[N_PLAYERS][N_SQUARES][N_SQUARES])
{
	/*
	We want to order the moves such that the best moves are more likely to be further towards the front.

	The order is as follows:

	1. Hash move												= 10m
	2. Queen Promotions											= 9m
	3. Winning captures											= +8m
	4. Killer moves												= ~7m
	5. Losing captures											= -6m
	6. Quiet moves (further sorted by history matrix values)	= 0-1m
	7. Underpromotions											= -1

	Note that typically the maximum value of the history matrix does not exceed 1,000,000 after a minute
	and as such we choose 1m to be the maximum allowed value

	*/

	for (size_t i = 0; i < moves.size(); i++)
	{
		//Hash move
		if (moves[i] == hashMove)
		{
			moves[i].orderScore = 10000000;
		}

		//Promotions
		else if (moves[i].IsPromotion())
		{
			if (moves[i].GetFlag() == QUEEN_PROMOTION || moves[i].GetFlag() == QUEEN_PROMOTION_CAPTURE)
			{
				moves[i].orderScore = 9000000;
			}
			else
			{
				moves[i].orderScore = -1;
			}
		}

		//Captures
		else if (moves[i].IsCapture())
		{
			int SEE = 0;

			if (moves[i].GetFlag() != EN_PASSANT)
			{
				SEE = seeCapture(position, moves[i]);
			}

			if (SEE >= 0)
			{
				moves[i].orderScore = 8000000 + SEE;
			}

			if (SEE < 0)
			{
				moves[i].orderScore = 6000000 + SEE;
			}
		}

		//Killers
		else if (moves[i] == KillerMoves.at(distanceFromRoot).move[0])
		{
			moves[i].orderScore = 7500000;
		}

		else if (moves[i] == KillerMoves.at(distanceFromRoot).move[1])
		{
			moves[i].orderScore = 6500000;
		}

		//Quiet
		else
		{
			moves[i].orderScore = std::min(1000000U, HistoryMatrix[position.GetTurn()][moves[i].GetFrom()][moves[i].GetTo()]);
		}
	}

	std::stable_sort(moves.begin(), moves.end(), [](const Move& a, const Move& b)
		{
			return a.orderScore > b.orderScore;
		});
}

int see(Position& position, int square, bool side)
{
	int value = 0;
	Move capture = GetSmallestAttackerMove(position, square, side);

	if (!capture.IsUninitialized())
	{
		int captureValue = PieceValues(position.GetSquare(capture.GetTo()));

		position.ApplySEECapture(capture);
		value = std::max(0, captureValue - see(position, square, !side));	// Do not consider captures if they lose material, therefor max zero 
		position.RevertSEECapture();
	}

	return value;
}

int seeCapture(Position& position, const Move& move)
{
	assert(move.GetFlag() == CAPTURE);	//Don't seeCapture with promotions or en_passant!

	bool side = position.GetTurn();

	int value = 0;
	int captureValue = PieceValues(position.GetSquare(move.GetTo()));

	position.ApplySEECapture(move);
	value = captureValue - see(position, move.GetTo(), !side);
	position.RevertSEECapture();

	return value;
}

bool QuiesMoveGenerator::GetNext(Move& move)
{
	if (stage == Stage::GEN_ALL)
	{
		stage = Stage::GIVE_ALL;
		QuiescenceMoves(position, moveList);
		OrderMoves(moveList, position, distanceFromRoot, hashMove, killerMoves, historyMatrix);
		index = 0;
	}

	if (stage == Stage::GIVE_ALL)
	{
		if (index < moveList.size())
		{
			move = moveList[index++];
			return true;
		}
	}

	return false;
}

QuiesMoveGenerator::QuiesMoveGenerator(Position& Position, int DistanceFromRoot, Move HashMove, std::vector<Killer>& KillerMoves, unsigned int(&HistoryMatrix)[N_PLAYERS][N_SQUARES][N_SQUARES]) : position(Position), distanceFromRoot(DistanceFromRoot), hashMove(HashMove), killerMoves(KillerMoves), historyMatrix(HistoryMatrix)
{
	stage = Stage::GEN_ALL;
	index = 0;
}
