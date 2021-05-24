//
// Created by samuel on 24-05-21.
//

#include "ID_sniffer.h"

bool ID_sniffer::processMessage(const CAN_Message * message) {
    if (hasMessageId(message->id)) {
        return false;
    }

    ids[id_pointer++] = message->id;

    if (id_pointer >= MAX_ID_COUNT) {
        id_pointer = 0;
    }

    return true;
}

bool ID_sniffer::hasMessageId(unsigned long id) {
    for (uint16_t i = 0; i < id_pointer; i++) {
        if (ids[i] == id) {
            return true;
        }
    }
    return false;
}
