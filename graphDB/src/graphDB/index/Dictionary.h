#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace graph_db::index {

    /**
     * Implements the dictionary encoding of RDF resources as unique integers.
     */
    class Dictionary {
    public:
        Dictionary();
        /**
         * If an id for the given resource already exists, returns that id.
         * Otherwise creates a new id and returns it.
         * Note that the resource should include the delimiters, i.e. it should be of the form <s> or "c".
         */
        unsigned GetId(std::string resource);
        std::string_view GetResource(unsigned id) const;
        static unsigned INVALID_ID;

    private:
        unsigned count = 1;  // we start counting at 1, so 0 can be used as an invalid id
        std::unordered_map<std::string, unsigned> resourceToId;
        std::vector<std::string> idToResource;
    };

}
