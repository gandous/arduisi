#include <ESP8266WiFi.h>
#include "HeaterIcon.h"
#include "Heater.h"

namespace window {

static const int BUFFER_LEN = 512;
static const int TEMP_OFFSET = 60;

Heater::Heater(): _temp(99)
{}

Heater::~Heater()
{}

void Heater::init()
{
    update_data();
}

void Heater::update(Matrix &matrix, int x, int y)
{
    matrix.drawRGB(x, y, IMG_HOUSE_TEMP, IMG_HOUSE_TEMP_W, IMG_HOUSE_TEMP_H);
    matrix.setCursor(x + 9, y);
    matrix.printf("%d", _temp);
}

void Heater::update_data()
{
    WiFiClient client;
    char buffer[BUFFER_LEN];
    size_t read_size;
    size_t index = 0;
    size_t end_index = 0;

    client.connect(IPAddress(192, 168, 1, 198), 23);
    if  (!client.connected()) {
        Serial.println("Failed to connect to heater");
        return;
    }
    read_size = client.readBytesUntil('\n', buffer, BUFFER_LEN - 1);
    buffer[read_size] = '\0';
    Serial.println(buffer);
    for (uint8_t space_count = 0; buffer[index] != '\0' && space_count < TEMP_OFFSET; index++)
        if (buffer[index] == ' ')
            space_count++;
    Serial.printf("index %d\n", index);
    end_index = index;
    while (buffer[end_index] != ' ' && buffer[end_index] != '\0')
        end_index++;
    buffer[end_index] = '\0';
    Serial.printf("%d %d %s\n", index, end_index, &buffer[index]);
    String temp(&buffer[index]);
    _temp = temp.toInt();
    client.stop();
}

}