#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <cstdlib>
#include <ostream>
#include <time.h>
#include <chrono>

#define CHAR_WAY 255
#define CHAR_FIELD 35
#define CHAR_OBSTRACLES 64
#define CHAR_INTERMED_POINTS 175
#define OBSTRACLES 0
#define WAY -2
#define FIELD -1
#define INTERMED_POINTS -4

using namespace std;

typedef struct pair<int, int> PAIR;

void EnteringValidInput(int& number);
void EnterAreaSizes(int& AREA_HEIGHT, int& AREA_WIDTH);

void CreateArray(int AREA_HEIGHT, int AREA_WIDTH, vector<vector<int>> &mass);
void EnterPoints(PAIR& BEGIN, PAIR& END, int AREA_HEIGHT, int AREA_WIDTH);
void BuildObstacles(int AREA_HEIGHT, int AREA_WIDTH, PAIR BEGIN, PAIR END, vector<PAIR>& obstracles);
void InsertObstracles(vector<PAIR> obstracles, vector<vector<int>>& mass);
void AddIntermedPoints(const PAIR BEGIN, const PAIR END, int AREA_HEIGHT, int AREA_WIDTH, vector<PAIR>& intermediate, vector<vector<int>>& mass);

int  WaveProgration(PAIR BEGIN, PAIR END, int AREA_HEIGHT, int AREA_WIDTH, vector<vector<int>>& mass);
void FindHoming(int distanse, const PAIR BEGIN, const PAIR END, int AREA_HEIGHT, int AREA_WIDTH, vector<PAIR>& Way, vector<vector<int>>& mass);

bool HaveFreeUpperNeighborhood(PAIR PresentPos, int VALUE, const vector<vector<int>> &mass);
bool HaveFreeRightNeighborhood(PAIR PresentPos, int VALUE, int AREA_WIDTH, const vector<vector<int>>& mass);
bool HaveFreeLeftNeighborhood (PAIR PresentPos, int VALUE, const vector<vector<int>> &mass);
bool HaveFreeLowerNeighborhood(PAIR PresentPos, int VALUE, int AREA_HEIGHT, const vector<vector<int>> &mass);

void FindOptimalWayByIntermedPoints(PAIR BEGIN, int AREA_HEIGHT, int AREA_WIDTH, vector<PAIR>& Way, vector<PAIR>& intermediate, vector<vector<int>>& mass);
PAIR FindNearestIntermedPoint(PAIR& BEGIN, int AREA_HEIGHT, int AREA_WIDTH, vector<PAIR>& intermediate, vector<vector<int>>& mass);

void ClearArea(int AREA_HEIGHT, int AREA_WIDTH, vector<vector<int>>& mass);

void PrintArray(int AREA_HEIGHT, int AREA_WIDTH, vector<PAIR> intermediate, vector<vector<int>>& mass);
char Pr(int point);

int main()
{
	int AREA_WIDTH, AREA_HEIGHT;
	PAIR BEGIN, END;
	EnterAreaSizes(AREA_HEIGHT, AREA_WIDTH);
	vector<vector<int>> mass;
	vector<PAIR> obstracles;
	vector<PAIR> intermediate;
	
	CreateArray(AREA_HEIGHT, AREA_WIDTH, mass);
	EnterPoints(BEGIN, END, AREA_HEIGHT, AREA_WIDTH);

	BuildObstacles(AREA_HEIGHT, AREA_WIDTH, BEGIN, END, obstracles);
	InsertObstracles(obstracles, mass);

	AddIntermedPoints(BEGIN, END, AREA_HEIGHT, AREA_WIDTH, intermediate, mass);


	chrono::steady_clock::time_point begin1 = std::chrono::steady_clock::now();
	//int distanse = WaveProgration(BEGIN, END, AREA_HEIGHT, AREA_WIDTH, mass);
	chrono::steady_clock::time_point end1 = std::chrono::steady_clock::now(); 

	vector<PAIR> Way;
	chrono::steady_clock::time_point begin2 = std::chrono::steady_clock::now();
	//FindHoming(distanse, BEGIN, END, AREA_HEIGHT, AREA_WIDTH, Way, mass);
	FindOptimalWayByIntermedPoints(BEGIN, AREA_HEIGHT, AREA_WIDTH, Way, intermediate, mass);
	chrono::steady_clock::time_point end2 = std::chrono::steady_clock::now();

	PrintArray(AREA_HEIGHT, AREA_WIDTH, intermediate, mass);
	cout << "\nWave progration time: " << chrono::duration_cast<std::chrono::nanoseconds> (end1 - begin1).count() / 1000000000. << " seconds.\n";
	cout << "Finding way time: " << chrono::duration_cast<std::chrono::nanoseconds> (end2 - begin2).count() / 1000000000. << " seconds.\n";
	
	system("pause");
	return 0;
}

