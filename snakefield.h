#ifndef SNAKEFIELD_H
#define SNAKEFIELD_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QTimer;
QT_END_NAMESPACE

typedef enum DIRECTION {UP,DOWN,RIGHT,LEFT} Direction;

typedef struct SNAKE
{
	QPoint		point;
	struct SNAKE	*next;
	Direction		direction;
} Snake;

class SnakeField : public QWidget
{
	Q_OBJECT
public:
	SnakeField(QWidget *parent = 0);
	void killSnake();
	void growSnake();
	void newFood();

			
public slots:
	void newGame();
	void snakeUp();
	void snakeDown();
	void snakeRight();
	void snakeLeft();

signals:
	void hit();	
	
protected:
	void paintEvent(QPaintEvent *event);
	
private slots:
	void moveSnake();
		
private:
	void paintSnake(QPainter &painter);
	QRect snakeRect(Snake *body);
	void paintFood(QPainter &painter);
	QRect foodRect();	
	bool isCollision(Snake *snake);
	bool isEatting(Snake *snake);
			

	Snake *snakeHead;
	QTimer *moveSnakeTimer;
	
	QPoint food;
	Direction nextStepDirection;
	bool gameEnded;
};

#endif
