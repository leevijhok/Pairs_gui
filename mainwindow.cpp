//The ui-objects and their placements.
//All of the ui-objects and fuctions are defined here.
//Also the buttons (cards).

#include "mainwindow.hh"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Grid layout for letter cards:
    central = new QWidget(this);
    layout = new QGridLayout(central);

    //All of the functions needed to make the start-menu:
    make_start_widgets();
    make_actions();
    make_menus();

    time->setText(START_TIME);

    //The timer for the delay after every card-turn.
    turn_delay_timer = new QTimer(this);
    connect(turn_delay_timer, SIGNAL(timeout()), this, SLOT(delay()));

    //The timer for a game-run-time.
    //Resets after each game.
    game_run_time = new QTimer(this);
    connect(game_run_time, SIGNAL(timeout()), this, SLOT(update_time()));


}

MainWindow::~MainWindow()
{
    reset_time();
    delete_players();
    delete ui;
}

void MainWindow::turnClick()
{
    //Finds out, which button was pressed:
    QObject* sender_obj = sender();
    QPushButton* button = qobject_cast<QPushButton*>(sender_obj);

    //Sets the button font visible and turns it white:
    QFont font = button->font();
    font.setPointSize(12);
    button->setFont(font);
    button->setEnabled(false);
    button->setStyleSheet("* { background-color: rgb(255,255,255) }");

    // One card has already previously been turned during turn:
    if(card_has_been_turned == true)
    {
        card_has_been_turned = false;
        QString current_card_mark = button->text();
        QString previous_card_mark =first_turned->text();

        // The cards match:
        if(previous_card_mark == current_card_mark)
        {
            // Adds the card to the players score and updates game-status-label:
            game_status->setText(SCORE);
            letters_found.push_back(current_card_mark);
            in_turn->add_score(current_card_mark);
            cards_found_by_player->
                    setText(HAS_CARDS+in_turn->get_letter_QString());

            // Disables the button for a second:
            second_turned = button;
            change_button_enability(false);
            turn_delay_timer->start(1000);
        }
        //The cards don't match
        else
        {
            //Disables the button for a second:
            second_turned = button;
            change_button_enability(false);
            turn_delay_timer->start(1000);
        }
    }
    // No cards have been turned during turn:
    else
    {
        card_has_been_turned = true;
        first_turned = button;
    }

}

void MainWindow::update_time()
{
    seconds += 1;

    //Checks if a minute has passed.
    if(seconds == 60)
    {
        seconds = 0;
        minutes += 1;

        //Checks if an hour has passed.
        if(minutes == 60)
        {
            minutes = 0;
            hours += 1;
        }
    }

    // Updates the timer-label:
    QString timer = int_to_clock_QString(hours)+":"
            +int_to_clock_QString(minutes)
            +":"+int_to_clock_QString(seconds);

    time->setText(timer);
}

void MainWindow::delay()
{
    delay_seconds += 1;

    //Enables the buttons again after being disabled in turnClick():
    if(delay_seconds == 1)
    {
        change_button_enability(true);

        // Turns the cards back around if the cards didn't match
        // and changes the player-turn:
        // Matching cards will be left disabled.
        if(first_turned->text() != second_turned->text())
        {
            QFont font = first_turned->font();
            font.setPointSize(1);

            first_turned->setFont(font);
            second_turned->setFont(font);

            first_turned->setStyleSheet("* { background-color: rgb(255,0,0) }");
            second_turned->setStyleSheet("* { background-color: rgb(255,0,0) }");

            change_player_turn();
        }

        first_turned = nullptr;
        second_turned = nullptr;

        turn_delay_timer->stop();
        delay_seconds = 0;

        //Checks if all of the cards have been turned:
        // The amount of card letters is the same as amount of card pairs.
        if(letters_found.size() == cards_.size()/2)
        {
            game_run_time->stop();
            check_winner();
        }
        //Updates game-status-label for the next in-turn.
        else
        {
        game_status->setText(in_turn->get_name()+TURN);
        }
    }
}

void MainWindow::makecardgame(int pair_amount, int row_amout, int height)
{
    //Removes the cards found by the player, when new game is started:
    if(game_has_started == true)
    {
        for (player* p:players)
        {
            p->reset_score();
        }
        game_status->setText(in_turn->get_name()+TURN);
        cards_found_by_player->setText(HAS_CARDS);
    }
    //Makes the player-variables, if it's first game in the round:
    else
    {
        game_has_started = true;
        reset_->setEnabled(true);
        scores->setEnabled(true);
         make_players();
         change_player_turn();
    }

    //Makes the card-buttons and places them randomly in to a layout:
    remove_cards();
    make_buttons(pair_amount, height);
    make_random_grid(row_amout);
    reset_time();
    game_run_time->start(1000);

}

void MainWindow::display_scores()
{
    QString message = "SCORES:\n";

    // Makes the QString to be displayed in a messagebox:
    for(player* p : players)
    {
        QString name = p->get_name();
        QString score = QString::number(p->get_score());
        QString wins = QString::number(p->get_wins());
        QString letters = p->get_letter_QString();

        message += name+":\n";
        message += "Has cards: " + letters
                + " = " + score + "\n";
        message += "Wins: " + wins + "\n";
    }

    QMessageBox::information(this,"Scores",message);
}

