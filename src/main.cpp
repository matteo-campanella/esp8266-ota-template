#include "main.h"
#include "ota.h"

#define led_on() digitalWrite(LED_BUILTIN, LOW);
#define led_off() digitalWrite(LED_BUILTIN, HIGH);

bool isTimeValid = false;

void timeIsValid() {
    isTimeValid = true;
    log_println("System time has been set.");
}

bool connect_wifi() {
    const char *found_ssid = NULL;
    int n = 0;

    settimeofday_cb(timeIsValid);
    sntp_servermode_dhcp(0);
    configTime(TZ_Etc_UTC, "pool.ntp.org", "time.nist.gov", "ntp1.inrim.it");

    for (int i = 0; i < 3; i++) {
        n = WiFi.scanNetworks();
        if (n > 0) {
            break;
        }
        delay(250);
    }

    for (int i = 0; i < n; ++i) {
        int j = 0;
        while (WIFI_CREDENTIALS[j][0] != NULL) {
            if (WiFi.SSID(i) == WIFI_CREDENTIALS[j][0]) {
                found_ssid = WIFI_CREDENTIALS[j][0];
                const char *passphrase = WIFI_CREDENTIALS[j][1];
                WiFi.begin(found_ssid, passphrase);
                break;
            }
            j++;
        }
    }

    if (found_ssid == NULL) {
        log_println("No known WiFi found.");
        return false;
    }

    log_printfln("Connecting to WiFi: %s ...", found_ssid);

    int tries = 50;
    while (WiFi.status() != WL_CONNECTED && tries > 0) {
        delay(250);
        tries--;
    }
    if (tries == 0) {
        log_println("Failed to connect to WiFi!");
        return false;
    }
    WiFi.mode(WIFI_STA);

    log_print("Received IP: ");
    log_println(WiFi.localIP().toString());
    return true;
}

void setup() {
    ota_setup();
    pinMode(LED_BUILTIN, OUTPUT);
    led_off();
    connect_wifi();    
}
 


void loop() {

}