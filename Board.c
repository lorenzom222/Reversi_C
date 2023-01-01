#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "hashset.h"
#include "hashset_itr.h"

#include "Move.h"
#include "Player.h"

const int loss = -1;
const int win = 1;
const int draw = 0;

typedef struct Board
{
	int **board;
	int size;
	int whitePieceCount;
	int blackPieceCount;
	hashset_t *reachedMoves;
} Board;

Board *createBoard(int size)
{
	Board *board = malloc(sizeof(Board));
	board->board = malloc(size * sizeof(int *));
	for (int i = 0; i < size; i++)
	{
		board->board[i] = malloc(size * sizeof(int));
	}
	board->size = size;
	board->whitePieceCount = 2;
	board->blackPieceCount = 2;
	board->reachedMoves = hashset_create();

	board->board[size / 2 - 1][size / 2 - 1] = 1;
	board->board[size / 2][size / 2 - 1] = 2;
	board->board[size / 2 - 1][size / 2] = 2;
	board->board[size / 2][size / 2] = 1;

	return board;
}

void getScore(Board b)
{
	hashset_t *score = hashset_create();
}

hashset_t *reachedSpaceMoves(Board *b, Player *p, Move *m)
{
	hashset_t *tiles = hashset_create();
	for (int i = 0; i < b->size; i++)
	{
		for (int j = 0; j < b->size; j++)
		{
			// Check if the current space is a valid move
			// If it is, add it to the set of reached moves
			if (isValidMove(b, p, i, j))
			{
				Move *newMove = createMoveInt(i, j);
				hashset_add(tiles, newMove);
			}
		}
	}
	return tiles;
}

hashset_t *getLegalMovesBlack(Board *b, Player *p)
{
	hashset_t *legalMoves = hashset_create(); // Create an empty HashSet

	// Iterate through every position on the board
	for (int i = 0; i < b->size; i++)
	{
		for (int j = 0; j < b->size; j++)
		{
			// Create a Move object for the current position
			Move *tempMove = createMoveInt(i, j);
			// Get the set of adjacent enemy pieces for Black
			hashset_t *emenySet = adjacentToEnemyBlack(b, tempMove);

			// If the set is empty, continue to the next iteration
			if (hashset_iterator_has_next(hashset_iterator(emenySet)) == 0)
			{
				continue;
			}
			// If the position on the board is not empty, continue to the next iteration
			if (!(b->board[tempMove->row][tempMove->col] == 0))
			{
				continue;
			}

			// Get the set of legal directions for Black
			hashset_t *legalDirections = getLegalDirectionsBlack(b, *tempMove, p);
			// If the set of legal directions is not empty and the set of adjacent enemies is not empty, add the Move object to the legalMoves set
			if (hashset_iterator_has_next(hashset_iterator(getLegalDirectionsBlack(b, *tempMove, p))) == 1 && hashset_iterator_has_next(hashset_iterator(emenySet)) == 1)
			{
				hashset_add(legalMoves, tempMove);
			}
		}
	}

	return legalMoves;
}

hashset_t *getLegalMovesWhite(Board *b, Player *p)
{
	hashset_t *legalMoves = hashset_create(); // create empty set
	for (int i = 0; i < b->size; i++)
	{ // Double for loop across the board
		for (int j = 0; j < b->size; j++)
		{
			Move *tempMove = createMoveInt(i, j);					 // goes to coordinate (i, j)
			hashset_t *enemySet = adjacentToEnemyWhite(b, tempMove); // checks adjacent enemies to White

			if (hashset_iterator_has_next(hashset_iterator(enemySet)) == 0)
			{ // if it is empty, skip
				continue;
			}

			if (!(b->board[tempMove->row][tempMove->col] == 0))
			{
				continue;
			}

			// legal direction not empty and enemy set not empty
			if (hashset_iterator_has_next(hashset_iterator(getLegalDirectionsWhite(b, *tempMove, p))) == 1 && hashset_iterator_has_next(hashset_iterator(enemySet)) == 1)
			{
				hashset_add(legalMoves, tempMove);
			}
		}
	}

	return legalMoves;
}

