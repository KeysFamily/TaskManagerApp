//-----------------------------------------------------------------------------
// タスク管理アプリ/タスク一覧
// 作　成　者：長谷川勇一朗
// 作成年月日：2023/3/22
// 機　　　能：タスクの一覧表示
//-----------------------------------------------------------------------------

#include "TaskList.hpp"

//コンストラクタ
TaskList::TaskList(const InitData& init)
	: IScene{ init }
	, selectNum{ none }
	, isChanged{ false }
{
	Scene::SetBackground(Palette::Darkgreen);

	if (not getData().isFileOpen) {
		//ファイルが開かれてなければ
		getData().isFileOpen = true;
		getData().isCreate = false;
		getData().editMode = AppDate::EditMode::Non;

		//データの読み込み
		LoadData();
	}
	else {
		//ファイルが開かれていれば

		if (getData().editMode == AppDate::EditMode::Create) {//新規作成されたものの場合
			//新規作成されたものなら
			if (getData().EditBox.GetTaskDate().GetTaskTitle().size() > 0) {
				//リストに追加（IDは自動決定）
				TaskCreate(getData().EditBox.GetTaskDate());

			}
			//新規作成モードを終了する
			getData().isCreate = false;
		}
		else if(getData().editMode == AppDate::EditMode::Edit){
			//編集されたものなら
			TaskObject data = getData().EditBox;

			if (data.GetTaskDate().GetTaskSituation() == Situation::Completion) {
				//もしタスクが完了したものなら
				TaskManagement save = data.GetTaskDate();

				//CSVにおいて、処理行う
				TextWriter writer{ getData().fp + U"Subtask.csv" ,OpenMode::Append };
				if (not writer) {/*エラーあり*/ }
				String output = U"\n";
				output += U"{},"_fmt(data.GetTaskID());				//タスクID
				output += save.GetTaskTitle() + U",";				//タイトル
				output += save.GetTaskDescription() + U",";			//説明
				output += save.GetTaskPriorityString() + U",";		//優先度
				output += save.GetTaskCreateDate().format() + U",";	//作成日
				output += save.GetTaskDeadline().format() + U",";	//締切日
				output += Date::Today().format() + U",";			//完了時間
				output += save.GetTaskManager();					//担当者
				writer.write(output);

				//以下はWindowsのみ動作(トースト通知)
				ToastNotificationItem item{
					.title = U"タスク完了のお知らせ",
					.message = U"TaskID{}番：【{}】を完了しました。"_fmt(
						data.GetTaskID(),
						save.GetTaskTitle())
				};
				Platform::Windows::ToastNotification::Show(item);
				isChanged = true;
			}
			else {

				//編集したデータを追加する
				add.emplace_back(std::shared_ptr<TaskObject>(new TaskObject(data.GetTaskPos(), data.GetTaskID(), data.GetTaskDate())));
				isChanged = true;

			}
		}
		getData().editMode = AppDate::EditMode::Non;
		System::SetTerminationTriggers({});
	}
}

TaskList::~TaskList() {
	if (not getData().isFileOpen) {

		//データはすべて削除する
		getData().vec.clear();
	}
}

void TaskList::update() {

	//タイトルに戻ります。データは保存されません。
	if (SimpleGUI::Button(U"タイトル(BackSpace)", Vec2{ 1020,20 }, 240) || KeyBackspace.down()){

		if (isChanged) {
			if (System::MessageBoxOKCancel(U"変更が保存されていません。\n変更は保存されません。\nタイトルに戻りますか？\n", MessageBoxStyle::Info) == MessageBoxResult::OK) {
				changeScene(State::Start, 0.1s);
				getData().isFileOpen = false;	//ファイルを閉じる判定にする
			}
		}
		else {
			changeScene(State::Start, 0.1s);
			getData().isFileOpen = false;	//ファイルを閉じる判定にする
		}

	}
	if (SimpleGUI::Button(U"データ保存(Ctrl+S)", Vec2{ 1020,60 }, 240) || (KeyControl + KeyS).down()) {
		//データの保存
		SaveDate();
		isChanged = false;

		//以下はWindowsのみ動作(トースト通知)
		ToastNotificationItem item{
			.title = U"保存のお知らせ",
			.message = U"データを保存しました。"
		};
		Platform::Windows::ToastNotification::Show(item);
	}


	if (SimpleGUI::Button(U"タスク作成(Ctrl+N)", Vec2{ 1020,100 }, 240, getData().vec.size() < 50) || (KeyControl + KeyN).down()) {
		getData().isCreate = true;	//新規作成をする
		getData().editMode = AppDate::EditMode::Create;
		changeScene(State::TaskEdit, 0.1s);
		isChanged = true;
	}

	if (SimpleGUI::Button(U"タスク編集(F2)", Vec2{ 1020,140 }, 240, selectNum.has_value()) || (KeyF2.down() && selectNum.has_value())) {
	//	LoadData();
		getData().editMode = AppDate::EditMode::Edit;
		isChanged = true;
		
		//選択されたデータを取得
		auto data = TaskSearch(selectNum.value());

		//編集データに代入する。
		getData().EditBox = (*data);

		//コピー元を削除する
		TaskRemove(selectNum.value());
		
		changeScene(State::TaskEdit, 0.1s);
	}
	if (SimpleGUI::Button(U"タスク削除(Delete)", Vec2{ 1020,180 }, 240, selectNum.has_value()) || (KeyDelete.down() && selectNum.has_value())) {
		if (System::MessageBoxOKCancel(U"削除しますか？", MessageBoxStyle::Info) == MessageBoxResult::OK) {
			TaskRemove(selectNum.value());
			isChanged = true;
		}
	}

	selectNum = none;
	for (auto it = getData().vec.rbegin(); it != getData().vec.rend(); ++it) {
		if (!selectNum.has_value()) {
			(*it)->update();
		}
		if ((*it)->isSelect()) {
			if (!selectNum.has_value()) {
				selectNum = (*it)->GetTaskID();

			}
			else {
				(*it)->SetUnSelect();
			}
		}
	}

	//-------------------------------------------------------------------------
	// 以下、オブジェクトの管理＜変更を禁ずる＞
	// タスクオブジェクトの追加・削除
	//-------------------------------------------------------------------------
	getData().vec.insert(getData().vec.end(), add.begin(), add.end());
	add.clear();//追加用の配列のクリア
	//削除用の処理
	getData().vec.erase(
		std::remove_if(std::begin(getData().vec), std::end(getData().vec), [](TaskObject::SP c) { return  c->isDead(); }),
		std::cend(getData().vec));

}

