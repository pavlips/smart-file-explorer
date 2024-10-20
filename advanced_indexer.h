#pragma once
#include <filesystem>
#include "vendor/hnswlib/hnswlib.h"
namespace fs = std::filesystem;

typedef std::pair<fs::path,float> match;

struct advancedIndexer {
    hnswlib::L2Space space;  
    hnswlib::HierarchicalNSW<float>* graph; 
    std::unordered_map<hnswlib::labeltype,fs::path> label_to_path;
    std::hash<std::string> hasher;

    advancedIndexer(int max_elements);
    advancedIndexer(int max_elements, fs::path indexer_path);

    int getPathsStored();

    void addToGraph(std::vector<float>& embedding, fs::path p);

    std::vector<match> getClosestFromGraph(const std::vector<float>& embedding,int  n_closest);

    void save(fs::path p);
    
    void load(fs::path p);

    bool checkIfInGraph(fs::path);

    ~advancedIndexer();
};
