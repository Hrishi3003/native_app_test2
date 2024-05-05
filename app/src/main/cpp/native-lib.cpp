// native-lib.cpp
#include <jni.h>
#include <string>
#include <android/asset_manager_jni.h>
#include <android/log.h>

extern "C" JNIEXPORT void JNICALL
Java_com_example_native_1app_1test2_MainActivity_00024Companion_readMatrix(JNIEnv *env, jobject /* this */, jobject assetManager) {
    // Open the matrix.txt file from the assets folder
    AAssetManager *nativeAssetManager = AAssetManager_fromJava(env, assetManager);
    AAsset *asset = AAssetManager_open(nativeAssetManager, "matrix.txt", AASSET_MODE_BUFFER);

    if (asset == nullptr) {
        __android_log_print(ANDROID_LOG_ERROR, "NativeApp", "Failed to open matrix.txt");
        return;
    }

    // Read the contents of the file
    const char *fileContents = static_cast<const char *>(AAsset_getBuffer(asset));
    off_t fileLength = AAsset_getLength(asset);

    // Process the file contents (e.g., parse the matrix)
    std::string matrixData(fileContents, fileLength);
    __android_log_print(ANDROID_LOG_INFO, "NativeApp", "Matrix data: %s", matrixData.c_str());

    // Clean up
    AAsset_close(asset);
}