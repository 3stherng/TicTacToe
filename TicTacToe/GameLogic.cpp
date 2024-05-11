#include "pch.h"

#include "GameLogic.h"

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
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace TicTacToe
{
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Class TicTacToe::Player
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  class TicTacToe::Player
  {
  public:
    Player(size_t i_game_size, char i_mark, std::string i_name);

    void SetCannotWinForwardSlashDiagonal();
    void SetCannotWinBackwardSlashDiagonal();
    void RemoveChanceOnRow(const size_t& i_row_index);
    void RemoveChanceOnCol(const size_t& i_col_index);

    char GetMarker() const;
    std::string GetName() const;

    bool HasChanceForwardSlashDiagonal() const;
    bool HasChanceBackwardSlashDiagonal() const;
    std::vector<bool> GetChanceRows() const;
    std::vector<bool> GetChanceColumns() const;

    bool DidLoose() const;

  private:
    char m_marker;
    std::string m_name;

    bool m_has_chance_forward_slash_diagonal;  // can add a comment for the group of variables for chance (or can create a struct for these group of variables and move respective logic there? then it would be easy to pass the struct <can arrange a call to discuss this>) to inform reader that they are used to optimize the checking of chance to win
    bool m_has_chance_backward_slash_diagonal;
    std::vector<bool> m_has_chance_row;
    std::vector<bool> m_has_chance_col;

  };

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  TicTacToe::Player::Player(size_t i_game_size, char i_mark, std::string i_name)
    : m_marker(i_mark), m_name(i_name), m_has_chance_forward_slash_diagonal(true), m_has_chance_backward_slash_diagonal(true)
  {
    m_has_chance_row.resize(i_game_size);
    std::fill(m_has_chance_row.begin(), m_has_chance_row.end(), true);
    m_has_chance_col.resize(i_game_size);
    std::fill(m_has_chance_col.begin(), m_has_chance_col.end(), true);
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  void TicTacToe::Player::SetCannotWinForwardSlashDiagonal()
  {
    m_has_chance_forward_slash_diagonal = false;
  };

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  void TicTacToe::Player::SetCannotWinBackwardSlashDiagonal()
  {
    m_has_chance_backward_slash_diagonal = false;
  };

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  void TicTacToe::Player::RemoveChanceOnRow(const size_t& i_row_index)
  {
    m_has_chance_row[i_row_index] = false;
  };

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  void TicTacToe::Player::RemoveChanceOnCol(const size_t& i_col_index)
  {
    m_has_chance_col[i_col_index] = false;
  };

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  char TicTacToe::Player::GetMarker() const
  {
    return m_marker;
  };

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  std::string TicTacToe::Player::GetName() const
  {
    return m_name;
  };

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  bool TicTacToe::Player::HasChanceForwardSlashDiagonal() const
  {
    return m_has_chance_forward_slash_diagonal;
  };

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  bool TicTacToe::Player::HasChanceBackwardSlashDiagonal() const
  {
    return m_has_chance_backward_slash_diagonal;
  };

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  std::vector<bool> TicTacToe::Player::GetChanceRows() const
  {
    return m_has_chance_row;
  };

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  std::vector<bool> TicTacToe::Player::GetChanceColumns() const
  {
    return m_has_chance_col;
  };

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  bool TicTacToe::Player::DidLoose() const
  {
    auto has_no_chance = [](bool chance) { return chance == false; };

    return (std::all_of(m_has_chance_row.cbegin(), m_has_chance_row.cend(), has_no_chance) &&
      std::all_of(m_has_chance_col.cbegin(), m_has_chance_col.cend(), has_no_chance) &&
      !m_has_chance_backward_slash_diagonal &&
      !m_has_chance_forward_slash_diagonal);
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Class TicTacToe
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  TicTacToe::TicTacToe(size_t i_size)
    : m_game_iteration(0)
    , m_curr_player_idx(0)
    , m_players({})
    , mp_winner(nullptr)
    , m_game_status(GameStatus::IN_PROGRESS)
  {
    for (auto i = 0; i < i_size; ++i)
      m_board.emplace_back(i_size, ' ');
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  void TicTacToe::AddPlayer(const size_t& i_number_of_player)
  {
    std::vector<char> marker = { 'X', 'O', '*', '#' };
    for (int i = 0; i < i_number_of_player; ++i)
      m_players.push_back(std::make_shared<Player>(m_board.size(), marker[i], "Player " + std::to_string(i + 1)));
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  void TicTacToe::ContinueGame(const size_t& i_current_player_input_position)
  {
    ++m_game_iteration;

    // Update board
    m_board[i_current_player_input_position / m_board.size()][i_current_player_input_position % m_board.size()] =
      m_players[m_curr_player_idx]->GetMarker();

    _UpdateOpponentChances(i_current_player_input_position);
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  bool TicTacToe::HasGameEnded()
  {
    const auto& curr_player = m_players[m_curr_player_idx];

    if (m_game_iteration >= (m_board.size() * 2) - 1 && curr_player->DidLoose() &&
      std::all_of(m_players.cbegin(), m_players.cend(),
        [](std::shared_ptr<Player> ip_player) { return ip_player->DidLoose() == true; }))
    {
      m_game_status = GameStatus::DRAW;
      return true;
    }

    if (m_game_iteration >= (m_board.size() * 2) - 1 &&
      (_HasWonOnColumnOrRow(curr_player->GetChanceColumns(), curr_player->GetChanceRows()) ||
        _HasWonOnDiagonals()))
    {
      m_game_status = GameStatus::WON;
      mp_winner = curr_player;
      return true;
    }

    _SetNextPlayersTurn();
    return false;
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  std::string TicTacToe::GetWinnerName()
  {
    return mp_winner->GetName();
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  GameStatus TicTacToe::GetGameStatus() const
  {
    return m_game_status;
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  void TicTacToe::_SetNextPlayersTurn()
  {
    if (m_curr_player_idx >= m_players.size() - 1)
      m_curr_player_idx = 0;
    else
      ++m_curr_player_idx;
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  char TicTacToe::GetCurrentPlayerMarker() const
  {
    return m_players[m_curr_player_idx]->GetMarker();
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  void TicTacToe::_UpdateOpponentChances(const size_t& i_input_pos)
  {
    std::for_each(m_players.begin(), m_players.end(), [&](std::shared_ptr<Player> opponent)
      {
        if (opponent->GetMarker() != m_players[m_curr_player_idx]->GetMarker())
        {
          if (_IsBackwardSlash(i_input_pos, m_board.size()))
            opponent->SetCannotWinBackwardSlashDiagonal();
          else if (_IsForwardSlash(i_input_pos, m_board.size()))
            opponent->SetCannotWinForwardSlashDiagonal();
          if (opponent->GetChanceRows()[_GetRow(i_input_pos)])
            opponent->RemoveChanceOnRow(_GetRow(i_input_pos));
          if (opponent->GetChanceColumns()[_GetColumn(i_input_pos)])
            opponent->RemoveChanceOnCol(_GetColumn(i_input_pos));
        }
      });
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  bool TicTacToe::_HasWonOnDiagonals() const
  {
    if (m_players[m_curr_player_idx]->HasChanceBackwardSlashDiagonal())
    {
      bool has_won_backward_slash = true;
      for (size_t i = 0; i < m_board.size(); ++i)
      {
        if (m_board[i][i] != m_players[m_curr_player_idx]->GetMarker())
        {
          has_won_backward_slash = false;
          break;
        }
      }
      if (has_won_backward_slash)
        return true;
    }

    if (m_players[m_curr_player_idx]->HasChanceForwardSlashDiagonal())
    {
      bool has_won_forward_slash = true;
      for (size_t i = 0; i < m_board.size(); ++i)
      {
        if (m_board[i][m_board.size() - 1 - i] != m_players[m_curr_player_idx]->GetMarker())
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

  bool TicTacToe::_HasWonOnColumnOrRow(const std::vector<bool>& i_has_chance_col, const std::vector<bool>& i_has_chance_row) const // minor comment: might be helpful to add one liner on what this function accomplishes 
  {
    auto has_no_chance = [](bool chance) { return chance == false; };
    const auto curr_player_marker = m_players[m_curr_player_idx]->GetMarker();
    auto is_curr_user_marker = [curr_player_marker](char i_marker) { return i_marker == curr_player_marker; };

    if (!std::all_of(i_has_chance_col.cbegin(), i_has_chance_col.cend(), has_no_chance))
    {
      for (auto col = 0; col < i_has_chance_col.size(); ++col)
        if (i_has_chance_col[col])
        {
          bool has_won = true;

          if (std::all_of(m_board.begin(), m_board.end(),
            [col, curr_player_marker](std::vector<char> rows)
            {
              return rows[col] == curr_player_marker;
            }))
          {
            return true;
          }
        }
    }

    if (!std::all_of(i_has_chance_row.cbegin(), i_has_chance_row.cend(), has_no_chance))
    {
      for (auto row = 0; row < i_has_chance_row.size(); ++row)
      {
        if (i_has_chance_row[row] && std::all_of(m_board[row].cbegin(), m_board[row].cend(), is_curr_user_marker))
          return true;
      }
    }

    return false;
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  size_t TicTacToe::_GetRow(const size_t& i_marker_position) const
  {
    return i_marker_position / m_board.size();
  };

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  size_t TicTacToe::_GetColumn(const size_t& i_marker_position) const
  {
    return i_marker_position % m_board.size();
  };
}
