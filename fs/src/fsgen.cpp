#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstring>
#include <cstdint>

/*
    Filesystem format:
        sectors 0..n = kernel
        sector n = fs_desc
        sectors n... = files
    fs_desc layout:
        xxxxxxxxxxxxx0ns
        x = name (13 bytes or less)
        n = sector # (sector after fs_desc is 0)
        s = # of sectors
*/

struct FsEntry {
    char name[11];
    uint16_t sector_num;
    uint16_t num_sectors;
    char* content;
};

FsEntry entries[32];

uint8_t fs_desc[512];

int main(int argc, const char* const* argv) {
    using std::ifstream;
    using std::ofstream;
    using std::ios;
    using std::filesystem::file_size;

    uint16_t sector_num = 0;
    size_t num_entries = argc - 1;
    
    for (size_t i = 0; i < num_entries; ++i) {
        FsEntry entry;
        auto name = argv[i + 1];
        size_t size = file_size(name);
        
        strncpy(entry.name, name, 13);
        entry.num_sectors = (size + 511) / 512;
        entry.sector_num = sector_num;
        entry.content = new char[entry.num_sectors * 512] { 0 };

        sector_num += entry.num_sectors;
        
        ifstream file(name, ios::in | ios::binary);
        file.read(entry.content, size);
        file.close();

        entries[i] = entry;
    }

    ofstream fs("fs.bin", ios::out | ios::binary);
    
    for (size_t i = 0; i < num_entries; ++i) {
        auto e = entries[i];
        strncpy((char*)fs_desc + i * 16, e.name, 13);
        fs_desc[i * 16 + 13] = 0x00;
        ((uint8_t*)fs_desc)[i * 16 + 14] = e.sector_num;
        ((uint8_t*)fs_desc)[i * 16 + 15] = e.num_sectors;
    }

    fs.write((char*)fs_desc, 512);

    for (size_t i = 0; i < num_entries; ++i) {
        auto e = entries[i];
        fs.write(e.content, e.num_sectors * 512);
    }

    fs.close();
    
    return 0;
}