hashset_t *getLegalDirectionsBlack(Board *b, Move m, Player *p)
{
	hashset_t *legalDirections = hashset_create(); // create empty set
												   // printSet(legalDirections);
												   // 1 2 3
												   // 8 m 4 m is target move
												   // 7 6 5

	hashset_t *directionList = adjacentToEnemyBlack(b, m);
	int cycle = 1;
	if (onBoard(m, b) == false)
	{
		return legalDirections;
	}
	Move initialMove = m;

	if (hashset_is_member(directionList, 1) != 0)
	{
		m.row = m.row - cycle;
		m.col = m.col - cycle;
		while (onBoard(m, b))
		{

			if (b->board[m.row][m.col] == 0)
			{
				m.row = m.row + cycle;
				m.col = m.col + cycle;

				cycle = 1;
				break;
			}
			if (b->board[m.row][m.col] == 2)
			{
				hashset_add(legalDirections, 1);
			}

			m.col = m.col - 1;
			m.row = m.row - 1;
			cycle++;
		}
		if (!onBoard(m, b))
		{
			m.row = m.row + cycle;
			m.col = m.col + cycle;

			cycle = 1;
		}
	}
	m = initialMove;
	if (hashset_is_member(directionList, 2) != 0)
	{
		m.row = m.row - cycle;
		while (onBoard(m, b))
		{

			if (b->board[m.row][m.col] == 0)
			{
				m.row = m.row + cycle;
				cycle = 1;
				break;
			}
			if (b->board[m.row][m.col] == 2)
			{
				hashset_add(legalDirections, 2);
			}

			m.row = m.row - 1;
			cycle++;
		}
		if (!onBoard(m, b))
		{
			m.row = m.row + cycle;

			cycle = 1;
		}
	}
	m = initialMove;
	if (hashset_is_member(directionList, 3) != 0)
	{
		m.row = m.row - cycle;
		m.col = m.col + cycle;
		while (onBoard(m, b))
		{

			if (b->board[m.row][m.col] == 0)
			{
				m.row = m.row + cycle;
				m.col = m.col - cycle;
				cycle = 1;
				break;
			}
			if (b->board[m.row][m.col] == 2)
			{
				hashset_add(legalDirections, 3);
			}

			m.col = m.col + 1;
			m.row = m.row - 1;
			cycle++;
		}
		if (!onBoard(m, b))
		{
			m.row = m.row + cycle;
			m.col = m.col - cycle;

			cycle = 1;
		}
	}
	m = initialMove;
	if (hashset_is_member(directionList, 4) != 0)
	{

		m.col = m.col + cycle;
		while (onBoard(m, b))
		{

			if (b->board[m.row][m.col] == 0)
			{
				m.col = m.col - cycle;
				cycle = 1;
				break;
			}
			if (b->board[m.row][m.col] == 2)
			{
				hashset_add(legalDirections, 4);
			}

			m.col = m.col + 1;
			cycle++;
		}
		if (!onBoard(m, b))
		{
			m.col = m.col - cycle;

			cycle = 1;
		}
	}
	m = initialMove;
	if (hashset_is_member(directionList, 5) != 0)
	{

		m.row = m.row + cycle;
		m.col = m.col + cycle;

		while (onBoard(m, b))
		{

			if (b->board[m.row][m.col] == 0)
			{
				m.row = m.row - cycle;
				m.col = m.col - cycle;
				cycle = 1;
				break;
			}
			if (b->board[m.row][m.col] == 2)
			{
				hashset_add(legalDirections, 5);
			}

			m.col = m.col + 1;
			m.row = m.row + 1;
			cycle++;
		}
		if (!onBoard(m, b))
		{
			m.row = m.row - cycle;
			m.col = m.col - cycle;

			cycle = 1;
		}
	}
	m = initialMove;
	if (hashset_is_member(directionList, 6) != 0)
	{
		m.row = m.row + cycle;

		while (onBoard(m, b))
		{

			if (b->board[m.row][m.col] == 0)
			{
				m.row = m.row - cycle;
				cycle = 1;
				break;
			}
			if (b->board[m.row][m.col] == 2)
			{
				hashset_add(legalDirections, 6);
			}

			;
			m.row = m.row + 1;
			cycle++;
		}
		if (!onBoard(m, b))
		{
			m.row = m.row - cycle;

			cycle = 1;
		}
	}
	m = initialMove;
	if (hashset_is_member(directionList, 7) != 0)
	{
		m.row = m.row + cycle;
		m.col = m.col - cycle;
		while (onBoard(m, b))
		{

			if (b->board[m.row][m.col] == 0)
			{
				m.row = m.row - cycle;
				m.col = m.col + cycle;
				cycle = 1;
				break;
			}
			if (b->board[m.row][m.col] == 2)
			{
				hashset_add(legalDirections, 7);
			}

			m.col = m.col - 1;
			m.row = m.row + 1;
			cycle++;
		}
		if (!onBoard(m, b))
		{
			m.row = m.row - cycle;
			m.col = m.col + cycle;

			cycle = 1;
		}
	}
	m = initialMove;
	if (hashset_is_member(directionList, 8) != 0)
	{

		m.col = m.col - cycle;
		while (onBoard(m, b))
		{

			if (b->board[m.row][m.col] == 0)
			{
				m.col = m.col + cycle;
				cycle = 1;

				break;
			}
			if (b->board[m.row][m.col] == 2)
			{
				hashset_add(legalDirections, 8);
			}

			m.col = m.col - 1;
			cycle++;
		}
		if (!onBoard(m, b))
		{
			m.col = m.col + cycle;

			cycle = 1;
		}
	}
	m = initialMove;
	return legalDirections;
}

