#pragma once
#include "TMEnginePreCompile.h"
#include <iostream>
#include <fstream>

namespace nsTMEngine
{
    inline nlohmann::json LoadScene(const std::string& filepath)
    {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            return false;
        }

        nlohmann::json jsonRoot;
        file >> jsonRoot;

        return jsonRoot;
    }

}