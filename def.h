#pragma once

#ifndef DEF_H
#define DEF_H

typedef struct {
	unsigned char from, to;    //from:起始位置  to:终点
	unsigned char capture;     //表示被吃掉的棋子  保存
}Move;



#endif 