void CreateArray(int AREA_HEIGHT, int AREA_WIDTH, vector<vector<int>>& mass)
{
	mass.reserve(AREA_HEIGHT);
	mass.resize(AREA_HEIGHT);
	for (int i = 0; i < AREA_HEIGHT; i++)
	{
		mass[i].reserve(AREA_WIDTH);
		mass[i].resize(AREA_WIDTH);
		for (int j = 0; j < AREA_WIDTH; j++)
		{
			mass[i][j] = FIELD;
		}
	}
}

void EnteringValidInput(int& number)
{
	cin >> number;
	while (cin.fail())
	{
		cout << "Entered number must be integer type! Try one more time: ";
		cin.clear();
		cin.ignore(20, '.');
		cin >> number;
	}
}

void EnterAreaSizes(int& AREA_HEIGHT, int& AREA_WIDTH)
{
	cout << "Enter height of area: ";
	EnteringValidInput(AREA_HEIGHT);
	cout << "Enter width  of area: ";
	cin.clear();
	EnteringValidInput(AREA_WIDTH);
}

void EnterPoints(PAIR& BEGIN, PAIR& END, int AREA_HEIGHT, int AREA_WIDTH)
{
	cin.clear();
	bool correct_input = false;
	while (!correct_input) {
		cout << "Enter coordinates of begin point: ";
		cin >> BEGIN.first >> BEGIN.second;
		BEGIN.first--; BEGIN.second--;
		correct_input = true;
		if ((BEGIN.first < 0 || BEGIN.first >= AREA_HEIGHT) || (BEGIN.second < 0 || BEGIN.second >= AREA_WIDTH))
		{
			cout << "Try enter one more time!\n";
			correct_input = false;
		}
	}
	while (correct_input) {
		cout << "Enter coordinates of final point: ";
		cin >> END.first >> END.second;
		END.first--; END.second--;
		correct_input = false;
		if ((END.first < 0 || END.first >= AREA_HEIGHT) || (END.second < 0 || END.second >= AREA_WIDTH))
		{
			cout << "Try enter one more time!\n";
			correct_input = true;
		}
	}
}

void BuildObstacles(int AREA_HEIGHT, int AREA_WIDTH, PAIR BEGIN, PAIR END, vector<PAIR>& obstracles)
{
	cout << "Enter obstracles coordinates (0 for finish entering obstracles):\n";
	int i(1), line, column;
	while (true)
	{
		cout << i << ") ";
		cin >> line;
		if (line == 0) break;
		cin >> column;
		line--; column--;
		if ((line == BEGIN.first && column == BEGIN.second) || (line == END.first && column == END.second)) {
			cout << "Can't make obstracle there!\n";
			continue;
		}
		if (line < AREA_HEIGHT && line >= 0)
		{
			if (column < AREA_WIDTH && column >= 0) {
				obstracles.push_back({ line, column });
				i++;
			}
			else cout << "I can't put this obstacle: point doesn't lie within the specified area. Try reenter second coordinate)\n";
		}
		else cout << "I can't put this obstacle: point doesn't lie within the specified area. Try reenter first coordinate)\n";
	}
}

void InsertObstracles(vector<PAIR> obstracles, vector<vector<int>>& mass)
{
	for (int i(0); i < obstracles.size(); i++)	
	{
		mass[obstracles[i].first][obstracles[i].second] = OBSTRACLES;
	}
}

