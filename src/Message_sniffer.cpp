//
// Created by samuel on 24-05-21.
//

#include "Message_sniffer.h"

bool Message_sniffer::isNewMessage(const CAN_Message *message) {
    if (message->id != match_id) {
        return false;
    }

    if (message->id == last_message.id) {
        if (message->len == last_message.len) {

            bool isTheSame = true;
            for (uint8_t i = 0; i < message->len; i++) {
                if (message->rxBuf[i] != last_message.rxBuf[i]) {
                    isTheSame = false;
                    break;
                }
            }
            if (isTheSame) {
                return false;
            }
        }
    }

    last_message.id = message->id;
    last_message.len = message->len;
    memcpy(last_message.rxBuf, message->rxBuf, sizeof(last_message.rxBuf));
    return true;
}
