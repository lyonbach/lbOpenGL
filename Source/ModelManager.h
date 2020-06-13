#pragma once

#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "OBJLoader.h"

class ModelManager
{
    private:
        std::vector<const char*> m_ModelFilesList;
        std::vector<ModelData*> m_ModelDataList;

    private:
        std::string GetFileExtension(const char* filePath) const;
        void* GetLoader(std::string fileExtension);
        void LoadOne(const char* filePath, ModelData* modelData);

    public:
        ModelManager();
        ~ModelManager();
        void Load();
        void Push(const char* filePath);
        std::vector<const char*> GetFilesList() const;
        std::vector<ModelData*>  GetModelDataArray() const;
};
