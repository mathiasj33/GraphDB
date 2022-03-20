#include <iostream>
#include "graphDB/evaluation/Scan.h"
#include "graphDB/index/IndexedTripleTable.h"

using namespace graph_db::index;
using namespace graph_db::evaluation;

int main() {
    IndexedTripleTable table;
    table.Add(1, 2, 3);
    table.Add(1, 4, 5);
    table.Add(1, 4, 6);
    table.Add(1, 7, 8);
    table.Add(1, 9, 10);
    table.Add(5, 11, 5);
    table.Add(5, 11, 10);
    table.Add(10, 11, 3);
    table.Add(1, 12, 13);
    table.Add(3, 12, 13);
    table.Add(10, 12, 14);
    table.Add(20, 20, 20);

    std::unique_ptr<Scan> scan = table.EvaluateSPO(true, true, true);
    while(!scan->ReachedEnd()) {
        unsigned s,p,o;
        scan->GetNext(s, p, o);
        printf("<%u, %u, %u>\n", s, p, o);
    }
}
