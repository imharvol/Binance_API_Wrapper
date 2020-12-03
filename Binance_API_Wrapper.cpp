#include "Binance_API_Wrapper.h"

#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

// Binance certificate
const char *root_ca =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIGgzCCBWugAwIBAgIQCnzrBLcwdC8D3jMqPjY6yDANBgkqhkiG9w0BAQsFADBe\n"
    "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
    "d3cuZGlnaWNlcnQuY29tMR0wGwYDVQQDExRHZW9UcnVzdCBSU0EgQ0EgMjAxODAe\n"
    "Fw0yMDAxMDcwMDAwMDBaFw0yMjA0MDcxMjAwMDBaMGsxCzAJBgNVBAYTAktZMRQw\n"
    "EgYDVQQHEwtHZW9yZ2UgVG93bjEhMB8GA1UEChMYQmluYW5jZSBIb2xkaW5ncyBM\n"
    "aW1pdGVkMQswCQYDVQQLEwJJVDEWMBQGA1UEAwwNKi5iaW5hbmNlLmNvbTCCASIw\n"
    "DQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMgLAlsC7vsz9LqLa6t918un0IWH\n"
    "0CvQLUxRRSHb6xlegPtEIkKYUX9UFSGNkGAizzcttvb+7EpUJ9JCdrG/xxd0Hb7Q\n"
    "ky+gu+z3pmmylyZrAqkFwmo65Lqw7f5+BXv+LWK8fKEnjJyuYVZRfhiW1I5E3bEn\n"
    "7j0S67ZAWYji0bPyuv+2hLBbVDZMDOUeW9ZqaswMKh8r3WpicdtG0+kkuj2F0q/3\n"
    "b/GpNXeHtkNJyGtTu5d8NG2WeMJ6eQfGfrYz3dVvLmmaETtolZDzIdsj5Y9JprVM\n"
    "NWJXw8rrR/7eBZUQg4g+CY3UREBrN/Nf/o25HY5dkiG9SMjYgnQFwOs7KscCAwEA\n"
    "AaOCAy4wggMqMB8GA1UdIwQYMBaAFJBY/7CcdahRVHex7fKjQxY4nmzFMB0GA1Ud\n"
    "DgQWBBT2cp2GFYaM0RTiddfJvm8QSYlSaDAlBgNVHREEHjAcgg0qLmJpbmFuY2Uu\n"
    "Y29tggtiaW5hbmNlLmNvbTAOBgNVHQ8BAf8EBAMCBaAwHQYDVR0lBBYwFAYIKwYB\n"
    "BQUHAwEGCCsGAQUFBwMCMD4GA1UdHwQ3MDUwM6AxoC+GLWh0dHA6Ly9jZHAuZ2Vv\n"
    "dHJ1c3QuY29tL0dlb1RydXN0UlNBQ0EyMDE4LmNybDBMBgNVHSAERTBDMDcGCWCG\n"
    "SAGG/WwBATAqMCgGCCsGAQUFBwIBFhxodHRwczovL3d3dy5kaWdpY2VydC5jb20v\n"
    "Q1BTMAgGBmeBDAECAjB1BggrBgEFBQcBAQRpMGcwJgYIKwYBBQUHMAGGGmh0dHA6\n"
    "Ly9zdGF0dXMuZ2VvdHJ1c3QuY29tMD0GCCsGAQUFBzAChjFodHRwOi8vY2FjZXJ0\n"
    "cy5nZW90cnVzdC5jb20vR2VvVHJ1c3RSU0FDQTIwMTguY3J0MAkGA1UdEwQCMAAw\n"
    "ggGABgorBgEEAdZ5AgQCBIIBcASCAWwBagB3AKS5CZC0GFgUh7sTosxncAo8NZgE\n"
    "+RvfuON3zQ7IDdwQAAABb33x88wAAAQDAEgwRgIhAJbU+eBbGqfXhtxG6DliDoJ+\n"
    "CuCQiSn1f5RULBRpimKEAiEA/zqWHjbTYroqkML6q4zSO184gIxZptNjT9ggPyKe\n"
    "I9kAdgCHdb/nWXz4jEOZX73zbv9WjUdWNv9KtWDBtOr/XqCDDwAAAW998fSdAAAE\n"
    "AwBHMEUCIQDxllrDA65IkVgnDyj2Qx/sHPJBsJBMEpxBffuyNO5+iQIgYGwSFmHd\n"
    "4sK/5nigiYTzBb5mC8icG09hpkRtD4bhuPIAdwBWFAaaL9fC7NP14b1Esj7HRna5\n"
    "vJkRXMDvlJhV1onQ3QAAAW998fSPAAAEAwBIMEYCIQDAaesGOJzYExZsfnKDIkik\n"
    "6hJNCCTVnFFxqzHJTloi2gIhAPzJczF096Ryy6/6IGjU4LEvK+Fx/lYgAKBkXnIj\n"
    "BwrxMA0GCSqGSIb3DQEBCwUAA4IBAQAIb3Tn1JV4n8/AB/5LDnnO2p6gpiebNPAR\n"
    "W6RaAEQBxn8+9rvGcsIWKF2e3Vf9HwDEB6Mu4Xdigpxtc4TuBM9UF7vRK3tUlqKl\n"
    "lrLLBPV6ye8dFvSPFsLT5jo/ShGy4qmQ8o939cEiR0gWS1vMW6Ea4uJokMazgsMn\n"
    "TLWq2YGThaC520vl7qWwPMRqJ4amkVJEKa5di0dU8nXUesKX94Syqvh66WTPX94+\n"
    "3VuAheNtHoMscnpofQm2MZR6NJ6y1wbvp07ozZIpTVaFg5hUSI3vYfabo6+Uwh/K\n"
    "4MHUZMFpluvak/QXH69FurM7eTgViphocJd4H6I7DboSQVK/OBMK\n"
    "-----END CERTIFICATE-----\n";

/*Binance_API::Binance_API () {
    return;
}*/

float Binance_API::getPrice(String pair) {
    float price = 0.0;

    if ((WiFi.status() == WL_CONNECTED)) { // Check if it's connected to a network
        HTTPClient http;

        const String url = "https://api.binance.com/api/v3/ticker/price?symbol=" + pair;

        http.begin(url, root_ca);
        int httpCode = http.GET(); // Make the request

        if (httpCode == 200) { // Check response code
            String payload = http.getString();
            Serial.println(httpCode);
            Serial.println(payload);
            JSONVar response = JSON.parse(payload);
            price = String((const char *)response["price"]).toFloat();
            //Serial.println(price);
        } else {
            String error = "Error on HTTP request";
            Serial.println(error);
            throw error;
        }

        http.end(); //Free the resources
    }
    
    return price;
}
