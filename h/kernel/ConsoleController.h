//
// Created by os on 7/21/22.
//

#ifndef PROJECT_CONSOLECONTROLLER_H
#define PROJECT_CONSOLECONTROLLER_H
#include "../../lib/hw.h"

namespace kernel {
    namespace ConsoleController {
        uint8 status();
        char receiveData();
        void transmitData(char data);
        bool isReadable();
        bool isWritable();
    };

} // kernel

#endif //PROJECT_CONSOLECONTROLLER_H