hashset_t *getLegalDirectionsWhite(Board *b, Move m, Player *p)
{
	hashset_t *legalDirections = hashset_create(); // create empty set
												   // printSet(legalDirections);
												   // 1 2 3
												   // 8 m 4 m is target move
												   // 7 6 5

	hashset_t *directionList = adjacentToEnemyWhite(b, m);
	int cycle = 1;
	if (onBoard(m, b) == false)
	{
		return legalDirections;
	}
	Move initialMove = m;

	if (hashset_is_member(directionList, 1) != 0)
	{
		m.row = m.row - cycle;
		m.col = m.col - cycle;
		while (onBoard(m, b))
		{

			if (b->board[m.row][m.col] == 0)
			{
				m.row = m.row + cycle;
				m.col = m.col + cycle;

				cycle = 1;
				break;
			}
			if (b->board[m.row][m.col] == 1)
			{
				hashset_add(legalDirections, 1);
			}

			m.col = m.col - 1;
			m.row = m.row - 1;
			cycle++;
		}
		if (!onBoard(m, b))
		{
			m.row = m.row + cycle;
			m.col = m.col + cycle;

			cycle = 1;
		}
	}
	m = initialMove;
	if (hashset_is_member(directionList, 2) != 0)
	{
		m.row = m.row - cycle;
		while (onBoard(m, b))
		{

			if (b->board[m.row][m.col] == 0)
			{
				m.row = m.row + cycle;
				cycle = 1;
				break;
			}
			if (b->board[m.row][m.col] == 1)
			{
				hashset_add(legalDirections, 2);
			}

			m.row = m.row - 1;
			cycle++;
		}
		if (!onBoard(m, b))
		{
			m.row = m.row + cycle;

			cycle = 1;
		}
	}
	m = initialMove;
	if (hashset_is_member(directionList, 3) != 0)
	{
		m.row = m.row - cycle;
		m.col = m.col + cycle;
		while (onBoard(m, b))
		{

			if (b->board[m.row][m.col] == 0)
			{
				m.row = m.row + cycle;
				m.col = m.col - cycle;
				cycle = 1;
				break;
			}
			if (b->board[m.row][m.col] == 1)
			{
				hashset_add(legalDirections, 3);
			}

			m.col = m.col + 1;
			m.row = m.row - 1;
			cycle++;
		}
		if (!onBoard(m, b))
		{
			m.row = m.row + cycle;
			m.col = m.col - cycle;

			cycle = 1;
		}
	}
	m = initialMove;
	if (hashset_is_member(directionList, 4) != 0)
	{

		m.col = m.col + cycle;
		while (onBoard(m, b))
		{

			if (b->board[m.row][m.col] == 0)
			{
				m.col = m.col - cycle;
				cycle = 1;
				break;
			}
			if (b->board[m.row][m.col] == 1)
			{
				hashset_add(legalDirections, 4);
			}

			m.col = m.col + 1;
			cycle++;
		}
		if (!onBoard(m, b))
		{
			m.col = m.col - cycle;

			cycle = 1;
		}
	}
	m = initialMove;
	if (hashset_is_member(directionList, 5) != 0)
	{

		m.row = m.row + cycle;
		m.col = m.col + cycle;

		while (onBoard(m, b))
		{

			if (b->board[m.row][m.col] == 0)
			{
				m.row = m.row - cycle;
				m.col = m.col - cycle;
				cycle = 1;
				break;
			}
			if (b->board[m.row][m.col] == 1)
			{
				hashset_add(legalDirections, 5);
			}

			m.col = m.col + 1;
			m.row = m.row + 1;
			cycle++;
		}
		if (!onBoard(m, b))
		{
			m.row = m.row - cycle;
			m.col = m.col - cycle;

			cycle = 1;
		}
	}
	m = initialMove;
	if (hashset_is_member(directionList, 6) != 0)
	{
		m.row = m.row + cycle;

		while (onBoard(m, b))
		{

			if (b->board[m.row][m.col] == 0)
			{
				m.row = m.row - cycle;
				cycle = 1;
				break;
			}
			if (b->board[m.row][m.col] == 1)
			{
				hashset_add(legalDirections, 6);
			}

			;
			m.row = m.row + 1;
			cycle++;
		}
		if (!onBoard(m, b))
		{
			m.row = m.row - cycle;

			cycle = 1;
		}
	}
	m = initialMove;
	if (hashset_is_member(directionList, 7) != 0)
	{
		m.row = m.row + cycle;
		m.col = m.col - cycle;
		while (onBoard(m, b))
		{

			if (b->board[m.row][m.col] == 0)
			{
				m.row = m.row - cycle;
				m.col = m.col + cycle;
				cycle = 1;
				break;
			}
			if (b->board[m.row][m.col] == 1)
			{
				hashset_add(legalDirections, 7);
			}

			m.col = m.col - 1;
			m.row = m.row + 1;
			cycle++;
		}
		if (!onBoard(m, b))
		{
			m.row = m.row - cycle;
			m.col = m.col + cycle;

			cycle = 1;
		}
	}
	m = initialMove;
	if (hashset_is_member(directionList, 8) != 0)
	{

		m.col = m.col - cycle;
		while (onBoard(m, b))
		{

			if (b->board[m.row][m.col] == 0)
			{
				m.col = m.col + cycle;
				cycle = 1;

				break;
			}
			if (b->board[m.row][m.col] == 1)
			{
				hashset_add(legalDirections, 8);
			}

			m.col = m.col - 1;
			cycle++;
		}
		if (!onBoard(m, b))
		{
			m.col = m.col + cycle;

			cycle = 1;
		}
	}
	m = initialMove;
	return legalDirections;
}

