//-----------------------------------------------------------------------------
// プログラム名：TaskManagement
// 作　成　者：長谷川英一
// 作成年月日：2023/3/11作成開始
// 機　　　能：タスク管理クラスの定義
// 注　　　意：ここで使用するメンバはすべてSiv3D用になっている。
// 　　　　　　C++などで使用する場合置き換える必要がある。
// 　　　　　　String -> std::string などが該当する。
// 　　　　　　学習もかねて作成しているため、コメントが多量になっている。
//-----------------------------------------------------------------------------

#include "TaskManagement.h"

TaskManagement::TaskManagement()
	: title(U"")
	, description(U"")
	, priority(Priority::Medium)
	, situation(Situation::NotStarted)
	, createDate(2000, 1, 1)
	, deadline(2000, 1, 1)
	, manager(U"")
{

}

TaskManagement::TaskManagement(
	String title_,
	String description_,
	Priority priority_,
	Situation situation_,
	Date createDate_,
	Date deadline_,
	String manager_)
	: title(title_)
	, description(description_)
	, priority(priority_)
	, situation(situation_)
	, createDate(createDate_)
	, deadline(deadline_)
	, manager(manager_)
{
    
}


//-----------------------------------------------------------------------------
// メンバデータを取得する
//-----------------------------------------------------------------------------
String TaskManagement::GetTaskTitle() const { return title; }
String TaskManagement::GetTaskDescription() const { return description; }
Priority TaskManagement::GetTaskPriority() const { return priority; }
int32 TaskManagement::GetTaskPriorityNum() const { return int32(priority); }
String TaskManagement::GetTaskPriorityString() const {
	switch (priority)
	{
	case Priority::Low:
		return U"低";
	case Priority::Medium:
		return U"中";
	case Priority::High:
		return U"高";
	}
	return U"";
}

Situation TaskManagement::GetTaskSituation() const { return situation; }
int32 TaskManagement::GetTaskSituationNum() const { return int32(situation); }
String TaskManagement::GetTaskSituationString() const {
	switch (situation)
	{
	case Situation::NotStarted:
		return U"未開始";
	case Situation::Running:
		return U"実行中";
	case Situation::Completion:
		return U"完了";
	case Situation::Postponement:
		return U"延期";
	case Situation::Waiting:
		return U"中断";
	}
	return U"";
}

Date TaskManagement::GetTaskCreateDate() const { return createDate; }
Date TaskManagement::GetTaskDeadline() const { return deadline; }
String TaskManagement::GetTaskManager() const { return manager; }

//-----------------------------------------------------------------------------
// メンバデータを書き換える
//-----------------------------------------------------------------------------

void TaskManagement::SetTaskTitle(String title_) { title = title_; }
void TaskManagement::SetTaskDescription(String description_) { description = description_; }
void TaskManagement::SetTaskPriority(Priority priority_) { priority = priority_; }
void TaskManagement::SetTaskSituation(Situation situation_) { situation = situation_; }
void TaskManagement::SetTaskCreateDate(Date createDate_) { createDate = createDate_; }
void TaskManagement::SetTaskDeadline(Date deadline_) { deadline = deadline_; }
void TaskManagement::SetTaskManager(String manager_) { manager = manager_; }

