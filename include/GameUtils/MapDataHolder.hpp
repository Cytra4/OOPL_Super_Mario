#ifndef MAPDATAHOLDER_HPP
#define MAPDATAHOLDER_HPP

#include "pch.hpp"

//Basically a place where map data is stored
//All the data's are stored in MapDataHolder.cpp (Prevent multiple definition)

namespace MapDataHolder{
    std::vector<std::vector<int>> GetBlockData(std::string level);

    std::vector<std::vector<int>> GetFloorData(std::string level);

    std::vector<std::vector<float>> GetPipeData(std::string level);

    std::vector<std::vector<double>> GetHiddenPipeData(std::string level);

    glm::vec2 GetFlagPosition(std::string level);

    glm::vec2 GetMarioPosition(std::string level);

    glm::vec2 GetCastlePosition(std::string level);

    glm::vec2 GetCastleFlagPosition(std::string level);

    std::string GetSecretLevel(std::string level);

    std::string GetNextLevel(std::string level);

    std::vector<std::vector<int>> GetPiranhaSetting(std::string level);
};

#endif