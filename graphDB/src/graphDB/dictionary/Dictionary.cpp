#include "Dictionary.h"

namespace graph_db {
    unsigned Dictionary::count = 0;

    unsigned Dictionary::GetId(std::string resource) {
        auto search = resourceToId.find(resource);
        if (search != resourceToId.end()) {
            return search->second;
        }
        resourceToId[resource] = count;
        idToResource.push_back(std::move(resource));
        return count++;
    }

    std::string_view Dictionary::GetResource(unsigned int id) {
        return idToResource[id];
    }
}