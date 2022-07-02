class TBot {

  private:

    String _chatID;
    UniversalTelegramBot *_bot;
    WiFiClientSecure _client;

    String _allowedUser = "";
    String _wakeupTime;
    bool _isOpenAuthorized = false;

    void _handleNewMessages(int numMsg) {
      Serial.print("[INFO] Processing messages: ");
      Serial.print(numMsg);

      for (int i = 0; i < numMsg; i++) {
        String chatID = _bot->messages[i].chat_id;
        String date = _bot->messages[i].date;

        if ((_isAllowedUser(chatID)) && (_isMsgDateValid(date))) {
          String text = _bot->messages[i].text;
          _actOnMessage(text);
        }
      }

    }

    void _actOnMessage(String text) {
      if (text = '/POBox open') {
        _isOpenAuthorized = true;
        _bot->sendMessage(_chatID, "[PObox] Authorized to open.\nOpening box...");
      }
    }

    void _setWakeupTime() {
      time_t tnow = time(nullptr);
      while (tnow < 100000) {
        tnow = time(nullptr);
        delay(200);
      }
      _wakeupTime = String(tnow);
      Serial.print("[INFO] Wakeup time: ");
      Serial.println(_wakeupTime);
    }

    void _setAllowedUser(String chatID) {
      _allowedUser = chatID;
    }

    bool _isAllowedUser(String chatID) {
      if (chatID == _allowedUser) {
        return true;
      }
      return false;
    }

    bool _isMsgDateValid(String date) {
      unsigned long wuTimeLong = atol(_wakeupTime.c_str());
      unsigned long dateLong = atol(date.c_str());

      if (dateLong < wuTimeLong){
        return false;
      }
      return true;
    }
  

  public:

    void Init(String token, String chatID, X509List *cert) {
      _chatID = chatID;
      _setWakeupTime();
      _setAllowedUser(_chatID);
      _client.setTrustAnchors(cert);
      _bot = new UniversalTelegramBot(token, _client);
    }

    bool NotifyButtonPressed() {
      Serial.println("[INFO] Notifying Telegram...");
      return _bot->sendMessage(_chatID, "[POBox] Requesting to open the Lid. Reply with \"/POBox open\" to authorize. \nTimeout: 60s");
    }

    void PollNewMessages() {
      int numMsg = _bot->getUpdates(_bot->last_message_received + 1);
      Serial.println("[DEBUG] Polling msg:");
      Serial.println(numMsg);

      while (numMsg) {
        _handleNewMessages(numMsg);
        numMsg = _bot->getUpdates(_bot->last_message_received + 1);
      }

    }
    
    bool IsOpenAuthorized() {
      return _isOpenAuthorized;
    }

};

TBot TBot;