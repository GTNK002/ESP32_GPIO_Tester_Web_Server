#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <AsyncWebSocket.h>

const char* ssid = "ssid-xMxUU";
const char* password = "password";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<body>
<div class="container">
  <div class="smart-home">
    <h1>Smart GPIO Tester</h1>
  <div class="button-container">
    <button onclick="sendws('gpio1')">GPIO 1<br><span id="gpio1Status">Status</span></button>
    <button onclick="sendws('gpio2')">GPIO 2<br><span id="gpio2Status">Status</span></button>
    <button onclick="sendws('gpio3')">GPIO 3<br><span id="gpio3Status">Status</span></button>
    <button onclick="sendws('gpio4')">GPIO 4<br><span id="gpio4Status">Status</span></button>
    <button onclick="sendws('gpio5')">GPIO 5<br><span id="gpio5Status">Status</span></button>
    <button onclick="sendws('gpio12')">GPIO 12<br><span id="gpio12Status">Status</span></button>
    <button onclick="sendws('gpio13')">GPIO 13<br><span id="gpio13Status">Status</span></button>
    <button onclick="sendws('gpio14')">GPIO 14<br><span id="gpio14Status">Status</span></button>
    <button onclick="sendws('gpio15')">GPIO 15<br><span id="gpio15Status">Status</span></button>
    <button onclick="sendws('gpio16')">GPIO 16<br><span id="gpio16Status">Status</span></button>
    <button onclick="sendws('gpio17')">GPIO 17<br><span id="gpio17Status">Status</span></button>
    <button onclick="sendws('gpio18')">GPIO 18<br><span id="gpio18Status">Status</span></button>
    <button onclick="sendws('gpio19')">GPIO 19<br><span id="gpio19Status">Status</span></button>
    <button onclick="sendws('gpio20')">GPIO 20<br><span id="gpio20Status">Status</span></button>
    <button onclick="sendws('gpio21')">GPIO 21<br><span id="gpio21Status">Status</span></button>
    <button onclick="sendws('gpio22')">GPIO 22<br><span id="gpio22Status">Status</span></button>
    <button onclick="sendws('gpio23')">GPIO 23<br><span id="gpio23Status">Status</span></button>
    <button onclick="sendws('gpio24')">GPIO 24<br><span id="gpio24Status">Status</span></button>
    <button onclick="sendws('gpio25')">GPIO 25<br><span id="gpio25Status">Status</span></button>
    <button onclick="sendws('gpio26')">GPIO 26<br><span id="gpio26Status">Status</span></button>
    <button onclick="sendws('gpio27')">GPIO 27<br><span id="gpio27Status">Status</span></button>
    <button onclick="sendws('gpio28')">GPIO 28<br><span id="gpio28Status">Status</span></button>
    <button onclick="sendws('gpio29')">GPIO 29<br><span id="gpio29Status">Status</span></button>
    <button onclick="sendws('gpio30')">GPIO 30<br><span id="gpio30Status">Status</span></button>
    <button onclick="sendws('gpio31')">GPIO 31<br><span id="gpio31Status">Status</span></button>
    <button onclick="sendws('gpio32')">GPIO 32<br><span id="gpio32Status">Status</span></button>
    <button onclick="sendws('gpio33')">GPIO 33<br><span id="gpio33Status">Status</span></button>
    <button onclick="sendws('gpio34')">GPIO 34<br><span id="gpio34Status">Status</span></button>
    <button onclick="sendws('gpio35')">GPIO 35<br><span id="gpio35Status">Status</span></button>
    <button onclick="sendws('gpio36')">GPIO 36<br><span id="gpio36Status">Status</span></button>
    <button onclick="sendws('gpio37')">GPIO 37<br><span id="gpio37Status">Status</span></button>
    <button onclick="sendws('gpio38')">GPIO 38<br><span id="gpio38Status">Status</span></button>
    <button onclick="sendws('gpio39')">GPIO 39<br><span id="gpio39Status">Status</span></button>
  </div>
