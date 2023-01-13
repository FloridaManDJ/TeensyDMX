#define TX_EN_PIN 2
#define RX_WATCH_PIN 3

#include <TeensyDMX.h>

namespace teensydmx = ::qindesign::teensydmx;

teensydmx::Receiver dmxRx{Serial1};

teensydmx::Receiver::ErrorStats newStats;
teensydmx::Receiver::ErrorStats oldStats;
teensydmx::Receiver::PacketStats packetStats;

uint8_t newBuf[515]{0};
uint8_t oldBuf[515]{0};

void setup() {
    pinMode(RX_WATCH_PIN, INPUT);

    pinMode(TX_EN_PIN, OUTPUT);
    digitalWriteFast(TX_EN_PIN, LOW);

    Serial.begin(115200);
    while (!Serial && millis() < 4000) {
    }
    Serial.println("boot");

    dmxRx.begin();
    dmxRx.setRXWatchPin(RX_WATCH_PIN);
}

void loop() {
    int read = dmxRx.readPacket(newBuf, 1, 512);
    if (read > 0) {
        newStats = dmxRx.errorStats();
        packetStats = dmxRx.packetStats();
        debugPacketStats();
        debugErrorStats();
        oldStats = newStats;

        if (read > 0) {
            bool new_data = false;
            bool nonzero_data = false;
            for (int i=0;i<512;i++) {
                if (oldBuf[i] != newBuf[i]) {
                    new_data = true;
                }
                if (newBuf[i] != 0) {
                nonzero_data = true;
                }
            }

            if (new_data && nonzero_data) {
                Serial.printf("Got %d channels\n", read);
                for (int i=0;i<515;i++) {
                    Serial.print(newBuf[i], HEX);
                    Serial.print(", ");
                }
                Serial.println("");
            }
        }

        for (int i=0;i<512;i++) {
            oldBuf[i] = newBuf[i];
        }
    }
    delay(50);
}

void debugErrorStats() {
    if (newStats.packetTimeoutCount != oldStats.packetTimeoutCount) {
        Serial.printf("packetTimeoutCount: %d\n", newStats.packetTimeoutCount);
    }

    if (newStats.framingErrorCount1 > 0) {
        Serial.printf("framingErrorCount1:  %d\n", newStats.framingErrorCount1);
    }
    if (newStats.framingErrorCount2 > 0 ) {
        Serial.printf("framingErrorCount2:  %d\n", newStats.framingErrorCount2);
    }
    if (newStats.framingErrorCount3a > 0) {
        Serial.printf("framingErrorCount3a: %d\n", newStats.framingErrorCount3a);
    }
    if (newStats.framingErrorCount3b > 0) {
        Serial.printf("framingErrorCount3b: %d\n", newStats.framingErrorCount3b);
    }
    if (newStats.framingErrorCount4 > 0) {
        Serial.printf("framingErrorCount4:  %d\n", newStats.framingErrorCount4);
    }
    if (newStats.framingErrorCount5 > 0) {
        Serial.printf("framingErrorCount5:  %d\n", newStats.framingErrorCount5);
    }
    if (newStats.framingErrorCount6 > 0) {
        Serial.printf("framingErrorCount6:  %d\n", newStats.framingErrorCount6);
    }
    if (newStats.framingErrorCount7 > 0) {
        Serial.printf("framingErrorCount7:  %d\n", newStats.framingErrorCount7);
    }
    if (newStats.framingErrorCount8 > 0) {
        Serial.printf("framingErrorCount8:  %d\n", newStats.framingErrorCount8);
    }
    if (newStats.framingErrorCount9 > 0) {
        Serial.printf("framingErrorCount9:  %d\n", newStats.framingErrorCount9);
    }
    if (newStats.framingErrorCount10 > 0) {
        Serial.printf("framingErrorCount10: %d\n", newStats.framingErrorCount10);
    }
    if (newStats.framingErrorCount11 > 0) {
        Serial.printf("framingErrorCount11: %d\n", newStats.framingErrorCount11);
    }
    if (newStats.framingErrorCount != oldStats.framingErrorCount) {
        Serial.println("");
    }

    if (newStats.shortPacketCount != oldStats.shortPacketCount) {
        Serial.printf("shortPacketCount: %d\n", newStats.shortPacketCount);
    }
    if (newStats.longPacketCount != oldStats.longPacketCount) {
        Serial.printf("longPacketCount: %d, extraSize=%d\n", newStats.longPacketCount, packetStats.extraSize);
    }
}

void debugPacketStats() {
    // Serial.printf("Packet size:     %d\n", packetStats.size);
    // Serial.printf("Is short?  :     %d\n", packetStats.isShort);
    if (packetStats.breakPlusMABTime < 88 + 8) {
      Serial.printf("Break+MAB time   %d\n", packetStats.breakPlusMABTime);
    }
    // Serial.printf("Break to Break:  %d\n", packetStats.breakToBreakTime);

    if (packetStats.breakTime < 88) {
      Serial.printf("Break time:      %d\n", packetStats.breakTime);
    }

    if (packetStats.mabTime < 8) {
      Serial.printf("MAB time<8:      %d\n", packetStats.mabTime);
      while (1) {
        
      }
    }

    // Serial.printf("Frame timestamp: %d\n", packetStats.frameTimestamp);
    // Serial.printf("Packet time:     %d\n", packetStats.packetTime);
    if (packetStats.extraSize > 0) {
      Serial.printf("ExtraSize:       %d\n", packetStats.extraSize);
    }
    // Serial.println("");
}