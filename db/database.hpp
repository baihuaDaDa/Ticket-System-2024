#ifndef B_PLUS_TREE_DATABASE_HPP
#define B_PLUS_TREE_DATABASE_HPP

#include <iostream>
#include <fstream>
#include <filesystem>
#include "STLite/list.hpp"
#include "STLite/map.hpp"

namespace baihua {

    using std::string;
    using std::fstream;
    using std::ifstream;
    using std::ofstream;

    template<class T, int block_size = 1, int info_len = 0>
    class Database {
    private:
        string filename;
        fstream file;
        int sizeofT = sizeof(T);

    public:
        Database() = default;

        ~Database() = default;

        Database(const string &_filename) : filename(_filename) {}

        bool isFileExist() {
            ifstream tmp_file(filename);
            return tmp_file.good();
        }

        void initialize(int info = 0, const string &FN = "") {
            if (FN != "") filename = FN;
            file.open(filename, std::ios::out | std::ios::binary);
            for (int i = 0; i < info_len; ++i)
                file.write(reinterpret_cast<char *>(&info), sizeof(int));
            file.close();
        }

        void clear(int info = 0, const string &FN = "") {
            std::filesystem::remove(filename);
            initialize(info, FN);
        }

        // Read the @n-th info into @tmp. (1-base)
        void ReadInfo(int &tmp, int n) {
            if (n > info_len) return;
            file.open(filename, std::ios::in | std::ios::binary);
            file.seekg((n - 1) * sizeof(int));
            file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
            file.close();
        }

        // Write @tmp into the @n-th info. (1-base)
        void WriteInfo(int tmp, int n) {
            if (n > info_len) return;
            file.open(filename, std::ios::out | std::ios::in | std::ios::binary);
            file.seekp((n - 1) * sizeof(int));
            file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
            file.close();
        }

        int SingleAppend(T &t) {
            file.open(filename, std::ios::out | std::ios::in | std::ios::binary);
            file.seekp(0, std::ios::end);
            int index = (int(file.tellp()) - info_len * sizeof(int)) / sizeofT;
            file.write(reinterpret_cast<char *>(&t), sizeofT);
            file.close();
            return index;
        }

        int BlockAppend(T *t) {
            file.open(filename, std::ios::out | std::ios::in | std::ios::binary);
            file.seekp(0, std::ios::end);
            int block_num = (int(file.tellp()) - info_len * sizeof(int)) / sizeofT / block_size;
            file.write(reinterpret_cast<char *>(t), sizeofT * block_size);
            file.close();
            return block_num;
        }

        int RangeAppend(T *t, int size) {
            file.open(filename, std::ios::out | std::ios::in | std::ios::binary);
            file.seekp(0, std::ios::end);
            int head_index = (int(file.tellp()) - info_len * sizeof(int)) / sizeofT;
            file.write(reinterpret_cast<char *>(t), sizeofT * size);
            file.close();
            return head_index;
        }

        // Update the data at @index with the value of @t. (0-base)
        void SingleUpdate(T &t, int index) {
            file.open(filename, std::ios::out | std::ios::in | std::ios::binary);
            file.seekp(index * sizeofT + info_len * sizeof(int));
            file.write(reinterpret_cast<char *>(&t), sizeofT);
            file.close();
        }

        // Update the data at @index with the @size values after the pointer @t.
        void BlockUpdate(T *t, int block_num) {
            file.open(filename, std::ios::out | std::ios::in | std::ios::binary);
            file.seekp(block_num * block_size * sizeofT + info_len * sizeof(int));
            file.write(reinterpret_cast<char *>(t), block_size * sizeofT);
            file.close();
        }

        void RangeUpdate(T *t, int size, int head_index) {
            file.open(filename, std::ios::out | std::ios::in | std::ios::binary);
            file.seekp(head_index * sizeofT + info_len * sizeof(int));
            file.write(reinterpret_cast<char *>(t), size * sizeofT);
            file.close();
        }

        // Read the data at @index into @t
        void SingleRead(T &t, int index) {
            file.open(filename, std::ios::in | std::ios::binary);
            file.seekg(index * sizeofT + info_len * sizeof(int));
            file.read(reinterpret_cast<char *>(&t), sizeofT);
            file.close();
        }

        // Read the data at @index into the space the pointer @t points to, and the size of the data is @size.
        void BlockRead(T *t, int block_num) {
            file.open(filename, std::ios::in | std::ios::binary);
            file.seekg(block_num * block_size * sizeofT + info_len * sizeof(int));
            file.read(reinterpret_cast<char *>(t), block_size * sizeofT);
            file.close();
        }

        void RangeRead(T *t, int size, int head_index) {
            file.open(filename, std::ios::in | std::ios::binary);
            file.seekg(head_index * sizeofT + info_len * sizeof(int));
            file.read(reinterpret_cast<char *>(t), size * sizeofT);
            file.close();
        }

    };

}

#endif //B_PLUS_TREE_DATABASE_HPP
