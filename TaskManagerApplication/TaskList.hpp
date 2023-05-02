//-----------------------------------------------------------------------------
// タスク管理アプリ/タスク一覧
// 作　成　者：長谷川勇一朗
// 作成年月日：2023/3/22
// 機　　　能：タスクの一覧表示
//-----------------------------------------------------------------------------
#include "TaskAppCommon.hpp"

#pragma once
class TaskList :public App::Scene
{
	//int32 idMax;//IDの最大値
	Array<TaskObject::SP> add;

	Optional<int> selectNum;

public:
	TaskList(const InitData& init);
	~TaskList();
	void update() override;
	void draw() const override;
	

	//以下はこのシーンで処理をする


	//data.taskに処理を行う
	bool LoadData();	//ファイルの読み込みを行う

	bool SaveDate();	//ファイルの保存を行う


	//タスクオブジェクトを生成する
	bool TaskCreate(TaskManagement task_);

	//選択したオブジェクトを削除する
	bool TaskRemove();

	//タスクオブジェクトの検索
	TaskObject::SP TaskSearch(int32 id_);
};
