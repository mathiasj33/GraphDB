#include <iostream>
#include "graphDB/index/Dictionary.h"
#include "graphDB/index/IndexedTripleTable.h"

using namespace graph_db::index;

int main() {
    Dictionary dict;
    IndexedTripleTable table;
    table.Add(1, 3, 0);
    table.Add(1, 3, 0);
    table.Add(1, 3, 1);
    table.Add(1, 2, 0);
    table.Add(1, 4, 0);
    table.Add(1, 3, 2);
    table.Add(1, 2, 1);
    table.Add(7, 3, 0);
    table.Add(7, 4, 0);
}
