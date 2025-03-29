/*!
 * @brief	シンプルなモデルシェーダー。
 */

////////////////////////////////////////////////
// 定数
////////////////////////////////////////////////
static const int MAX_POINT_LIGHT = 32;	//ポイントライトの最大数
static const int MAX_SPOT_LIGHT = 32;	//スポットライトの最大数

////////////////////////////////////////////////
// ライト構造体
////////////////////////////////////////////////
//ディレクションライト構造体
struct DirectionLig
{
    float3 direction;	//方向
    float3 color;		//色
};
//ポイントライト構造体
struct PointLig
{
    float3	position;	//位置
    int		use;		//使用状況
    float3	color;		//色
    float	range;		//影響範囲
};
//ポイントライトライト構造体
struct SpotLig
{
    float3  position;   //位置
    int     use;        //使用状況
    float3  color;      //色
    float   range;      //影響距離
    float3  direction;  //向き
    float   angle;      //影響範囲
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


//ライト用の定数バッファ
cbuffer LightCb : register(b1)
{
    DirectionLig	m_directionLig;					//ディレクションライト
    PointLig		m_pointLig[MAX_POINT_LIGHT];	//ポイントライト
    SpotLig         m_spotLig[MAX_SPOT_LIGHT];      //スポットライト
    int				m_numPointLig;					//ポイントライトの使用数
    float3          m_eyePos;                       //視点の位置
    int             m_numSpotLig;                   //スポットライトの使用数
    float3          m_ambientLight;                 //環境光
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
    float3 worldPos : TEXCOORD1;		// ワールド空間でのピクセルの座標

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
float3 CalcDirectionLight(SPSIn psIn);
//ポイントライトの計算
float3 CalcPointLight(SPSIn psIn, PointLig pointLig);
//スポットライト
float3 CalcSpotLight(SPSIn psIn, SpotLig spotLig);

//Lambert拡散反射光の計算
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal);
//Phon鏡面反射光の計算
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 normal, float3 worldPos);

////////////////////////////////////////////////
// 関数定義。
////////////////////////////////////////////////
/// <summary>
/// ディレクションライトの計算
/// </summary>
float3 CalcDirectionLight(SPSIn psIn)
{	
    //拡散反射
    float3 diffuseLig = CalcLambertDiffuse(m_directionLig.direction, m_directionLig.color, psIn.normal);
    //鏡面反射
    float3 specularLig = CalcPhongSpecular(m_directionLig.direction, m_directionLig.color, psIn.normal, psIn.worldPos);
    
    float3 finalLig = diffuseLig + specularLig;
	
    return finalLig;
}
/// <summary>
/// ポイントライトの計算
/// </summary>
float3 CalcPointLight(SPSIn psIn, PointLig pointLig)
{
    //ライトの位置からサーフェイスに向かう方向ベクトルの計算
    float3 ligDir = psIn.worldPos - pointLig.position;
    ligDir = normalize(ligDir);
    //拡散反射の計算
    float3 diffPoint = CalcLambertDiffuse(ligDir, pointLig.color, psIn.normal);
    //鏡面反射の計算
    float3 specPoint = CalcPhongSpecular(ligDir, pointLig.color, psIn.normal, psIn.worldPos);
    
    //距離を計算
    float distance = length(psIn.worldPos - pointLig.position);
    
    //影響力の計算
    float affect = 1.0f - 1.0f / pointLig.range * distance;    
    affect = max(0.0f, affect);
    affect = pow(affect, 3.0f);
    
    diffPoint *= affect;
    specPoint *= affect;
    
    return diffPoint + specPoint;

}
/// <summary>
/// ポイントライトの計算
/// </summary>
float3 CalcSpotLight(SPSIn psIn, SpotLig spotLig)
{
    //ライトの位置からサーフェイスに向かう方向ベクトルの計算
    float3 ligDir = psIn.worldPos - spotLig.position;
    ligDir = normalize(ligDir);
    //拡散反射の計算
    float3 diffPoint = CalcLambertDiffuse(ligDir, spotLig.color, psIn.normal);
    //鏡面反射の計算
    float3 specPoint = CalcPhongSpecular(ligDir, spotLig.color, psIn.normal, psIn.worldPos);
    
    //距離を計算
    float distance = length(psIn.worldPos - spotLig.position);
    
    //影響力の計算
    float affect = 1.0f - 1.0f / spotLig.range * distance;
    affect = max(0.0f, affect);
    affect = pow(affect, 3.0f);
    diffPoint *= affect;
    specPoint *= affect;
    
    //内積を求める
    float angle = dot(ligDir, spotLig.direction);
    //角度を求める
    angle = abs(acos(angle));
    
    affect = 1.0f - 1.0f / spotLig.angle * angle;
    affect = max(0.0f, affect);
    affect = pow(affect, 3.0f);
    diffPoint *= affect;
    specPoint *= affect;
    
    return diffPoint + specPoint;
}

/// <summary>
//Lambert拡散反射光の計算
/// </summary>
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal)
{
    //法線とディレクションライトの向きの内積を求める
    float t = dot(normal, lightDirection) * -1.0f;
    //0以下は0にする
    t = max(0.0f, t);
	//陰をつける
    float3 diffuseLig = lightColor * t;
    
    return diffuseLig;
}
/// <summary>
//Phon鏡面反射光の計算
/// </summary>
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 normal, float3 worldPos)
{
    //ディレクションライトの向きの反射ベクトルを求める
    float3 refVec = reflect(lightDirection, normal);
    
	//サーフェイスから視点に向かうベクトルを作る
    float3 toEye = m_eyePos - worldPos;
    toEye = normalize(toEye);
    
    float t;
	//鏡面反射の強さを求める
    t = dot(refVec, toEye);
    //0以下は0にする
    t = max(0.0f, t);
    //鏡面反射の強さを絞る
    t = pow(t, 5.0f);
    
	//鏡面反射をつける
    float3 specularLig = lightColor * t;
	
    return specularLig;
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
    float3 finalLig;
	//ディレクションライトの計算
    finalLig = CalcDirectionLight(psIn);
	
	//ポイントライトの使用状況の確認
    if (m_numPointLig > 0)
    {
        int count = 0;  //処理したポイントライトの数
        for (int i = 0; i < MAX_POINT_LIGHT; i++)
        {
            if (m_pointLig[i].use)
            {
                //ポイントライトの計算
                finalLig += CalcPointLight(psIn,m_pointLig[i]);
                count++;
            }
            if (count == m_numPointLig) //使用中のライトを処理し終えたらブレイク
            {
                break;
            }
        }

    }
    
    //ポイントライトの使用状況確認
    if (m_numSpotLig > 0)
    {
        int count = 0; //処理したポイントライトの数
        for (int i = 0; i < MAX_SPOT_LIGHT; i++)
        {
            if (m_spotLig[i].use)
            {
                //ポイントライトの計算
                finalLig += CalcSpotLight(psIn, m_spotLig[i]);
                count++;
            }
            if (count == m_numSpotLig) //使用中のライトを処理し終えたらブレイク
            {
                break;
            }
        }

    }
    
    //環境光の設定
    //finalLig += m_ambientLight;
    finalLig.x += 0.1f;
    finalLig.y += 0.1f;
    finalLig.z += 0.1f;
	
    //最終合成
    float4 finalColor = g_albedo.Sample(g_sampler, psIn.uv);
    finalColor.xyz *= finalLig;

	return finalColor;
}
