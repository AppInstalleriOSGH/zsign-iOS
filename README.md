# zsign iOS

To build the zsign static library run these commands.

```
clang++ -c *.cpp common/*.cpp -arch arm64 -isysroot $(xcrun --sdk iphoneos --show-sdk-path) $(pkg-config --cflags libcrypto) -Llib -lcrypto -lstdc++
ar rcs libzsign.a archo.o base64.o bundle.o common.o json.o macho.o openssl.o signing.o zsign.o
```

To use zsign in an iOS app add libzsign.a and lib/libcrypto.a to your Xcode Project as static libraries then add this code to your header file. You'll also need to link stdc++ to your Xcode Project.

```
int zsign(char* appPath, char* p12Path, char* provPath, char* pass, char* bundleID, char* bundleVersion, char* displayName, char* tweakDylib);
```

## Credits

[@zhlynn](https://github.com/zhlynn) - zsign
