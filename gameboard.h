#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLCDNumber;
class SnakeField;
QT_END_NAMESPACE

class GameBoard : public QWidget
{
	Q_OBJECT
public:
	GameBoard(QWidget *parent = 0);
	
public slots:
	void hit();
	void newGame();
	
private:
	QLCDNumber *hits;
	SnakeField *snakeField;
};

#endif
