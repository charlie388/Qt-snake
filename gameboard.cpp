#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QFont>
#include <QLabel>
#include <QShortcut>
#include <QLCDNumber>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

#include "gameboard.h"
#include "snakefield.h"

GameBoard::GameBoard(QWidget *parent)
	: QWidget(parent)
{
	QPushButton *quit = new QPushButton();
	quit->setText(tr("&Quit"));
	quit->setFont(QFont("Times", 18, QFont::Bold));
	connect(quit, SIGNAL(clicked()), qApp, SLOT(quit()));

	SnakeField *snakeField = new SnakeField;

	QPushButton *newGame = new QPushButton(tr("&New Game"));
	newGame->setFont(QFont("Times", 18, QFont::Bold));
	connect(newGame, SIGNAL(clicked()), snakeField, SLOT(newGame()));
	connect(newGame, SIGNAL(clicked()), this, SLOT(newGame()));
	
	hits = new QLCDNumber(2);
	hits->setSegmentStyle(QLCDNumber::Filled);	
	connect(snakeField, SIGNAL(hit()), this, SLOT(hit()));
	
	QLabel *scoreLabel = new QLabel(tr("SCORE"));	
	scoreLabel->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
	
	(void) new QShortcut(Qt::Key_Up, snakeField, SLOT(snakeUp()));
	(void) new QShortcut(Qt::Key_Down, snakeField, SLOT(snakeDown()));
	(void) new QShortcut(Qt::Key_Right, snakeField, SLOT(snakeRight()));
	(void) new QShortcut(Qt::Key_Left, snakeField, SLOT(snakeLeft()));
				
	QHBoxLayout *topLayout = new QHBoxLayout;
	topLayout->addWidget(newGame);
	topLayout->addStretch(1);

	QVBoxLayout *leftLayout = new QVBoxLayout;
	leftLayout->addWidget(hits);
	leftLayout->addWidget(scoreLabel);	
	
	QGridLayout *gridLayout = new QGridLayout;
	gridLayout->addWidget(quit, 0, 0);
	gridLayout->addLayout(topLayout, 0, 1);
	gridLayout->addLayout(leftLayout, 1, 0);	
	gridLayout->addWidget(snakeField, 1, 1, 2, 1);
	gridLayout->setColumnStretch(1, 10);
	setLayout(gridLayout);
	
	snakeField->setFocus();
}

void GameBoard::hit()
{
	hits->display(hits->intValue() + 1);
}

void GameBoard::newGame()
{
	hits->display(0);
	//snakeField->newGame();
}
