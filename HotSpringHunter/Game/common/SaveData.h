#pragma once

class SaveData
{
private:
	SaveData() {}
	~SaveData() {}

public:
	float GetResultTime()const  //リザルトタイムを取得(constを右に置くと関数内の数字は変わらない)
	{
		return m_resultTime;
	}
	void SetResultTime(const float time) //リザルトタイムを設定
	{
		m_resultTime = time;
	}
private:
	float m_resultTime; //結果時間

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

