#pragma once

#include <algorithm>
#include <vector>
#include <string>


namespace TicTacToe
{
  class TicTacToe
  {
  private:
    class Player;

    size_t m_game_iteration;
    size_t m_curr_player_idx;
    std::vector<Player> m_players;
    std::vector<std::vector<char>> m_board;

    size_t _GetPlayerInput(); // minor suggestion: _GetCurrentPlayersInput
    void _SetNextPlayer();  // minor suggestion: _SetNextPlayersTurn
    void _UpdateOpponentChances(const size_t& i_input_pos); // minor suggestion: i_marker_position?

    bool _CheckHasWonDiagonal() const; // minor suggestion:_HasWonOnDiagonals 
    bool _CheckHasWonColOrRow(const std::vector<bool>& i_has_chance_col, const std::vector<bool>& i_has_chance_row) const; // personal opinion/question: would _HasWonOnColumnOrRow be suitable?

    size_t GetRow(const size_t& i_input_position) const { return i_input_position / m_board.size(); };  // minor suggestion: i_marker_position
    size_t GetCol(const size_t& i_input_position) const { return i_input_position % m_board.size(); };  // minor suggestion: i_marker_position


  public:
    TicTacToe(size_t i_size);

    void AddPlayer(const char& i_marker, const std::string& i_name);
    void Proceed(); // optional: would Continue sound better as function name?
    bool Ended(); // minor suggestion: IsEnded or HasEnded might sound better?
  };

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  class TicTacToe::Player // just a suggestion: I think it might be okay to move class definition to cpp as it is not needed by other file that import this header file
  {
  private:
    char m_marker;
    std::string m_name;

    bool m_has_chance_forward_slash;       // can add a comment for the group of variables for chance (or can create a struct for these group of variables and move respective logic there? then it would be easy to pass the struct <can arrange a call to discuss this>) to inform reader that they are used to optimize the checking of chance to win
    bool m_has_chance_backward_slash;      // 
    std::vector<bool> m_has_chance_row;    // minor suggestion: m_has_chance_forward_slash and m_has_chance_backward_slash can be renamed to m_has_chance_forward_slash_diagonal and m_has_chance_backward_slash_diagonal respectively
    std::vector<bool> m_has_chance_col;    // 

  public:
    Player(size_t i_game_size, char i_mark, std::string i_name);

    void RemoveChanceForwardSlash() { m_has_chance_forward_slash = false; }; // optional: SetCannotWinForwardSlashDiagonal
    void RemoveChanceBackwardSlash() { m_has_chance_backward_slash = false; };
    void RemoveChanceRow(const size_t& i_row) { m_has_chance_row[i_row] = false; }; // minor suggestion: RemoveChanceOnRow   and i_row_index
    void RemoveChanceCol(const size_t& i_col) { m_has_chance_col[i_col] = false; }; // minor suggestion: RemoveChanceOnColumn  and i_col_index

    char GetMarker() const { return m_marker; };
    std::string GetName() const { return m_name; };
    bool GetHasChanceForwardSlash() const { return m_has_chance_forward_slash; }; // minor suggestion: GetHasChanceForwardSlashDiagonal or simply HasChanceForwardSlashDiagonal? similar for GetHasChanceBackwardSlash
    bool GetHasChanceBackwardSlash() const { return m_has_chance_backward_slash; };
    std::vector<bool> GetHasChanceRow() const { return m_has_chance_row; }; // minor suggestion: GetChanceRows
    std::vector<bool> GetHasChanceCol() const { return m_has_chance_col; }; // minor suggestion: GetChanceColumns

    bool SureLost() const // would be better to rename the function, some suggestions: DidLoose, CanContinuePlaying (for this return value has to be inverted)
    {
      auto has_no_chance = [](bool chance) { return chance == false; };

      return (std::all_of(m_has_chance_row.cbegin(), m_has_chance_row.cend(), has_no_chance) &&
        std::all_of(m_has_chance_col.cbegin(), m_has_chance_col.cend(), has_no_chance) &&
        !m_has_chance_backward_slash &&
        !m_has_chance_forward_slash);
    }
  };
}
