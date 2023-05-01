//-----------------------------------------------------------------------------
// タスク管理アプリ/スタート画面
// 作　成　者：長谷川英一
// 作成年月日：2023/3/20
// 機　　　能：タスク管理アプリ/スタート画面
//-----------------------------------------------------------------------------
#pragma once
#include "TaskAppCommon.hpp"

class Start :public App::Scene
{
	//実質private

	//ボタン用矩形
	RoundRect buttonRect{
		Arg::center = Point{0,0},
		Point{500,100},10
	};
public:
	Start(const InitData& init);
	void update() override;
	void draw() const override;
};