void TaskList::draw() const {
	for (auto& o : getData().vec) {
		(*o).draw();
	}
}

//タスクファイルの読み込みと書き込み(Loadする)
bool TaskList::LoadData() {
	//ここで読込を行う
	//バイナリファイル形式を使用する

	getData().vec.clear();

	Deserializer<BinaryReader> reader{ getData().fp + U"data.task" };
	if (not reader) {
		System::MessageBoxOK(U"Dataの読み込みに失敗しました。", MessageBoxStyle::Error);
		return false;
	}

	size_t counter;						//タスクオブジェクトの数

	reader(getData().ID_Max,counter);	//IDの最大値を読み込む

	Vec2 pos;            //タスクの場所
	int32 taskID;        //タスクID
	String title;        //タイトル
	String description;  //概要
	int32 priority;      //タスクの優先度
	int32 situation;     //タスクの状況
	Date createDate;     //作成日
	Date deadline;       //締め切り日
	String manager;      //担当者

	for (auto i : step(counter)) {
		//読み込んだデータをタスクオブジェクトを代入します
		reader(
			pos,
			taskID,
			title,
			description,
			priority,
			situation,
			createDate,
			deadline,
			manager
		);
		TaskObject::SP to (new TaskObject(
			pos,
			taskID,
			TaskManagement(
				title,
				description,
				Priority(priority),
				Situation(situation),
				createDate,
				deadline,
				manager
			)));
		
		add.emplace_back(to);
		
	}

	//読込を終了する
	reader->close();

	return true;
}

bool TaskList::SaveDate() {
	//書き出しを行う
	//書き出しはバイナリファイルを使う

	Serializer<BinaryWriter> writer{ getData().fp + U"data.task" };
	if (not writer) {
		System::MessageBoxOK(U"Dataの読み込みに失敗しました。", MessageBoxStyle::Error);
		return false;
	}

	//ファイルデータを一度消去する
	writer->clear();

	//ファイルの書き込みをする
	//IDの最大値とタスクオブジェクトの総数を入力する
	writer(getData().ID_Max, getData().vec.size());
	for (auto& o : getData().vec) {
		TaskManagement data = (*o).GetTaskDate();
		writer(
			(*o).GetTaskPos(),
			(*o).GetTaskID(),
			data.GetTaskTitle(),
			data.GetTaskDescription(),
			data.GetTaskPriorityNum(),
			data.GetTaskSituationNum(),
			data.GetTaskCreateDate(),
			data.GetTaskDeadline(),
			data.GetTaskManager()
		);
	}
	//書き込みを終了する
	writer->close();

	return true;
}

//タスクオブジェクトの生成と削除

bool TaskList::TaskCreate(TaskManagement task_) {
	//指定した数を超えている場合、生成しない
	if (getData().vec.size() > 50) { return false; }

	//末尾に追加
	add.emplace_back(std::shared_ptr<TaskObject>(new TaskObject(RandomVec2(200, 200), getData().ID_Max, task_)));

	//IDの最大値を一つ上げる
	++getData().ID_Max;
	return true;
}

bool TaskList::TaskRemove(int32 id_) {
	//有効値が入っていれば実行
	auto o = TaskSearch(id_);

	//何も見つからなければ修了する
	if (o == nullptr)return false;

	//削除させる。
	o->SetDead();
	return true;
}

TaskObject::SP TaskList::TaskSearch(int32 id_) {
	for (auto& o : getData().vec) {
		//IDを返す（同じ番号が二つ以上あるなら先に見つかったほうを返す）
		if ((*o).GetTaskID() == id_) {
			return o;
		}
	}
	//もし見つからなければnullptrを返す
	return nullptr;
}
