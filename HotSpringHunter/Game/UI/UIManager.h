#pragma once

class EnemyDetectionUI;

class UIManager : public IGameObject
{
public:
	struct EnemyInformation
	{
		IGameObject* m_enemyOwner = nullptr;
		EnemyDetectionUI* m_ui = nullptr;
		Vector3 m_position = Vector3::Zero;
		bool m_isCreated = false;				//警告シグナルを作ったか
		bool m_isUpdate = true;			//
		// 
		EnemyInformation(IGameObject* enemyOwner, const Vector3& position)
			: m_enemyOwner(enemyOwner)
			,m_position(position)
		{
		}
	};

public:
	UIManager();
	~UIManager();
	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override;

	/// <summary>
	/// 警告シグナルを更新
	/// NOTE: 中で必要であれば生成をしている
	/// </summary>
	void UpdateTriangle();
	/// <summary>
	/// 警告シグナルを破棄
	/// </summary>
	void DeleteTriangle();

public:
	/// <summary>
	/// 敵の情報を追加
	/// </summary>
	void AddEnemyInformation(IGameObject* enemyOwner, const Vector3& position)
	{
		m_enemyInformationList.push_back(EnemyInformation(enemyOwner, position));
	}
	/// <summary>
	/// 敵の情報を設定
	/// </summary>
	void SetEnemyInformation(IGameObject* enemyOwner, const Vector3& position)
	{
		EnemyInformation* information = FindEnemyInformation(enemyOwner);
		information->m_position = position;
		information->m_isUpdate = true;
	}
	/// <summary>
	/// 敵の情報を削除
	/// </summary>
	/// <param name="enemyOwner"></param>
	void RequestDelete(IGameObject* enemyOwner)
	{
		for (EnemyInformation& information : m_enemyInformationList)
		{
			if (information.m_enemyOwner == enemyOwner)
			{
				information.m_isUpdate = true;
				break;
			}
		}
	}
	/// <summary>
	/// 既に存在する敵の情報を探して取得
	/// </summary>
	EnemyInformation* FindEnemyInformation(IGameObject* enemyOwner)
	{
		for(EnemyInformation& information : m_enemyInformationList)
		{
			if(information.m_enemyOwner == enemyOwner)
			{
				return &information;
			}
		}
		return nullptr;
	}

	void SetPlayerPosition(const Vector3& position)
	{
		m_playerPosition = position;
	}

private:
	Vector3 m_playerPosition = Vector3::Zero;					// プレイヤーの座標
	std::vector<EnemyInformation> m_enemyInformationList;		// 敵の情報リスト(座標とUIを生成したか)
};
