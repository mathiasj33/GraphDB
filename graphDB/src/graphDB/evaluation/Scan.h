#pragma once

#include "graphDB/index/IndexedTripleTable.h"

namespace graph_db::evaluation {

    using namespace index;

    /**
     * A scan that retrieves triples in a one-by-one fashion.
     */
    class Scan {
    public:
        explicit Scan(const IndexedTripleTable& table, unsigned first) : table(table), current(first) {}

        virtual ~Scan() = default;
        /**
         * Retrieves the next element into the given parameters.
         * Assumes that ReachedEnd() == false.
         */
        void GetNext(unsigned& sOut, unsigned& pOut, unsigned& oOut);

        [[nodiscard]] bool ReachedEnd() const;

    protected:
        /**
         * Return true if the current triple should be included in the result, and false otherwise.
         * Also sets {@done} if no more triples can match this scan.
         */
        virtual bool Matches(const Triple& triple) = 0;
        /**
         * Increases {@current} to point to the next element considered in this scan.
         */
        virtual void IncreaseCurrent() = 0;
        /**
        * Moves the {@current} pointer to the next element that will be returned in this scan.
        */
        void MoveToNextMatchingElement();

        const IndexedTripleTable& table;
        unsigned current;
        bool done = false;
    };

}
