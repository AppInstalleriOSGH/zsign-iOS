# ZSign-iOS

To build the ZSign static library run these commands.

```
clang++ -c *.cpp common/*.cpp -arch arm64 -isysroot $(xcrun --sdk iphoneos --show-sdk-path) -lcrypto $(pkg-config --cflags libcrypto) -Llib -lstdc++
ar rcs libzsign.a archo.o base64.o bundle.o common.o json.o macho.o openssl.o signing.o zsign.o
```
