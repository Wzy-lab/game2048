#include<iostream>
#include<iomanip>
#include<stdlib.h>
#include<windows.h>
#include<string>
#include<fstream>
using namespace std;
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

class g2048
{
private:
	const char* rankURL = "./phb.txt";
	const static int defaultBoardSize = 4;
	const static int defaultRankSize = 10;
	int BoardSize;
	int RankSize;
	//int board[BoardSize][BoardSize] = { 0 };
	//int boardCopy[BoardSize][BoardSize] = { 0 };
	int** board = NULL;
	int** boardCopy = NULL;
	int score = 0;//�÷�
	int maxNum = 2;
	int maxNumLen = 4;
	bool isEliminated = false;//��һ���Ƿ�������
	bool isMoved = false;//��һ���Ƿ����ƶ�
	int eliminationLevel = 0;//���������ȼ���Ӱ�������ɵ����ֺͷ���
	int zeroNum;//��λ����
	int* ranking_list = NULL;//���а�
	int rank_len = 0;//ʵ�����а񳤶�

	void initBoard();
	void initRank();
	//�ĸ��ƶ��ϲ�����
	void up();
	void down();
	void left();
	void right();
public:
	bool isPlaying = false;
	void init();
	g2048();
	g2048(int bdSize, int rkSize);
	bool nextNum();//������������һ��2��4����Ҫ�õ�eliminationLevel �� zeroNum,�޷�������һ�������ж�ʧ��
	//�����̷����ı�ſ��Բ����µ���
	void showBoard();//չʾ����
	bool input();
	bool isChanged();//�ж����̵ı仯�������仯����true�����򷵻�false
	//bool gameoverCheck();//��Ϸ������飬1��δ�����µķ��� 2���޷��ϲ�
	bool movable();
	void resetAll(int bdSize, int rkSize);//����size
	void resetMark();//ÿһ����ʾ���������ñ�־λ
	void cleanBoard();//������

	//������Ϸ���
	int getScore() { return score; }
	int get_maxNum() { return maxNum; }
	void readranking();
	void showranking();
	void writeranking();
};

class play
{
private:
	const static int maxBoardSize = 10;
	const static int minBoardSize = 3;
	const static int maxRankSize = 20;
	const static int minRankSize = 5;
	int BoardSize = 3;
	int RankSize = 10;
	g2048* game = NULL;
public:
	play() {
		initGame();
		gameMenu();
	}
	void initGame()
	{
		if (game)
			free(game);
		game = new g2048(BoardSize, RankSize);
	}
	void playGame()
	{
		if (game == NULL)
		{
			cout << "��Ϸ�쳣 ! " << endl;
			return;
		}

		game->cleanBoard();
		game->isPlaying = true;
		while (game->isPlaying)
		{
			system("cls");
			game->showBoard();
			game->resetMark();
			if (game->input() && game->isChanged())
			{
				game->nextNum();
				if (!game->movable())
				{
					game->isPlaying = false;
				}
			}
		}
		system("cls");
		game->showBoard();
		cout << "GAME OVER ! ! !" << endl;
		game->writeranking();
	}
	void showRank() {
		game->showranking();
	}
	void setting()
	{
		int choice;
		while (1)
		{
			system("cls");
			printf("���̴�С : %d\n���а��С : %d\n", BoardSize, RankSize);
			cout << "��ѡ����Ҫ���õĲ�����" << endl;
			cout << "1�����̴�С" << endl;
			cout << "2�����а��С" << endl;
			cout << "3�����沢�˻����˵�" << endl;
			cin >> choice;
			switch (choice)
			{
			case 1:
				printf("������һ�����ʵ����̴�С(%d~%d) : ", minBoardSize, maxBoardSize);
				cin >> choice;
				if (choice >= minBoardSize && choice <= maxBoardSize)
				{
					BoardSize = choice;
					cout << "�޸ĳɹ� ��" << endl;
				}
				else
				{
					cout << "�޸�ʧ�� ��" << endl;
				}
				break;
			case 2:
				printf("������һ�����ʵ����а��С(%d~%d) : ", minRankSize, maxRankSize);
				cin >> choice;
				if (choice >= minRankSize && choice <= maxRankSize)
				{
					RankSize = choice;
					cout << "�޸ĳɹ� ��" << endl;
				}
				else
				{
					cout << "�޸�ʧ�� ��" << endl;
				}
				break;
			case 3:
				game->resetAll(BoardSize, RankSize);
				return;
			default:
				cout << "�����ʽ���� ! " << endl;
				break;
			}
			system("pause");
		}
	}
	void gameMenu()
	{
		int choice;
		while (1)
		{
			system("cls");
			cout << "1����ʼ��Ϸ(b)" << endl;
			cout << "2�����а�(r)" << endl;
			cout << "3������(s)" << endl;
			cout << "4���˳���Ϸ(e)" << endl;
			cin >> choice;
			switch (choice)
			{
			case 1:
				playGame();
				system("pause");
				break;
			case 2:
				showRank();
				system("pause");
				break;
			case 3:
				setting();
				break;
			case 4:
				return;
				break;
			default:
				break;
			}
		}
	}
};

