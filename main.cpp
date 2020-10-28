#include<iostream>
#include<iomanip>
#include<stdlib.h>
#include<windows.h>
#include<string>
#include<fstream>
#include <stack>
using namespace std;
//����Դ��������
class g2048
{
private:
	const string rankURL = "./phb.txt";
	const static int BoardSize = 4;
	const static int RankSize = 10;
	const static int maxNumLen = 4;

	int board[BoardSize][BoardSize] = { 0 };
	int boardCopy[BoardSize][BoardSize] = { 0 };
	int temp[4];
	int score = 0;//�÷�
	int maxNum = 2;
	int eliminationLevel = 0;//���������ȼ���Ӱ�������ɵ����ֺͷ���	
	int zeroNum = BoardSize * BoardSize;//��λ����

	int ranking_list[RankSize] = { 0 };//���а�
	int rank_len = 0;//ʵ�����а񳤶�

	bool isEliminated = false;//��һ���Ƿ�������
	bool isMoved = false;//��һ���Ƿ����ƶ�
	bool allowback = false;//�ܷ���˵ı�־����

	void up();
	void down();
	void left();
	void right();

public:
	bool isPlaying = false;
	g2048();
	bool nextNum();//������������һ��2��4����Ҫ�õ�eliminationLevel �� zeroNum,�޷�������һ�������ж�ʧ��
	//�����̷����ı�ſ��Բ����µ���
	void showBoard();//չʾ����
	bool input();
	bool isChanged();//�ж����̵ı仯�������仯����true�����򷵻�false
	bool setalback(bool newAlb) { return (allowback = newAlb); }
	//bool gameoverCheck();//��Ϸ������飬1��δ�����µķ��� 2���޷��ϲ�
	bool movable();
	void resetMark();//ÿһ����ʾ���������ñ�־λ
	void cleanBoard();//������
	void goback();
	void copy();
	int getScore() { return score; }
	int get_maxNum() { return maxNum; }
	void readranking();
	void showranking();
	void writeranking();
};
//��ҫ��
class play
{
private:
	g2048 game;//�����Ա
public:
	void playGame();
	void showRank() { game.showranking(); }
	void gameMenu();
};
//������--��ҫ��
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
	string choice;//���ѡ����
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
				GAME.showRank();//��ʾ���а�
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
//���캯��--��ȡ���а��ļ� ������
g2048::g2048()
{
	readranking();

}
//��ȡ���а�--������
void g2048::readranking()
{
	ifstream ifile(rankURL);
	if (!ifile)
	{
		ofstream ofile(rankURL);
		if (!ofile)
		{
			cout << "���а����ݶ�ȡʧ�ܣ�" << endl;
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
//��ʾ���а�--������
void g2048::showranking()
{
	system("cls");
	cout << "TOP " << RankSize << endl;
	for (int i = 0; i < rank_len; i++)
	{
		printf("��%d����%d\n", i + 1, ranking_list[i]);
	}
}
//д�����а�--������
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
				break;//�ҵ����ʵ�λ�ú��˳�ѭ��
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
//���ϻ���--����Դ
void g2048::up()
{
	copy();
	//����
	for (int j = 0; j < BoardSize; j++)
		for (int i = 0; i < BoardSize - 1; i++)
		{
			if (board[i][j] != 0)
			{
				int k;
				for (k = i + 1; k < BoardSize && board[k][j] == 0; k++)//�������е���һ��������
					continue;

				if (k >= BoardSize)//���û�о��������е���һ��
					break;
				else if (board[i][j] == board[k][j])//��������Ҷ�����Ⱦͺϲ�������һ�п�ʼ���
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
				else if (board[i][j] != board[k][j])//��������Ҷ��߲��ȣ���Ӻ��߿�ʼ���¼��
					i = k - 1;
			}
		}
	//�ƶ�
	for (int j = 0; j < BoardSize; j++)
	{
		int i = 0, k = 0;
		while (k < BoardSize && board[k][j]) k++;//�ҵ���һ����λ
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
//���»���--����Դ
void g2048::down()
{
	copy();
	//����
	for (int j = 0; j < BoardSize; j++)
		for (int i = BoardSize - 1; i > 0; i--)
		{
			if (board[i][j] != 0)
			{
				int k;
				for (k = i - 1; k >= 0 && board[k][j] == 0; k--)//�������е���һ��������
					continue;

				if (k < 0)//���û�о��������е���һ��
					break;
				else if (board[i][j] == board[k][j])//��������Ҷ�����Ⱦͺϲ�������һ�п�ʼ���
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
				else if (board[i][j] != board[k][j])//��������Ҷ��߲��ȣ���Ӻ��߿�ʼ���¼��
					i = k + 1;
			}
		}
	//�ƶ�
	for (int j = 0; j < BoardSize; j++)
	{
		int i = BoardSize - 1, k = BoardSize - 1;
		while (k >= 0 && board[k][j]) k--;//�ҵ���һ����λ
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
//���󻬶�--����Դ
void g2048::left()
{
	copy();
	//����
	for (int i = 0; i < BoardSize; i++)
		for (int j = 0; j < BoardSize - 1; j++)
		{
			if (board[i][j] != 0)
			{
				int k;
				for (k = j + 1; k < BoardSize && board[i][k] == 0; k++)//�������е���һ��������
					continue;

				if (k >= BoardSize)//���û�о��������е���һ��
					break;
				else if (board[i][j] == board[i][k])//��������Ҷ�����Ⱦͺϲ�������һ�п�ʼ���
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
				else if (board[i][j] != board[i][k])//��������Ҷ��߲��ȣ���Ӻ��߿�ʼ���¼��
					j = k - 1;
			}
		}
	//�ƶ�
	for (int i = 0; i < BoardSize; i++)
	{
		int j = 0, k = 0;
		while (k < BoardSize && board[i][k]) k++;//�ҵ���һ����λ
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
//���һ���--����Դ
void g2048::right()
{
	copy();
	//����
	for (int i = 0; i < BoardSize; i++)
		for (int j = BoardSize - 1; j > 0; j--)
		{
			if (board[i][j] != 0)
			{
				int k;
				for (k = j - 1; k >= 0 && board[i][k] == 0; k--)//�������е���һ��������
					continue;

				if (k < 0)//���û�о��������е���һ��
					break;
				else if (board[i][j] == board[i][k])//��������Ҷ�����Ⱦͺϲ�������һ�п�ʼ���
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
				else if (board[i][j] != board[i][k])//��������Ҷ��߲��ȣ���Ӻ��߿�ʼ���¼��
					j = k + 1;
			}
		}
	//�ƶ�
	for (int i = 0; i < BoardSize; i++)
	{
		int j = BoardSize - 1, k = BoardSize - 1;
		while (k >= 0 && board[i][k]) k--;//�ҵ���һ����λ
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
//��������
void g2048::copy()
{
	for (int i = 0; i < BoardSize; i++)
		for (int j = 0; j < BoardSize; j++)
			boardCopy[i][j] = board[i][j];
	temp[0] = score;
	temp[1] = maxNum;
	temp[2] = eliminationLevel;
	temp[3] = zeroNum;
}

//���˲���
void g2048::goback()
{
	if (!allowback)
		return;
	for (int i = 0; i < BoardSize; i++)
		for (int j = 0; j < BoardSize; j++)
			board[i][j] = boardCopy[i][j];
	score = temp[0];
	maxNum = temp[1];
	eliminationLevel = temp[2];
	zeroNum = temp[3];
}
//���ñ�ʶλ--����Դ
void g2048::resetMark()
{
	isEliminated = false;//������
	isMoved = false;//���ƶ�
}
//���̿��ƶ����--������
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
//���̸Ķ����--����Դ
bool g2048::isChanged()
{
	return (isEliminated || isMoved);
}
//��ʾ��ǰ����--����Դ
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
	cout << "allowback:" << allowback << endl;
}
//�����λ�����µķ���--������
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
		cout << "�޷������µķ���" << endl;
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
//��Ϸ��ָ��Ŀ���--������
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
		case 'B':
			goback();
			return false;
		case 'E':
			isPlaying = false;
			return false;
		case 'R':
			cleanBoard();
			isPlaying = true;
			return false;
		default:
			cout << "ERROR" << endl;
			system("pause");
			break;
		}
	}
	return false;
}
//��������--����Դ
void g2048::cleanBoard()
{
	for (int i = 0; i < BoardSize; i++)
	{
		for (int j = 0; j < BoardSize; j++)
		{
			board[i][j] = 0;
			boardCopy[i][j] = 0;
		}
	}
	score = 0;
	maxNum = 2;
	eliminationLevel = 0;
	resetMark();
	zeroNum = BoardSize * BoardSize;
	isPlaying = false;
	allowback = false;

	nextNum();
	nextNum();
	copy();
}
//��Ϸ�˵���ʾ--��ҫ��
void play::gameMenu()
{
	system("cls");
	cout << "+-------------+" << endl;
	cout << "| 1����ʼ��Ϸ |" << endl;
	cout << "| 2�����а�   |" << endl;
	cout << "| 3���˳���Ϸ |" << endl;
	cout << "+-------------+" << endl;
}
//����������Ϸ�Ĺ���--��ҫ��
void play::playGame()
{
	game.cleanBoard();
	game.isPlaying = true;
	while (game.isPlaying)
	{
		system("cls");
		game.showBoard();
		//game.copy();
		game.resetMark();//����������־λ
		if (game.input() && game.setalback(game.isChanged()))
		{
			game.nextNum();//����һ���µķ���
			if (!game.movable())
			{
				game.isPlaying = false;
			}
		}
	}
	system("cls");
	game.showBoard();
	cout << "GAME OVER ! ! !" << endl;
	game.writeranking();
}