//-----------------------------------------------------------------------------
// タスク管理アプリ/タスクオブジェクトデータクラス
// 作　成　者：長谷川英一
// 作成年月日：2023/3/21
// 機　　　能：タスクファイルの読み込み
//-----------------------------------------------------------------------------

#include "TaskObject.hpp"

//コンストラクタ
//TaskObject::TaskObject(Vec2 pos_ = Vec2(10.0, 10.0), int32 taskId_ = 0, TaskManagement data_ = TaskManagement())
TaskObject::TaskObject(Vec2 pos_ , int32 taskId_, TaskManagement data_ )
    : pos{ pos_ }
    , taskID{ taskId_ }
    , taskDate{ data_ }
	, taskBox{ Size{200,200} }
	, deadFlag{ false }
	, grip{ false }
	, selected{ false }
{
}

void TaskObject::update() {
	RectF hitBox = taskBox.movedBy(pos);

	if (MouseL.down()) {
		selected = hitBox.leftClicked();
	}

	if (grip && Cursor::OnClientRect())
	{
		// 移動量分だけ円を移動
		pos += Cursor::Delta();
	}

	if (hitBox.leftClicked()) // オブジェクトを左クリックしたら
	{
		grip = true;
	}
	else if (MouseL.up()) // マウスの左ボタンが離されたら
	{
		grip = false;
	}

	if (grip) {

		//外にはみ出すことを防ぐ

		if (pos.x < 0) { pos.x = 0; }
		else if (pos.x > Scene::Rect().w - taskBox.w) { pos.x = Scene::Rect().w - taskBox.w; }

		if (pos.y < 0) { pos.y = 0; }
		else if (pos.y > Scene::Rect().h - taskBox.h) { pos.y = Scene::Rect().h - taskBox.h; }
	}
	
}

void TaskObject::draw() {
	RectF drawBox = taskBox.movedBy(pos); //オブジェクト

	Color stringColor = Palette::White;   //文字の色

	//優先度によってオブジェクトの色と文字色を変更する。
	switch (taskDate.GetTaskPriority())
	{
	case Priority::Low:
		//優先度が低いので緑系で表示
		drawBox
			.drawShadow(Vec2(10, 10), 5)
			.draw(Palette::Skyblue)
			.drawFrame(2, 0, selected ? Palette::Deepskyblue : Palette::Darkgray);
		stringColor = Palette::Deepskyblue;
		break;
	case Priority::Medium:
		//優先度は普通なので黄系で表示
		drawBox
			.drawShadow(Vec2(10, 10), 5)
			.draw(Palette::Yellow)
			.drawFrame(2, 0, selected ? Palette::Olive : Palette::Darkgray);
		stringColor = Palette::Olive;
		break;
	case Priority::High:
		//優先度が高いので赤系で表示する
		drawBox
			.drawShadow(Vec2(10, 10), 5)
			.draw(Palette::Red)
			.drawFrame(2, 0, selected ? Palette::Darkred : Palette::Darkgray);
		stringColor = Palette::Darkred;
		break;
	}

	//締切に近い場合、表示文字を変更する
	if (Date::Subtract(taskDate.GetTaskDeadline(), Date::Today()) <= Days(10_d)) {
		stringColor = Palette::Black;
	}

	//以下で表示文字を記述
	//タスクID
	FontAsset(U"TaskViewFont")(U"TaskID:{}"_fmt(taskID))
		.draw(pos + Vec2{ 20,20 }, stringColor);
	//タスクの名前（矩形を用意その中に文字を描画する）
	FontAsset(U"CreateFont")(U"{}"_fmt(taskDate.GetTaskTitle()))
		.draw(RectF{ pos + Vec2{ 20,40 }, 160,80 }, stringColor);

	//進捗状況
	FontAsset(U"TaskViewFont")(U"進捗状況:{}"_fmt(taskDate.GetTaskSituationString()))
		.draw(pos + Vec2{ 20,120 }, stringColor);

	//担当者
	FontAsset(U"TaskViewFont")(U"担当者:{}"_fmt(taskDate.GetTaskManager()))
		.draw(pos + Vec2{ 20,140 }, stringColor);

	//締切
	FontAsset(U"TaskViewFont")(U"締切:{}"_fmt(taskDate.GetTaskDeadline()))
		.draw(pos + Vec2{ 20,160 }, stringColor);
}

bool TaskObject::isSelect() const {
	return selected;
}

bool TaskObject::isDead() const {
	return deadFlag;
}

int32 TaskObject::GetTaskID() const {
	return taskID;
}

TaskManagement TaskObject::GetTaskDate() const {
	return taskDate;
}

Vec2 TaskObject::GetTaskPos() const {
	return pos;
}

void TaskObject::SetTaskDate(TaskManagement date_) {
	taskDate = date_;
}

void TaskObject::SetDead() {
	deadFlag = true;
}
