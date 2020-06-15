
#include "../extern/beatsaber-hook/shared/utils/il2cpp-functions.hpp"

#include "../extern/beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "../extern/beatsaber-hook/shared/utils/utils.h"
#include "../extern/beatsaber-hook/include/modloader.hpp"

#include <math.h>

#include <cmath>

using namespace il2cpp_utils;

static const Logger* logger;

struct SaberTransform {
    Vector3 pos;
    Vector3 rot;
};

/*
LeftHand = 4
RightHand = 5
*/

Vector3 getVelocity(Vector3 a, Vector3 b) {
    Vector3 newVector;
    newVector.x = a.x - b.x;
    newVector.y = a.y - b.y;
    newVector.z = a.z - b.z;
    return newVector;
}

SaberTransform rightVelocity;
SaberTransform leftVelocity;
SaberTransform currentRightSaber;
SaberTransform prevRightSaber;
SaberTransform currentLeftSaber;
SaberTransform prevLeftSaber;

Vector3 ThrowMultiplier = {1, 1, 1};

bool thrown = false;

float LeftSpin = 0.0f;
float RightSpin = 0.0f;
float Speed = 5.0f;
bool LeftSpinning = true;
bool RightSpinning = true;

float LeftThumbstickVal;
float RightThumbstickVal;

MAKE_HOOK_OFFSETLESS(PlayerController_Update, void, Il2CppObject* self) {
    PlayerController_Update(self);

    if(!LeftSpinning)
    {
        LeftSpin = 0;
        return;
    }
    if(!RightSpinning)
    {
        RightSpin = 0;
        return;
    }

    Il2CppObject* leftSaber = *il2cpp_utils::GetFieldValue(self, "_leftSaber");
    Il2CppObject* rightSaber = *il2cpp_utils::GetFieldValue(self, "_rightSaber");

    if(leftSaber != nullptr && rightSaber != nullptr) {
        logger->info("Saber's online");
        Il2CppObject* leftSaberTransform = nullptr;
        Il2CppObject* rightSaberTransform = nullptr;

        Il2CppClass* componentsClass = il2cpp_utils::GetClassFromName("", "Saber");
        leftSaberTransform = *il2cpp_utils::RunMethod(leftSaber, il2cpp_functions::class_get_method_from_name(componentsClass, "get_transform", 0));
        rightSaberTransform = *il2cpp_utils::RunMethod(rightSaber, il2cpp_functions::class_get_method_from_name(componentsClass, "get_transform", 0));

        if(leftSaberTransform != nullptr && rightSaberTransform != nullptr) {

            Il2CppClass* transformClass = il2cpp_utils::GetClassFromName("UnityEngine", "Transform");
            const MethodInfo* getMethod = il2cpp_functions::class_get_method_from_name(transformClass, "get_localPosition", 0);
            const MethodInfo* setMethod = il2cpp_functions::class_get_method_from_name(transformClass, "set_localPosition", 1);
            const MethodInfo* setRotate = il2cpp_functions::class_get_method_from_name(transformClass, "Rotate", 1);
            const MethodInfo* setTranslate = il2cpp_functions::class_get_method_from_name(transformClass, "Translate", 1);
            const MethodInfo* getLocalRotation = il2cpp_functions::class_get_method_from_name(transformClass, "get_localRotation", 0);
            const MethodInfo* setLocalRotation = il2cpp_functions::class_get_method_from_name(transformClass, "set_localRotation", 1);

                Vector3 rightSaberLocalPosition;
                rightSaberLocalPosition = *il2cpp_utils::RunMethod<Vector3>(rightSaberTransform, getMethod);
                Quaternion rightSaberLocalRotation;
            rightSaberLocalRotation = *il2cpp_utils::RunMethod<Quaternion>(rightSaberTransform, getLocalRotation);

            il2cpp_utils::RunMethod(leftSaberTransform, setRotate, Vector3{ 0, LeftSpin, 0});
            il2cpp_utils::RunMethod(rightSaberTransform, setRotate, Vector3{ 0, RightSpin, 0});
        }
    }
    //Spinning = LeftThumbstickVal > 0.08f;
    LeftSpin += Speed;
}
bool IsCurrentSaberSpinning;
MAKE_HOOK_OFFSETLESS(HandleNoteWasCut, void, Il2CppObject* self, Il2CppObject* noteController, Il2CppObject* noteCutInfo) {
    int SaberType = *GetPropertyValue<int>(noteCutInfo, "saberType");
    if(SaberType == 0)
    {
        IsCurrentSaberSpinning = LeftSpinning;
    }
    if(SaberType == 1)
    {
        IsCurrentSaberSpinning = RightSpinning;
    }
    
    if(!IsCurrentSaberSpinning)
    {
        HandleNoteWasCut(self, noteController, noteController);
    }
}

static bool isInMenu = false;
#define WAIT_TIME 175
static long long LastLeftThumbstickVertical = 0;
static long long LastRightThumbstickVertical = 0;

MAKE_HOOK_OFFSETLESS(VRPlatformHelper_Update, void, Il2CppObject* self) {
    VRPlatformHelper_Update(self);
    if (!isInMenu) return;

    long long currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();;
    float LeftthumbstickVertical = il2cpp_utils::RunMethod<float>("UnityEngine", "Input", "GetAxis", il2cpp_utils::createcsstr("VerticalRightHand")).value_or(0);
    float RightthumbstickVertical = il2cpp_utils::RunMethod<float>("UnityEngine", "Input", "GetAxis", il2cpp_utils::createcsstr("VerticalLeftHand")).value_or(0);


    if (RightthumbstickVertical < -0.5f && currentTime-LastRightThumbstickVertical > WAIT_TIME) {
        LastRightThumbstickVertical = currentTime;
        RightThumbstickVal = RightthumbstickVertical;
    }
    else
    {
        RightThumbstickVal = 0;
    }
    if (LeftthumbstickVertical < 0.5f && currentTime-LastLeftThumbstickVertical > WAIT_TIME) {
        LastLeftThumbstickVertical = currentTime;
        LeftThumbstickVal = LeftthumbstickVertical;
    }
    else
    {
        LeftThumbstickVal = 0;
    }
    
}

extern "C" void setup(ModInfo& info) {
    info.id = "Tricksaber";
    info.version = "0.0.0.0.0.0.0.0.0.0.1";
    // Create logger
    static std::unique_ptr<const Logger> ptr(new Logger(info));
    logger = ptr.get();
    logger->info("Completed setup!");
}

extern "C"
void load() {
    INSTALL_HOOK_OFFSETLESS(PlayerController_Update, FindMethodUnsafe("", "PlayerController", "Update", 0));
    INSTALL_HOOK_OFFSETLESS(HandleNoteWasCut, FindMethodUnsafe("", "BeatmapObjectManager", "HandleNoteWasCut", 2));
    //INSTALL_HOOK_OFFSETLESS(VRPlatformHelper_Update, il2cpp_utils::FindMethod("", "VRPlatformHelper", "Update"));
}