void handleAllCapturesBlack(Board *b, Move m, Player *p)
{
	int cycle = 1;
	hashset_t *legalDircetions = getLegalDirectionsBlack(b, m, p);
	Move initialMove = m;

	if (hashset_contains(legalDircetions, 1))
	{
		m.row = m.row - cycle;
		m.col = m.col - cycle;

		while (onBoard(m, b))
		{
			if (b->board[m.row][m.col] == 1)
			{
				b->board[m.row][m.col] = 2;
				m.col = m.col - 1;
				m.row = m.row - 1;
				cycle++;
			}

			if (b->board[m.row][m.col] == 2)
			{
				m.col = m.col + cycle;
				m.row = m.row + cycle;
				b->whitePieceCount = b->whitePieceCount - cycle;
				b->blackPieceCount = b->blackPieceCount + cycle;
				cycle = 1;
				break;
			}
		}
	}
	m = initialMove;
	if (hashset_contains(legalDircetions, 2))
	{
		m.row = m.row - cycle;

		while (onBoard(m, b))
		{
			if (b->board[m.row][m.col] == 1)
			{
				b->board[m.row][m.col] = 2;
				m.row = m.row - 1;
				cycle++;
			}

			if (b->board[m.row][m.col] == 2)
			{
				m.row = m.row + cycle;
				b->whitePieceCount = b->whitePieceCount - cycle;
				b->blackPieceCount = b->blackPieceCount + cycle;
				cycle = 1;
				break;
			}
		}
	}

	if (hashset_contains(legalDircetions, 3))
	{
		m.row = m.row - cycle;
		m.col = m.col + cycle;
		if (b->board[m.row][m.col] == 0)
		{
			return;
		}
		while (onBoard(m, b))
		{

			if (b->board[m.row][m.col] == 1)
			{
				b->board[m.row][m.col] = 2;
				m.col = m.col + 1;
				m.row = m.row - 1;
				cycle++;
			}

			if (b->board[m.row][m.col] == 2)
			{
				m.row = m.row + cycle;
				m.col = m.col - cycle;
				cycle = 1;
				b->whitePieceCount = b->whitePieceCount - cycle;
				b->blackPieceCount = b->blackPieceCount + cycle;
				break;
			}
		}
	}
	m = initialMove;
	if (hashset_contains(legalDircetions, 4))
	{

		m.col = m.col + cycle;

		while (onBoard(m, b))
		{

			if (b->board[m.row][m.col] == 1)
			{
				b->board[m.row][m.col] = 2;
				m.col = m.col + 1;
				cycle++;
			}

			if (b->board[m.row][m.col] == 2)
			{
				m.col = m.col - cycle;
				b->whitePieceCount = b->whitePieceCount - cycle;
				b->blackPieceCount = b->blackPieceCount + cycle;
				cycle = 1;

				break;
			}
		}
	}
	m = initialMove;
	if (hashset_contains(legalDircetions, 5))
	{

		m.col = m.col - cycle;

		while (onBoard(m, b))
		{

			if (b->board[m.row][m.col] == 1)
			{
				b->board[m.row][m.col] = 2;
				m.col = m.col - 1;
				cycle++;
			}

			if (b->board[m.row][m.col] == 2)
			{
				m.col = m.col + cycle;
				b->whitePieceCount = b->whitePieceCount - cycle;
				b->blackPieceCount = b->blackPieceCount + cycle;
				cycle = 1;

				break;
			}
		}
	}
	m = initialMove;
	if (hashset_contains(legalDircetions, 6))
	{
		m.row = m.row + cycle;
		m.col = m.col - cycle;
		while (onBoard(m, b))
		{
			if (b->board[m.row][m.col] == 1)
			{
				b->board[m.row][m.col] = 2;
				m.col = m.col - 1;
				m.row = m.row + 1;
				cycle++;
			}

			if (b->board[m.row][m.col] == 2)
			{
				m.row = m.row - cycle;
				m.col = m.col + cycle;
				cycle = 1;
				b->whitePieceCount = b->whitePieceCount - cycle;
				b->blackPieceCount = b->blackPieceCount + cycle;
				break;
			}
		}
	}
	m = initialMove;
	if (hashset_contains(legalDircetions, 7))
	{
		m.row = m.row + cycle;
		while (onBoard(m, b))
		{
			if (b->board[m.row][m.col] == 1)
			{
				b->board[m.row][m.col] = 2;
				m.row = m.row + 1;
				cycle++;
			}

			if (b->board[m.row][m.col] == 2)
			{
				m.row = m.row - cycle;
				b->whitePieceCount = b->whitePieceCount - cycle;
				b->blackPieceCount = b->blackPieceCount + cycle;
				cycle = 1;
				break;
			}
		}
	}
	m = initialMove;
	if (hashset_contains(legalDircetions, 8))
	{
		m.row = m.row + cycle;
		m.col = m.col + cycle;
		while (onBoard(m, b))
		{
			if (b->board[m.row][m.col] == 1)
			{
				b->board[m.row][m.col] = 2;
				m.col = m.col + 1;
				m.row = m.row + 1;
				cycle++;
			}

			if (b->board[m.row][m.col] == 2)
			{
				m.row = m.row - cycle;
				m.col = m.col - cycle;
				cycle = 1;
				b->whitePieceCount = b->whitePieceCount - cycle;
				b->blackPieceCount = b->blackPieceCount + cycle;
				break;
			}
		}
	}
}

