#include "stdafx.h"
#include "SaveData.h"
#include "json/json.hpp"
#include <iostream>
#include <fstream>

namespace
{
	// 保存データのファイルパス
	constexpr const char* SAVE_DATA_FILE_PATH = "Assets/saveData/savedata.json";
}

SaveData* SaveData::m_instance = nullptr; //初期化

SaveData::SaveData()
{
	// jsonからプログラム側で使用する形にする
	Serialize();
}

SaveData::~SaveData()
{
	// プログラム側で使用している形からjsonに戻す
	Deserialize();
}

void SaveData::Serialize()
{
	std::ifstream file(SAVE_DATA_FILE_PATH);
	if (!file.is_open()) {
		return;
	}

	nlohmann::json jsonRoot;
	file >> jsonRoot;

	auto resultTimeList = jsonRoot["resultScores"];
	for (auto& score : resultTimeList) {
		m_resultTimeList.push_back(score);
	}
}

void SaveData::Deserialize()
{
	std::ofstream file(SAVE_DATA_FILE_PATH);

	nlohmann::json resultTimeList;
	for (auto& score : m_resultTimeList) {
		resultTimeList.push_back(score);
	}

	nlohmann::json jsonRoot;
	jsonRoot["resultScores"] = resultTimeList;

	file << jsonRoot.dump(4);  // 4 はインデント（見やすくする）
	file.close();
}