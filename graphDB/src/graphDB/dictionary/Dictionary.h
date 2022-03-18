#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace graph_db {

    /**
     * Implements the dictionary encoding of RDF resources as unique integers.
     */
    class Dictionary {
    public:
        /**
         * If an id for the given resource already exists, returns that id.
         * Otherwise creates a new id and returns it.
         * Note that the resource should include the delimiters, i.e. it should be of the form <s> or "c".
         */
        unsigned GetId(std::string resource);
        std::string_view GetResource(unsigned id);

    private:
        static unsigned count;
        std::unordered_map<std::string, unsigned> resourceToId;
        std::vector<std::string> idToResource;
    };

}