int WaveProgration(PAIR BEGIN, PAIR END,int AREA_HEIGHT, int AREA_WIDTH, vector<vector<int>>& mass)
{
	vector<vector<PAIR>> Set;
	int j(0);
	Set.resize(1);
	Set[0].push_back(BEGIN);
	mass[BEGIN.first][BEGIN.second] = j++;
	while (mass[END.first][END.second] == FIELD) {
		Set.reserve(2);
		Set.resize(2);
		for (int i(0); i < Set[0].size(); i++)
		{
			if (HaveFreeUpperNeighborhood(Set[0][i], FIELD, mass))
			{
				mass[Set[0][i].first - 1][Set[0][i].second] = j;
				Set[1].push_back({ Set[0][i].first - 1, Set[0][i].second });
			}
			if (HaveFreeRightNeighborhood(Set[0][i], FIELD, AREA_WIDTH, mass)) {
				mass[Set[0][i].first][Set[0][i].second + 1] = j;
				Set[1].push_back({ Set[0][i].first, Set[0][i].second + 1 });
			}
			if (HaveFreeLeftNeighborhood(Set[0][i], FIELD, mass)) {
				mass[Set[0][i].first][Set[0][i].second - 1] = j;
				Set[1].push_back({ Set[0][i].first, Set[0][i].second - 1 });
			}
			if (HaveFreeLowerNeighborhood(Set[0][i], FIELD, AREA_HEIGHT, mass)) {
				mass[Set[0][i].first + 1][Set[0][i].second] = j;
				Set[1].push_back({ Set[0][i].first + 1, Set[0][i].second });
			}
		}
		j++;
		Set.erase(Set.begin());
	}
	return j--;
}

bool HaveFreeUpperNeighborhood(PAIR PresentPos, int VALUE, const vector<vector<int>> &mass)
{
	if (PresentPos.first != 0 && mass[PresentPos.first - 1][PresentPos.second] == VALUE && mass[PresentPos.first - 1][PresentPos.second] != OBSTRACLES && mass[PresentPos.first - 1][PresentPos.second] != WAY) 
		return true;
	return false;
}
bool HaveFreeRightNeighborhood(PAIR PresentPos, int VALUE, int AREA_WIDTH, const vector<vector<int>> &mass)
{
	if (PresentPos.second != AREA_WIDTH - 1 && mass[PresentPos.first][PresentPos.second + 1] == VALUE && mass[PresentPos.first][PresentPos.second + 1] != OBSTRACLES && mass[PresentPos.first][PresentPos.second + 1] != WAY)
		return true;
	return false;
}
bool HaveFreeLeftNeighborhood(PAIR PresentPos, int VALUE, const vector<vector<int>> &mass)
{
	if (PresentPos.second != 0 && mass[PresentPos.first][PresentPos.second - 1] == VALUE && mass[PresentPos.first][PresentPos.second - 1] != OBSTRACLES && mass[PresentPos.first][PresentPos.second - 1] != WAY)
		return true;
	return false;
}
bool HaveFreeLowerNeighborhood(PAIR PresentPos, int VALUE, int AREA_HEIGHT, const vector<vector<int>> &mass)
{
	if (PresentPos.first != AREA_HEIGHT - 1 && mass[PresentPos.first + 1][PresentPos.second] == VALUE && mass[PresentPos.first + 1][PresentPos.second] != OBSTRACLES && mass[PresentPos.first + 1][PresentPos.second] != WAY) 
		return true;
	return false;
}

void FindHoming(int distanse, const PAIR BEGIN, const PAIR END, int AREA_HEIGHT, int AREA_WIDTH, vector<PAIR>& Way, vector<vector<int>>& mass)
{
	distanse -= 2;
	vector<PAIR> ForRand;
	Way.push_back(END);
	srand(time(NULL));
	for (distanse; distanse > 0; distanse--) {
		if (HaveFreeUpperNeighborhood(Way[0], distanse, mass)) {
			PAIR UPPER = { Way[0].first - 1, Way[0].second };
			ForRand.push_back(UPPER);
		}
		if (HaveFreeRightNeighborhood(Way[0], distanse, AREA_WIDTH, mass)) {
			PAIR RIGHT = { Way[0].first, Way[0].second + 1};
			ForRand.push_back(RIGHT);
		}
		if (HaveFreeLeftNeighborhood(Way[0], distanse, mass)) {
			PAIR LEFT = { Way[0].first, Way[0].second - 1};
			ForRand.push_back(LEFT);
		}
		if (HaveFreeLowerNeighborhood(Way[0], distanse, AREA_HEIGHT,  mass)) {
			PAIR LOWER = { Way[0].first + 1, Way[0].second };
			ForRand.push_back(LOWER);
		}
		int WINNER = rand() % ForRand.size();
		Way.insert(Way.begin(), ForRand[WINNER]);
		ForRand.erase(ForRand.begin(),ForRand.end());
	}
	Way.insert(Way.begin(), BEGIN);
	for (int i = 0; i < Way.size(); i++)
	{
		mass[Way[i].first][Way[i].second] = WAY;
	}
	Way.erase(Way.begin(),Way.end());
}

