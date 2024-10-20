#include "process_file.h"
#include <cstring>
#include "openai.h"


void processFile(fs::path p, advancedIndexer* indexer) { // for now assume image

    std::string extension = p.extension();

    if ((extension != ".png") && (extension != ".jpg")){
        return;
    }

    std::cout << "processing " << p.string() << std::endl;
    std::string imageDescription = describeImage(p);
    std::vector<float> embedding = getEmbedding(imageDescription);
    indexer->addToGraph(embedding,p);
}
