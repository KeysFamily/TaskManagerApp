#pragma once
//-----------------------------------------------------------------------------
//以下のクラスはGPG環境のプログラムをもとに作成したSiv3D用のエンジンである
//SivResource    タスクリソース基本クラス
//SivTask        Task基本クラス（タスクの処理）
//SivEngine        ゲームエンジン用
//-----------------------------------------------------------------------------

class SivResource {
protected:
    //コンストラクタ
    SivResource(){}
public:
    //デストラクタ
    ~SivResource(){}

    //必ずオーバーライド（再定義）しなければならない
    virtual bool Initialize() = 0;
    virtual bool Finalize() = 0;

    using SP = std::shared_ptr<SivResource>;
    using WP = std::weak_ptr<SivResource>;

};

class SivTask
{
protected:
    bool dead;            //タスクが死ぬ場合trueにする
public:
    String groupName;    //タスクグループ名
    String name;        //タスク名


    using SP = std::shared_ptr<SivTask>;
    using WP = std::weak_ptr<SivTask>;

    //処理系
    virtual void draw() const {};    //描画
    virtual void update() {};        //アップデート

    bool isDead();                    //消去するか？

    bool Kill();                    //自身を消滅させる

};

class SivEngine {
    Array<SivTask::SP> task;        //実行用のタスク
    Array<SivTask::SP> add_Tasks;    //登録待ちのタスク
public:
    void TaskUpDate();                //タスクの更新処理（描画も行う）

    void TaskAdd();                    //タスクの追加
    void TaskRemove();                //タスクの消滅

    //bool Kill_GN(const String& gn_);//同名の物をすべて消滅させる。
    template <class T>    std::shared_ptr<T> GetTask(String gn_, String n_);//指定したタスクを1つ返す
    template <class T>  std::shared_ptr<Array<std::shared_ptr<T>>> GetTasks(String gn_);
}; 


