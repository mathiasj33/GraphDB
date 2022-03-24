#include "graphDB/GraphDB.h"
#include <chrono>
#include <iostream>

using namespace graph_db;
using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::chrono::milliseconds;

int main() {
    GraphDB db;

    db.LoadFile("data/familyGuy.ttl");
    db.PrintQueryAnswers("SELECT ?X ?Z WHERE { ?X <hasAge> ?Y . ?Z <hasAge> ?Y . }");

    /*auto start = high_resolution_clock::now();
    db.LoadFile("data/LUBM-010-mat.ttl");
    auto end = high_resolution_clock::now();
    duration<double, std::milli> timeElapsed = (end - start);
    std::cout << timeElapsed.count() << std::endl;*/
}
