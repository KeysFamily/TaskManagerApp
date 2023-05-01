﻿//-----------------------------------------------------------------------------
// タスク管理アプリ/スタート画面
// 作　成　者：長谷川英一
// 作成年月日：2023/3/20
// 機　　　能：タスク管理アプリ/スタート画面
//-----------------------------------------------------------------------------

#include "Start.hpp"

//コンストラクタ
Start::Start(const InitData& init)
	:IScene{ init }
{
	//初期化処理
	Scene::SetBackground(Palette::Lightgreen);	//背景色の設定

	//終了キーのボタン
	System::SetTerminationTriggers(UserAction::Default);
}

//更新処理
void Start::update() {
	
	//ボタンをクリックされたら
	if (buttonRect.movedBy(Scene::Center()).leftClicked()) {

		//「新規作成ボタン」
		changeScene(State::CreateNewTaskFile, 0.1s);
	}
	else if (buttonRect.movedBy(Scene::Center().x, Scene::Size().y * 3 / 4).leftClicked()) {
		
		//「データの読み込み」
		changeScene(State::LoadingTaskFile, 0.1s);
	}
		

}

//描画処理
void Start::draw() const
{
	//タイトル表示
	FontAsset(U"TitleFont")(U"タスク管理アプリ")
		.drawAt(Scene::Center().x, Scene::Size().y / 4, Palette::Green);

	//for (auto i = 2; i <= 3; ++i)
	for (auto i : Range(2, 3)) {
		//ボタンの表示
		RoundRect box = buttonRect.movedBy(Scene::Center().x, Scene::Size().y * i / 4);

		box.drawShadow(Vec2(10, 10), 5)
			.draw(box.mouseOver() ? Palette::Lime : Palette::Limegreen)
			.drawFrame(5, 0, Palette::Green);
	}

	//ボタン表示
	
	FontAsset(U"MenuFont")(U"新規作成")
		.drawAt(Scene::Center(), Palette::Green);

	FontAsset(U"MenuFont")(U"開く")
		.drawAt(Scene::Center().x, Scene::Size().y * 3 / 4, Palette::Green);

	//製作者表示
	FontAsset(U"CreateFont")(U"Create by KeysFamily with Siv3D / ライセンス情報は[F1]を押してください。")
		.draw(Arg::bottomRight = Scene::Size(), Palette::Green);

}