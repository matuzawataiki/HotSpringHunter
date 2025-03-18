/*!
 * @brief	シンプルなモデルシェーダー。
 */

////////////////////////////////////////////////
// 定数
////////////////////////////////////////////////
//int MAX_DIRECTION_LIGHT = 4;	//ディレクションライトの最大数

////////////////////////////////////////////////
// ライト構造体
////////////////////////////////////////////////
struct DirectionLig
{
    float3 direction;	//方向
    float3 color;		//色
    float3 eyePos;		//視点の位置
};

////////////////////////////////////////////////
// 定数バッファ。
////////////////////////////////////////////////
//モデル用の定数バッファ
cbuffer ModelCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};

//追加
//ライト用の定数バッファ
cbuffer LightCb : register(b1)
{
    DirectionLig m_directionLig;	//ディレクションライト
}

////////////////////////////////////////////////
// 構造体
////////////////////////////////////////////////
//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn{
	int4  Indices  	: BLENDINDICES0;
    float4 Weights  : BLENDWEIGHT0;
};
//頂点シェーダーへの入力。
struct SVSIn{
	float4 pos 		: POSITION;		//モデルの頂点座標。
	float2 uv 		: TEXCOORD0;	//UV座標。
    float3 normal	: NORMAL;		//法線ベクトル
	SSkinVSIn skinVert;				//スキン用のデータ。
};
//ピクセルシェーダーへの入力。
struct SPSIn{
	float4 pos 			: SV_POSITION;	//スクリーン空間でのピクセルの座標。
	float2 uv 			: TEXCOORD0;	//uv座標。
    float3 normal		: NORMAL;		//法線ベクトル
    float3 worldPos : TEXCOORD1; // ワールド空間でのピクセルの座標

};

////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0);				//アルベドマップ
StructuredBuffer<float4x4> g_boneMatrix : register(t3);	//ボーン行列。
sampler g_sampler : register(s0);	//サンプラステート。

////////////////////////////////////////////////
//関数宣言
////////////////////////////////////////////////
//ディレクションライトの計算
float3 DirectionLightCalculation(SPSIn psIn, DirectionLig directionLig);

////////////////////////////////////////////////
// 関数定義。
////////////////////////////////////////////////

float3 DirectionLightCalculation(SPSIn psIn)
{
    float t;	//割合
	//法線とディレクションライトの向きの内積を求める
	t= dot(psIn.normal, m_directionLig.direction);	
	//向きを反転させる
    t *= -1.0;
    if (t < 0.0f)
    {
        t = 0;
    }
	//陰をつける
    float3 diffuseLig = m_directionLig.color * t;
	
	//ディレクションライトの向きの反射ベクトルを求める
    float3 refVec = reflect(m_directionLig.direction, psIn.normal);
	//サーフェイスから視点に向かうベクトルを作る
    float3 toEye = m_directionLig.eyePos - psIn.worldPos;
    toEye = normalize(toEye);
	//鏡面反射の強さを求める
    t = dot(refVec, toEye);
    if (t < 0.0f)
    {
        t = 0.0f;
    }
    t = pow(t, 5.0f);	//鏡面反射の強さを絞る
	//鏡面反射をつける
    float3 specularLig = m_directionLig.color * t;
	
	//拡散反射と鏡面反射を足して最終的な光を決める
    float3 finalLig = diffuseLig + specularLig;	
	
    return finalLig;
}
/// <summary>
//スキン行列を計算する。
/// </summary>
float4x4 CalcSkinMatrix(SSkinVSIn skinVert)
{
	float4x4 skinning = 0;	
	float w = 0.0f;
	[unroll]
    for (int i = 0; i < 3; i++)
    {
        skinning += g_boneMatrix[skinVert.Indices[i]] * skinVert.Weights[i];
        w += skinVert.Weights[i];
    }
    
    skinning += g_boneMatrix[skinVert.Indices[3]] * (1.0f - w);
	
    return skinning;
}

/// <summary>
/// 頂点シェーダーのコア関数。
/// </summary>
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
	SPSIn psIn;
	float4x4 m;
	if( hasSkin ){
		m = CalcSkinMatrix(vsIn.skinVert);
	}else{
		m = mWorld;
	}
	psIn.pos = mul(m, vsIn.pos);
    psIn.worldPos = psIn.pos;
	psIn.pos = mul(mView, psIn.pos);
	psIn.pos = mul(mProj, psIn.pos);

	psIn.uv = vsIn.uv;
	
	//追加
    psIn.normal = mul(mWorld, vsIn.normal);//法線を回転させる

	return psIn;
}

/// <summary>
/// スキンなしメッシュ用の頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
	return VSMainCore(vsIn, false);
}
/// <summary>
/// スキンありメッシュの頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSSkinMain( SVSIn vsIn ) 
{
	return VSMainCore(vsIn, true);
}
/// <summary>
/// ピクセルシェーダーのエントリー関数。
/// </summary>
float4 PSMain( SPSIn psIn) : SV_Target0
{
	float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);
	
	//追加
    float4 finalColor = albedoColor;
	
    finalColor.xyz *= DirectionLightCalculation(psIn);

	return finalColor;
}
