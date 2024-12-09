#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
using namespace std;

#include <filesystem> //require C++17
namespace fs = std::filesystem;

#include "list/listheader.h"
// #include "sformat/fmt_lib.h"
// #include "tensor/xtensor_lib.h"
// #include "ann/annheader.h"
// #include "loader/dataset.h"
// #include "loader/dataloader.h"
// #include "config/Config.h"
// #include "dataset/DSFactory.h"
// #include "optim/Adagrad.h"
// #include "optim/Adam.h"
// #include "modelzoo/twoclasses.h"
// #include "modelzoo/threeclasses.h"

#include "graph/AbstractGraph.h"
#include "graph/IGraph.h"
#include "graph/DGraphModel.h"
#include "graph/UGraphModel.h"

// #include "graph/DGraphDemo.h"
// #include "graph/UGraphDemo.h"
#include "graph/sampleDemo.h"

int main(int argc, char** argv) {
    //dataloader:
    //case_data_wo_label_1();
    //case_data_wi_label_1();
    //case_batch_larger_nsamples();
    
    //Classification:
    // twoclasses_classification();
    //threeclasses_classification();
    DGraphDemo1();
    DGraphDemo2();
    DGraphDemo3();
    dijkstraDemo();
    ugraphDemo1();
    ugraphDemo2();
    ugraphDemo3();

    return 0;
}
