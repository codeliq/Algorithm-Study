#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <queue>
#include <vector>
#include <cstring>

using namespace std;

struct Customer
{
	int visitTime; // 방문 시간
	int receptionNum; // 접수 데스크 번호
	int repairNum; // 정비 데스크 번호 
};

struct Desk
{
	int processTime; // 담당자의 처리 시간
	int workingTime; // 담당자의 일한 시간 
	int customerNum; // 현재 처리 중인 고객 번호
};

int N; // 접수 창구 개수
int M; // 정비 창구 개수
int K; // 차량 정비소 방문 고객 수
int A, B; // 지갑 두고 간 고객의 접수 창구번호, 정비 창구번호


Customer customer[1001];  // 고객 최대 1000명
Desk reception[21]; // 고장 접수 창구
Desk repair[21]; // 차량 정비 창구

int NumReceptionDesk; // 사용중인 접수 창구 개수
int NumRepairDesk; // 사용중인 차량 정비 창구 
int numSum;  // 정답 - 고객 번호의 합

void init()
{
	NumReceptionDesk = 0;
	NumRepairDesk = 0;

	numSum = 0;
}

void input()
{
	cin >> N;
	cin >> M;
	cin >> K;
	cin >> A;
	cin >> B;

	for (int i = 1; i <= N; i++) // 접수 창구 i 고장 접수 처리 시간 저장
	{
		cin >> reception[i].processTime;
		reception[i].customerNum = -1; // 손님 번호 -1로 초기화 
	}

	for (int i = 1; i <= M; i++) // 정비 창구 j 차량 정비 처리 시간
	{
		cin >> repair[i].processTime;
		repair[i].customerNum = -1; // 손님 번호 -1로 초기화 
	}

	for (int i = 1; i <= K; i++) // 고객 정보 저장
	{
		cin >> customer[i].visitTime;
	}
}

void solve()
{
	queue<int> q1; // 접수 창구 대기 큐
	queue<int> q2; // 정비 창구 대기 큐 

	int successCnt = 0;  // 처리 끝난 손님 카운트
	int timeCnt = 0; // 시간 카운트

	while (1)
	{	//-------------------------------1
		for (int i = 1; i <= K; i++)  // 도착한 순서대로 시간에 맞춰 접수 창구 큐에 넣기
		{
			if (customer[i].visitTime == timeCnt)  // 현재 시간이 손님의 방문 시간과 같다면 
			{
				q1.push(i); // 고객 번호 넣기	
			}
		}
		//-------------------------------2
		for (int i = 1; i <= N; i++)
		{
			if (reception[i].customerNum == -1)  // 창구 남아있으면
			{
				if (q1.empty()) break; // q1에서 손님 다 나가면 종료
				int num = q1.front();
				reception[i].customerNum = num;
				customer[num].receptionNum = i; // 손님이 사용하고 있는 접수 창구 번호 넣어주기
				q1.pop();
			}
		}
		//-------------------------------3
		for (int i = 1; i <= N; i++) // 접수 창구마다 일한 시간 증가
		{
			if(reception[i].customerNum > 0) // 손님 들어 있으면 손님 번호는 0보다 클 것
				reception[i].workingTime++;
		}
		//-------------------------------4
		for (int i = 1; i <= N; i++)
		{
			if (reception[i].workingTime == reception[i].processTime && reception[i].customerNum > 0) // 일한 시간과 처리 시간이 같다면 한 고객 다 처리한 것 (그리고 고객이 있는 경우)
			{
				//int num = q1.front(); // 맨 앞에 있는 고객 번호
				q2.push(reception[i].customerNum); // 정비 창구 대기열에 넣어주고
				reception[i].workingTime = 0; // 일한 시간 다시 초기화 
				reception[i].customerNum = -1; // 손님 번호도 초기화
			}
		}

		for (int i = 1; i <= M; i++)
		{
			if (repair[i].customerNum == -1) // 정비 창구 남아있으면
			{
				if (q2.empty()) break; // q1에서 손님 다 나가면 종료
				int num = q2.front();
				repair[i].customerNum = num;
				customer[num].repairNum = i; // 손님이 사용하고 있는 접수 창구 번호 넣어주기
				q2.pop();
			}
		}

		for (int i = 1; i <= M; i++) // 정비 창구마다 일한 시간 증가
		{
			if (repair[i].customerNum > 0) // 손님 들어 있으면 손님 번호는 0보다 클 것
				repair[i].workingTime++;
		}

		for (int i = 1; i <= M; i++)
		{
			if (repair[i].workingTime == repair[i].processTime && repair[i].customerNum > 0) // 일한 시간과 처리 시간이 같다면 한 고객 다 처리한 것 (그리고 고객이 있는 경우)
			{
				repair[i].workingTime = 0; // 일한 시간 다시 초기화 
				repair[i].customerNum = -1; // 손님 번호도 초기화
				successCnt++; // 처리 끝난 손님 카운트 증가
			}
		}
		
		if (successCnt == K) // 처리 끝낸 손님이 총 손님 수와 같다면 종료
			break;

		timeCnt++; // 시간 증가

	}

	for (int i = 1; i <= K; i++)
	{
		if (customer[i].receptionNum == A && customer[i].repairNum == B)
			numSum += i;
	}

	if (numSum == 0)
		numSum = -1;

}

int main()
{
	int T;
	int test_case;

	//freopen("sample_input.txt", "r", stdin);

	cin >> T;

	for (test_case = 1; test_case <= T; test_case++)
	{
		init();
		input();
		solve();

		cout << "#" << test_case << " " << numSum << "\n";
	}

	return 0;
}