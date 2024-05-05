#include "pch.h"
#include "Game.h"

#include <iostream>


namespace
{
  inline bool _IsBackwardSlash(const size_t& i_pos, const size_t& i_size)
  {
    return i_pos / i_size == i_pos % i_size;
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  inline bool _IsForwardSlash(const size_t& i_pos, const size_t& i_size)
  {
    return (i_pos / i_size) + (i_pos % i_size) == i_size - 1;
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  void _PrintBoard(std::vector<std::vector<char>>& ip_board)
  {
    std::cout << std::endl;

    for (const auto& row : ip_board)
    {
      for (const auto& col : row)
      {
        std::cout << ' ' << col << ' ';
        if (&col != &row.back()) // general comment: cool!
          std::cout << '|';      // I was curious so after looking into stackoverflow, found an answer with thumbs up: https://stackoverflow.com/questions/27585021/how-can-i-check-if-im-on-the-last-element-when-iterating-using-foreach-syntax
      }
      std::cout << std::endl;

      if (&row != &ip_board.back())
      {
        for (auto i = 0; i < ip_board.size(); ++i)
          std::cout << "----";
        std::cout << std::endl;
      }
    }
    std::cout << std::endl;
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace TicTacToe
{
  TicTacToe::TicTacToe(size_t i_size) : m_game_iteration(0), m_curr_player_idx(0)
  {
    for (size_t i = 0; i < i_size; ++i)
      m_board.emplace_back(i_size, ' ');
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  TicTacToe::Player::Player(size_t i_game_size, char i_mark, std::string i_name)
    : m_marker(i_mark), m_name(i_name), m_has_chance_forward_slash(true), m_has_chance_backward_slash(true)
  {
    m_has_chance_row.resize(i_game_size);
    std::fill(m_has_chance_row.begin(), m_has_chance_row.end(), true);
    m_has_chance_col.resize(i_game_size);
    std::fill(m_has_chance_col.begin(), m_has_chance_col.end(), true);
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  size_t TicTacToe::_GetPlayerInput()
  {
    size_t input_position;

    const auto& curr_player = m_players[m_curr_player_idx];
    auto is_first_time = true;
    do
    {
      if (!is_first_time)
        std::cout << "Invalid input. Please retry." << std::endl;
      is_first_time = false;

      std::cout << curr_player.GetName() << "'s turn (0-8): ";
      std::cin >> input_position;

    } while (input_position > (m_board.size() * m_board.size() - 1) ||
      m_board[input_position / m_board.size()][input_position % m_board.size()] != ' ');

    ++m_game_iteration;
    return input_position;
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  void TicTacToe::_SetNextPlayer()
  {
    if (m_curr_player_idx >= m_players.size() - 1)
      m_curr_player_idx = 0;
    else
      ++m_curr_player_idx;
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  void TicTacToe::_UpdateOpponentChances(const size_t& i_input_pos)
  {
    auto update_chances = [&](Player& opponent)
      {
        if (opponent.GetMarker() != m_players[m_curr_player_idx].GetMarker())
        {
          if (_IsBackwardSlash(i_input_pos, m_board.size()))
            opponent.RemoveChanceBackwardSlash();
          else if (_IsForwardSlash(i_input_pos, m_board.size()))
            opponent.RemoveChanceForwardSlash();
          if (opponent.GetHasChanceRow()[GetRow(i_input_pos)])
            opponent.RemoveChanceRow(GetRow(i_input_pos));
          if (opponent.GetHasChanceCol()[GetCol(i_input_pos)])
            opponent.RemoveChanceCol(GetCol(i_input_pos));
        }
      };

    std::for_each(m_players.begin(), m_players.end(), update_chances); // minor suggestion: lambda can be used directly without storing it in update_chances
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  bool TicTacToe::_CheckHasWonDiagonal() const
  {
    if (m_players[m_curr_player_idx].GetHasChanceBackwardSlash())
    {
      bool has_won_backward_slash = true;
      for (size_t i = 0; i < m_board.size(); ++i)
      {
        if (m_board[i][i] != m_players[m_curr_player_idx].GetMarker())
        {
          has_won_backward_slash = false;
          break;
        }
      }
      if (has_won_backward_slash)
        return true;
    }

    if (m_players[m_curr_player_idx].GetHasChanceForwardSlash())
    {
      bool has_won_forward_slash = true;
      for (size_t i = 0; i < m_board.size(); ++i)
      {
        if (m_board[i][m_board.size() - 1 - i] != m_players[m_curr_player_idx].GetMarker())
        {
          has_won_forward_slash = false;
          break;
        }
      }
      if (has_won_forward_slash)
        return true;
    }

    return false;
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  bool TicTacToe::_CheckHasWonColOrRow(const std::vector<bool>& i_has_chance_col, const std::vector<bool>& i_has_chance_row) const // minor comment: might be helpful to add one liner on what this function accomplishes 
    {
    auto has_no_chance = [](bool chance) { return chance == false; };
    const auto curr_player_marker = m_players[m_curr_player_idx].GetMarker();
    auto is_curr_user_marker = [curr_player_marker](char i_marker) { return i_marker == curr_player_marker; };

    if (!std::all_of(i_has_chance_col.cbegin(), i_has_chance_col.cend(), has_no_chance))
    {
      for (auto col = 0; col < i_has_chance_col.size(); ++col)
        if (i_has_chance_col[col])
        {
          bool has_won = true;
          for (auto& row : m_board) // optional: can std::all_of be used here?
          {
            if (row[col] != m_players[m_curr_player_idx].GetMarker())
            {
              has_won = false;
              break;
            }
          }
          if (has_won)
            return true;
        }
    }

    if (!std::all_of(i_has_chance_row.cbegin(), i_has_chance_row.cend(), has_no_chance))
    {
      for (auto row = 0; row < i_has_chance_row.size(); ++row)
      {
        if (i_has_chance_row[row])
          if (std::all_of(m_board[row].cbegin(), m_board[row].cend(), is_curr_user_marker)) // minor suggestion: can this line be merged with "if (i_has_chance_row[row])" by using &&, it might simplify
            return true;
      }
    }

    return false;
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  void TicTacToe::AddPlayer(const char& i_marker, const std::string& i_name)
  {
    // Question: Would m_players<std::unique_ptr<Player>> better? If so, how to implement?
    m_players.push_back(Player(m_board.size(), i_marker, i_name));
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  void TicTacToe::Proceed() // minor comment: might be helpful to add one liner on what this function accomplishes
  {
    try
    {
      _PrintBoard(m_board);
      if (m_players.size() < 2)
        throw std::invalid_argument("There must be at least 2 players to start the game!");
      const auto& input_position = _GetPlayerInput();

      // Update board
      m_board[input_position / m_board.size()][input_position % m_board.size()] =
        m_players[m_curr_player_idx].GetMarker();

      _UpdateOpponentChances(input_position);
    }
    catch (const std::invalid_argument& ex) {
      std::cerr << "Invalid argument error: " << ex.what() << std::endl;
    }
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  bool TicTacToe::Ended() // minor comment: might be helpful to add one liner on what this function accomplishes 
  {
    const auto& curr_player = m_players[m_curr_player_idx];
    const auto& sure_lost = [](const Player& i_player) { return i_player.SureLost() == true; };

    if (m_game_iteration >= (m_board.size() * 2) - 1 && curr_player.SureLost())
      if (std::all_of(m_players.cbegin(), m_players.cend(), sure_lost)) // minor suggestion: lambda can be used directly without sure_lost
      {
        std::cout << "Draw!" << std::endl;
        return true;
      }

    if (m_game_iteration >= (m_board.size() * 2) - 1 &&
      (_CheckHasWonColOrRow(curr_player.GetHasChanceCol(), curr_player.GetHasChanceRow()) ||
        _CheckHasWonDiagonal()))
    {
      system("CLS");
      std::cout << curr_player.GetName() << " Won!" << std::endl;
      _PrintBoard(m_board);
      return true;
    }

    _SetNextPlayer();
    return false;
  }
}