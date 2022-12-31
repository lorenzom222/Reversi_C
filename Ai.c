#include <stdio.h>
#include <stdbool.h>

#define win INT_MAX
#define loss INT_MIN
#define draw 0

typedef struct {
  bool Color;
  int difficulty;
  int count;
} Ai;

typedef struct {
  int row;
  int col;
  char colChar;
  int numCaps;
} Move;

bool getColor(Ai *ai) {
  return ai->Color;
}

void setColor(Ai *ai, bool color) {
  ai->Color = color;
}

Ai createAi(bool color, int dif) {
  Ai ai;
  ai.Color = color;
  ai.difficulty = dif;
  return ai;
}

Ai createAiColor(bool color) {
  Ai ai;
  ai.Color = color;
  return ai;
}

Move choseMove(Board b, Player p, int depth, Ai *ai) {
  Move chosenMove;

  if (ai->difficulty == 1) {
    chosenMove = Random(b);
  }

  if (ai->difficulty == 2) {
    chosenMove = MINIMAX(b, ai, p);
  }
  if (ai->difficulty == 3) {
    chosenMove = HMINIMAXABP(b, ai, p, depth);
  }

  return chosenMove;
}

Move MINIMAX(Board b, Ai *ai, Player p) {
  Move chosenMove;

  HashSet<Move> legalMoves;

  if (ai->Color == false) {
    legalMoves = b.getLegalMovesBlack(b, ai);
  } else {
    legalMoves = b.getLegalMovesWhite(b, ai);
  }
  if (legalMoves.size() == 1) {
    for (Move m : legalMoves) {
      b.makeMove(b, m, ai);
      return m;
    }
  }

  HashMap<int, Move> moveToVal;
  for (Move m : legalMoves) {
    moveToVal.put(min(b, ai,
