#include <ESP8266WiFi.h>
#include "HeaterIcon.h"
#include "Heater.h"

namespace window {

static const int BUFFER_LEN = 512;

Heater::Heater()
{}

Heater::~Heater()
{}

void Heater::init()
{
    update_data();
}

void Heater::update_data()
{
    WiFiClient client;
    char buffer[BUFFER_LEN];
    size_t read_size;

    client.connect(IPAddress(192, 168, 1, 198), 23);
    if  (!client.connected()) {
        Serial.println("Failed to connect to heater");
        _status = UpdateStatus::FAILED;
        return;
    }
    read_size = client.readBytesUntil('\n', buffer, BUFFER_LEN - 1);
    buffer[read_size] = '\0';
    parse_data(buffer);
    client.stop();
    _status = UpdateStatus::SUCCESS;
}

void Heater::draw_temperature(Matrix &matrix, int x, int y, const uint16_t img[], int temp)
{
    matrix.drawRGB(x, y, img, IMG_HOUSE_TEMP_INSIDE_W, IMG_HOUSE_TEMP_INSIDE_H);
    matrix.setCursor(x + 9, y);
    matrix.printf("%2dc", temp);
    matrix.drawPixel(x + 19, y + 0, matrix.Color(0, 0, 255));
}

int Heater::extract_int(char *buffer, size_t value_idx)
{
    size_t index = 0;
    size_t end_index = 0;
    bool end_char;

    for (uint8_t space_count = 0; buffer[index] != '\0' && space_count < value_idx - 1; index++)
        if (buffer[index] == ' ')
            space_count++;
    if (buffer[index] == '\0')
        return (HEATER_FETCH_ERROR);
    end_index = index;
    while (buffer[end_index] != ' ' && buffer[end_index] != '\0')
        end_index++;
    end_char = buffer[end_index] == '\0';
    buffer[end_index] = '\0';
    String tmp(&buffer[index]);
    buffer[end_index] = end_char ? '\0' : ' ';
    return (round(tmp.toFloat()));
}

}