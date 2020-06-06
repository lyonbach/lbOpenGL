#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "Containers.h"


class OBJLoader
{
    private:
        const char* m_FilePath;

    private:
        void GetOneIndicesInfo(std::string &line, std::vector<std::string>& infos);
        void GetOneAttribute(std::vector<float>* attributeList, std::string line, std::string prefix);
        void GetAttributes(std::vector<std::string>* lines, std::vector<float>*vertices,
            std::vector<float>* uvs, std::vector<float>* normals);
        void InitializeModelData(std::vector<std::string>* lines);
        void GetUniques(std::unordered_map<std::string, UniqueBuffer>* map,
            std::vector<std::string>& lines, std::vector<float>* vertices,
            std::vector<float>* uvs, std::vector<float>* normals);
        void GenerateNonIndexedBuffer(std::unordered_map<std::string, UniqueBuffer> map,
            std::vector<std::string>* lines, ModelData* modelData);
        void GenerateIndexedBuffer(std::unordered_map<std::string, UniqueBuffer>& map,
            std::vector<std::string>& lines, ModelData* modelData);

    public:
        OBJLoader(const char* filePath);
        ~OBJLoader();
        void Load(ModelData* modelData, bool indexed = true);
};
