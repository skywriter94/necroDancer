#pragma once
#include "stdafx.h"

#define SCREENMOVESPEED 10

// 무브 팔레트 
struct MovePalette
{
	RECT terrainTile;
	RECT wallTile;
	RECT trapTile;
	RECT armorTile;
	RECT weaponTile;
	RECT stuffTile;
	RECT characterTile;
	POINT pos_Start;	// 마우스로 클릭한 지점 좌표를 담아둔다.
	POINT pos_End;		// 마우스의 현재 위치 좌표를 담아둔다.
	bool isClick;		// 마우스를 눌렀는지 안 눌렀는지
	bool choice;		// 그리길 원하는 이미지를 클릭 했다.
};


struct mousePointRect
{
	RECT mouseRect;		// 마우스 포인터가 가리키고 있는 타일 렉트 정보를 담는다.
	int frameX, frameY;	// 이미지 프레임 x, y
};


//맵툴 비트에 맞춰서 타일이 바뀌는것
//체력 포션 아이템 두기
//이미지 프래임 돌리게 하기
//