hashset_t *adjacentToEnemyBlack(Board b, Move m)
{ // retruns a set which indicates which direction
	// the enemy peices are
	hashset_t *adjacentEnemies = hashset_create();
	Move m1 = {m.row - 1, m.col - 1};
	Move m2 = {m.row - 1, m.col};
	Move m3 = {m.row - 1, m.col + 1};
	Move m4 = {m.row, m.col + 1};
	Move m5 = {m.row + 1, m.col + 1};
	Move m6 = {m.row + 1, m.col};
	Move m7 = {m.row + 1, m.col - 1};
	Move m8 = {m.row, m.col - 1};

	if (onBoard(m1, b))
	{
		if (b.board[m1.row][m1.col] == 1)
		{
			hashset_add(adjacentEnemies, 1);
		}
	}
	if (onBoard(m2, b))
	{
		if (b.board[m2.row][m2.col] == 1)
		{
			hashset_add(adjacentEnemies, 2);
		}
	}
	if (onBoard(m3, b))
	{
		if (b.board[m3.row][m3.col] == 1)
		{
			hashset_add(adjacentEnemies, 3);
		}
	}
	if (onBoard(m4, b))
	{
		if (b.board[m4.row][m4.col] == 1)
		{
			hashset_add(adjacentEnemies, 4);
		}
	}
	if (onBoard(m5, b))
	{
		if (b.board[m5.row][m5.col] == 1)
		{
			hashset_add(adjacentEnemies, 5);
		}
	}
	if (onBoard(m6, b))
	{
		if (b.board[m6.row][m6.col] == 1)
		{
			hashset_add(adjacentEnemies, 6);
		}
	}
	if (onBoard(m7, b))
	{
		if (b.board[m7.row][m7.col] == 1)
		{
			hashset_add(adjacentEnemies, 7);
		}
	}
	if (onBoard(m8, b))
	{
		if (b.board[m8.row][m8.col] == 1)
		{
			hashset_add(adjacentEnemies, 8);
		}
	}

	// 1 2 3
	// 8 m 4 m is target move
	return adjacentEnemies;
}

