#include "common/common.h"
#include "common/json.h"
#include "openssl.h"
#include "macho.h"
#include "bundle.h"

extern "C" int zsign(char* appPath, char* p12Path, char* provPath, char* pass, char* bundleID, char* bundleVersion, char* displayName);

int zsign(char* appPath, char* p12Path, char* provPath, char* pass, char* bundleID, char* bundleVersion, char* displayName) {
    bool bForce = false;
    bool bWeakInject = false;
    if (!IsFileExists(appPath)) {
        ZLog::ErrorV(">>> Invalid Path! %s\n", appPath);
        return -1;
    }
    ZSignAsset zSignAsset;
    if (!zSignAsset.Init("", p12Path, provPath, "", pass)) {
        return -1;
    }
    bool bEnableCache = true;
    ZAppBundle bundle;
    bool bRet = bundle.SignFolder(&zSignAsset, appPath, bundleID, bundleVersion, displayName, "", bForce, bWeakInject, bEnableCache);
    return bRet ? 0 : 1;
}
