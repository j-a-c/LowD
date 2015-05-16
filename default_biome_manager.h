#ifndef DEFAULT_BIOME_MANAGER_H
#define DEFAULT_BIOME_MANAGER_H

#include <vector>

#include "biome_manager.h"    

class DefaultBiomeManager : public BiomeManager
{
    public:
        DefaultBiomeManager();
        ~DefaultBiomeManager();

        Biome* get_biome(int x, int y, int z) override;
    
    private:
        std::vector<Biome*> biomes;
};

#endif
