#include "ModelManager.h"

ModelManager::ModelManager()
{
}

ModelManager::~ModelManager()
{
    for(ModelData* modelData : m_ModelDataList) delete modelData;
    m_ModelFilesList.clear();
    m_ModelDataList.clear();
}

void ModelManager::Load()
{
    std::cout << "[DEBUG][MODEL_LOADER]: Total " << m_ModelFilesList.size() <<" models to be loaded." << std::endl;
    for(int idx = 0; idx < m_ModelFilesList.size(); idx++)
    {
        ModelData* modelData = new ModelData();
        m_ModelDataList.push_back(modelData);
    }

    const char* filePath;
    for(int idx = 0; idx < m_ModelFilesList.size(); idx++)
    {
        filePath = m_ModelFilesList.at(idx);
        ModelData* modelData = m_ModelDataList.at(idx);
        LoadOne(filePath, modelData);
    }
}

void ModelManager::LoadOne(const char* filePath, ModelData* modelData)
{

    std::string ext = GetFileExtension(filePath);
    if (ext == OBJ)
    {
        std::cout << "[DEBUG][MODEL_LOADER]: Loading model:\n\t" << filePath << std::endl;
        OBJLoader loader(filePath);
        loader.Load(modelData);
    }
    else
    {
        std::cout << "[ERROR][MODEL_LOADER]: " << ext << " type is unsupported." << std::endl;
        std::cout << "\t" << filePath << std::endl;
    }
}

void ModelManager::Push(const char* filePath)
{
    bool itemExists = false;
    for(const char* file : m_ModelFilesList)
    {
        if (file == filePath)
            itemExists = true;
    }
    if(!itemExists)
        m_ModelFilesList.push_back(filePath);
}

std::string ModelManager::GetFileExtension(const char* filePath) const
{
    std::string tp = std::string(filePath);
    return tp.substr(tp.find_last_of('.') + 1);
}

std::vector<ModelData*> ModelManager::GetModelDataArray() const
{
    return m_ModelDataList;
}

std::vector<const char*> ModelManager::GetFilesList() const
{
    return m_ModelFilesList;
}