#pragma once

#ifndef CBOARD_H
#define CBOARD_H


#include "def.h"

class CBoard
{
public:
	CBoard();
	virtual ~CBoard();

	//��������
	int side;     //�췽�ߣ�0 �ڷ��ߣ�1
	unsigned char board[256];   //��������
	unsigned char piece[48];    //�������� �±��ʾ��Ӧ���������� ��ֵ��ʾλ��

	Move MoveStack[128];    //�߷�ջ
	int StackTop;    //ջ��ָ��
	Move BestMove;   //����߷�
	int DepthNow;    //��ǰ�������
	int MaxDepth;    //����������
	const int MaxValue = 3000;    //����ֵ


								  //�����ʾ��غ���
	void ClearBoard();                //�����������
	void OutputBoard();               //�����������
	void OutputPiece();               //�����������
	char intToChar(int a);            //����ֵ->�ַ�ֵ

									  //�߷�������غ���
	int saveMove(unsigned char from, unsigned char to, Move *m); //���������߷�������ɹ�����1
	int GenAllMove(Move *MoveArray);   //���������߷�
	int Check(int lside);  //���lsideһ��һ�񱻽���

	void addPiece(int pos, int pc);


	int LegalMove(Move mv);            //�жϵ�ǰ�߷��Ƿ����
	int HasLegalMove();                //�жϵ�ǰ�Ƿ��в�������

									   //�����ֵ��غ���
	short eval();

	//��������
	void ComputerThink();
	int NegaMaxSearch(int depth, int alpha, int beta);  //alpha-beta��֦��ֵ��������
	bool MakeMove(Move mv);
	void UnMakeMove();
	void ChangeSide();

	void AddPiece(int pos, int pc);
	int CharToSubscript(char ch);	// FEN�������Ӷ�Ӧ�������±�
									//�±�0��1��2��3��4��5��6�ֱ��Ӧ��ʾ�����ˣ����������ڣ���
	void StringToArray(const char *FenStr); // ��FEN����ʾ�ľ���ת����һά����
};

#endif