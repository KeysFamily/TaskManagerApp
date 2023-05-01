//-----------------------------------------------------------------------------
// タスク管理アプリ/タスクファイル作成
// 作　成　者：長谷川英一
// 作成年月日：2023/3/23
// 機　　　能：タスクファイルの新規作成
//-----------------------------------------------------------------------------

#pragma once

#include "TaskAppCommon.hpp"
class CreateNewTaskFile :public App::Scene
{
	TextEditState tes;
public:
	CreateNewTaskFile(const InitData& init);
	void update() override;
	void draw() const override;
};

