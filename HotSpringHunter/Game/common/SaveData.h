#pragma once

class SaveData
{
private:
	SaveData();
	~SaveData();

	void Serialize();
	void Deserialize();


public:
	//リザルトタイムを取得(constを右に置くと関数内の数字は変わらない)
	float GetResultTime()const
	{
		return m_resultTime;
	}
	//リザルトタイムを設定
	//デフォルト引数(引数を描かなければここで設定した値が使われる。書けばいつも通りの引数として使用できる)
	void SetResultTime(const float time, const bool isSave = false)
	{
		m_resultTime = time;
		if (isSave) {
			//結果時間リストに追加
			m_resultTimeList.push_back(time);
		}
	}

	// リザルトタイムのリストを取得
	// ランキングなどを表示する際に使用する
	// 授業でならったvector使ってみた！便利！
	const std::vector<float>& GetResultTimeList() const
	{
		return m_resultTimeList;
	}
private:
	float m_resultTime;						//結果時間
	std::vector<float> m_resultTimeList;	//結果時間リスト。保存している情報


	/**
	 * シングルトン用
	 */
public:
/// <summary>
/// インスタンスを作る
/// </summary>
	static void CreateInstance()
	{
		if (m_instance == nullptr)
		{
			m_instance = new SaveData();
		}
	}
	
/// <summary>
/// インスタンスを取得
/// </summary>
	static SaveData* GetInstance()
	{
		return m_instance;
	}

/// <summary>
/// インスタンスを破棄
/// </summary>
	static void DestroyInstance()
	{
		if (m_instance != nullptr)
		{
			delete m_instance;
			m_instance = nullptr;
		}
	}

private:
	static SaveData* m_instance; //シングルトンインスタンス
};

