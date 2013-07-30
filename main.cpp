#include <QApplication>
#include <QSizePolicy>

#include "gameboard.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	GameBoard board;
	board.setGeometry(100, 100, 500, 400);
	board.setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);	
	board.show();
	return app.exec();
}
