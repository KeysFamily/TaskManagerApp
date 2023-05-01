//-----------------------------------------------------------------------------
// タスク管理アプリ/タスク一覧
// 作　成　者：長谷川英一
// 作成年月日：2023/3/22
// 機　　　能：タスクの一覧表示
//-----------------------------------------------------------------------------

#include "TaskList.hpp"

//コンストラクタ
TaskList::TaskList(const InitData& init)
	: IScene{ init }
	, selectNum{ none }
{
	Scene::SetBackground(Palette::Darkgreen);

	if (not getData().isFileOpen) {
		//ファイルが開かれてなければ。
		getData().isFileOpen = true;
		getData().isCreate = false;

		//データの読み込み
		LoadData();
	}
	else {
		//ファイルが開かれていれば

		if (getData().isCreate) {//新規作成されたものの場合
			//新規作成されたものなら
			if (getData().EditBox.GetTaskDate().GetTaskTitle().size() > 0) {
				//リストに追加（IDは自動決定）
				TaskCreate(getData().EditBox.GetTaskDate());

			}
			//新規作成モードを終了する
			getData().isCreate = false;
		}
		else {
			//編集されたものなら
			TaskObject data = getData().EditBox;

			if (data.GetTaskDate().GetTaskSituation() == Situation::Completion) {
				//もしタスクが完了したものなら

				CSV csv;
				TaskManagement save = data.GetTaskDate();
				csv.writeRow(
					data.GetTaskID(),				//タスクID
					save.GetTaskTitle(),			//タイトル
					save.GetTaskDescription(),		//説明
					save.GetTaskPriorityString(),	//優先度
					save.GetTaskSituationString(),	//進捗状況
					save.GetTaskCreateDate(),		//作成日
					save.GetTaskDeadline(),			//締切日
					Date::Today(),					//完了時間
					save.GetTaskManager()			//担当者
				);
				csv.save(getData().fp + U"subtask_{}.csv"_fmt(data.GetTaskID()));

				//以下はWindowsのみ動作
				ToastNotificationItem item{ .title = U"タスク完了のお知らせ", .message = U"TaskID{}番：【{}】を完了しました。"_fmt(data.GetTaskID(),save.GetTaskTitle()) };
				Platform::Windows::ToastNotification::Show(item);
				
			}
			else {

				//編集したデータを追加する
				add.emplace_back(std::shared_ptr<TaskObject>(new TaskObject(data.GetTaskPos(), data.GetTaskID(), data.GetTaskDate())));
			}
		}
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
	if (SimpleGUI::Button(U"タイトルに戻る", Vec2{ 1060,20 }, 200)){
		if (System::MessageBoxOKCancel(U"最後に保存したデータが保持されます。\nそれ以降に変更したデータは保持されません。\nタイトルに戻りますか？", MessageBoxStyle::Info) == MessageBoxResult::OK) {
			changeScene(State::Start, 0.1s);
			getData().isFileOpen = false;	//ファイルを閉じる判定にする
		}
	}
	if (SimpleGUI::Button(U"データ保存", Vec2{ 1060,60 }, 200)) {
		//データの保存
		SaveDate();
	}


	if (SimpleGUI::Button(U"タスク作成", Vec2{ 1060,100 }, 200, getData().vec.size() < 50)) {
		getData().isCreate = true;	//新規作成をする

		changeScene(State::TaskEdit, 0.1s);
	}

	if (SimpleGUI::Button(U"タスク編集", Vec2{ 1060,140 }, 200, selectNum.has_value())) {
	//	LoadData();
		//選択されたデータを取得
		auto data = TaskSearch(selectNum.value());

		//編集データに代入する。
		getData().EditBox = (*data);

		//コピー元を削除する
		TaskRemove();
		
		changeScene(State::TaskEdit, 0.1s);
	}
	if (SimpleGUI::Button(U"タスク削除", Vec2{ 1060,180 }, 200, selectNum.has_value())) {
		if (System::MessageBoxOKCancel(U"削除しますか？",MessageBoxStyle::Info) == MessageBoxResult::OK) {
			TaskRemove();
		}
	}
	//タスクオブジェクトの更新処理

	for (auto& o : getData().vec) {
		(*o).update();
		if ((*o).isSelect() == true) {
			selectNum = (*o).GetTaskID();

			if (KeyEnter.down()) {
				System::MessageBoxOK(
					U"TaskID:{}\nタイトル:{}\n説明:{}\n締切:{}\n担当者:{}"_fmt(
						(*o).GetTaskID(),
						(*o).GetTaskDate().GetTaskTitle(),
						(*o).GetTaskDate().GetTaskDescription(),
						(*o).GetTaskDate().GetTaskDeadline(),
						(*o).GetTaskDate().GetTaskManager()
					)
				);
			}
			break;
		}
		else {
			selectNum = none;
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

	size_t counter;

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
		Logger << U"ループ頭。02";
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
		Logger << U"読み込みました。03";
		//読み込んだファイルをタスクオブジェクトを代入します
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
		Logger << U"データを書き込みました。04";
		//オブジェクトを追加する
		add.emplace_back(to);
		
	}

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

	writer->clear();
	//ファイルの書き込みをする
	
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

bool TaskList::TaskRemove() {
	//有効値が入っていれば実行
	if (selectNum.has_value()) {
		auto o = TaskSearch(selectNum.value());

		//何も見つからなければ修了する
		if (o == nullptr)return false;

		//削除させる。
		o->SetDead();
		return true;
	}
	return false;
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