bool onBoard(Move m, Board b)
{
	bool onBoard = true;
	if (m.row < 0)
	{
		onBoard = false;
	}
	if (m.row > b.size - 1)
	{
		onBoard = false;
	}
	if (m.col < 0)
	{
		onBoard = false;
	}
	if (m.col > b.size - 1)
	{
		onBoard = false;
	}

	return onBoard;
}

void makeMove(Board b, Move *m, Player *p)
{
	if (getColor(p) == true)
	{
		b.board[getRow(m)][getCol(m)] = 1;
		handleAllCapturesWhite(b, m, p);
	}
	else
	{
		b.board[getRow(m)][getCol(m)] = 2;
		handleAllCapturesBlack(b, m, p);
	}
}

bool isGameOver(Board b)
{
	bool gameOver = false;
	int zeroCount = 0;
	int blackCount = 0;
	int whiteCount = 0;

	for (int i = 0; i < b.size; i++)
	{
		for (int j = 0; j < b.size; j++)
		{
			if (b.board[i][j] == 0)
			{
				zeroCount++;
			}
			if (b.board[i][j] == 1)
			{
				whiteCount++;
			}
			if (b.board[i][j] == 2)
			{
				blackCount++;
			}
		}
	}

	if (zeroCount == 0)
	{
		gameOver = true;
	}
	if (whiteCount == 0)
	{
		gameOver = true;
	}

	if (blackCount == 0)
	{
		gameOver = true;
	}

	return gameOver;
}

