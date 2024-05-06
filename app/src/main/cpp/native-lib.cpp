#include <jni.h>
#include <string>
#include <android/asset_manager_jni.h>
#include <chrono>
#include <fstream>
#include <sstream>
#include <vector>

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_native_1app_1test2_MainActivity_00024Companion_readMatrix(JNIEnv *env, jobject /* this */, jobject assetManager) {
    // Open the matrix.txt file from the assets folder
    AAssetManager *nativeAssetManager = AAssetManager_fromJava(env, assetManager);
    AAsset *asset = AAssetManager_open(nativeAssetManager, "matrix.txt", AASSET_MODE_BUFFER);

    if (asset == nullptr) {
        return env->NewStringUTF("Failed to open matrix.txt");
    }

    // Read the contents of the file
    const char *fileContents = static_cast<const char *>(AAsset_getBuffer(asset));
    off_t fileLength = AAsset_getLength(asset);

    // Process the file contents (e.g., parse the matrix)
    std::string matrixData(fileContents, fileLength);

    // Close the asset
    AAsset_close(asset);

    // Parse the matrix data into a 2D array
    std::vector<std::vector<int>> matrix;
    std::istringstream iss(matrixData);
    std::string line;
    while (std::getline(iss, line)) {
        std::vector<int> row;
        std::istringstream rowStream(line);
        int num;
        while (rowStream >> num) {
            row.push_back(num);
        }
        matrix.push_back(row);
    }

    // Transpose the matrix
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    std::vector<std::vector<int>> transposedMatrix(matrix[0].size(), std::vector<int>(matrix.size()));
    for (size_t i = 0; i < matrix.size(); ++i) {
        for (size_t j = 0; j < matrix[0].size(); ++j) {
            transposedMatrix[j][i] = matrix[i][j];
        }
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

    // Convert the transposed matrix back to a string
    std::ostringstream oss;
    for (size_t i = 0; i < transposedMatrix.size(); ++i) {
        for (size_t j = 0; j < transposedMatrix[0].size(); ++j) {
            oss << transposedMatrix[i][j];
            if (j != transposedMatrix[0].size() - 1) {
                oss << " ";
            }
        }
        oss << "\n";
    }
    std::string transposedMatrixString = oss.str();

    // Write the transposed matrix to a new file in the app's internal storage
    std::ofstream outputFile("/data/data/com.example.native_app_test2/transposed_matrix.txt");
    if (outputFile.is_open()) {
        outputFile << transposedMatrixString;
        outputFile.close();
    } else {
        return env->NewStringUTF("Failed to write transposed matrix to file");
    }

    // Return the time taken as a Java string
    return env->NewStringUTF(("Time taken to transpose and write transposed matrix to file: " + std::to_string(duration.count()) + " milliseconds").c_str());
}
