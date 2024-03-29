﻿//-----------------------------------------------------------------------------
// タスク管理アプリ/タスクファイル作成
// 作　成　者：長谷川勇一朗
// 作成年月日：2023/3/23
// 機　　　能：タスクファイルの新規作成
//-----------------------------------------------------------------------------
#include "CreateNewTaskFile.hpp"

CreateNewTaskFile::CreateNewTaskFile(const InitData& init)
	: IScene{ init }
	, tes{ U"" }
{
	Scene::SetBackground(Palette::Yellowgreen);
	System::SetTerminationTriggers(UserAction::CloseButtonClicked);
}

void CreateNewTaskFile::update() {
	
	//入力ボックス
	SimpleGUI::TextBoxAt(tes, Scene::Center(), 1050, 50);


	//リセットボタン
	if (SimpleGUI::Button(U"入力リセット(Ctrl+R)", Vec2{ 925,400 }, 240) || (KeyControl + KeyR).down()) {
		tes.clear();
	}
	//作成ボタン（入力されていなければ使用不可）
	if (SimpleGUI::Button(U"作成(Ctrl+Enter)", Vec2{ 925,550 }, 240, tes.text.size() > 0)
		|| (KeyControl + KeyEnter).down() && tes.text.size() > 0
		) {
		//フォルダを作成（この中にタスクデータを保存する）
		//現在のカレントディレクトリを取得する
		FilePath fp = FileSystem::CurrentDirectory();
		fp += tes.text;

		//ディレクトリを作成する
		if (FileSystem::Exists(fp) == false) {	//もしも、同名ディレクトリが存在していなければ
			FileSystem::CreateDirectories(fp);	//ディレクトリを作成

			//data.taskを生成する（ただし、何も書き込まない）
			// シリアライズ機能を持つバイナルファイル
			Serializer<BinaryWriter> writer{ fp + U"/data.task" };

			if (not writer) // もしオープンに失敗したら
			{
				throw Error{ U"Failed to open `data.task`" };
			}

			writer(
				int32(0),
				size_t(0)
			);

			writer->close();

			//subtask.csvを生成
			TextWriter writerCSV{ fp + U"/Subtask.csv" };
			if(not writerCSV){ /*エラーあり*/ }
			String output;
			output += U"タスクID,タイトル,説明,優先度,作成日,締切日,完了時間,担当者";
			writerCSV.write(output);

			writerCSV.close();

			getData().isFileOpen = false;	//ファイルを開いた。

			getData().fp = fp + U'/';

			changeScene(State::TaskList, 0.1s);
		}
		else {
			//同名ディレクトリが存在した場合、別名を入力させる。
			System::MessageBoxOK(U"{}は既に存在します。別の名前を指定してください。"_fmt(tes.text), MessageBoxStyle::Warning);
			tes.clear();
		}	
	}

	//キャンセルボタン
	if (SimpleGUI::Button(U"キャンセル(ESC)", Vec2{ 925,600 }, 240) || KeyEscape.down()) {
		changeScene(State::Start, 0.1s);
	}

}

void CreateNewTaskFile::draw() const {
	FontAsset(U"CreateFont")(U"タスクファイル名（全角半角問わず50字まで）：" )
		.drawAt(Vec2{ 640,300 }, Palette::Green);
}