bool changeTurn(bool turn)
{
	if (turn == true)
	{
		turn = false;
	}
	else if (turn == false)
	{
		turn = true;
	}
	return turn;
}

void userInterface(int size, int board[size][size])
{
	// Creates the Visual Board
	for (int i = 0; i < size; i++)
	{
		if (i == 0)
		{
			printf(" a ");
		}
		if (i == 1)
		{
			printf(" b ");
		}
		if (i == 2)
		{
			printf(" c ");
		}
		if (i == 3)
		{
			printf(" d ");
		}
		if (i == 4)
		{
			printf(" e ");
		}
		if (i == 5)
		{
			printf(" f ");
		}
		if (i == 6)
		{
			printf(" g ");
		}
		if (i == 7)
		{
			printf(" h ");
		}
	}
	printf("\n");

	for (int i = 0; i < size; i++)
	{
		printf("%d", i + 1);

		for (int j = 0; j < size; j++)
		{

			if (board[i][j] == 0)
			{
				printf("   ");
			}
			if (board[i][j] == 1)
			{
				printf(" O ");
			}
			if (board[i][j] == 2)
			{
				printf(" X ");
			}
		}
		printf("%d\n", i + 1);
	}
	for (int i = 0; i < size; i++)
	{
		if (i == 0)
		{
			printf(" a ");
		}
		if (i == 1)
		{
			printf(" b ");
		}
		if (i == 2)
		{
			printf(" c ");
		}
		if (i == 3)
		{
			printf(" d ");
		}
		if (i == 4)
		{
			printf(" e ");
		}
		if (i == 5)
		{
			printf(" f ");
		}
		if (i == 6)
		{
			printf(" g ");
		}
		if (i == 7)
		{
			printf(" h ");
		}
	}
	printf("\n");
	printf("Black Piece Count: ");
	printf("%d", getBlackScore(size, board));
	printf("White Piece Count: ");
	printf("%d", getWhiteScore(size, board));
	if (isGameOver(size, board))
	{
		int white = getWhiteScore(size, board);
		int black = getBlackScore(size, board);

		if (white > black)
		{
			printf("White Wins!\n");
		}
		else if (white == black)
		{
			printf("Draw\n");
		}
		else
		{
			printf("Black Wins!\n");
		}
	}
}

int getBlackScore(Board b)
{
	int score = 0;
	for (int i = 0; i < b.board.length; i++)
	{
		for (int j = 0; j < b.board.length; j++)
		{
			if (b.board[i][j] == 2)
			{
				score++;
			}
		}
	}
	return score;
}

int getWhiteScore(Board b)
{
	int score = 0;
	for (int i = 0; i < b.board.length; i++)
	{
		for (int j = 0; j < b.board.length; j++)
		{
			if (b.board[i][j] == 1)
			{
				score++;
			}
		}
	}

	return score;
}

int winChecker(Board b)
{
	int white = getWhiteScore(b);
	int black = getBlackScore(b);

	if (white > black)
	{
		return win;
	}
	else if (white == black)
	{
		return 0;
	}
	else
	{
		return loss;
	}
}

int getNumCaps(Board b, Move m, Player p)
{
	int numCaps = 0;
	int scoreBefore = 0;
	int scoreAfter = 0;
	Board copy;
	copy.size = b.size;
	for (int i = 0; i < b.size; i++)
	{
		for (int j = 0; j < b.size; j++)
		{
			copy.board[i][j] = b.board[i][j];
		}
	}

	if (p.Color)
	{
		scoreBefore = getWhiteScore(b);
		copy.makeMove(copy, m, p);
		scoreAfter = getWhiteScore(b);
		numCaps = scoreAfter - scoreBefore;
	}
	else
	{
		scoreBefore = getBlackScore(b);
		copy.makeMove(copy, m, p);
		scoreAfter = getBlackScore(b);
		numCaps = scoreAfter - scoreBefore;
	}

	return numCaps;
}