
#include "../extern/beatsaber-hook/shared/utils/il2cpp-functions.hpp"

#include "../extern/beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "../extern/beatsaber-hook/shared/utils/utils.h"
#include "../extern/beatsaber-hook/include/modloader.hpp"
#include "../extern/BeatSaberQuestCustomUI-master/shared/customui.hpp"

#include <math.h>

#include <cmath>

using namespace il2cpp_utils;

static const Logger* logger;

Il2CppObject* _playCount;

MAKE_HOOK_OFFSETLESS(StandardLevelDetailView_RefreshContent, void, Il2CppObject* self) {
    StandardLevelDetailView_RefreshContent(self);
    bool ShowPlayerStats = *GetFieldValue<bool>(self, "_showPlayerStats");
    if(!ShowPlayerStats) return;
    Il2CppObject* Level = *GetFieldValue(self, "_level");
    Il2CppString* LevelID = *GetPropertyValue<Il2CppString*>(Level, "levelID");

    Il2CppObject* SelectedBeatmapDifficulty = *GetFieldValue(self, "_selectedDifficultyBeatmap");
    int Difficulty = *GetPropertyValue<int>(SelectedBeatmapDifficulty, "difficulty");
    Il2CppObject* beatMapData = *GetPropertyValue(SelectedBeatmapDifficulty, "beatmapData");
    Il2CppObject* parentDifficultyBeatmapSet = *GetPropertyValue(SelectedBeatmapDifficulty, "parentDifficultyBeatmapSet");
    Il2CppObject* beatmapCharacteristic = *GetPropertyValue(parentDifficultyBeatmapSet, "beatmapCharacteristic");

    Il2CppObject* PlayerData = *GetFieldValue(self, "_playerData");
    Il2CppObject* playerLevelStatsData = *RunMethod(PlayerData, "GetPlayerLevelStatsData", LevelID, Difficulty, beatmapCharacteristic);
    int Plays = *GetPropertyValue<int>(playerLevelStatsData, "playCount");
    //StandardLevelDetail_Transform
    Il2CppObject* StandardLevelDetailView_Transform = *GetPropertyValue(self, "transform");
    
}

extern "C" void setup(ModInfo& info) {
    info.id = "SongPlayHistory";
    info.version = "0.0.1";
    // Create logger
    static std::unique_ptr<const Logger> ptr(new Logger(info));
    logger = ptr.get();
    logger->info("Completed setup!");
}

extern "C"
void load() {
    INSTALL_HOOK_OFFSETLESS(StandardLevelDetailView_RefreshContent, il2cpp_utils::FindMethod("", "StandardLevelDetailView", "RefreshContent"));
}
