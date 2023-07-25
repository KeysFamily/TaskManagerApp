//-----------------------------------------------------------------------------
// タスク管理アプリ/ファイル読込処理
// 作　成　者：長谷川勇一朗
// 作成年月日：2023/3/21
// 機　　　能：タスクファイルの読み込み
//-----------------------------------------------------------------------------

#pragma once
#include "TaskAppCommon.hpp"

class LoadingTaskFile :public App::Scene
{
	Array<FilePath> dataPaths;//ファイル一覧を取得

	bool isNoFile;			//ファイルがなければtrue

	ListBoxState ls;		//ファイル一覧
public:
	LoadingTaskFile(const InitData& init);
	void update() override;
	void draw() const override;
};

