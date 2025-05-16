#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>

using namespace std;

int N, K;
int W[101];
int V[101];
int dp[101][100001];


void input()
{
	cin >> N >> K;

	for (int i = 1; i <= N; i++)
	{
		cin >> W[i] >> V[i];
	}
}
void solve()
{
	for (int i = 1; i <= N; i++)
	{
		for (int j = 1; j <= K; j++)
		{
			if (j < W[i])
				dp[i][j] = dp[i - 1][j];
			else
				dp[i][j] = max(dp[i - 1][j], V[i] + dp[i - 1][j - W[i]]);
		}
	}
}

int main() 
{
	input();
	solve();

	cout << dp[N][K] << "\n";
	return 0;
}