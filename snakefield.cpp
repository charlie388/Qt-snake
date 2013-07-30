#include <QPaintEvent>
#include <QPainter>
#include <QTime>
#include <QDateTime>
#include <QTimer>
#include <math.h>

#include "snakefield.h"

SnakeField::SnakeField(QWidget *parent)
	: QWidget(parent)
{	
	snakeHead = NULL;
	gameEnded = false;
	
	QTime midnight(0,0,0);
	qsrand(midnight.secsTo(QTime::currentTime()));
	
	moveSnakeTimer = new QTimer(this);
	connect( moveSnakeTimer, SIGNAL(timeout()), this, SLOT(moveSnake()));
				
	setPalette(QPalette(QColor(250, 250, 200)));
	setAutoFillBackground(true);	
}

void SnakeField::killSnake()
{
	Snake *next;
	while(snakeHead != NULL) {
		next = snakeHead->next;
		free(snakeHead);
		snakeHead = next;
	}

}

void SnakeField::growSnake()
{
	int x0,y0;
	Snake *body;
	body = (Snake*)malloc(sizeof(Snake));
	if(snakeHead==NULL){
	
		body->point = QPoint(5, 135);
		body->direction = UP;
		body->next = NULL;
		snakeHead = body;
		nextStepDirection = UP;
	}else{
		x0 = snakeHead->point.x();
		y0 = snakeHead->point.y();
		switch(snakeHead->direction)
		{
			case UP:
				body->point = QPoint(x0,y0-10);
				break;
			case DOWN:
				body->point = QPoint(x0,y0+10);
				break;
			case RIGHT:
				body->point = QPoint(x0+10,y0);
				break;
			case LEFT:
				body->point = QPoint(x0-10,y0);
				break;
		}
		body->direction = snakeHead->direction;
		body->next = snakeHead;
		snakeHead = body;
	}	
}

void SnakeField::snakeUp()
{
	if(snakeHead==NULL) return;
	if(snakeHead->direction==DOWN) return;
	nextStepDirection = UP;
}

void SnakeField::snakeDown()
{
	if(snakeHead==NULL) return;
	if(snakeHead->direction==UP) return;
	nextStepDirection = DOWN;
}


void SnakeField::snakeRight()
{
	if(snakeHead==NULL) return;
	if(snakeHead->direction==LEFT) return;
	nextStepDirection = RIGHT;
}

void SnakeField::snakeLeft()
{
	if(snakeHead==NULL) return;
	if(snakeHead->direction==RIGHT) return;
	nextStepDirection = LEFT;	
}

void SnakeField::moveSnake()
{
	if(snakeHead==NULL)
		return;
		
	snakeHead->direction = nextStepDirection;
	if( isCollision(snakeHead) ) {
		moveSnakeTimer->stop();
		gameEnded = true;		
	}
	
	if( isEatting(snakeHead) ){
		newFood();
		growSnake();
		emit hit();
		update();
		return;
	}
		
	Snake *body = snakeHead;
	Direction preDir = snakeHead->direction;
	Direction tmp;
	int x0,y0;
	while( body!=NULL )
	{
		x0 = body->point.x();
		y0 = body->point.y();		 		
		switch(body->direction)
		{
			case UP:
				body->point = QPoint(x0,y0-10);
				break;
			case DOWN:
				body->point = QPoint(x0,y0+10);
				break;
			case RIGHT:
				body->point = QPoint(x0+10,y0);
				break;
			case LEFT:
				body->point = QPoint(x0-10,y0);
				break;
		}
		if (body->direction != preDir)
		{
			tmp = body->direction;
			body->direction = preDir;
			preDir = tmp;
		}	

		body = body->next;
	}
	
	update();
}

