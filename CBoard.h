#pragma once

#ifndef CBOARD_H
#define CBOARD_H


#include "def.h"

class CBoard
{
public:
	CBoard();
	virtual ~CBoard();

	//变量声明
	int side;     //红方走：0 黑方走：1
	unsigned char board[256];   //棋盘数组
	unsigned char piece[48];    //棋子数组 下标表示对应的棋子数字 数值表示位置

	Move MoveStack[128];    //走法栈
	int StackTop;    //栈顶指针
	Move BestMove;   //最佳走法
	int DepthNow;    //当前搜索深度
	int MaxDepth;    //最大搜索深度
	const int MaxValue = 3000;    //最大估值


								  //局面表示相关函数
	void ClearBoard();                //清空棋盘数组
	void OutputBoard();               //输出棋盘数组
	void OutputPiece();               //输出棋子数组
	char intToChar(int a);            //整数值->字符值

									  //走法生成相关函数
	int saveMove(unsigned char from, unsigned char to, Move *m); //保存生成走法，保存成功返回1
	int GenAllMove(Move *MoveArray);   //生成所有走法
	int Check(int lside);  //检查lside一方一否被将军

	void addPiece(int pos, int pc);


	int LegalMove(Move mv);            //判断当前走法是否合理
	int HasLegalMove();                //判断当前是否还有步数可走

									   //局面估值相关函数
	short eval();

	//搜索函数
	void ComputerThink();
	int NegaMaxSearch(int depth, int alpha, int beta);  //alpha-beta剪枝负值极大搜索
	bool MakeMove(Move mv);
	void UnMakeMove();
	void ChangeSide();

	void AddPiece(int pos, int pc);
	int CharToSubscript(char ch);	// FEN串中棋子对应的数组下标
									//下标0，1，2，3，4，5，6分别对应表示将，仕，象，马，车，炮，兵
	void StringToArray(const char *FenStr); // 将FEN串表示的局面转换成一维数组
};

#endif