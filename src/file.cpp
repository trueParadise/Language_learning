//
// Created by LIU Yucheng on 2021/1/23.
//

#include <iostream>
#include <vector>
#include <string>
#include <cstdio>

using namespace std;
static string filepath {"../data/data.txt"};
static const int byte_of_count_ = 4;
static vector<uint32_t> buffer;
static vector<uint32_t> src = {4,1,2,3,4};

bool write_to(const char *file_path, vector<uint32_t>& data, uint32_t total_count) {
    FILE *fp = fopen(file_path, "wb");
    if (fp == nullptr) {
        printf("File open failed Path: %s\n", file_path);
        return false;
    }

    size_t count = data.size();
    if (total_count == 0) {
        total_count = count;
    }
    //先写入4byte，内容是此次写入record的总数
    size_t ret = fwrite(&total_count, byte_of_count_, 1, fp);
    if (ret != 1) {
        fclose(fp);
        printf("Write byte of count error. Byte: %zu\n", ret);
        return false;
    }

    uint32_t *data_ptr = &data[0];
    ret = fwrite((void*)data_ptr, sizeof(uint32_t), count, fp);
    if (ret != count) {
        fclose(fp);
        printf("Write data error. Write count: %zu\n", ret);
        return false;
    }
    fclose(fp);
    return false;
}

uint32_t read_from(const char* file_path, vector<uint32_t>& data) {
    FILE *fp = fopen(file_path, "rb");
    if (fp == nullptr) {
        printf("File open failed: %s\n", file_path);
        return 0;
    }

    uint64_t result = 0;
    //只读一个byte
    //函数返回成功读取的对象数
    //源代码文件中的前4个字节是个数
    size_t ret = fread(&result, byte_of_count_, 1, fp);
    if (ret != 1) {
        fclose(fp);
        printf("Read byte of count error. Byte: %zu\n", ret);
        return 0;
    }

    if (result == 0) {
        printf("Read count is zero.\n");
        return 0;
    }

    data.resize(result);
    //指向数组的第一个字节
    uint32_t * data_ptr = &data[0];
    fread((void*)data_ptr, sizeof(uint32_t), result, fp);
    if ((uint32_t)ret != result) {
        fclose(fp);
        printf("Read data error. Read count: %zu\n", ret);
        return 0;
    }
    fclose(fp);
    cout<<result<<endl;
    return result;
}

int main() {
    write_to(filepath.c_str(), src, 0);
    read_from(filepath.c_str(), buffer);
    for (int i = 0; i<buffer.size(); i++) {
        cout<<buffer[i]<<endl;
    }
    return 0;
}
