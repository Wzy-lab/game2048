#include<iostream>
#include<iomanip>
#include<stdlib.h>
#include<windows.h>
#include<string>
#include<fstream>
#include <stack>
using namespace std;
//贾泽源，王振宇
class g2048
{
private:
	const string rankURL = "./phb.txt";
	const static int BoardSize = 4;
	const static int RankSize = 10;
	const static int maxNumLen = 4;

	int board[BoardSize][BoardSize] = { 0 };
	int boardCopyPast[BoardSize][BoardSize] = { 0 };
	int boardCopyNow[BoardSize][BoardSize] = { 0 };
	int tempNow[4];
	int tempPast[4];
	int score = 0;//得分
	int maxNum = 2;
	int eliminationLevel = 0;//连续消除等级，影响新生成的数字和分数	
	int zeroNum = BoardSize * BoardSize;//空位数量

	int ranking_list[RankSize] = { 0 };//排行榜
	int rank_len = 0;//实际排行榜长度

	bool isEliminated = false;//上一步是否有消除
	bool isMoved = false;//上一步是否有移动
	//bool allowback = false;//能否回退的标志变量
	void up();
	void down();
	void left();
	void right();

public:
	g2048();
	bool nextNum();//在随机坐标产生一个2或4，需要用到eliminationLevel 与 zeroNum,无法产生下一个数则判定失败
	//当棋盘发生改变才可以产生新的数
	void showBoard();//展示棋盘
	bool input();
	bool isChanged();//判断棋盘的变化，产生变化返回true，否则返回false
	//bool setalback(bool newAlb) { return (allowback= newAlb); }
	//bool gameoverCheck();//游戏结束检查，1、未产生新的方块 2、无法合并
	bool movable();
	void resetMark();//每一次显示结束后重置标志位
	void cleanBoard();//清空面板
	void goback();
	void copy();
	int getScore() { return score; }
	int get_maxNum() { return maxNum; }
	void readranking();
	void showranking();
	void writeranking();
};
//梁耀文
class play
{
private:
	g2048 game;//对象成员
public:
	void playGame();
	void showRank() { game.showranking(); }
	void gameMenu();
};
//主函数--梁耀文
int main()
{
	cout << "=====================" << endl;
	cout << " **** **** *  * **** " << endl;
	cout << "    * *  * *  * *  * " << endl;
	cout << " **** *  * **** **** " << endl;
	cout << " *    *  *    * *  * " << endl;
	cout << " **** ****    * **** " << endl;
	cout << "=====================" << endl;
	system("pause");
	play GAME;
	string choice;//存放选择结果
	while (1)
	{
		GAME.gameMenu();
		cin >> choice;
		if (choice.length() == 1)
			switch (choice[0])
			{
			case '1':
				GAME.playGame();
				system("pause");
				break;
			case '2':
				GAME.showRank();//显示排行榜
				system("pause");
				break;
			case '3':
				return 0;
				break;
			default:
				cout << "ERROR!" << endl;
				system("pause");
				break;
			}
		else
		{
			cout << "ERROR!" << endl;
			system("pause");
		}
	}
	return 0;
}
//构造函数--读取排行榜文件 王振宇
g2048::g2048()
{
	readranking();

}
//读取排行榜--王振宇
void g2048::readranking()
{
	ifstream ifile(rankURL);
	if (!ifile)
	{
		ofstream ofile(rankURL);
		if (!ofile)
		{
			cout << "排行榜数据读取失败！" << endl;
			return;
		}
		ofile.close();
	}
	for (rank_len = 0; rank_len < RankSize; rank_len++)
	{
		if (!ifile.eof())
		{
			ifile >> ranking_list[rank_len];
			if (ranking_list[rank_len] == 0)
				break;
		}
		else
			break;
	}
	ifile.close();
}
//显示排行榜--王振宇
void g2048::showranking()
{
	system("cls");
	cout << "TOP " << RankSize << endl;
	for (int i = 0; i < rank_len; i++)
	{
		printf("第%d名：%d\n", i + 1, ranking_list[i]);
	}
}
//写入排行榜--王振宇
void g2048::writeranking()
{
	if (score == 0)
		return;
	if (score <= ranking_list[rank_len - 1])
	{
		if (rank_len == RankSize)
			return;
		else
		{
			ranking_list[rank_len] = score;
			rank_len++;
		}
	}
	else
	{
		int t;
		if (rank_len < RankSize)
		{
			t = rank_len - 1;
			rank_len++;
		}
		else
			t = rank_len - 2;
		for (t; t >= 0; t--)
		{
			if (ranking_list[t] < score)
			{
				int temp = ranking_list[t];
				ranking_list[t + 1] = temp;
			}
			else
			{
				break;//找到合适的位置后退出循环
			}
		}
		ranking_list[t + 1] = score;
	}
	ofstream ofile(rankURL);
	for (int i = 0; i < rank_len - 1; i++)
	{
		ofile << ranking_list[i] << endl;
	}
	ofile << ranking_list[rank_len - 1];
	ofile.close();
}
//向上滑动--贾泽源
void g2048::up()
{
	//消除
	for (int j = 0; j < BoardSize; j++)
		for (int i = 0; i < BoardSize - 1; i++)
		{
			if (board[i][j] != 0)
			{
				int k;
				for (k = i + 1; k < BoardSize && board[k][j] == 0; k++)//锁定该列的下一个非零数
					continue;

				if (k >= BoardSize)//如果没有就跳出该列到下一列
					break;
				else if (board[i][j] == board[k][j])//如果存在且二者相等就合并，从下一行开始检查
				{
					board[i][j] += board[k][j];
					score += board[i][j];
					if (board[i][j] > maxNum)
						maxNum = board[i][j];
					board[k][j] = 0;
					i = k;
					isEliminated = true;
					eliminationLevel++;
					zeroNum++;
				}
				else if (board[i][j] != board[k][j])//如果存在且二者不等，则从后者开始重新检查
					i = k - 1;
			}
		}
	//移动
	for (int j = 0; j < BoardSize; j++)
	{
		int i = 0, k = 0;
		while (k < BoardSize && board[k][j]) k++;//找到第一个空位
		for (int i = k + 1; i < BoardSize; i++)
		{
			if (board[i][j] != 0)
			{
				int temp = board[i][j];
				board[i][j] = 0;
				board[k][j] = temp;
				isMoved = true;
				do { k++; } while (k < BoardSize && board[k][j]);
			}
			else
				continue;
		}
	}
	if (isMoved && !isEliminated)
	{
		eliminationLevel = 0;
	}
}
//向下滑动--贾泽源
void g2048::down()
{
	//消除
	for (int j = 0; j < BoardSize; j++)
		for (int i = BoardSize - 1; i > 0; i--)
		{
			if (board[i][j] != 0)
			{
				int k;
				for (k = i - 1; k >= 0 && board[k][j] == 0; k--)//锁定该列的下一个非零数
					continue;

				if (k < 0)//如果没有就跳出该列到下一列
					break;
				else if (board[i][j] == board[k][j])//如果存在且二者相等就合并，从下一行开始检查
				{
					board[i][j] += board[k][j];
					score += board[i][j];
					if (board[i][j] > maxNum)
						maxNum = board[i][j];
					board[k][j] = 0;
					i = k;
					isEliminated = true;
					eliminationLevel++;
					zeroNum++;
				}
				else if (board[i][j] != board[k][j])//如果存在且二者不等，则从后者开始重新检查
					i = k + 1;
			}
		}
	//移动
	for (int j = 0; j < BoardSize; j++)
	{
		int i = BoardSize - 1, k = BoardSize - 1;
		while (k >= 0 && board[k][j]) k--;//找到第一个空位
		for (int i = k - 1; i >= 0; i--)
		{
			if (board[i][j] != 0)
			{
				int temp = board[i][j];
				board[i][j] = 0;
				board[k][j] = temp;
				isMoved = true;
				do { k--; } while (k >= 0 && board[k][j]);
			}
			else
				continue;
		}
	}
	if (isMoved && !isEliminated)
	{
		eliminationLevel = 0;
	}
}
//向左滑动--贾泽源
void g2048::left()
{
	//消除
	for (int i = 0; i < BoardSize; i++)
		for (int j = 0; j < BoardSize - 1; j++)
		{
			if (board[i][j] != 0)
			{
				int k;
				for (k = j + 1; k < BoardSize && board[i][k] == 0; k++)//锁定该列的下一个非零数
					continue;

				if (k >= BoardSize)//如果没有就跳出该列到下一列
					break;
				else if (board[i][j] == board[i][k])//如果存在且二者相等就合并，从下一行开始检查
				{
					board[i][j] += board[i][k];
					score += board[i][j];
					if (board[i][j] > maxNum)
						maxNum = board[i][j];
					board[i][k] = 0;
					j = k;
					isEliminated = true;
					eliminationLevel++;
					zeroNum++;
				}
				else if (board[i][j] != board[i][k])//如果存在且二者不等，则从后者开始重新检查
					j = k - 1;
			}
		}
	//移动
	for (int i = 0; i < BoardSize; i++)
	{
		int j = 0, k = 0;
		while (k < BoardSize && board[i][k]) k++;//找到第一个空位
		for (int j = k + 1; j < BoardSize; j++)
		{
			if (board[i][j] != 0)
			{
				int temp = board[i][j];
				board[i][j] = 0;
				board[i][k] = temp;
				isMoved = true;
				do { k++; } while (k < BoardSize && board[i][k]);
			}
			else
				continue;
		}
	}
	if (isMoved && !isEliminated)
	{
		eliminationLevel = 0;
	}
}
//向右滑动--贾泽源
void g2048::right()
{
	//消除
	for (int i = 0; i < BoardSize; i++)
		for (int j = BoardSize - 1; j > 0; j--)
		{
			if (board[i][j] != 0)
			{
				int k;
				for (k = j - 1; k >= 0 && board[i][k] == 0; k--)//锁定该列的下一个非零数
					continue;

				if (k < 0)//如果没有就跳出该列到下一列
					break;
				else if (board[i][j] == board[i][k])//如果存在且二者相等就合并，从下一行开始检查
				{
					board[i][j] += board[i][k];
					score += board[i][j];
					if (board[i][j] > maxNum)
						maxNum = board[i][j];
					board[i][k] = 0;
					j = k;
					isEliminated = true;
					eliminationLevel++;
					zeroNum++;
				}
				else if (board[i][j] != board[i][k])//如果存在且二者不等，则从后者开始重新检查
					j = k + 1;
			}
		}
	//移动
	for (int i = 0; i < BoardSize; i++)
	{
		int j = BoardSize - 1, k = BoardSize - 1;
		while (k >= 0 && board[i][k]) k--;//找到第一个空位
		for (int j = k - 1; j >= 0; j--)
		{
			if (board[i][j] != 0)
			{
				int temp = board[i][j];
				board[i][j] = 0;
				board[i][k] = temp;
				isMoved = true;
				do { k--; } while (k >= 0 && board[i][k]);
			}
			else
				continue;
		}
	}
	if (isMoved && !isEliminated)
	{
		eliminationLevel = 0;
	}
}
//拷贝棋盘
void g2048::copy()
{
	for (int i = 0; i < BoardSize; i++)
		for (int j = 0; j < BoardSize; j++)
		{
			boardCopyPast[i][j] = boardCopyNow[i][j];
			boardCopyNow[i][j] = board[i][j];
		}
	tempPast[0] = tempNow[0];
	tempNow[0] = score;
	tempPast[1] = tempNow[1];
	tempNow[1] = maxNum;
	tempPast[2] = tempNow[2];
	tempNow[2] = eliminationLevel;
	tempPast[3] = tempNow[3];
	tempNow[3] = zeroNum;
}
//回退操作
void g2048::goback()
{
	for (int i = 0; i < BoardSize; i++)
		for (int j = 0; j < BoardSize; j++)
		{
			boardCopyNow[i][j] = boardCopyPast[i][j];
			board[i][j] = boardCopyNow[i][j];
		}
	tempNow[0] = tempPast[0];
	score = tempNow[0];
	tempNow[1] = tempPast[1];
	maxNum = tempNow[1];
	tempNow[2] = tempPast[2];
	eliminationLevel = tempNow[2];
	tempNow[3] = tempPast[3];
	zeroNum = tempNow[3];
}
//重置标识位--贾泽源
void g2048::resetMark()
{
	isEliminated = false;//无消除
	isMoved = false;//无移动
}
//棋盘可移动检查--王振宇
bool g2048::movable()
{
	if (zeroNum > 0)
		return true;
	for (int i = 1; i < BoardSize; i++)
	{
		if (board[i][0] == board[i - 1][0] || board[0][i] == board[0][i - 1])
			return true;
		for (int j = 1; j < BoardSize; j++)
		{
			if (board[i][j] == board[i][j - 1] || board[i][j] == board[i - 1][j])
				return true;
		}
	}
	return false;
}
//棋盘改动检查--贾泽源
bool g2048::isChanged()
{
	return (isEliminated || isMoved);
}
//显示当前棋盘--贾泽源
void g2048::showBoard()
{
	cout << '+';
	for (int j = 0; j < BoardSize; j++)
	{
		for (int k = 0; k < maxNumLen; k++)
			cout << '-';
		cout << '+';
	}
	cout << endl;
	for (int i = 0; i < BoardSize; i++)
	{
		cout << '|';
		for (int j = 0; j < BoardSize; j++)
		{
			if (board[i][j] == 0)
				cout << setw(maxNumLen) << ' ' << '|';
			else
				cout << setw(maxNumLen) << board[i][j] << '|';
		}
		cout << endl;
		cout << '+';
		for (int j = 0; j < BoardSize; j++)
		{
			for (int k = 0; k < maxNumLen; k++)
				cout << '-';
			cout << '+';
		}
		cout << endl;
	}
	cout << "=====================================" << endl;
	for (int i = 0; i < BoardSize; i++)
	{
		for (int j = 0; j < BoardSize; j++)
			cout << boardCopyNow[i][j] << ' ';
		cout << endl;
	}
	cout << "----------------------------------------" << endl;
	for (int i = 0; i < BoardSize; i++)
	{
		for (int j = 0; j < BoardSize; j++)
			cout << boardCopyPast[i][j] << ' ';
		cout << endl;
	}
	cout << "----------------------------------------" << endl;
	cout << "isEliminated:" << isEliminated << endl;
	cout << "eliminationLevel:" << eliminationLevel << endl;
	cout << "isMoved:" << isMoved << endl;
	cout << "zeroNum:" << zeroNum << endl;
	cout << "maxNum:" << maxNum << endl;
	cout << "score:" << score << endl;
	cout << "allowback:" << allowback << endl;
}
//随机空位产生新的方块--王振宇
bool g2048::nextNum()
{
	int rdmnum = 0, row = 0;
	if (zeroNum > 1)
	{
		srand(time(NULL));
		rdmnum = rand() % zeroNum;
	}
	else if (!zeroNum)
	{
		cout << "无法产生新的方块" << endl;
		return false;
	}
	zeroNum--;
	for (int i = 0; i < BoardSize; i++)
	{
		for (int j = 0; j < BoardSize; j++)
		{
			if (board[i][j] == 0)
			{
				if (rdmnum == 0)
				{
					if (eliminationLevel >= 3)
					{
						board[i][i] = 4;
						eliminationLevel = 0;
					}
					else
					{
						board[i][j] = 2;
					}
					return true;
				}
				rdmnum--;
			}
		}
	}
}
//游戏中指令的控制--王振宇
bool g2048::input()
{
	string ss;
	cin >> ss;
	char choice = toupper(ss[0]);
	switch (choice)
	{
	case 'W':
		up();
		break;
	case 'A':
		left();
		break;
	case 'S':
		down();
		break;
	case 'D':
		right();
		break;
	case 'B':
		goback();
		break;
	case 'E':
		return false;
	case 'R':
		cleanBoard();
		break;
	default:
		cout << "ERROR" << endl;
		system("pause");
		break;
	}
	return true;
}
//棋盘清理--贾泽源
void g2048::cleanBoard()
{
	for (int i = 0; i < BoardSize; i++)
	{
		for (int j = 0; j < BoardSize; j++)
		{
			board[i][j] = 0;
			boardCopyPast[i][j] = 0;
			boardCopyNow[i][j] = 0;
		}
	}
	score = 0;
	maxNum = 2;
	eliminationLevel = 0;
	resetMark();
	zeroNum = BoardSize * BoardSize;
	nextNum();
	nextNum();

	copy();
	copy();
}
//游戏菜单显示--梁耀文
void play::gameMenu()
{
	system("cls");
	cout << "+-------------+" << endl;
	cout << "| 1、开始游戏 |" << endl;
	cout << "| 2、排行榜   |" << endl;
	cout << "| 3、退出游戏 |" << endl;
	cout << "+-------------+" << endl;
}
//控制整个游戏的过程--梁耀文
void play::playGame()
{
	game.cleanBoard();
	while (1)
	{
		system("cls");
		game.showBoard();
		game.resetMark();//重置二个标志位
		if (game.input())
		{
			if (game.isChanged())
			{
				game.nextNum();//产生一个新的方块
				if (!game.movable())
				{
					break;
				}
				game.copy();
			}
		}
		else
			break;
	}
	system("cls");
	game.showBoard();
	cout << "GAME OVER ! ! !" << endl;
	game.writeranking();
}