void AddIntermedPoints(const PAIR BEGIN, const PAIR END, int AREA_HEIGHT, int AREA_WIDTH, vector<PAIR>& intermediate, vector<vector<int>>& mass)
{
	cout << "Enter intermediate points coordinates (0 for finish entering points):\n";
	int i(1), line, column;
	while (true)
	{
		cout << i << ") ";
		cin >> line;
		if (line == 0) break;
		cin >> column;
		line--; column--;
		if ((line == BEGIN.first && column == BEGIN.second) || (line == END.first && column == END.second) || mass[line][column] == OBSTRACLES) {
			cout << "Can't make point there!\n";
			continue;
		}
		if (line < AREA_HEIGHT && line >= 0)
		{
			if (column < AREA_WIDTH && column >= 0) {
				intermediate.push_back({ line, column });
				i++;
			}
			else cout << "I can't make intermediate point here: it doesn't lie within the specified area. Try reenter second coordinate)\n";
		}
		else cout << "I can't make intermediate point here: it doesn't lie within the specified area. Try reenter first coordinate)\n";
	}
	intermediate.push_back(END);
}

void FindOptimalWayByIntermedPoints(PAIR BEGIN, int AREA_HEIGHT, int AREA_WIDTH, vector<PAIR>& Way, vector<PAIR>& intermediate, vector<vector<int>>& mass)
{
	int intersize = intermediate.size();
	PAIR Begin = BEGIN;
	PAIR End;
	for (int i(0); i < intersize; i++)
	{ 
		End = FindNearestIntermedPoint(BEGIN, AREA_HEIGHT, AREA_WIDTH, intermediate, mass);
		int distance = WaveProgration(Begin, End, AREA_HEIGHT, AREA_WIDTH, mass);
		FindHoming(distance, Begin, End, AREA_HEIGHT, AREA_WIDTH, Way, mass);
	}
}

PAIR FindNearestIntermedPoint(PAIR& BEGIN,int AREA_HEIGHT, int AREA_WIDTH, vector<PAIR>& intermediate,vector<vector<int>>& mass)
{
	vector<int> all;
	int intersize = intermediate.size();
	for (int i(0); i < intersize; i++ )
	{
		all.push_back(WaveProgration(BEGIN, intermediate[i], AREA_HEIGHT, AREA_WIDTH, mass));
		ClearArea(AREA_HEIGHT, AREA_WIDTH, mass);
	}
	int winner = 0;
	for (int i(1); i < intersize; i++)
	{
		if (all[i] < all[i - 1]) {
			BEGIN = intermediate[i];
			winner = i;
		}
	}
	intermediate.erase(intermediate.begin() + winner);
	return BEGIN;
}

void ClearArea(int AREA_HEIGHT, int AREA_WIDTH, vector<vector<int>>& mass)
{
	for (int i(0); i < AREA_HEIGHT; i++)
	{
		for (int j(0); j < AREA_WIDTH; j++)
		{
			if (mass[i][j] != OBSTRACLES && mass[i][j] != WAY) mass[i][j] = FIELD;
		}
	}
}

char Pr(int point)
{

	if (point == OBSTRACLES) return CHAR_OBSTRACLES;
	if (point == WAY) return CHAR_WAY;
	if (point == INTERMED_POINTS) return CHAR_INTERMED_POINTS;
	return CHAR_FIELD;
}

void PrintArray(int AREA_HEIGHT, int AREA_WIDTH, vector<PAIR> intermediate, vector<vector<int>>& mass)
{
	for(int i(0); i < intermediate.size(); i++)
	{
		mass[intermediate[i].first][intermediate[i].second] = INTERMED_POINTS;
	}
	for (int i = 0; i < AREA_HEIGHT; i++)
	{
		for (int j = 0; j < AREA_WIDTH; j++)
		{
			cout.setf(ios::right);
			cout.width(2);
			if (j == AREA_WIDTH - 1)
				cout << Pr(mass[i][j]) << endl;
			else cout << Pr(mass[i][j]);
		}
	}
}


























