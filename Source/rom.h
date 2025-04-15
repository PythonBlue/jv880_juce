#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <array>

struct RomInfo {
  size_t length;
  const char *filename;
  const char *checksum;
  bool needsUnscramble;
  const char *checksumUnscrambled;
};

constexpr size_t romCount = 26;
extern RomInfo romInfos[romCount];

int getRomIndex(const char *filename);
bool loadRom(int romI, uint8_t *dst, std::array<uint8_t *, romCount> &cache);
bool preloadAll(std::array<uint8_t *, romCount> &cache);

void unscrambleRom(const uint8_t *src, uint8_t *dst, int len);
