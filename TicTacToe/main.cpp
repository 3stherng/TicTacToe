#include "pch.h"
#include "Game.h"

#include <iostream>


void PrintInstruction(size_t i_size) // would it be better to make it part of TicTacToe class? as I see that the board is also printed by TicTacToe class
{
  std::cout << "==========================================" << std::endl;
  std::cout << "Tic-Tac-Toe Multiplayers Game"              << std::endl;
  std::cout << "==========================================" << std::endl;
  for (size_t i = 0; i < i_size * i_size; ++i)
  {
    std::cout << ' ';
    if (i < 10)
      std::cout << ' ';
    std::cout << i << ' ';
    if (i == 0 || (i + 1) % i_size != 0)
      std::cout << '|';

    if (i != 0 && (i + 1) % i_size == 0)
    {
      std::cout << std::endl;
      for (auto i = 0; i < i_size; ++i)
        std::cout << "-----";
      std::cout << std::endl;
    }
  }

  std::cout << std::endl << "##########################################" << std::endl;

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// int main()
// {
// 
//   int game_size = 4;
//   auto p_game = std::make_unique<TicTacToe::TicTacToe>(game_size); // it can be stack variable instead of pointer
// 
//   p_game->AddPlayer('X', "Player A");
//   p_game->AddPlayer('O', "Player B");
//   p_game->AddPlayer('*', "Player C");
// 
//   do
//   {
//     system("CLS");
//     PrintInstruction(game_size);
//     p_game->Proceed();
//   } while (!p_game->Ended());
// 
//   return 0;
// }
// 