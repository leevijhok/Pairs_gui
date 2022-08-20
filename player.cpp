//Player variables:
//Used for storing scores and wins of each player in a game.
//The variables are and fuctions are defined here.

// Creator : Leevi Hokkanen,
// Studentnumber : 050253975
// Username : tbleho
// E-mail : leevi.hokkanen@tuni.fi

#include "player.hh"

player::player(QString player_name):
    player_name_(player_name)
{

}

player::~player()
{

}

QString player::get_name()
{
    return player_name_;
}

QString player::get_letter_QString()
{
    return cards_found_QString;
}

int player::get_score()
{
    return score;
}

int player::get_wins()
{
    return wins;
}

void player::add_score(QString letter)
{
    score += 1;
    letters.insert(letter);

    bool is_first = true;
    cards_found_QString = "";

    //Makes cards_found_QString form letters in an alphabetical order
    //Output example: cards_found_QString = a,c,d,e,h
    for(QString letter : letters)
    {
        if(is_first == true)
        {
            is_first = false;
        }
        else
        {
            cards_found_QString += ", ";
        }
        cards_found_QString += letter;
    }
}

void player::add_win()
{
    wins += 1;
}

void player::reset_score()
{
    score = 0;
    cards_found_QString = "";
    letters.clear();
}
