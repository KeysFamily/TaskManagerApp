#pragma once

//このファイルを全体で全てのシーン管理ファイルで読み込むこと

#include <Siv3D.hpp>
#include "TaskObject.hpp"	//タスクオブジェクトの定義

//シーン名称
enum class State {
    Start,             //スタート画面
    LoadingTaskFile,   //タスクファイル読込
    CreateNewTaskFile, //タスクファイル新規作成
    TaskList,          //タスク一覧
	TaskEdit           //タスクの編集
};

struct AppDate {
    //アプリ内で共有するデータをここに書くこと

	int32 ID_Max;

	//フラグデータ

	// TaskList初期化でtrueになる。
	// 終了時にfalseにする
	bool isFileOpen;			//データファイルが開かれていれば、true

	//新規作成ならtrue
	//TaskList終盤でtrueにして、編集画面に飛ぶ
	//
	bool isCreate;				//新規作成ならtrue

	TaskObject EditBox;			//編集用データ

	FilePath fp;

	Array<TaskObject::SP> vec;
};

using App = SceneManager<State, AppDate>;

