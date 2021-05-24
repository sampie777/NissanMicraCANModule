#include <Arduino.h>
#include "../lib/MCP_CAN_lib-master/mcp_can.h"
#include "ID_sniffer.h"
#include "CAN_Message.h"
#include "Message_sniffer.h"

#define ID_SNIFFER 0
#define MESSAGE_SNIFFER 1

#define USE_SNIFFER MESSAGE_SNIFFER

// Set INT to pin 2
#define CAN0_INT 2
// Set CS to pin 10
MCP_CAN CAN0(10);

#if USE_SNIFFER == ID_SNIFFER
ID_sniffer sniffer = ID_sniffer();
#elif USE_SNIFFER == MESSAGE_SNIFFER
Message_sniffer sniffer = Message_sniffer();
#endif

void initCan() {
    Serial.print("Initializing CAN... ");
    // Initialize MCP2515 running at 8MHz with a baudrate of 500kb/s and the masks and filters disabled.
    while (CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK) {
        Serial.println("Error Initializing MCP2515...");
        delay(2000);
    }

    Serial.println("MCP2515 Initialized Successfully!");

//    CAN0.setMode(MCP_NORMAL);                     // Set operation mode to normal so the MCP2515 sends acks to received data.
    CAN0.setMode(MCP_LISTENONLY);
    pinMode(CAN0_INT, INPUT);
}

bool canMessageAvailable() {
    // If CAN0_INT pin is low, read receive buffer
    return !digitalRead(CAN0_INT);
}

CAN_Message readCANMessage() {
    unsigned long rxId;
    uint8_t len = 0;
    uint8_t rxBuf[8];

    // Read data: len = data length, buf = data byte(s)
    CAN0.readMsgBuf(&rxId, &len, rxBuf);
    CAN_Message message = CAN_Message(rxId, len, rxBuf);
    return message;
}

void printCANMessage(const CAN_Message *message) {// Array to store serial string
    char msgString[128];

    // Determine if ID is standard (11 bits) or extended (29 bits)
    if ((message->id & 0x80000000) == 0x80000000)
        sprintf(msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", (message->id & 0x1FFFFFFF), message->len);
    else
        sprintf(msgString, "Standard ID: 0x%.3lX       DLC: %1d  Data:", message->id, message->len);

    Serial.print(msgString);

    // Determine if message is a remote request frame
    if ((message->id & 0x40000000) == 0x40000000) {
        sprintf(msgString, " REMOTE REQUEST FRAME");
        Serial.print(msgString);
    } else {
        for (byte i = 0; i < message->len; i++) {
            sprintf(msgString, " 0x%.2X", message->rxBuf[i]);
            Serial.print(msgString);
        }
    }

    Serial.println();
}

void setup() {
    Serial.begin(115200);

    initCan();

    Serial.println("Ready.");
}

unsigned long setIdFilter() {
    char hex[6];
    Serial.readBytes(hex, 6);

    unsigned long idFilter = strtol(hex, NULL, 16);

    Serial.print("Setting new ID filter to: ");
    Serial.println(idFilter, HEX);

    return idFilter;
}

void clearSerialInput() {
    while (Serial.available()) {
        Serial.read();
        delay(1);
    }
}

void loop() {
#if USE_SNIFFER == ID_SNIFFER
    if (canMessageAvailable()) {
        CAN_Message message = readCANMessage();

        // Check if it's a new unknown ID
        if (!sniffer.processMessage(&message)) {
            return;
        }

        printCANMessage(&message);
    }
#elif USE_SNIFFER == MESSAGE_SNIFFER
    if (Serial.available() >= 6) {
        sniffer.match_id = setIdFilter();
        clearSerialInput();
    }

    if (canMessageAvailable()) {
        CAN_Message message = readCANMessage();

        // Check if it's a new unknown ID
        if (!sniffer.isNewMessage(&message)) {
            return;
        }

        printCANMessage(&message);
    }
#endif
}
