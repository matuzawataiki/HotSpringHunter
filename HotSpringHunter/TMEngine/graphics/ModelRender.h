#pragma once

namespace nsTMEngine {
	/// <summary>
	/// スキンモデルレンダー
	/// </summary>
	class ModelRender : public IRenderer
	{
	public:
		ModelRender();
		~ModelRender();
		
		/// <summary>
		/// モデルの初期化用関数
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		/// <param name="animationeClips">アニメーションクリップ</param>
		/// <param name="numAnimationClips">アニメーションの数</param>
		/// <param name="enModelUpAxiz">モデルの上方向</param>
		void Init(
			const char* filePath,
			AnimationClip* animationeClips = nullptr,
			int numAnimationClips = 0,
			bool islighting = true,
			EnModelUpAxis enModelUpAxiz = enModelUpAxisZ);

		/// <summary>
		/// アニメーションの再生
		/// </summary>
		/// <param name="animNo">アニメーションクリップの番号</param>
		/// <param name="interpolateTime">補完時間(単位：秒)</param>
		void PlayAnimation(int animNo, float interpolateTime = 0.0f) {
			m_animation.Play(animNo, interpolateTime);
		}

		/// <summary>
		/// アニメーションが再生中か
		/// </summary>
		/// <returns></returns>
		bool IsPlayAnimation() const {
			return m_animation.IsPlaying();
		}

		/// <summary>
		/// 描画処理。
		/// </summary>
		void Draw(RenderContext& rc);

		/// <summary>
		/// 座標の設定
		/// </summary>
		/// <param name="pos">位置</param>
		/// <param name="rot">回転</param>
		/// <param name="sce">大きさ</param>
		void SetTRS(const Vector3& pos, const Quaternion& rot, const Vector3& sca) {
			m_pos = pos;
			m_rot = rot;
			m_sca = sca;
		}

		/// <summary>
		/// 位置の設定
		/// </summary>
		/// <param name="pos"></param>
		void SetPosition(const Vector3& pos)
		{
			m_pos = pos;
		}

		/// <summary>
		/// 位置の設定
		/// </summary>
		/// <param name="x"></param>
		/// <param name="y"></param>
		/// <param name="z"></param>
		void SetPosition(const float& x, const float& y, const float& z)
		{
			m_pos = Vector3(x, y, z);
		}

		/// <summary>
		/// 回転の設定
		/// </summary>
		/// <param name="rot"></param>
		void SetRotation(const Quaternion& rot) 
		{
			m_rot = rot;
		}

		/// <summary>
		/// 大きさの設定
		/// </summary>
		/// <param name="sca"></param>
		void SetScale(const Vector3& sca)
		{
			m_sca = sca;
		}
		void SetScale(const float& x, const float& y, const float& z)
		{
			m_sca = Vector3(x, y, z);
		}

		/// <summary>
		/// モデルを取得。
		/// </summary>
		/// <returns>モデル</returns>
		Model& GetModel()
		{
			return m_model;
		}

		void AddAnimationEvent(AnimationEventListener eventListener)
		{
			m_animation.AddAnimationEventListener(eventListener);
		}

		/// <summary>
		/// 更新処理。
		/// </summary>
		void Update();

	private:
		/// <summary>
		/// シャドウマップへの描画パスから呼ばれる処理。
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		/// <param name="ligNo">ライト番号</param>
		/// <param name="shadowMapNo">シャドウマップ番号</param>
		/// <param name="lvpMatrix">ライトビュープロジェクション行列</param>
		void OnRenderShadowMap(RenderContext& rc) override;

		/// <summary>
		/// スケルトンの初期化用関数
		/// </summary>
		/// <param name="filePath"></param>
		void InitSkeleton(const char* filePath);

		/// <summary>
		/// アニメーションの初期化用変数
		/// </summary>
		/// <param name="animtionClips"></param>
		/// <param name="numAnimationClips"></param>
		/// <param name="enModelUpAxis"></param>
		void InitAnimation(
			AnimationClip* animtionClips,
			int numAnimationClips,
			EnModelUpAxis enModelUpAxis
		);

		/// <summary>
		/// アニメーション済み頂点バッファ計算処理の初期化
		/// </summary>
		/// <param name="tkmFilePath"></param>
		/// <param name="enModelUpAxis"></param>
		void InitComputeAnimatoinVertexBuffer(
			const char* tkmFilePath,
			EnModelUpAxis enModelUpAxis
		);

		/// <summary>
		/// シェーダーエントリーポイントの設定
		/// </summary>
		/// <param name="modelInitData"></param>
		void SetupShaderEntryPointFunc(ModelInitData& modelInitData);

		//void ComputeAnimatoinVertexBuffer()

		/// <summary>
		/// 各種モデルのワールド行列の更新
		/// </summary>
		void UpdateWorldMatrixInModes() ;



	private:

		Vector3			m_pos = Vector3::Zero;			//位置
		Vector3			m_sca = Vector3::One;			//大きさ
		Quaternion		m_rot = Quaternion::Identity;	//回転

		Model			m_model;					//モデル
		Model			m_shadowModels;

		Skeleton		m_skeleton;					//ボーン
		Animation		m_animation;				//アニメーション
		AnimationClip*	m_animationClips = nullptr;	//アニメーションクリップ
		ConstantBuffer	m_drawShadowMapCameraParamCB;

		int				m_maxInstance = 1;			// 最大インスタンス数。
		int				m_numAnimationClips = 0;	//アニメーションクリップの数
		float			m_animationSpeed = 1.0f;	//アニメーションの再生速度
};
}