void MainWindow::reset_game()
{
    // Resets everything and brings the program back to beginning:
    reset_time();
    game_has_started = false;
    delete_players();
    remove_cards();
    change_radio_button_visibility();
    game_status->setText(WELCOME);
    cards_found_by_player->setText("");
    scores->setEnabled(false);
    reset_->setEnabled(false);
    in_turn_index = -1;
    in_turn = nullptr;

    setCentralWidget(central);
}



void MainWindow::display_credits()
{
    QString message = "Creator:\nLeevi Hokkanen\ntbleho\n050253975\n"
                      "leevi.hokkanen@tuni.fi\nHervanta";
    QMessageBox::information(this,"Credits",message);

}



void MainWindow::end_program()
{
    close();
}

QString MainWindow::int_to_clock_QString(int unit)
{
    QString unit_QString = "";

    //Adds a zero in front of the number, if it's less than 10.
    if(unit<10)
    {
        unit_QString += "0";
    }
    unit_QString += QString::number(unit);

    return unit_QString;
}



void MainWindow::make_start_widgets()
{
    //Defines the labels:
    game_status = new QLabel;
    cards_found_by_player = new QLabel;
    time = new QLabel;
    game_status->setText(WELCOME);
    game_status->setFixedHeight(LABEL_HEIGHT);
    cards_found_by_player->setFixedHeight(LABEL_HEIGHT);
    time->setFixedHeight(LABEL_HEIGHT);

    //Defines the radio buttons:
    two_players = new QRadioButton(tr("2 Players"));
    three_players = new QRadioButton(tr("3 Players"));
    four_players = new QRadioButton(tr("4 Players"));
    two_players->setFixedHeight(RADIO_HEIGHT);
    three_players->setFixedHeight(RADIO_HEIGHT);
    four_players->setFixedHeight(RADIO_HEIGHT);

    //Places both labels and radio buttons in to layout:
    layout->addWidget(game_status,0,0);
    layout->addWidget(cards_found_by_player,0,1);
    layout->addWidget(time,0,2);
    layout->addWidget(two_players,1,0);
    layout->addWidget(three_players,2,0);
    layout->addWidget(four_players,3,0);

    two_players->setChecked(true);
    setCentralWidget(central);
}



void MainWindow::make_actions()
{
    //All actions defined:
    six_cards = new QAction(tr("6 Cards"), this);
    connect(six_cards,&QAction::triggered, [this]()
    {makecardgame(3,2,SIX_HEIGHT);});

    twelve_cards = new QAction(tr("12 Cards"), this);
    connect(twelve_cards,&QAction::triggered, [this]()
    {makecardgame(6,4,TWELVE_HEIGHT);});

    sixteen_cards = new QAction(tr("16 Cards"), this);
    connect(sixteen_cards,&QAction::triggered, [this]()
    {makecardgame(8,4,SIXTEEN_HEIGHT);});

    twenty_cards = new QAction(tr("20 Cards"), this);
    connect(twenty_cards,&QAction::triggered, [this]()
    {makecardgame(10,5,TWENTY_HEIGHT);});

    twentyfour_cards = new QAction(tr("24 Cards"), this);
    connect(twentyfour_cards,&QAction::triggered, [this]()
    {makecardgame(12,6,TWENTYFOUR_HEIGHT);});

    twentyfour_cards = new QAction(tr("24 Cards"), this);
    connect(twentyfour_cards,&QAction::triggered, [this]()
    {makecardgame(12,6,TWENTYFOUR_HEIGHT);});

    scores = new QAction(tr("Scores"),this);
    connect(scores, &QAction::triggered, this, &MainWindow::display_scores);
    scores->setEnabled(false);

    reset_ = new QAction(tr("Reset"),this);
    connect(reset_, &QAction::triggered, this, &MainWindow::reset_game);
    reset_->setEnabled(false);

    credits = new QAction(tr("Credits"),this);
    connect(credits, &QAction::triggered, this, &MainWindow::display_credits);

    end_ = new QAction(tr("Close"),this);
    connect(end_, &QAction::triggered, this, &MainWindow::end_program);
}

void MainWindow::make_menus()
{
    //Defines the menubar and its submenus:
    menu_bar = new QMenuBar(this);
    actions_menu = menu_bar->addMenu(tr("&Actions"));
    others_menu = menu_bar->addMenu(tr("&Others"));

    //Places the actions in to submenus:
    actions_menu->addAction(six_cards);
    actions_menu->addAction(twelve_cards);
    actions_menu->addAction(sixteen_cards);
    actions_menu->addAction(twenty_cards);
    actions_menu->addAction(twentyfour_cards);
    actions_menu->addAction(scores);
    actions_menu->addAction(reset_);

    others_menu->addAction(credits);
    others_menu->addAction(end_);

}


