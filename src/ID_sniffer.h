//
// Created by samuel on 24-05-21.
//

#ifndef NISSANMICRACANMODULE_ID_SNIFFER_H
#define NISSANMICRACANMODULE_ID_SNIFFER_H

#include <Arduino.h>
#include "CAN_Message.h"
#define MAX_ID_COUNT 255

class ID_sniffer {
public:
    bool processMessage(const CAN_Message * message);
    bool hasMessageId(uint16_t id);

    uint16_t ids[MAX_ID_COUNT] = {0};
    uint16_t id_pointer = 0;
};


#endif //NISSANMICRACANMODULE_ID_SNIFFER_H
