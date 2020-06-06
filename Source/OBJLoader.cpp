
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

#include "OBJLoader.h"

OBJLoader::OBJLoader(const char* filePath)
    : m_FilePath(filePath)
{
}

OBJLoader::~OBJLoader()
{
}

void OBJLoader::InitializeModelData(std::vector<std::string>* lines)
{
    std::ifstream stream (m_FilePath);
    std::string line;

    while(std::getline(stream, line))
    {
        lines->push_back(line);
    }
    stream.close();
}

void OBJLoader::GetOneIndicesInfo(std::string &line, std::vector<std::string>& infos)
{
    std::stringstream lineStream(line);
    std::string oneInfo = "";
    while(std::getline(lineStream, oneInfo, ' '))
    {
        // Ignore face data indicator.
        if(oneInfo.at(0) == 'f') continue;
        infos.push_back(oneInfo);
    }
}

void OBJLoader::GetOneAttribute(std::vector<float>* attributesList, std::string line, std::string prefix)
{
    std::stringstream lineStream(line);
    std::string subString;
    while(std::getline(lineStream, subString, ' '))
    {
        if(subString == prefix) 
            continue;  // Ignore prefixes.
        float value = std::stof(subString);
        attributesList->push_back(value);
    }
}

void OBJLoader::GetAttributes(std::vector<std::string>* lines, std::vector<float>* vertices, std::vector<float>* uvs, std::vector<float>* normals)
{
    for(std::string line : *lines)
    {
        if(line.find("v ") != std::string::npos)
            GetOneAttribute(vertices, line, "v");
        if(line.find("vt ") != std::string::npos)
            GetOneAttribute(uvs, line, "vt");
        if(line.find("vn ") != std::string::npos)
            GetOneAttribute(normals,  line, "vn");
    }
}

void OBJLoader::GetUniques(std::unordered_map<std::string, UniqueBuffer>* map,
                           std::vector<std::string>& lines, std::vector<float>* vertices,
                           std::vector<float>* uvs, std::vector<float>* normals)
{
    unsigned int lineCount = 0;
    for(std::string line : lines)
    {
        if(line.at(0) != 'f') continue;
        std::stringstream subStream(line);
        std::string section;
        std::vector<float> targetArray;
        std::vector<float>* sourceArray;
        int arrayElementCount;

        unsigned int index = 0;
        while(std::getline(subStream, section, ' '))
        {
            if(section.at(0) == 'f') continue;

            std::stringstream indicesStream(section);
            std::string value;
            targetArray.clear();
            targetArray.shrink_to_fit();

            int indexCounter = 0;
            while(std::getline(indicesStream, value, '/'))
            {
                int idx = std::stoi(value) - 1;  // obj files use 1 based indexing.
                switch (indexCounter)
                {
                case 0:
                    arrayElementCount = 3;
                    sourceArray = vertices;
                    break;
                case 1:
                    arrayElementCount = 2;
                    sourceArray = uvs;
                    break;
                case 2:
                    arrayElementCount = 3;
                    sourceArray = normals;
                    break;
                }
                for(int i = 0; i < arrayElementCount; i++)
                {
                    targetArray.push_back(sourceArray->at(idx * arrayElementCount + i));
                }

                indexCounter++;
            }
            UniqueBuffer buffer = {lineCount * 3 + index, targetArray};
            map->insert({section, buffer});
            index++;
        }
        lineCount++;
    }
}

void OBJLoader::GenerateNonIndexedBuffer(std::unordered_map<std::string, UniqueBuffer> map,
                                         std::vector<std::string>* lines, ModelData* modelData)
{
    // Create temporary objects to store the buffers.
    int faceCount = lines->size();

    std::vector<float> bufferData;
    int faceCounter = 0;
    for(std::string line : *lines)
    {
        if(line.at(0) != 'f') continue;  // Read only faces data.
        std::stringstream facesDataStream(line);
        std::string key;
        while(std::getline(facesDataStream, key, ' '))
        {
            // First Index Buffer is being filled.
            std::stringstream indexDataStream(key);
            std::string index;
            while(std::getline(indexDataStream, index, '/'))
            {
                if(key.at(0) == 'f') continue;  // Ignore face line indicator.
                modelData->indexBuffer.push_back(0);
                break;
            }

            if(key.at(0) == 'f') continue;  // Ignore face line indicator.
            UniqueBuffer buffer = map[key];

            // Then Different Vertex Buffers are being filled.
            for(int i = 0; i < buffer.data.size(); i++)
            {
                modelData->vertexBuffer.push_back(buffer.data.at(i));
            }
            faceCounter++;
        }
    }
}

void OBJLoader::GenerateIndexedBuffer(std::unordered_map<std::string, UniqueBuffer>& map,
                                      std::vector<std::string>& lines, ModelData* modelData)
{
    for (std::string line : lines)
    {
        std::vector<std::string> indicesInfo;
        if(line.at(0) != 'f') continue;
        
        // Split current line to get the individual indices information.
        GetOneIndicesInfo(line, indicesInfo);
        // For each indices information.
        for(std::string indices : indicesInfo)
        {
            // Get the prebuilt vertex data from uniques map and add it to the relevant output buffer.
            UniqueBuffer buffer = map[indices];
            
            for(int i = 0; i < buffer.data.size(); i++)
            {
                if(i < 3)
                    modelData->vertexCoordsBuffer.push_back(buffer.data.at(i));
                else if(i >= 3 && i < 5)
                    modelData->textureCoordsBuffer.push_back(buffer.data.at(i));
                else
                    modelData->vertexNormalsBuffer.push_back(buffer.data.at(i));
            }

            // Also fill in the index buffer data.
            modelData->indexBuffer.push_back(buffer.index);
        }
    }
}

void OBJLoader::Load(ModelData* modelData, bool indexed)
{
    std::cout << "[DEBUG][OBJ_LOADER]: Reading model..." << std::endl;
    std::unordered_map<std::string, UniqueBuffer> m;

    std::vector<std::string> lines;
    std::vector<float> vertices;
    std::vector<float> uvs;
    std::vector<float> normals;
    std::unordered_map<std::string, UniqueBuffer> uniquesMap;

    InitializeModelData(&lines);
    GetAttributes(&lines, &vertices, &uvs, &normals);
    GetUniques(&uniquesMap, lines, &vertices, &uvs, &normals);
    if (indexed)
        GenerateIndexedBuffer(uniquesMap, lines, modelData);
    else
        GenerateNonIndexedBuffer(uniquesMap, &lines, modelData);

    std::cout << "[DEBUG][OBJ_LOADER]: Model loaded." << std::endl;
}
