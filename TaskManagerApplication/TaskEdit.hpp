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
	TextEditState titleLS;
	TextEditState descriptionLS;
	TextEditState managerLS;

	//作成日
	Date createDate;

	Date deadlineDate;

	//締め切り日の設定
	TextEditState deadlineYearLS;
	TextEditState deadlineMonthLS;
	TextEditState deadlineDayLS;

	String text;

	size_t selectPriority;
	Array<String> priorityStr = {
		U"優先度【低】",
		U"優先度【中】",
		U"優先度【大】"
	};

	size_t selectSituation;
	Array<String> situationStr = {
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

