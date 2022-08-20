//The ui-objects and their placements.
//All of the ui-objects and fuctions are initialized here.
//Except buttons (cards).

// Creator : Leevi Hokkanen,
// Studentnumber : 050253975
// Username : tbleho
// E-mail : leevi.hokkanen@tuni.fi

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <player.hh>
#include <QPushButton>
#include <vector>
#include <algorithm>
#include <random>
#include <QHBoxLayout>
#include <QTimer>
#include <QToolBar>
#include <QAction>
#include <QLabel>
#include <QRadioButton>
#include <QMessageBox>
#include <QMenu>
#include <QMenuBar>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
//namespace Ui {class MainWindow;}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    //Disables the clicked button, adds to players score
    void turnClick();

    //Updates the time-label.
    void update_time();

    //Ends the delay after button press, checks winner.
    void delay();

    //Function that calls all of need functions needed for a game.
    //int pair_amount = Amount of card-pairs in game.
    //row_amount = Amount rows in a game-layout.
    //int height = Fixed button height
    void makecardgame(int pair_amount, int row_amout, int height);

    //Displays the scores.
    void display_scores();

    //Returns the game back beginning-state.
    void reset_game();

    //Displays the name of the best-programmer-ever.
    void display_credits();

    //Shuts down the program.
    void end_program();


private:
    Ui::MainWindow *ui;


    // CONSTANT HEIGHTS

    const int TWENTYFOUR_HEIGHT = 125;
    const int TWENTY_HEIGHT = 150;
    const int SIXTEEN_HEIGHT = 190;
    const int TWELVE_HEIGHT = 190;
    const int SIX_HEIGHT = 390;
    const int RADIO_HEIGHT = 200;
    const int LABEL_HEIGHT = 75;


    // CONSTANT LABEL TEXTS


    const QString WELCOME = "Welcome!";
    const QString TURN  = "'s turn";
    const QString SCORE = "SCORE!";
    const QString DRAW = "DRAW!";
    const QString WINS = " WINS!";
    const QString HAS_CARDS = "HAS ";
    const QString START_TIME = "00:00:00";


    //LAYOUT VARIABLES


    QWidget* central;
    QGridLayout* layout;
    bool game_has_started = false;


    //BUTTON(CARD) POINTERS AND OBJECTS.


    QPushButton* first_turned = nullptr;
    QPushButton* second_turned = nullptr;
    std::vector<QPushButton*> cards_;
    std::vector<QString> letters_found;
    bool card_has_been_turned = false;


    //TIMERS

    //Timer for delay's between turns:
    QTimer* turn_delay_timer;
    int delay_seconds = 0;

    //Timer for each game.
    QTimer* game_run_time;
    int seconds = 0;
    int minutes = 0;
    int hours = 0;


    //LABELS


    QLabel* game_status;
    QLabel* cards_found_by_player;
    //Label for displaying the program's runtime:
    QLabel* time;


    //RADIO BUTTONS


    QRadioButton* two_players;
    QRadioButton* three_players;
    QRadioButton* four_players;


    //PLAYERS


    std::vector<player*> players;
    player* in_turn;
    //Index of the in-turn-player in vector players
    //Its beginning value is -1, so that it can works with change_player_turn.
    std::vector<player*>::size_type in_turn_index = -1;


    //ACTIONS


    //Makes a six-card-game.
    QAction* six_cards;

    //Makes a twelve-card-game.
    QAction* twelve_cards;

    //Makes a sixteen-card-game.
    QAction* sixteen_cards;

    //Makes a twenty-card-game.
    QAction* twenty_cards;

    //Makes a twentyfour-card-game.
    QAction* twentyfour_cards;

    //Displays player scores and wins.
    QAction* scores;

    //Returns the program to beginning state.
    QAction* reset_;

    //Displays the name of the best-programmer-ever.
    QAction* credits;

    //Ends the program.
    QAction* end_;


    //MENU OBJECTS


    QMenuBar* menu_bar;
    QMenu* actions_menu;
    QMenu* others_menu;


    //HELPER FUNCTIONS

    //Turns integers into suitable QStrings for the time-label(runtime)
    //int unit = number to be turned into QString
    //returns QString unit modified for the clock.
    QString int_to_clock_QString(int unit);

    //Makes the widgets in the start-screen.
    void make_start_widgets();

    //Defines all of the actions used in menubar.
    void make_actions();

    //Makes the menubar and and all of its submenus.
    void make_menus();

    //Defines all of the player-pointers.
    void make_players();

    //Defines all of buttons (cards).
    //int amount_of_pairs = Amount of card-pairs.
    //int height = Fixed button height.
    void make_buttons(int amount_of_pairs, int height);

    //Places buttons defined in make_buttons on onto a
    //rectangle shaped layout in a random order.
    //row_size = Amount rows in a game-layout.
    void make_random_grid(unsigned int row_amount);

    //DELETE THIS
    //void delete_button_pointers();

    //Changes the current player-in-turn
    void change_player_turn();

    //Changes the visibility of the radio buttons on
    //the start-screen.
    void change_radio_button_visibility();

    //Enables or disables all of unturned buttons (cards).
    //bool yes_or_no = true enables buttons, false disables them.
    void change_button_enability(bool yes_or_no);

    //Removes cards from layout and permanently deletes their pointers.
    void remove_cards();

    //Checks, who won the cardgame.
    void check_winner();

    //Resets time.
    void reset_time();

    //Deletes the player-pointers.
    void delete_players();
};
#endif // MAINWINDOW_HH
