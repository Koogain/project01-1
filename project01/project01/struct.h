#pragma once
#include <stdio.h>
#include <string.h>

typedef struct
{
	char userName[20]; // 사용자 이름
	char userID[20];       // 사용자 아이디
	char userPW[25];   // 사용자 비밀번호
}Member;

typedef struct
{
	char userID[20]; // 사용자 아이디
	char subjectCode[20];  // 과목 코드
}Timetable;

typedef struct
{
	char subjectCode[20]; // 과목 코드
	char subjectName[20]; // 과목명
	char subCategory[20]; // 과목 카테고리
	char professorName[20]; // 교수님 성함
	char week[20];   // 요일
	char time[20];  // 시간
}SubjectList;

typedef struct
{
	char userID[20]; // 사용자 아이디
	char friendName[20]; // 친구 이름
	char friendID[20];       // 친구 아이디
	char friendPin[5];  // 즐겨찾기 여부
}Friends;