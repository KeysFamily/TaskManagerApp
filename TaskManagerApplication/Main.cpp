//-----------------------------------------------------------------------------
// タスク管理アプリ(Siv3D版)
// 作　成　者：長谷川勇一朗
// 作成年月日：2023/3/20
// 機　　　能：タスク管理アプリのシーン読込処理
// 注　　　意：このアプリケーションはSiv3Dで作成している。
// 　　　　　　学習用に作成しているのでコメントを多用する
//-----------------------------------------------------------------------------
#include "TaskAppCommon.hpp"//シーン管理の際はこれを読み込むこと
#include "Start.hpp"
#include "LoadingTaskFile.hpp"
#include "TaskList.hpp"
#include "CreateNewTaskFile.hpp"
#include "TaskEdit.hpp"

void Main()
{
	//ウィンドウ設定
	Window::SetToggleFullscreenEnabled(false);	//全画面モードを禁止
	Window::Resize(1280, 720);					//画面サイズを変更
	Window::SetTitle(U"タスクボード");			//ウィンドウタイトルを変更

	//ライセンス情報
	LicenseInfo info;
	info.title = U"タスクボード";
	info.copyright = U"Copyright (C) 2023 KF";
	info.text = U"";
	LicenseManager::AddLicense(info);

	// ファイルパスのドロップを受け付けないようにする
	DragDrop::AcceptFilePaths(false);

	//アセット管理
	//フォント(使用フォントは【UD デジタル 教科書体 N-B 太字:  UDDigiKyokashoN-B.ttc】
	//        (【HG正楷書体-PRO 標準 : HGRSKP.TTF】) , HGRSMP.TTF)
	FontAsset::Register(
		U"TitleFont",
		100,
		FileSystem::GetFolderPath(SpecialFolder::SystemFonts) + U"HGRSKP.TTF");
	FontAsset::Register(
		U"MenuFont",
		75,
		FileSystem::GetFolderPath(SpecialFolder::SystemFonts) + U"HGRSKP.TTF");
	FontAsset::Register(
		U"CreateFont",
		30,
		FileSystem::GetFolderPath(SpecialFolder::SystemFonts) + U"HGRSKP.TTF");
	FontAsset::Register(
		U"TaskViewFont",
		20,
		FileSystem::GetFolderPath(SpecialFolder::SystemFonts) + U"HGRSKP.TTF");

	//シーン管理
	App manager;
	manager.add<Start>(State::Start);
	manager.add<LoadingTaskFile>(State::LoadingTaskFile);
	manager.add<TaskList>(State::TaskList);
	manager.add<CreateNewTaskFile>(State::CreateNewTaskFile);
	manager.add<TaskEdit>(State::TaskEdit);

	//任意のシーンから開始したいときにこの関数を書く
	manager.init(State::Start);

	while (System::Update()) {
		ClearPrint();
		if (not manager.update()) {
			break;
		}
	}
}



