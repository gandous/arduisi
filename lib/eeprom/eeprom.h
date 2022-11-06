#pragma once
#include <Arduino.h>
#include <EEPROM.h>

static const uint8_t SSID_LEN = 32 + 1; // MAX ssid len + 1 for the \0
static const uint8_t PASSWORD_LEN = 63 + 1;

typedef struct eeprom_data_s {
    char wifi_ssid[SSID_LEN];
    char wifi_password[PASSWORD_LEN];
} eeprom_data_t;

char *get_wifi_ssid();
char *get_wifi_password();
void set_wifi_ssid(const char *ssid);
void set_wifi_password(const char *password);