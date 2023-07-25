//-----------------------------------------------------------------------------
// タスク管理アプリ/タスク編集
// 作　成　者：長谷川勇一朗
// 作成年月日：2023/3/24
// 機　　　能：タスクの新規作成/概要表示/編集をすべて行えます
//-----------------------------------------------------------------------------

#pragma once

#include "TaskAppCommon.hpp"

class TaskEdit : public App::Scene
{
	//テキストボックスの情報
	TextEditState titleLS;			//タスク名入力欄
	TextEditState descriptionLS;	//タスク説明入力欄
	TextEditState managerLS;		//管理者入力欄
	TextEditState deadlineYearLS;	//締切年入力欄
	TextEditState deadlineMonthLS;	//締切月入力欄
	TextEditState deadlineDayLS;	//締切日入力欄

	//日付データ
	Date createDate;				//作成日のデータ型
	Date deadlineDate;				//締切日のデータ型

	//String text;
	//優先度[ラジオボタン]
	size_t selectPriority;			//何を選択しているか
	Array<String> priorityStr = {	//項目一覧
		U"優先度【低】",
		U"優先度【中】",
		U"優先度【大】"
	};

	//状態[ラジオボタン]
	size_t selectSituation;			//何を選択しているか
	Array<String> situationStr = {	//項目一覧
		U"未開始",
		U"実行中",
		U"完了",
		U"延期",
		U"中断"
	};
public:
	TaskEdit(const InitData& init);

	void update() override;
	void draw() const override;
};

