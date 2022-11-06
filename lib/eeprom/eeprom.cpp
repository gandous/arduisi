#include <EEPROM.h>
#include "eeprom.h"

void write_string(int address, const char *s, uint16_t size)
{
    uint16_t i = 0;

    for (; s[i] != '\0' && i < size; i++) {
        EEPROM.write(address + i, s[i]);
    }
    for (; i < size; i++) {
        EEPROM.write(address + i, '\0');
    }
}

char *get_wifi_ssid()
{
    return (((eeprom_data_t*)EEPROM.getDataPtr())->wifi_ssid);
}

char *get_wifi_password()
{
    return (((eeprom_data_t*)EEPROM.getDataPtr())->wifi_password);
}

void set_wifi_ssid(const char *ssid)
{
    write_string(offsetof(eeprom_data_t, wifi_ssid), ssid, SSID_LEN);
}

void set_wifi_password(const char *password)
{
    write_string(offsetof(eeprom_data_t, wifi_password), password, PASSWORD_LEN);
}