#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
#include <algorithm>

using namespace std;

struct Edge
{
	int a;
	int b;
	int cost;
};

bool compare(Edge a, Edge b)
{
	return a.cost < b.cost;
};

struct Point
{
	int y;
	int x;
};

int N, M; // 메인보드 가로, 세로 
int MAP[11][11];

// 방향 배열
int dy[4] = { -1, 1, 0, 0 };
int dx[4] = { 0, 0, -1, 1 };
int visited[11][11];

vector<Edge> v;	// kruskal 위한 노드-엣지 저장할 벡터
int compCnt;	// 부품 개수
int parent[7];
int answer;

void init()
{
	memset(MAP, 0, sizeof(MAP));
	memset(visited, 0, sizeof(visited));

	v.clear();

	for (int i = 0; i < 7; i++)	// parent 초기화
	{
		parent[i] = i;
	}
	compCnt = 0;
	answer = 0;
}

void input()
{
	cin >> N >> M;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			cin >> MAP[i][j];
			MAP[i][j] = -MAP[i][j];	// 1이면 -1 들어가도록 
		}
	}
}

void bfs(Point st, int num)	// 각 부품을 번호별로 셋팅 
{
	queue<Point> q;
	q.push(st);
	visited[st.y][st.x] = 1;
	MAP[st.y][st.x] = num;	// 번호로 셋팅

	while (!q.empty())
	{
		Point now = q.front();
		q.pop();

		for (int i = 0; i < 4; i++)
		{
			int ny = now.y + dy[i];
			int nx = now.x + dx[i];

			if (ny < 0 || nx < 0 || ny >= N || nx >= M)	continue;

			if (visited[ny][nx]) continue;
			
			visited[ny][nx] = 1;

			if (MAP[ny][nx] == 0) // 0인 부분 만나면 가장자리
				continue;	// 0인 부분은 안 감 

			MAP[ny][nx] = num;
			q.push({ ny, nx});
		}
	}

}

void findAnother(Point st, int dir, int myNum)	// 다른 부품까지의 거리 찾기
{
	int ny = st.y;
	int nx = st.x;
	int len = 0;

	while (1)
	{
		ny += dy[dir];
		nx += dx[dir];

		if (ny < 0 || nx < 0 || ny >= N || nx >= M)		
			return;
		if (MAP[ny][nx] == myNum)	// 같은 부품 만나면 pass
			return; 
		if (MAP[ny][nx] == 0) 
			len++;	// 케이블 길이 증가

		if (MAP[ny][nx] > 0 && len >= 2)	// 다른 부품 만나면 && 케이블 길이 2 이상이면 
		{
			v.push_back({ myNum, MAP[ny][nx], len });	// 엣지 추가해줌 
			return;
		}
		else if (MAP[ny][nx] > 0 && len < 2) return;	// 다른 부품 만났는데 길이 1이면 그냥 종료 
	}
}

int find(int tar)	// 루트 찾기
{
	if (parent[tar] == tar) return tar;

	int ret = find(parent[tar]);
	parent[tar] = ret;

	return ret;
}

void setUnion(int a, int b)
{
	int t1 = find(a);
	int t2 = find(b);

	if (t1 == t2) return;

	parent[t2] = t1;
}

int kruskal()
{
	int totalCost = 0;	// 총 가중치
	int edCnt = 0;	// 엣지 개수 = 노드 개수 - 1 면 종료

	for (Edge now : v)
	{
		int a = now.a;
		int b = now.b;
		int cost = now.cost;
		if (find(a) == find(b))	// 사이클 나올 거 같으면 pass
			continue;

		setUnion(a, b);
		totalCost += cost;
		edCnt++;

		if (edCnt == compCnt - 1)
			break;
	}

	return totalCost;
}

void solve()
{
	// 부품마다 번호 매겨주기 
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			if (MAP[i][j] == -1)
			{
				bfs({ i, j }, compCnt + 1);
				compCnt++;
			}
		}
	}
	memset(visited, 0, sizeof(visited));	// visited 초기화 

	// 다른 부품까지의 거리 찾기
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			if (MAP[i][j] > 0)	// 부품 찾으면 
			{
				for (int k = 0; k < 4; k++)	// 4 방향으로 꺾지 말고 일직선으로만 가야하므로 방향 하나씩 넣어주면서 쭉 찾음
				{
					findAnother({ i, j }, k, MAP[i][j]);
				}
			}
		}
	}

	sort(v.begin(), v.end(), compare);	// cost 오름차순으로 정렬

	answer = kruskal();

	int check = find(1);
	for (int i = 2; i <= compCnt; i++)
	{
		if (check != find(i))
		{
			answer = -1;
			break;
		}
	}

}

int main()
{
	// freopen("sample_input.txt", "r", stdin);

	init();
	input();
	solve();

	cout << answer << "\n";

	return 0;
}