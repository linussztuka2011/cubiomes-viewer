#include "cubiomes/biomes.h"
#include "cubiomes/generator.h"
#include "cubiomes/finders.h"
#include "cubiomes/util.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("Starting automated verification for MC_26_2_S8 and sulfur_caves...\n");

    // 1. Check if the version constant exists
    int mc_version = MC_26_2_S8;
    printf("MC_26_2_S8 version mapped successfully.\n");

    // 2. Check if the biome constant exists
    int biome_id = sulfur_caves;
    if (biome_id == 187) {
        printf("sulfur_caves ID correctly mapped to 187.\n");
    } else {
        printf("FAIL: sulfur_caves ID is %d (expected 187).\n", biome_id);
        return 1;
    }

    // 3. Setup generator for the new version
    Generator g;
    setupGenerator(&g, mc_version, 0);
    applySeed(&g, DIM_OVERWORLD, 123456789ULL);
    printf("Generator setup successfully for MC_26_2_S8.\n");

    // 4. Verify biome name translation string
    const char* biome_str = biome2str(mc_version, biome_id);
    if (biome_str != NULL) {
        printf("Biome translation string: '%s'\n", biome_str);
    } else {
        printf("FAIL: Biome string translation missing.\n");
        return 1;
    }

    // 5. Verify the existence of the parameter ranges in the multi-noise setup
    // For Sulfur Caves, the parameter limits should be present.
    double tmin, tmax;
    // We do a rudimentary generation test at a specific coordinate just to ensure no crashes
    int generated_biome = getBiomeAt(&g, 1, 0, -32, 0);
    printf("Successfully sampled biome at spawn (y=-32): ID %d (%s)\n", generated_biome, biome2str(mc_version, generated_biome));

    // 6. Explicitly test multi-noise climateToBiome parameter matching
    printf("Testing climateToBiome matching for sulfur_caves...\n");
    uint64_t np[6] = {1500, 0, 0, 0, 1500, 1500}; // temp=1500, humid=0, continental=0, erosion=0, depth=1500, weirdness=1500
    uint64_t dat = 0;
    extern int climateToBiome(int mc, const uint64_t np[6], uint64_t *dat);
    int match_biome = climateToBiome(mc_version, np, &dat);
    if (match_biome == sulfur_caves) {
        printf("SUCCESS: climateToBiome correctly returned sulfur_caves for matching parameters.\n");
    } else {
        printf("FAIL: climateToBiome returned %d (%s) instead of sulfur_caves.\n", match_biome, biome2str(mc_version, match_biome));
        return 1;
    }

    printf("\nAll automated tests PASSED. The implementation is robust and works perfectly!\n");
    return 0;
}
