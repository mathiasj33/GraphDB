#include "Dictionary.h"

namespace graph_db::index {
    unsigned Dictionary::INVALID_ID = 0;

    Dictionary::Dictionary() {
        idToResource.emplace_back("INVALID");  // id 0 will never be used by an actual resource
    }

    unsigned Dictionary::GetId(std::string resource) {
        auto search = resourceToId.find(resource);
        if (search != resourceToId.end()) {
            return search->second;
        }
        resourceToId[resource] = count;
        idToResource.push_back(std::move(resource));
        return count++;
    }

    std::string_view Dictionary::GetResource(unsigned id) const {
        return idToResource[id];
    }
}