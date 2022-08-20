//Player variables:
//Used for storing scores and wins of each player in a game.
//The variables are and fuctions are initialized here.

// Creator : Leevi Hokkanen,
// Studentnumber : 050253975
// Username : tbleho
// E-mail : leevi.hokkanen@tuni.fi

#ifndef PLAYER_HH
#define PLAYER_HH
#include <QString>
#include <set>


class player
{
public:
    player(QString player_name);
    ~player();

    //Returns the name of the player.
    QString get_name();

    //Returns all of the card-letters in a single QString.
    QString get_letter_QString();

    //Returns the amount of pairs found.
    int get_score();

    //Returns the amount of wins.
    int get_wins();

    //Adds a pair found into score.
    //QString letter = Letter of the pair found.
    void add_score(QString letter);

    //Adds a win.
    void add_win();

    //Makes score=0.
    void reset_score();


private:
    QString player_name_;
    QString cards_found_QString;
    std::set<QString> letters;
    int score = 0;
    int wins = 0;
};

#endif // PLAYER_HH
