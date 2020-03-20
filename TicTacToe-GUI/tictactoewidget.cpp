#include "tictactoewidget.h"
#include "ui_mainwindow.h"
#include <QGridLayout>
#include <QDebug>
#include <QSignalMapper>
#include <QLayoutItem>

TicTacToeWidget::TicTacToeWidget(QWidget *parent) : QWidget(parent)
{
    m_currentPlayer = Player::Invalid;
    QGridLayout *gridLayout = new QGridLayout(this);
    QSignalMapper *mapper = new QSignalMapper(this);
    for(int row = 0; row < 3; ++row) {
        for(int column = 0; column < 3; ++column) {
            QPushButton *button = new QPushButton(" ");
            button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            button->setFocusPolicy(Qt::NoFocus);

            gridLayout->addWidget(button, row, column);
            m_board.append(button);
            mapper->setMapping(button, m_board.count() - 1);
            connect(button, SIGNAL(clicked()), mapper, SLOT(map()));
        }
    }
    connect(mapper, SIGNAL(mapped(int)),
            this,   SLOT(handleButtonClick(int)));
}

void TicTacToeWidget::initNewGame() {
    for(QPushButton *button: m_board) {
        button->setText(" ");
        button->setIcon(QIcon());
        button->setFocusPolicy(Qt::NoFocus);
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        QFont font = button->font();
        font.setPointSize(1);
        button->setFont(font);
        if(roundCount == 0)
        {
            X = ":/icons/X3.png";
            O = ":/icons/O3.png";

            roundCount++;
        }
        else if(roundCount == 1)
        {
            X = ":/icons/X.png";
            O = ":/icons/O.png";
        roundCount++;
        }
        else if(roundCount == 2)
        {
            X = ":/icons/X3.png";
            O = ":/icons/O3.png";
        roundCount++;
        }
        else if(roundCount == 3)
        {
            X = ":/icons/X2.png";
            O = ":/icons/O2.png";
        roundCount = 0;
        }
    }
    setCurrentPlayer(Player::Player1);
}

void TicTacToeWidget::setCurrentPlayer(TicTacToeWidget::Player p)
{
    if(m_currentPlayer == p) {
        return;
    }
    m_currentPlayer = p;
    emit currentPlayerChanged(p);
}

void TicTacToeWidget::handleButtonClick(int index)
{
    if (m_currentPlayer == Player::Invalid) {
        return; // game is not started
    }
    if(index < 0 || index >= m_board.size()) {
        return; // out of bounds check
    }
    QPushButton *button = m_board[index];
    if(button->text() != " ") return; // invalid move
    button->setText(currentPlayer() == Player::Player1 ? "X" : "O");
    if(button->text() == "X")
    {
        button->setIcon(QIcon(X));
        button->setIconSize(QSize(button->size()/1.5));
    }
    else if(button->text() == "O")
    {
        button->setIcon(QIcon(O));
        button->setIconSize(QSize(button->size()/1.5));
    }
    Player winner = checkWinCondition();
    if(winner == Player::Invalid) {
        setCurrentPlayer(currentPlayer() == Player::Player1 ? Player::Player2 : Player::Player1);
        return;
    } else {
        qDebug() << "gameover" << winner;
        emit gameOver(winner);
    }
}

TicTacToeWidget::Player TicTacToeWidget::checkWinConditionForLine(
        int index1, int index2, int index3) const {
    QString text1 = m_board[index1]->text();
    if (text1 == " ") {
        return Player::Invalid;
    }
    QString text2 = m_board[index2]->text();
    QString text3 = m_board[index3]->text();
    if (text1 == text2 && text1 == text3) {
        return text1 == "X" ? Player::Player1 : Player::Player2;
    }
    return Player::Invalid;
}

TicTacToeWidget::Player TicTacToeWidget::checkWinCondition() const
{
    Player result = Player::Invalid;
    // check horizontals
    for(int row = 0; row < 3; ++row) {
        result = checkWinConditionForLine(row * 3,
                                          row * 3 + 1,
                                          row * 3 + 2);
        if (result != Player::Invalid) {
            return result;
        }
    }
    // check verticals
    for(int column = 0; column < 3; ++column) {
         result = checkWinConditionForLine(column,
                                           3 + column,
                                           6 + column);
        if (result != Player::Invalid) {
            return result;
        }
    }
    // check diagonals
    result = checkWinConditionForLine(0, 4, 8);
    if (result != Player::Invalid) {
        return result;
    }
    result = checkWinConditionForLine(2, 4, 6);
    if (result != Player::Invalid) {
        return result;
    }
    // check if there are unoccupied fields left
    for(QPushButton *button: m_board) {
        if(button->text() == " ") {
            return Player::Invalid;
        }
    }
    return Player::Draw;
}
