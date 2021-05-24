//
// Created by samuel on 24-05-21.
//

#ifndef NISSANMICRACANMODULE_MESSAGE_SNIFFER_H
#define NISSANMICRACANMODULE_MESSAGE_SNIFFER_H

#include <Arduino.h>
#include "CAN_Message.h"

class Message_sniffer {
public:
    bool isNewMessage(const CAN_Message * message);

    unsigned long match_id = 0;
    CAN_Message last_message = CAN_Message();
};


#endif //NISSANMICRACANMODULE_MESSAGE_SNIFFER_H
