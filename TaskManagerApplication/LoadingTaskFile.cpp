//-----------------------------------------------------------------------------
// タスク管理アプリ/ファイル読込処理
// 作　成　者：長谷川英一
// 作成年月日：2023/3/21
// 機　　　能：タスクファイルの読み込み
//-----------------------------------------------------------------------------
#include "LoadingTaskFile.hpp"

//コンストラクタ
LoadingTaskFile::LoadingTaskFile(const InitData& init)
	: IScene{ init }
	, isNoFile{ false }
{
	//初期化処理
	Scene::SetBackground(Palette::Lightgreen);	//背景色の設定

	FilePath fp = FileSystem::CurrentDirectory();	//.exe直下のパスを取得

	if (FileSystem::IsDirectory(fp)) {			//通常ここにタスクファイルデータが存在すること
		//フォルダ一覧を取得
		for (const auto path : FileSystem::DirectoryContents(fp, Recursive::No)) {
			
			//フォルダのみ読み込む（拡張子が存在しないデータをフォルダとみなす処理）
			if (FileSystem::Extension(path) == U"") {

				//指定したファイルが存在するか
				if (FileSystem::IsFile(path + U"data.task")) {

					//ディレクトリ名を取得
					dataPaths << FileSystem::FileName(path);
				}
			}
		}

		//以下のif文内でファイルをオープン。成功したらtrue
		if (dataPaths.size() > 0) {
			ls.items = dataPaths;
			getData().fp = fp;
		}//ファイルロード
		else {
			isNoFile = true;
		}
	}
	else {
		isNoFile = true;
	}
	System::SetTerminationTriggers({});

}

void LoadingTaskFile::update() {

	//ファイルが見つからなければエラー
	if (isNoFile) {
		System::MessageBoxOK(U"データが存在しません。\nタスク管理ファイルを作成してください", MessageBoxStyle::Warning);
		changeScene(State::Start, 0.1s);
	}

	//タスクデータ一覧を表示
	SimpleGUI::Headline(U"タスクファイル一覧", Vec2{ 50,20 });
	SimpleGUI::ListBox(ls, Vec2(50, 60), 700, 600);


	//ボタン表示
	//開く。もし選択されていなければ、このボタンは押せないようにする。
	if (SimpleGUI::Button(U"このファイルを開く", Vec2{ 800,575 }, 430, ls.selectedItemIndex.has_value())) {
		getData().fp += ls.items[*ls.selectedItemIndex] + U'/';
		getData().isFileOpen = false;	//ファイルを開いた。
		changeScene(State::TaskList, 0.1s);
	}

	if (SimpleGUI::Button(U"キャンセル", Vec2{ 800,625 }, 430)) {
		//スタート画面に戻る
		changeScene(State::Start, 0.1s);
	}

}

void LoadingTaskFile::draw() const {
	//選択されているデータがあれば
	FontAsset(U"CreateFont")(U"タスク名:{}"_fmt((not ls.selectedItemIndex) ? U"選択してください" : ls.items[*ls.selectedItemIndex]))
		.draw(Vec2{ 800,80 }, Palette::Green);

}
