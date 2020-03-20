#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include "configurationdialog.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->gameBoard->setEnabled(false);

    connect(ui->startNewGame, &QAction::triggered,
            this, &MainWindow::startNewGame);
    connect(ui->gameBoard, &TicTacToeWidget::currentPlayerChanged,
            this, &MainWindow::updateNameLabels);
    connect(ui->gameBoard, &TicTacToeWidget::gameOver,
            this, &MainWindow::handleGameOver);
    connect(ui->quit, &QAction::triggered,
            qApp, &QApplication::quit);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setLableBold(QLabel *label, bool isBold)
{
   QFont f = label->font();
   f.setBold(isBold);
   label->setFont(f);
}


void MainWindow::startNewGame()
{
    if(dialogBereitsGezeigt == false)
    {
        ConfigurationDialog dlg(this);
        if(dlg.exec() == QDialog::Rejected)
        {
            return; //do nothing if dialog rejected
        }
    ui->player1->setText(dlg.player1Name());
    ui->player2->setText(dlg.player2Name());
    dialogBereitsGezeigt = true;
    }
    QImage imageX(X);
    QImage imageO(O);
    ui->symbolPlayer1->setPixmap(QPixmap::fromImage(imageX.scaled(40,40)));
    ui->symbolPlayer2->setPixmap(QPixmap::fromImage(imageO.scaled(40,40)));
    ui->gameBoard->initNewGame();
    ui->gameBoard->setEnabled(true);
}

void MainWindow::updateNameLabels()
{
    QFont f = ui->player1->font();
    QImage imageO(O), imageX(X);
    f.setBold(ui->gameBoard->currentPlayer() == TicTacToeWidget::Player::Player1);
    ui->player1->setFont(f);
    ui->symbolPlayer1->setPixmap(QPixmap::fromImage(imageX.scaled(40,40)));
    f.setBold(ui->gameBoard->currentPlayer() == TicTacToeWidget::Player::Player2);
    ui->player2->setFont(f);
    ui->symbolPlayer2->setPixmap(QPixmap::fromImage(imageO.scaled(40,40)));
}

void MainWindow::handleGameOver(TicTacToeWidget::Player winner)
{
   ui->gameBoard->setEnabled(false);
   QString message, symbol;

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

   if(winner == TicTacToeWidget::Player::Draw)
   {
       message = "Oh no!\nThe Game ended with a draw.";
       symbol = ":/icons/newgame.png";
   }
   else
   {
       message = QString("Player %1 wins!\nCongratulations!!!").arg(winner == TicTacToeWidget::Player::Player1 ? ui->player1->text(): ui->player2->text());
        winner ==TicTacToeWidget::Player::Player1 ? symbol = X : symbol = O;



   }
  // QMessageBox::information(this, "Info", message);

   QMessageBox msgBox;
   QPushButton *newGameButton = msgBox.addButton(tr("Neues Spiel"), QMessageBox::ActionRole);
   QPushButton *endGameButton = msgBox.addButton(tr("Spiel Beenden"), QMessageBox::ActionRole);
   QFont font = msgBox.font();
   font.setPointSize(15);
   msgBox.setFont(font);
   msgBox.setText(message);
   msgBox.setWindowTitle("Fucking Tic Tac and Toe");
   msgBox.setWindowIcon(QPixmap(symbol));
   QImage winningIcon(symbol);
   msgBox.setIconPixmap(QPixmap::fromImage(winningIcon.scaled(80,80)));

   msgBox.exec();

   if (msgBox.clickedButton() == newGameButton)
   {
      startNewGame();
   }
   else if (msgBox.clickedButton() == endGameButton)
   {
        this->close();
   }
}