int main()
{
	loading(3);
	play hhh;
	return 0;
}
//��ȡ���а�
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
//��ʾ���а�
void g2048::showranking()
{
	system("cls");
	cout << "TOP " << RankSize << endl;
	for (int i = 0; i < rank_len; i++)
	{
		printf("��%d����%d\n", i + 1, ranking_list[i]);
	}
}
//д�����а�
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
//����
void g2048::up()
{
	for (int i = 0; i < BoardSize; i++)
		for (int j = 0; j < BoardSize; j++)
			boardCopy[i][j] = board[i][j];

	//isEliminated = false;
	//isMoved = false;
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
void g2048::down()
{
	for (int i = 0; i < BoardSize; i++)
		for (int j = 0; j < BoardSize; j++)
			boardCopy[i][j] = board[i][j];

	//isEliminated = false;
	//isMoved = false;
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
void g2048::left()
{
	for (int i = 0; i < BoardSize; i++)
		for (int j = 0; j < BoardSize; j++)
			boardCopy[i][j] = board[i][j];

	//isEliminated = false;
	//isMoved = false;
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
void g2048::right()
{
	for (int i = 0; i < BoardSize; i++)
		for (int j = 0; j < BoardSize; j++)
			boardCopy[i][j] = board[i][j];

	//isEliminated = false;
	//isMoved = false;
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
void g2048::resetAll(int bdSize, int rkSize)
{
	if (BoardSize != bdSize)
	{
		free(board); free(boardCopy);
		BoardSize = bdSize;
		initBoard();
	}
	if (RankSize != rkSize)
	{
		free(ranking_list);
		RankSize = rkSize;
		initRank();
		readranking();
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
	return (isEliminated || isMoved);
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
			cleanBoard();
			isPlaying = true;
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

	nextNum();
	nextNum();
}
void g2048::initBoard()
{
	board = new int* [BoardSize];
	boardCopy = new int* [BoardSize];
	for (int i = 0; i < BoardSize; i++)
	{
		board[i] = new int[BoardSize];
		boardCopy[i] = new int[BoardSize];
		for (int j = 0; j < BoardSize; j++)
		{
			board[i][j] = 0;
			boardCopy[i][j] = 0;
		}
	}
	zeroNum = BoardSize * BoardSize;
}
void g2048::initRank()
{
	ranking_list = new int[RankSize];
	for (int i = 0; i < RankSize; i++)
		ranking_list[i] = 0;
}
void g2048::init()
{
	initBoard();
	initRank();
}
g2048::g2048()
{
	BoardSize = defaultBoardSize;
	RankSize = defaultRankSize;
	init();
	readranking();
}
g2048::g2048(int bdSize, int rkSize)
{
	BoardSize = bdSize;
	RankSize = rkSize;
	init();
	readranking();
}