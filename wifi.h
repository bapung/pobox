class WNetwork {

  private:
    int _wifiTimeout = 1e4;
    
  public:

    // return 0 if successfully connect to wifi
    int Connect(char *ssid, char *password) {
      int counter = 0;

      WiFi.disconnect();

      delay(10);
      Serial.println("[INFO] Connecting to WiFi network...");

      WiFi.begin(ssid, password);
      while ((WiFi.status() != WL_CONNECTED) && (counter < _wifiTimeout)) {
        counter += 500;
        delay(500);
        Serial.print(".");
      }
      Serial.println("");

      if (WiFi.status() != WL_CONNECTED) {
        Serial.println("[INFO] NodeMcu FAIL to connect to WiFi network. Exiting...");
        return 1;
      }

      Serial.println("[INFO] NodeMcu connected to WiFi network...");

      return 0;
    }

    void SetTimeout(int seconds) {
      _wifiTimeout = seconds * 1e3;
    }
    
};

WNetwork WNetwork;