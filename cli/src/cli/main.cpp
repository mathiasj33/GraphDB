#include "graphDB/GraphDB.h"
#include "Cli.h"

using namespace graph_db;
using namespace graph_db::cli;

int main() {
    GraphDB db;
    Cli cli{db};
    cli.Start();
    return 0;
}