bool SnakeField::isCollision(Snake *snake)
{
	Snake body,*tmp; 
	body.point = snake->point;
	body.direction = snake->direction;
	
	int x0 = body.point.x();
	int y0 = body.point.y();
	
	switch(body.direction)
	{
		case UP:
			body.point = QPoint(x0,y0-10);
			break;
		case DOWN:
			body.point = QPoint(x0,y0+10);
			break;
		case RIGHT:
			body.point = QPoint(x0+10,y0);
			break;
		case LEFT:
			body.point = QPoint(x0-10,y0);
			break;
	}
	
	QRect bodyRect = snakeRect(&body);
	//Margin collision detect
	if( bodyRect.x()<0 || (bodyRect.x()+bodyRect.width())>width()
		|| bodyRect.y()<0 || (bodyRect.y()+bodyRect.height())>height() )
	{
		return true;
	}
	
	//snake body itself collision detect
	for( tmp=snakeHead; tmp!=NULL; tmp=tmp->next)
	{
		if(tmp==snake) continue;
				
		if(bodyRect.intersects(snakeRect(tmp)))
			if(tmp->next==NULL)
				break;
			else
				return true;	
	}
	
	return false;
}

bool SnakeField::isEatting(Snake *snake)
{
	Snake body; 
	body.point = snake->point;
	body.direction = snake->direction;
	
	int x0 = body.point.x();
	int y0 = body.point.y();
	
	switch(body.direction)
	{
		case UP:
			body.point = QPoint(x0,y0-10);
			break;
		case DOWN:
			body.point = QPoint(x0,y0+10);
			break;
		case RIGHT:
			body.point = QPoint(x0+10,y0);
			break;
		case LEFT:
			body.point = QPoint(x0-10,y0);
			break;
	}

	QRect bodyRect = snakeRect(&body);
	if(bodyRect.intersects(foodRect()))
		return true;
	else	
		return false;
}

void SnakeField::newFood()
{
	food = QPoint((qrand()%30)*10+5, (qrand()%30)*10+5);
}

void SnakeField::newGame()
{
	killSnake();
	gameEnded = false;
	
	for(int i=0; i<10; i++) 
		growSnake();
		
	newFood();

	moveSnakeTimer->start(250);
	update();
}

void SnakeField::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	
	if (gameEnded) {
	/*
		double pi = 3.14;
		double a    = pi/180 * 45.0;
		double sina = sin(a);
		double cosa = cos(a);
		QMatrix translationMatrix(1, 0, 0, 1, 0, 0);
		QMatrix rotationMatrix(cosa, sina, -sina,cosa,0,0);
		//QMatrix scalingMatrix(0.5, 0, 0, 0.5, 0, 0);
		QMatrix matrix = translationMatrix * rotationMatrix; //* scalingMatrix;
		painter.setMatrix(matrix);
	*/
		painter.setPen(Qt::black);
		painter.setFont(QFont("Courier", 48, QFont::Bold));
		painter.drawText(rect(), Qt::AlignCenter, tr("Game Over"));
	}
	
	if(moveSnakeTimer->isActive())
	{
		paintSnake(painter);
		paintFood(painter);	
	}	
		
}

void SnakeField::paintSnake(QPainter &painter)
{
	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::gray);
	
	Snake *body = snakeHead;
	while(body!=NULL){
	
		if( painter.brush() == Qt::gray )
			painter.setBrush(Qt::blue);
		else
			painter.setBrush(Qt::gray);
			
		painter.drawRect(snakeRect(body));
		body=body->next;
	}
}

QRect SnakeField::snakeRect(Snake *body)
{
	QRect result(0,0,10,10);
	result.moveCenter(body->point);
	return result;
}

void SnakeField::paintFood(QPainter &painter)
{
	painter.setPen(Qt::black);
	painter.setBrush(Qt::red);
	painter.drawEllipse(foodRect());
}

QRect SnakeField::foodRect()
{
	QRect result(0, 0, 10, 10);
	result.moveCenter(QPoint(food.x(), food.y()));
	return result;
}
