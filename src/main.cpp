
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

//TODO: Actually set these to not be null
Il2CppObject* _maxCombo;
Il2CppObject* _highScore;
Il2CppObject* _maxRank;

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
    Il2CppObject* StatsContainer = *GetFieldValue(self, "_playerStatsContainer");
    auto MaxRankValueText = *GetFieldValue(self, "_maxRankText");
    auto HighScoreValueText = *GetFieldValue(self, "_highScoreText");
    auto MaxComboValueText = *GetFieldValue(self, "_maxComboText");

    auto MaxRankGameObj = *GetPropertyValue(MaxRankValueText, "gameObject");
    auto HighScoreGameObj = *GetPropertyValue(HighScoreValueText, "gameObject");
    auto MaxComboGameObj = *GetPropertyValue(MaxComboValueText, "gameObject");
    _maxRank = *RunMethod(MaxRankGameObj, "GetComponent", createcsstr("RectTransform"));
    _maxCombo = *RunMethod(MaxComboGameObj, "GetComponent", createcsstr("RectTransform"));
    _highScore = *RunMethod(HighScoreGameObj, "GetComponent", createcsstr("RectTransform"));

    RunMethod(_maxCombo, "SetInsetAndSizeFromParentEdge", 0, -2.0f, 17.0f);
    RunMethod(_highScore, "SetInsetAndSizeFromParentEdge", 0, 15.0f, 17.0f);
    RunMethod(_maxRank, "SetInsetAndSizeFromParentEdge", 0, 32.0f, 16.0f);
    _playCount = *RunMethod("UnityEngine", "Object", "Instantiate", _maxRank, *GetPropertyValue(StatsContainer, "transform"));
    auto _playCountRect = *RunMethod(_playCount, "GetComponent", createcsstr("RectTransform"));
    //RunMethod(_playCountRect, "SetInsetAndSizeFromParentEdge", 0, 48.0f, 16.0f);
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
