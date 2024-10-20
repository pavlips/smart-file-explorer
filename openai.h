#pragma once
#include <filesystem>
#include <vector>
#include <cstdlib>
namespace fs = std::filesystem;


std::string describeImage(fs::path p);

std::vector<float> getEmbedding(std::string description);

