class TBot {

  private:

    String _chatID;
    UniversalTelegramBot *_bot;
    WiFiClientSecure _client;

    String _allowedUser = "";
    int _wakeupTime;
    bool _isOpenAuthorized = false;

    void _handleNewMessages(int numNewMessages) {
      Serial.print("Processing messages: ");
      Serial.print(numNewMessages);

      for (int i = 0; i < numNewMessages; i++) {
        String chatID = _bot->messages[i].chat_id;
        String date = _bot->messages[i].date;

        if ((_isAllowedUser(chatID)) && (_isMsgDateValid(date))) {
          String text = _bot->messages[i].text;
          _actOnMessage(text);
        }
      }

    }
    
    bool _isAllowedUser(String chatID) {
      if (chatID == _allowedUser) {
        return true;
      }
      return false;
    }

    void _setWakeupTime() {
      _wakeupTime = time(nullptr);
    }

    bool _isMsgDateValid(String date) {
      //if (date < _wakeupTime){
      //  return false;
      //}
      return true;
    }
  
    void _actOnMessage(String text) {
      if (text = '/open') {
        _isOpenAuthorized = true;
        _bot->sendMessage(_chatID, "Authorized to open.\n Opening box...");
      }
    }


  public:

    void Init(String token, String chatID, X509List *cert) {
      _chatID = chatID;
      configTime(0, 0, "pool.ntp.org");
      _setWakeupTime();
      _client.setTrustAnchors(cert);
      _bot = new UniversalTelegramBot(token, _client);
    }

    bool NotifyButtonPressed() {
      Serial.println("Notifying Telegram...");
      return _bot->sendMessage(_chatID, "Button pressed. Requesting to open the Lid. \nTimeout: 60s");
    }

    void PollNewMessages() {
      int numMsg = _bot->getUpdates(_bot->last_message_received + 1);

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
