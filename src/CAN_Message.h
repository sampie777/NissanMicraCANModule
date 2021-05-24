//
// Created by samuel on 24-05-21.
//

#ifndef NISSANMICRACANMODULE_CAN_MESSAGE_H
#define NISSANMICRACANMODULE_CAN_MESSAGE_H

#include <Arduino.h>

class CAN_Message {
public:
    CAN_Message(unsigned long id, uint8_t len, const uint8_t *rxBuf) : id(id), len(len) {
        memcpy(this->rxBuf, rxBuf, sizeof(this->rxBuf));
    }

    CAN_Message() = default;

    unsigned long id = 0;
    uint8_t len = 0;
    uint8_t rxBuf[8] = {0};
};


#endif //NISSANMICRACANMODULE_CAN_MESSAGE_H