void MainWindow::make_players()
{
    int amount_of_players = 2;

    //Checks which radio button was pressed in the beginning:
    //Determines how many players are in the game:
    if(four_players->isChecked())
    {
        amount_of_players = 4;
    }
    else if(three_players->isChecked())
    {
        amount_of_players = 3;
    }

    //Hides all of the radio buttons:
    change_radio_button_visibility();

    int player_number = 1;

    //Makes the player variables:
    while(player_number<amount_of_players+1)
    {
        QString player_name = "Player "
                +QString::number(player_number);
        player* p = new player(player_name);
        players.push_back(p);
        player_number += 1;
    }

}


void MainWindow::make_buttons(int amount_of_pairs, int height)
{
    int current_pair = 0;

    //Makes and defines the buttons:
    for(char c = 'A'; c <= 'Z'; ++c)
    {
        QString letter = "";
        letter += c;
        QPushButton* pair1 = new QPushButton(letter, this);
        QPushButton* pair2 = new QPushButton(letter, this);

        pair1->setFixedHeight(height);
        pair2->setFixedHeight(height);

        //Makes the button font so small that it's impossible to see:
        QFont font = pair1->font();
        font.setPointSize(1);
        pair1->setFont(font);
        pair2->setFont(font);

        pair1->setStyleSheet("* { background-color: rgb(255,0,0) }");
        pair2->setStyleSheet("* { background-color: rgb(255,0,0) }");

        cards_.push_back(pair1);
        cards_.push_back(pair2);

        current_pair += 1;

        connect(pair1, &QPushButton::clicked,
                this, &MainWindow::turnClick);
        connect(pair2, &QPushButton::clicked,
                this, &MainWindow::turnClick);

        //Stops making new cards, when desired pair amount is reached.
        if (current_pair == amount_of_pairs)
        {
            break;
        }
    }

}


void MainWindow::make_random_grid(unsigned int row_amount)
{
    //Shuffles the vector buttons are placed in:
    unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(cards_.begin(),cards_.end(),std::default_random_engine(seed));

    unsigned int column = 0;
    unsigned int row = 1;

    //Places the buttons in to layout:
    for (QPushButton* cards : cards_)
    {
        layout->addWidget(cards,row,column);
        row += 1;

        if (row == row_amount+1)
        {
            row = 1;
            column += 1;
        }
    }

    setCentralWidget(central);
}


void MainWindow::change_player_turn()
{

    //If the player is last in the players vector:
    if(in_turn_index == players.size()-1)
    {
        in_turn = players.front();
        in_turn_index = 0;
    }
    // Every other scenario:
    else
    {
        in_turn_index += 1;
        in_turn = players.at(in_turn_index);

    }
    game_status->setText(in_turn->get_name()+TURN);
    cards_found_by_player->
            setText(HAS_CARDS+in_turn->get_letter_QString());
}


void MainWindow::change_radio_button_visibility()
{
    // Hides the visible radio buttons:
    if(two_players->isVisible() == true)
    {
        two_players->setVisible(false);
        three_players->setVisible(false);
        four_players->setVisible(false);
    }
    // Shows the hidden radio buttons:
    else
    {
        two_players->setVisible(true);
        three_players->setVisible(true);
        four_players->setVisible(true);
    }
}


void MainWindow::change_button_enability(bool yes_or_no)
{
    //Disables or enables the buttons:
    for(QPushButton* card : cards_)
    {
        QString card_letter = card->text();

        //The already-turned matching card-buttons will be kept disabled:
        if(std::find(letters_found.begin(),letters_found.end(),
                     card_letter) != letters_found.end())
        {
            continue;
        }
        card->setEnabled(yes_or_no);
    }
}


void MainWindow::remove_cards()
{
    first_turned = nullptr;
    second_turned = nullptr;
    card_has_been_turned = false;

    //The all of the cards will be permanently deleted in the following loop.
    for(QPushButton* card : cards_)
    {
        layout->removeWidget(card);
        delete card;
    }
    cards_.clear();
    letters_found.clear();
}


void MainWindow::check_winner()
{
    player* player_who_scored_most = nullptr;
    std::vector<int> scores;

    //Finds out who had the biggest score:
    for (player* p : players)
    {
        if(player_who_scored_most == nullptr or
                p->get_score() > player_who_scored_most->get_score())
        {
            player_who_scored_most = p;
        }
        scores.push_back(p->get_score());
    }

    int winning_score = player_who_scored_most->get_score();

    //Finds out, if more than one people had the same score winning (Draw):
    if(std::count(scores.begin(),scores.end(),winning_score)>1)
    {
        game_status->setText(DRAW);
        cards_found_by_player->setText("");
    }
    //Declares the winner:
    else
    {
        game_status->setText(player_who_scored_most->get_name()+WINS);
        player_who_scored_most->add_win();
        cards_found_by_player->setText("");
    }

}

void MainWindow::reset_time()
{
    seconds = 0;
    minutes = 0;
    hours = 0;
    time->setText(START_TIME);
    game_run_time->stop();
}


void MainWindow::delete_players()
{
    //Deletes the player variables:
    for(player* p : players)
    {
        delete p;
    }
    players.clear();
}