</div>
<script>
  var gateway = `ws://${window.location.hostname}/ws`;
  var websocket;
  window.addEventListener('load', onLoad);
  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage; // <-- add this line
  }
  function onOpen(event) {
    console.log('Connection opened');
  }
  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }
  function onLoad(event) {
    initWebSocket();
  }
  function sendws(x) {
    websocket.send(x);
  }
  function onMessage(event) {
    var data = event.data;
    var gpioNumber = data.match(/\d+/)[0]; // Extract GPIO number from data

    if (data.endsWith('On')) {
        updateGPIOStatus(gpioNumber, 'On');
    } else if (data.endsWith('Off')) {
        updateGPIOStatus(gpioNumber, 'Off');
    }
  }

  function updateGPIOStatus(gpioNumber, status) {
    var gpioStatusElement = document.getElementById('gpio' + gpioNumber + 'Status');
    if (gpioStatusElement) {
        gpioStatusElement.innerText = status;
    }
  }
</script>
</body>
</html>
)rawliteral";

int gpioPins[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                  11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
                  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
                  31, 32, 33, 34, 35, 36, 37, 38, 39
                  };

// Define GPIO statuses for all 39 GPIOs
String gpioStatuses[] = {"On", "On", "On", "On", "On", "On", "On", "On", "On", "On",
                         "On", "On", "On", "On", "On", "On", "On", "On", "On", "On",
                         "On", "On", "On", "On", "On", "On", "On", "On", "On", "On",
                         "On", "On", "On", "On", "On", "On", "On", "On", "On", "On",
                         "On", "On", "On", "On", "On", "On", "On", "On", "On", "On"
                         };


void gpioFunction(int gpioIndex) {
  int pin = gpioPins[gpioIndex];
  String &status = gpioStatuses[gpioIndex];
  
  if (status == "On") {
    status = "Off";
    digitalWrite(pin, HIGH);
    String message = "gpio" + String(pin) + "Off";
    ws.textAll(message.c_str());
  } else {
    status = "On";
    digitalWrite(pin, LOW);
    String message = "gpio" + String(pin) + "On";
    ws.textAll(message.c_str());
  }
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    // Extract the pin number from the received message
    int pin = atoi((char*)data + 4); 
    // Assuming message format is "gpioX" where X is pin number
    gpioFunction(pin);
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

String processor(const String& var){
  Serial.println(var);
  return String();
}

void setup() {
  Serial.begin(115200);

  pinMode(1, OUTPUT);  // Tx0
  pinMode(2, OUTPUT);  // D2
  pinMode(3, OUTPUT);  // Rx0
  pinMode(4, OUTPUT);  // D4
  pinMode(5, OUTPUT);  // D5
  //pinMode(6, OUTPUT);  // D
  //pinMode(7, OUTPUT);  // D
  //pinMode(8, OUTPUT);  // D
  //pinMode(9, OUTPUT);  // D
  //pinMode(10, OUTPUT); // D
  //pinMode(11, OUTPUT); // D
  pinMode(12, OUTPUT); // D12
  pinMode(13, OUTPUT); // D13
  pinMode(14, OUTPUT); // D14
  pinMode(15, OUTPUT); // D15
  pinMode(16, OUTPUT); // Rx2
  pinMode(17, OUTPUT); // Tx2
  pinMode(18, OUTPUT); // D18
  pinMode(19, OUTPUT); // D19
  pinMode(20, OUTPUT); // D
  pinMode(21, OUTPUT); // D21
  pinMode(22, OUTPUT); // D22
  pinMode(23, OUTPUT); // D23
  pinMode(24, OUTPUT); // D
  pinMode(25, OUTPUT); // D25
  pinMode(26, OUTPUT); // D26
  pinMode(27, OUTPUT); // D27
  pinMode(28, OUTPUT); // D
  pinMode(29, OUTPUT); // D
  pinMode(30, OUTPUT); // D
  pinMode(31, OUTPUT); // D
  pinMode(32, OUTPUT); // D32
  pinMode(33, OUTPUT); // D33
  pinMode(34, OUTPUT); // D34
  pinMode(35, OUTPUT); // D35
  pinMode(36, OUTPUT); // Vp
  pinMode(37, OUTPUT); // D
  pinMode(38, OUTPUT); // D
  pinMode(39, OUTPUT); // Vn
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
  
  initWebSocket();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  AsyncElegantOTA.begin(&server);

  server.begin();
}

void loop() {
  ws.cleanupClients();
  AsyncElegantOTA.loop();
}
