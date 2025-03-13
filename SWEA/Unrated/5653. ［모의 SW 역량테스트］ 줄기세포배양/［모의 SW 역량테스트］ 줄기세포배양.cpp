#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <queue>
#include <vector>
#include <cstring>

using namespace std;

struct Edge
{
	int y;
	int x;
	int vitality; // 생명력
	int livetime; // 살아있던 시간 
	int activate; // 활성화 상태 (0: 비활성 상태, 1: 활성 상태, -1: 죽은 상태)
};

struct cmp
{
	bool operator() (Edge a, Edge b)
	{
		return a.vitality < b.vitality;
	}
};
///////////////////////////////////////////////////
int N, M; // 초기 상태 세포 세로, 가로 크기
int K; // 배양 시간
int MAP[350][350]; // 초기 세포 상태 정보

int timeCnt; // 시간 카운트
int liveCells; // 살이있는 줄기 세포 개수 (= 정답)

// 방향 배열 - 상하좌우
int dy[4] = { -1, 1, 0, 0 };
int dx[4] = { 0, 0, -1, 1 };

vector<Edge> cells; // 번식하는 세포 정보 담을 벡터 
///////////////////////////////////////////////////

void init()
{
	memset(MAP, 0, sizeof(MAP));
	timeCnt = 0;
	liveCells = 0;
	cells.clear();
}

void input()
{
	cin >> N >> M >> K;

	for (int i = 150; i < N + 150; i++) // 중간 지점부터 시작
	{
		for (int j = 150; j < M + 150; j++)
		{
			cin >> MAP[i][j];
			if (MAP[i][j] != 0) // 세포가 없는 곳이 아니라면 
				cells.push_back({ i, j, MAP[i][j], 0, 0 }); // 세포 상태 벡터에도 넣어주기 (활성화 상태는 0으로)
		}
	}
}

void bfs()
{
	priority_queue<Edge, vector<Edge>, cmp> pq; // 활성 상태인 세포 넣을 때 사용, 생명력 수치 비교해야하므로 pq 사용

	while (1)
	{
		for (int i = 0; i < cells.size(); i++)  // 세포 모두 검사
		{
			// 현재 세포의 생명력이 살아있던 시간과 같으면 활성 상태됨, 큐에 넣어줌 (번식)
			if (cells[i].vitality == cells[i].livetime)
			{
				cells[i].activate = 1;
				pq.push(cells[i]);
			}
			else if (cells[i].vitality * 2 <= cells[i].livetime) // 작으면 죽은 상태됨 
				cells[i].activate = -1;

			cells[i].livetime++; // 살아있던 시간 늘려줌

		}

		if (timeCnt == K) // 배양시간 되면 종료
			break;

		while (!pq.empty()) // 활성화된 세포들 돌면서 4방향 탐색 
		{
			Edge now = pq.top();
			pq.pop();

			for (int i = 0; i < 4; i++) // 4방향 검사
			{
				int ny = now.y + dy[i];
				int nx = now.x + dx[i];

				if (MAP[ny][nx] > 0) // 세포가 이미 있는 곳이면 pass
					continue;

				cells.push_back({ ny, nx, now.vitality, 0, 0 }); // 세포 벡터에 다음 좌표와 현재 생명력을 옮겨줌
				MAP[ny][nx] = now.vitality; // 맵에도 생명력 값 업데이트 해줌
			}
		}

		timeCnt++; // 시간 증가 
	}
}

void solve()
{
	bfs();

	for (int i = 0; i < cells.size(); i++) // 현재 모든 세포 돌면서
	{
		if (cells[i].activate != -1) // 죽은 상태가 아니라면 개수 더해줌 
			liveCells++;
	}
}

int main()
{
	int T;
	int test_case;

	//freopen("sample_input.txt", "r", stdin);
	cin >> T;

	for (int test_case = 1; test_case <= T; test_case++)
	{
		init();
		input();
		solve();

		cout << "#" << test_case << " " << liveCells << "\n";
	}
	return 0;
}