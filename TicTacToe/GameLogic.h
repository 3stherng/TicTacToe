#pragma once

namespace TicTacToe
{
  enum class GameStatus
  {
    IN_PROGRESS,
    DRAW,
    WON
  };

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // class TicTacToe
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  class TicTacToe
  {
  private:
    class Player;

  public:
    explicit TicTacToe(size_t i_size);

    void AddPlayer(const size_t& i_number_of_player);
    void ContinueGame(const size_t& i_current_player_input_position);
    bool HasGameEnded();
    std::string GetWinnerName();
    GameStatus GetGameStatus() const;
    char GetCurrentPlayerMarker() const;

  private:
    void _SetNextPlayersTurn();
    void _UpdateOpponentChances(const size_t& i_marker_position);

    bool _HasWonOnDiagonals() const;
    bool _HasWonOnColumnOrRow(const std::vector<bool>& i_has_chance_col, const std::vector<bool>& i_has_chance_row) const;

    size_t _GetRow(const size_t& i_marker_position) const;
    size_t _GetColumn(const size_t& i_marker_position) const;

  private:
    size_t m_game_iteration;
    size_t m_curr_player_idx;
    std::vector<std::shared_ptr<Player>> m_players;
    std::vector<std::vector<char>> m_board;
    std::shared_ptr<Player> mp_winner;
    GameStatus m_game_status;
  };
}
