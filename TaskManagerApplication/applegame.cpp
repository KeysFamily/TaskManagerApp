# include <Siv3D.hpp>


enum class Angle {
    Down,    //下
    Left,    //左
    Right,    //右
};
//構造体定義
struct Apple {
    Vec2 pos;        //座標
    Rect hitBase;    //当たり判定
    Vec2 move;        //移動量
    double ang;    //角度
    double addAng;    //回転量

    Apple(Vec2 pos_, Vec2 move_, double ang_, double addAng_) {
        this->pos = pos_;
        this->move = move_;
        this->hitBase = Rect{ Arg::center = Point{0,0},48,48 };
        this->ang = ang_;
        this->addAng = addAng_;
    }
};
struct Player {
    Angle angle;
    Vec2 pos;        //座標
    Rect hitBase;    //当たり判定
    int32 animCnt;    //アニメーカウンタ

    Player(Vec2 pos_, Rect hit_) {
        this->angle = Angle::Down;
        this->pos = pos_;
        this->hitBase = hit_;
        this->animCnt = 0;
    }
};

double deltaTimeFrame;//フレーム時間 Scene::DeltaTime()の呼び出しを減らす
int32 score;        //スコア

void Player_UpDate(Player& p_);    //プレイヤーの処理
bool Apple_UpDate(Apple& a_, Player p_);    //リンゴの処理

void Main()
{
    //ウィンドウ設定
    Window::SetPos(100, 100);//実行画面が中心に表示されることを防ぐ

    //列挙型定義
    enum class GameMode {
        Non,//なし
        Title,    //タイトル
        Game,    //Game進行モード
        Over,    //ゲームオーバー画面
        Result,    //リザルト
    };
    GameMode mode = GameMode::Title;

    Player player = Player(
        Vec2(400, 480),
        Rect(Arg::center = Point{ 0,0 }, 64, 64)
    );
    Array<Apple> arr;        //リンゴ管理

    double appearTime;        //出現までの時間
    double appearCounter;    //出現カウンタ
    double gameTime;        //ゲームタイム

    //画像データ
    Texture backImage{ U"applegame/image/bg_natural_mori.jpg" };//背景画像
    Texture appleImage{ U"applegame/image/fruit_ringo.png" };    //リンゴイメージ
    Texture playerImage{ U"applegame/image/chara.png" };        //プレイヤー画像
    Texture titleImage{ U"applegame/image/title_text.png" };    //タイトル画像
    Texture startImage{ U"applegame/image/StartKey.png" };        //スタート
    Texture overImage{ U"applegame/image/gameOver.png" };        //ゲームオーバー画像

    //音声データ
    const Audio bgm{ U"applegame/sound/bgm1.mp3",Loop::Yes };    //bgm
    const Audio pressSe{ U"applegame/sound/ok.wav" };            //決定音声
    const Audio pokuSe{ U"applegame/sound/poku.wav" };            //ぽくオン

    //MixBus0:BGM用のチャンネルに設定
    //MixBus1:SE用のチャンネルに設定
    GlobalAudio::BusSetVolume(MixBus0, 0.5);
    GlobalAudio::BusSetVolume(MixBus1, 1);
    //以下更新処理
    while (System::Update())
    {
        ClearPrint();
        //共通処理があればここに記述すること
        deltaTimeFrame = Scene::DeltaTime();
        backImage.resized(800.0, 600.0).draw(0.0, 0.0);

        Array<int32> anim = { 1,0,1,2 };
        double frash = Periodic::Square0_1(1s);
        switch (mode)
        {
        case GameMode::Title:
            //タイトル処理

            titleImage.drawAt(400, 200);
            startImage.scaled(0.75).drawAt(400, 400, ColorF(1.0, frash));


            if (KeyS.down()) {
                mode = GameMode::Game;    //ゲームモードを変更
                gameTime = 30.0;        //ゲーム時間の設定
                appearCounter = 0.0;    //出現カウンタの初期化
                appearTime = 0.0;        //出現時間の初期化
                score = 0;                //スコアの初期化

                //音楽の再生ルールは上記の通り
                bgm.play(MixBus0);
                pressSe.play(MixBus1);
            }
            break;
        case GameMode::Game:
            gameTime -= deltaTimeFrame;
            Print << U"Time:{:.1f}[s]"_fmt(gameTime);
            Print << U"Score:{}[point]"_fmt(score);
            appearCounter += deltaTimeFrame;

            Player_UpDate(player);

            //出現処理
            if (appearCounter >= appearTime) {
                //項目の追加
                arr << Apple(
                    Vec2{//現在位置
                        Random<double>(24.0,800 - 24.0),
                        Random<double>(24.0,200 - 24.0)
                    },
                    Vec2{//初速
                        Random<double>(-0.5,0.5) * 60,
                        Random<double>(-1.0,1.0) * 60
                    },
                    //角度
                    Random<double>(-40.0f, 40.0f),
                    //回転量
                    Random<double>(-1.0f, 1.0f) * 60
                );

                appearCounter = 0.0;
                appearTime = Random<double>(0.0833, 0.333);
            }

            //更新及び消滅処理
            for (auto it = arr.begin(); it != arr.end();) {
                //リンゴが削除対象なら、配列からデータを消す
                if (Apple_UpDate(*it, player)) { it = arr.erase(it); }
                else { ++it; }
            }
            //player.hitBase.movedBy(player.pos.x, player.pos.y).draw();
            playerImage(Vec2{ 64 * anim[player.animCnt / 15],64 * (int32)player.angle }, Vec2{ 64,64 }).drawAt(player.pos);
            for (const auto& value : arr) {
                //value.hitBase.movedBy(value.pos.x, value.pos.y).rotated(ToRadians(value.ang)).draw();
                appleImage.resized(48, 48).rotated(ToRadians(value.ang)).drawAt(value.pos.x, value.pos.y);
            }

            if (gameTime <= 0.0) {
                arr.clear();//配列要素をすべて削除
                mode = GameMode::Over;
            }
            break;
        case GameMode::Over:
            //ゲーム中断処理
            overImage.drawAt(400, 200);
            startImage.scaled(0.75).drawAt(400, 400, ColorF(1.0, frash));

            if (KeyS.down()) {
                //mode = GameMode::Result;
                bgm.stop(0.5s);
                pressSe.play(MixBus1);
            }
            if (not bgm.isPlaying()) {
                mode = GameMode::Result;
            }
            break;
        case GameMode::Result:
            //結果表示

            startImage.scaled(0.75).drawAt(400, 400, ColorF(1.0, frash));

            if (KeyS.down()) {
                mode = GameMode::Title;

                pressSe.play(MixBus1);
            }
            break;
        default:
            break;
        }


    }
}


