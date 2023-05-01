# include <Siv3D.hpp>

enum class ID {
	NO_00,
	NO_01,
	NO_02
};

// ユーザデータとゲームのスコアを記録する構造体
struct GameScore
{
	String name;

	ID id;

	int32 score;

	// シリアライズに対応させるためのメンバ関数を定義する
	template <class Archive>
	void SIV3D_SERIALIZE(Archive& archive)
	{
		archive(name, (int32)id, score);
	}
};

void Main()
{
	{
		// 記録したいデータ
		const Array<GameScore> scores =
		{
			{ U"KeysFamily鍵山英一", ID::NO_00, 1000 },
			{ U"PinHousehold鑰山英一", ID::NO_01, 2000 },
			{ U"K/e.y@s,h*!ouse鑰山家", ID::NO_02, 3000 },
		};

		// バイナリファイルをオープン
		Serializer<BinaryWriter> writer{ U"tutorial4.bin" };

		if (not writer) // もしオープンに失敗したら
		{
			throw Error{ U"Failed to open `tutorial4.bin`" };
		}

		// シリアライズに対応したデータを記録
		writer(scores);

		// writer のデストラクタで自動的にファイルがクローズされる
	}

	// 読み込み先のデータ
	Array<GameScore> scores;
	{
		// バイナリファイルをオープン
		Deserializer<BinaryReader> reader{ U"tutorial4.bin" };

		if (not reader) // もしオープンに失敗したら
		{
			throw Error{ U"Failed to open `tutorial4.bin`" };
		}

		// バイナリファイルからシリアライズ対応型のデータを読み込む
		// （Array は自動でリサイズが行われる）
		reader(scores);

		// reader のデストラクタで自動的にファイルがクローズされる
	}

	// 読み込んだスコアを確認
	for (const auto& score : scores)
	{
		Print << U"{}(id: {}): {}"_fmt(score.name, score.id, score.score);
	}

	while (System::Update())
	{

	}
}
