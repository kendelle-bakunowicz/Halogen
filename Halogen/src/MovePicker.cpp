#include "MovePicker.h"

bool GetHashMove(const Position& position, int distanceFromRoot, Move& move)
{
	TTEntry hash = tTable.GetEntry(position.GetZobristKey());
	if (CheckEntry(hash, position.GetZobristKey()))
	{
		tTable.SetNonAncient(position.GetZobristKey(), position.GetTurnCount(), distanceFromRoot);
		move = hash.GetMove();
		return true;
	}

	return false;
}

MovePicker::MovePicker(int DistanceFromRoot, bool Quies)
{
	quies = Quies;
	distanceFromRoot = DistanceFromRoot;
	index = 0;

	if (quies)
		stage = Stage::GENERATE_OTHERS;
	else
		stage = Stage::TTMOVE;
}

MovePicker::~MovePicker()
{
}

bool MovePicker::Next(Position& position, Move& move, std::vector<Killer>& KillerMoves, unsigned int(&HistoryMatrix)[N_PLAYERS][N_SQUARES][N_SQUARES])
{
	if (stage == Stage::TTMOVE)
	{
		stage = Stage::GENERATE_OTHERS;
		if (!quies && GetHashMove(position, distanceFromRoot, move) && MoveIsLegal(position, move))
		{	
			TTmove = move;
			return true;
		}
	}

	if (stage == Stage::GENERATE_OTHERS)
	{
		if (quies)
			QuiescenceMoves(position, moves);
		else
			LegalMoves(position, moves);
		OrderMoves(position, KillerMoves, HistoryMatrix);
		stage = Stage::ALL_OTHERS;
	}

	if (stage == Stage::ALL_OTHERS)
	{
		if (index >= moves.size())
			return false;

		move = moves[index++];
	}

	return true;
}

void MovePicker::OrderMoves(Position& position, std::vector<Killer>& KillerMoves, unsigned int(&HistoryMatrix)[N_PLAYERS][N_SQUARES][N_SQUARES])
{
	for (size_t i = 0; i < moves.size(); i++)
	{
		//Hash move
		if (moves[i] == TTmove)
		{
			moves.erase(moves.begin() + i);
			i--;
			continue;
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
