#pragma once

class EnemyDetectionUI;

class UIManager : public IGameObject
{
public:
	struct EnemyInfomation
	{
		IGameObject* m_enemyOwner = nullptr;
		EnemyDetectionUI* m_ui = nullptr;
		Vector3 m_position = Vector3::Zero;
		bool m_isCreated = false;
		bool m_requestDelete = false;
		// 
		EnemyInfomation(IGameObject* enemyOwner, const Vector3& position)
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
		m_enemyInfomationList.push_back(EnemyInfomation(enemyOwner, position));
	}
	/// <summary>
	/// 敵の情報を設定
	/// </summary>
	void SetEnemyInformatino(IGameObject* enemyOwner, const Vector3& position)
	{
		EnemyInfomation* information = FindEnemyInfomation(enemyOwner);
		information->m_position = position;
	}
	/// <summary>
	/// 敵の情報を削除
	/// </summary>
	/// <param name="enemyOwner"></param>
	void RequestDelete(IGameObject* enemyOwner)
	{
		for (EnemyInfomation& information : m_enemyInfomationList)
		{
			if (information.m_enemyOwner == enemyOwner)
			{
				information.m_requestDelete = true;
				break;
			}
		}
	}
	/// <summary>
	/// 既に存在する敵の情報を探して取得
	/// </summary>
	EnemyInfomation* FindEnemyInfomation(IGameObject* enemyOwner)
	{
		for(EnemyInfomation& information : m_enemyInfomationList)
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
	Vector3 m_playerPosition = Vector3::Zero;				// プレイヤーの座標
	std::vector<EnemyInfomation> m_enemyInfomationList;		// 敵の情報リスト(座標とUIを生成したか)
};
