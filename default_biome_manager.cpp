#include "default_biome_manager.h"

#include "biomes/wasteland.h"

DefaultBiomeManager::DefaultBiomeManager()
{
    biomes.push_back(new Wasteland());
}

DefaultBiomeManager::~DefaultBiomeManager()
{
    for (int i = 0; i < biomes.size(); i++)
    {
        delete biomes[i];
    }
}

Biome* DefaultBiomeManager::get_biome(int x, int y, int z){
    return biomes[0];
}
