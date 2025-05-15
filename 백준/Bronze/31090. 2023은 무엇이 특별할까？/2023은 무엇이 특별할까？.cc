#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

using namespace std;

int N;

void input()
{
	cin >> N;
}
void solve()
{
	int Nplus = N + 1;
	if (Nplus % (N % 100) == 0)
		cout << "Good" << "\n";
	else
		cout << "Bye" << "\n";
}

int main() 
{
	int T;
	int test_case;

	cin >> T;

	for (test_case = 1; test_case <= T; test_case++)
	{
		input();
		solve();
	}
	return 0;
}