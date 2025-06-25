#pragma once

class ItemExplanation : public IGameObject
{
public:
	//画像を表示
	void DrawImage();
	ItemExplanation();
	~ItemExplanation();
	bool Start()override;
	void Update()override;
	//テキストを設定
	void SetText(int base, int derivative);
	//画像を非表示へ
	void HideImage();
	void Render(RenderContext& rc)override;

private:

	SpriteRender m_itemImageBack;		//アイテムの説明画像の背景
	FontRender m_itemImageText;			//アイテムの説明のテキスト

	float m_elapsedTime = 0.0f;	//画像の表示時間

	bool m_isDrawImage = false;	//画像を描画するかどうかのフラグ
};

