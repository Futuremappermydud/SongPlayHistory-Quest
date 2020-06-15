
#include "../extern/beatsaber-hook/shared/utils/il2cpp-functions.hpp"

#include "../extern/beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "../extern/beatsaber-hook/shared/utils/utils.h"
#include "../extern/beatsaber-hook/include/modloader.hpp"

#include <math.h>

#include <cmath>

using namespace il2cpp_utils;

static const Logger* logger;

float Spin1 = 0.0f;
float Speed = 5.0f;

MAKE_HOOK_OFFSETLESS(PlayerController_Update, void, Il2CppObject* self) {
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

            il2cpp_utils::RunMethod(leftSaberTransform, setRotate, Vector3{ 0, Spin1, 0});
            il2cpp_utils::RunMethod(rightSaberTransform, setRotate, Vector3{ 0, Spin1, 0});
        }
    }
    Spin1 += Speed;
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
}