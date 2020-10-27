#include<iostream>
#include<iomanip>
#include<stdlib.h>
#include<windows.h>
#include<string>
#include<fstream>
using namespace std;

#define BoardSize 3
#define RankSize 10
class g2048
{
private:
	int board[BoardSize][BoardSize] = { 0 };
	int boardCopy[BoardSize][BoardSize] = { 0 };
	int score = 0;//得分
	int maxNum = 2;
	int maxNumLen = 4;
	bool isEliminated = false;//上一步是否有消除
	bool isMoved = false;//上一步是否有移动
	int eliminationLevel = 0;//连续消除等级，影响新生成的数字和分数
	int zeroNum = BoardSize * BoardSize;//空位数量
	int ranking_list[RankSize];//排行榜
	int rank_len=0;//实际排行榜长度
public:
	bool isPlaying = false;
	g2048();
	bool nextNum();//在随机坐标产生一个2或4，需要用到eliminationLevel 与 zeroNum,无法产生下一个数则判定失败
	//当棋盘发生改变才可以产生新的数
	void showBoard();//展示棋盘

	//四个移动合并操作
	void up();
	void down();
	void left();
	void right();

	bool input();
	bool isChanged();//判断棋盘的变化，产生变化返回true，否则返回false
	//bool gameoverCheck();//游戏结束检查，1、未产生新的方块 2、无法合并
	bool movable();
	void restart();//重新开始
	void resetMark();//每一次显示结束后重置标志位

	//返回游戏结果
	int getScore() { return score; }
	int get_maxNum() { return maxNum; }
	void readranking();
	void showranking();
	void writeranking();
};
void loading(int time)
{
	char a[] = { '|' , '/' ,'-','\\' };
	int i = time * 2, k;
	while (i--)
	{
		k = i % 4;
		cout << '\b' << a[k];
		Sleep(500);
	}
	cout << '\b' << "success!" << endl;
}

int main()
{
	loading(3);
	g2048 game;
	game.isPlaying = true;
	while (game.isPlaying)
	{
		system("cls");
		game.showBoard();
		game.resetMark();
		if (game.input() && game.isChanged())
		{
			game.nextNum();
			if (!game.movable())
			{
				game.isPlaying = false;
				system("cls");
				game.showBoard();
				cout << "GAME OVER ! ! !" << endl;
			}
		}
	}

	system("pause");
	return 0;
}
//读取排行榜
void g2048::readranking()
{
	ifstream ifile("E:\\phb.txt");
	if (!ifile)
	{
		cout << "排行榜数据读取失败！" << endl;
		ofstream ofile("E:\\p.txt");
		ofile.close();
	}
	for (rank_len = 0; rank_len < RankSize; rank_len++)
	{
		if (!ifile.eof())
		{
			ifile >> ranking_list[rank_len];
		}
		else
			break;
	}
	ifile.close();
}
//显示排行榜
void g2048::showranking()
{
	for (int i = 0; i < rank_len; i++)
	{
		printf("第%d名：%d\n", i + 1, ranking_list[i]);
	}
}
//写入排行榜
void g2048::writeranking()
{
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
	ofstream ofile("E:\\phb.txt");
	for (int i = 0; i < rank_len - 1; i++)
	{
		ofile << ranking_list[i] << endl;
	}
	ofile << ranking_list[rank_len - 1];
	ofile.close();
}
//向上滑动
void g2048::up()
{
	for (int i = 0; i < BoardSize; i++)
		for (int j = 0; j < BoardSize; j++)
			boardCopy[i][j] = board[i][j];

	//isEliminated = false;
	//isMoved = false;
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
void g2048::down()
{
	for (int i = 0; i < BoardSize; i++)
		for (int j = 0; j < BoardSize; j++)
			boardCopy[i][j] = board[i][j];

	//isEliminated = false;
	//isMoved = false;
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
void g2048::left()
{
	for (int i = 0; i < BoardSize; i++)
		for (int j = 0; j < BoardSize; j++)
			boardCopy[i][j] = board[i][j];

	//isEliminated = false;
	//isMoved = false;
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
void g2048::right()
{
	for (int i = 0; i < BoardSize; i++)
		for (int j = 0; j < BoardSize; j++)
			boardCopy[i][j] = board[i][j];

	//isEliminated = false;
	//isMoved = false;
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
void g2048::resetMark()
{
	isEliminated = false;
	isMoved = false;
}
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
bool g2048::isChanged()
{
	if (isEliminated || isMoved)
		return true;
	return false;
}
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
			cout << boardCopy[i][j] << ' ';
		cout << endl;
	}
	cout << "isEliminated:" << isEliminated << endl;
	cout << "eliminationLevel:" << eliminationLevel << endl;
	cout << "isMoved:" << isMoved << endl;
	cout << "zeroNum:" << zeroNum << endl;
	cout << "maxNum:" << maxNum << endl;
	cout << "score:" << score << endl;
}
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
				if (!rdmnum)
				{
					board[i][j] = 2;
					return true;
				}
				rdmnum--;
			}
		}
	}
}
bool g2048::input()
{
	string ss;
	cin >> ss;
	int len = ss.length();
	if (len == 1)
	{
		char choice = toupper(ss[0]);
		switch (choice)
		{
		case 'W':
			up();
			return true;
		case 'A':
			left();
			return true;
		case 'S':
			down();
			return true;
		case 'D':
			right();
			return true;
		case 'E':
			isPlaying = false;
			return false;
		case 'R':
			cout << 'r' << endl;
			return false;
		default:
			break;
		}
	}
	else if (len > 1)
	{

	}
	return false;
}
g2048::g2048()
{
	nextNum();
	nextNum();
}