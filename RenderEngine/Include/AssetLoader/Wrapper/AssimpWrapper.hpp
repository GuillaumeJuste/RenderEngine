#pragma once

#ifndef RENDERENGINE_ASSIMPWRAPPER
#define RENDERENGINE_ASSIMPWRAPPER

#include "AssetLoader/RawAsset/Mesh/RawMesh.hpp"
using namespace Loader;

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

namespace Wrapper
{
    /**
     * @brief wrapper for assimp library functions
    */
    class AssimpWrapper
    {
    private:
        /// assimp importer instance
        static Assimp::Importer importer;

        /// default constructor
        AssimpWrapper() = default;

        /**
         * @brief process loaded mesh and fill _output with its data
         * @param _scene assimp scene containing loaded data
         * @param _output mesh data structure
         * @return true if the mesh was properly loaded
        */
        static bool ProcessMesh(const aiScene* _scene, RawMesh& _output);

    public:

        /**
         * @brief Load a mesh from a file path
         * @param _filePath file path to load from
         * @param _output output mesh data structure
         * @return true if the mesh was properly loaded 
        */
        static bool LoadMesh(const std::string& _filePath, RawMesh& _output);
    };
}

#endif