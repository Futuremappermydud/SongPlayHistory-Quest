
#include "../extern/beatsaber-hook/shared/utils/il2cpp-functions.hpp"

#include "../extern/beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "../extern/beatsaber-hook/shared/utils/utils.h"
#include "../extern/beatsaber-hook/include/modloader.hpp"

#include <math.h>

#include <cmath>

using namespace il2cpp_utils;

static const Logger* logger;

MAKE_HOOK_OFFSETLESS(VRPlatformHelper_Update, void, Il2CppObject* self) {
    VRPlatformHelper_Update(self);

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
    INSTALL_HOOK_OFFSETLESS(VRPlatformHelper_Update, il2cpp_utils::FindMethod("", "VRPlatformHelper", "Update"));
}