void Player_UpDate(Player& p_) {    //プレイヤーの処理
    p_.angle = Angle::Down;
    if (KeyLeft.pressed() && p_.pos.x > 32) {
        p_.pos.x -= 300 * deltaTimeFrame;
        p_.angle = Angle::Left;
    }
    if (KeyRight.pressed() && p_.pos.x < 800 - 32) {
        p_.pos.x += 300 * deltaTimeFrame;
        p_.angle = Angle::Right;
    }
    //左右キーのどちらかが押されていたら
    if ((KeyLeft | KeyRight).pressed()) {
        if (++p_.animCnt >= 60) { p_.animCnt = 0; }
    }
    else {
        p_.animCnt = 0;
    }
}

//消滅する場合trueを返す
bool Apple_UpDate(Apple& a_, Player p_) {    //リンゴの処理
    //移動および回転処理
    a_.pos += a_.move * deltaTimeFrame;
    a_.ang += a_.addAng * deltaTimeFrame;

    //重力加速度の求め方
    // GPGの重力加速度の式
    // gf:フレーム単位の重力加速度
    // g :重力加速度
    // f :1秒間のフレーム数
    // m :1mに相当する大きさ
    // のとき
    // gf = ((g / f) / f) * m
    // 一フレームごとの経過時間をtとすると
    // f = 1/tとなる
    // これを代入して
    // gf = g * m * t * t
    //--
    if (a_.pos.y > 600 + 64) { return true; }//もし範囲外なら消滅させる

    a_.move.y += 9.80665 * 32 * deltaTimeFrame * 0.75;

    Rect you = p_.hitBase.movedBy(p_.pos.x, p_.pos.y);
    Rect me = a_.hitBase.movedBy(a_.pos.x, a_.pos.y);
    if (you.intersects(me)) {
        ++score;
        //消滅させる。
        return true;
    }

    //消滅しない
    return false;
}
