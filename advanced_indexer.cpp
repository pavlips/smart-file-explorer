#include "advanced_indexer.h"
#include <vector>
#include <string>

int M = 16;
int ef_construction = 200;
int dim = 1536;  

advancedIndexer::advancedIndexer(int max_elements):space(dim), graph(new hnswlib::HierarchicalNSW<float>(&space, max_elements, M, ef_construction)) {}

advancedIndexer::advancedIndexer(int max_elements, fs::path indexer_path):space(dim)
{
    load(indexer_path);
}

advancedIndexer::~advancedIndexer() {
    delete graph;
}

int advancedIndexer::getPathsStored(){
    return label_to_path.size();
}

void advancedIndexer::addToGraph(std::vector<float>& embedding, fs::path p) {
    hnswlib::labeltype label = static_cast<hnswlib::labeltype>(hasher(p.string()));
    label_to_path[label] = p;

    graph->addPoint(embedding.data(), label);
}

std::vector<match> advancedIndexer::getClosestFromGraph(const std::vector<float>& embedding,int n_closest) {

    auto result = graph->searchKnn(embedding.data(), n_closest);

    std::vector<match> neighbours;

    while (!result.empty()) {
        neighbours.push_back({label_to_path[result.top().second],result.top().first});
        result.pop();
    }

    return neighbours;
}

void advancedIndexer::save(fs::path save_path) {
    graph->saveIndex(save_path.string()); 

    std::ofstream save_stream(save_path.string() + "_labels.txt");
    for (const auto& [label, path] : label_to_path) {
        save_stream << label << "," << path.string() << "\n"; 
    }
    save_stream.close();

}

void advancedIndexer::load(fs::path load_path) {
    graph = new hnswlib::HierarchicalNSW<float>(&space, load_path.string()); 

    label_to_path.clear();  

    std::ifstream load_stream(load_path.string() + "_labels.txt");
    std::string line;


    while (std::getline(load_stream, line)) {
        size_t comma_pos = line.find(',');
        hnswlib::labeltype label = std::stoull(line.substr(0, comma_pos));
        fs::path path = line.substr(comma_pos + 1);  
        label_to_path[label] = path;  
    }
    load_stream.close();
}

bool advancedIndexer::checkIfInGraph(fs::path p)
{
    hnswlib::labeltype label = static_cast<hnswlib::labeltype>(hasher(p.string()));

    if (label_to_path.find(label) != label_to_path.end()){
        return true;
    }
    else{
        return false;
    }
}
