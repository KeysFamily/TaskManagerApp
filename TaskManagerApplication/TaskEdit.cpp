//-----------------------------------------------------------------------------
// タスク管理アプリ/タスク編集
// 作　成　者：長谷川勇一朗
// 作成年月日：2023/3/24
// 機　　　能：タスクの新規作成/概要表示/編集をすべて行えます
//-----------------------------------------------------------------------------

#include "TaskEdit.hpp"
TaskEdit::TaskEdit(const InitData& init)
	: IScene{ init }
	//, text{ U"" }
	, selectPriority{ 1 }
	, selectSituation{ 0 }
{
	Scene::SetBackground(Palette::Limegreen);


	//新規作成でなければ（編集なら）
	if (not getData().isCreate) {
		//現在のデータを読み込む
		TaskManagement data = getData().EditBox.GetTaskDate();
		titleLS.text = data.GetTaskTitle();
		descriptionLS.text = data.GetTaskDescription();
		selectPriority = data.GetTaskPriorityNum();
		selectSituation = data.GetTaskSituationNum();
		createDate = data.GetTaskCreateDate();

		//締切日入力場所の値を取得
		deadlineDate = data.GetTaskDeadline();

		deadlineYearLS.text = Format(deadlineDate.year);
		deadlineMonthLS.text = Format(deadlineDate.month);
		deadlineDayLS.text = Format(deadlineDate.day);

		managerLS.text = data.GetTaskManager();
	}
	else {
		//新規作成なら
		titleLS.text = U"タイトル";
		descriptionLS.text = U"説明";

		createDate = Date::Today();

		//次の日の値を入力する
		deadlineDate = Date::Tomorrow();

		deadlineYearLS.text = Format(deadlineDate.year);
		deadlineMonthLS.text = Format(deadlineDate.month);
		deadlineDayLS.text = Format(deadlineDate.day);

		managerLS.text = U"unknown";

	}
}

void TaskEdit::update() {
	//入力のGUI
	SimpleGUI::TextBox(titleLS, Vec2{ 100,100 }, 1080, 50);

	SimpleGUI::TextBox(descriptionLS, Vec2{ 100,200 }, 1080, 50);

	SimpleGUI::RadioButtons(selectSituation, situationStr, Vec2{ 100,300 }, 200);

	SimpleGUI::RadioButtons(selectPriority, priorityStr, Vec2{ 350,300 }, 200);

	SimpleGUI::TextBox(managerLS, Vec2{ 100,550 }, 1080, 50);

	//締切日入力欄のいずれかで入力があれば、
	if (SimpleGUI::TextBox(deadlineYearLS, Vec2{ 760,397 }, 70, 4) ||
		SimpleGUI::TextBox(deadlineMonthLS, Vec2{ 870,397 }, 70, 2) ||
		SimpleGUI::TextBox(deadlineDayLS, Vec2{ 970,397 }, 70, 2)) {

		//文字列に半角数字があれば置き換える。置き換えれない場合、-1を返す。
		deadlineDate.year = ParseOr<int32>(deadlineYearLS.text, -1);
		deadlineDate.month = ParseOr<int32>(deadlineMonthLS.text, -1);
		deadlineDate.day = ParseOr<int32>(deadlineDayLS.text, -1);
	}


	//アクション
	if (SimpleGUI::Button(getData().isCreate ? U"作成" : U"保存", Vec2{ 850,600 }, 150)) {
		//データ
		if (deadlineDate.isValid() &&			//日付が正しいか?
			titleLS.text.size() > 0 &&			//タイトル入力欄が空欄でないか？
			descriptionLS.text.size() > 0 &&	//タスク説明入力欄が空欄でないか？
			managerLS.text.size() > 0			//管理者入力欄が空欄でないか？
			) {
			//すべてのデータが正しく入力されていれば

			TaskManagement data;
			data.SetTaskTitle(titleLS.text);					//タイトル
			data.SetTaskDescription(descriptionLS.text);		//説明
			
			data.SetTaskPriority(Priority(selectPriority));		//優先度
			data.SetTaskSituation(Situation(selectSituation));	//進捗状況

			data.SetTaskCreateDate(createDate);					//作成日
			data.SetTaskDeadline(deadlineDate);					//締切日
			data.SetTaskManager(managerLS.text);				//担当者

			getData().EditBox.SetTaskDate(data);
			changeScene(State::TaskList, 0.1s);					//シーンを置き換える。
		}
		else {
			System::MessageBoxOK(U"データが入力されていないか、日付が間違っている可能性があります。", MessageBoxStyle::Warning);
		}
	}
	if (SimpleGUI::Button(U"キャンセル", Vec2{ 1030,600 }, 150)) {
		changeScene(State::TaskList, 0.1s);
	}
}

void TaskEdit::draw() const {
	FontAsset(U"CreateFont")(U"タスクタイトル名(50文字)：")
		.draw(Vec2{ 100,50 }, Palette::Green);

	FontAsset(U"CreateFont")(U"タスク概要(50文字)：")
		.draw(Vec2{ 100,150 }, Palette::Green);

	FontAsset(U"CreateFont")(U"進捗状況：")
		.draw(Vec2{ 100,250 }, Palette::Green);

	FontAsset(U"CreateFont")(U"優先度：")
		.draw(Vec2{ 350,250 }, Palette::Green);

	FontAsset(U"CreateFont")(U"担当者(50文字)：")
		.draw(Vec2{ 100,500 }, Palette::Green);

	FontAsset(U"CreateFont")(U"作成日：{: >4}年{: >2}月{: >2}日"_fmt(createDate.year, createDate.month, createDate.day))
		.draw(Vec2{ 640,300 }, Palette::Green);
	FontAsset(U"CreateFont")(U"締切日：     年     月     日")
		.draw(Vec2{ 640,400 }, Palette::Green);

	//入力された日付が正しいかどうかを審査する。
	if (not deadlineDate.isValid()) {
		//間違っていたら
		FontAsset(U"CreateFont")(U"日付が正しくありません。")
			.draw(Vec2{ 640,450 }, Palette::Darkred);
	}
}
