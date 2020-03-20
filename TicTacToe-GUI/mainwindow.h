#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <tictactoewidget.h>

namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
bool dialogBereitsGezeigt = false;

QString X = ":/icons/X3.png";
QString O = ":/icons/O3.png";
int roundCount = 0;

    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    static void setLableBold(QLabel *label, bool isBold);

private slots:
    void updateNameLabels();
    void handleGameOver(TicTacToeWidget::Player winner);
    void startNewGame();
};

#endif // MAINWINDOW_H
