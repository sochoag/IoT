#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//**************************************
//*********** MQTT CONFIG **************
//**************************************
const char *mqtt_server = "ioticos.org";
const int mqtt_port = 1883;
const char *mqtt_user = "BG6swRmFMa8RMyz";
const char *mqtt_pass = "AyZhgsFitmjMmvC";
const char *root_topic_subscribe = "ZRJuzjMfeOrJ57b/input";
const char *sub1 = "ZRJuzjMfeOrJ57b/input1";
const char *root_topic_publish = "ZRJuzjMfeOrJ57b/output";

//**************************************
//*********** WIFICONFIG ***************
//**************************************
const char* ssid = "Familia Ochoa";
const char* password = "0301072468";

//**************************************
//*********** GLOBALES   ***************
//**************************************
WiFiClient espClient;
PubSubClient client(espClient);
char msg[25];
long count=0;
long lastMsg = 0;


//**************************************
//*********** FUNCIONES   ***************
//**************************************
void callback(char* topic, byte* payload, unsigned int length);
void reconnect();
void setup_wifi();

void setup() 
{
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() 
{
  if(!client.connected())
  {
    reconnect();
  }

  if(client.connected())
  {
    if (millis() - lastMsg > 3000)
    {
      lastMsg = millis();
      String str = "La cuenta es -> " + String(count);
      str.toCharArray(msg,25);
      client.publish(root_topic_publish,msg);
      count++;
    }
  }
  
  client.loop();
}

//*****************************
//***    CONEXION WIFI      ***
//*****************************
void setup_wifi()
{
	delay(10);
	// Nos conectamos a nuestra red Wifi
	Serial.println();
	Serial.print("Conectando a ssid: ");
	Serial.println(ssid);

	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED) 
  {
		delay(500);
		Serial.print(".");
	}

	Serial.println("");
	Serial.println("Conectado a red WiFi!");
	Serial.println("Dirección IP: ");
	Serial.println(WiFi.localIP());
}

//*****************************
//***    CONEXION MQTT      ***
//*****************************

void reconnect() 
{
	while (!client.connected()) 
  {
		Serial.print("Intentando conexión Mqtt...");
		// Creamos un cliente ID
		String clientId = "ESP8266Davo";
		clientId += String(random(0xffff), HEX);
		// Intentamos conectar
		if(client.connect(clientId.c_str(),mqtt_user,mqtt_pass)) 
    {
			Serial.println("Conectado!");
			// Nos suscribimos
			if(client.subscribe(root_topic_subscribe))
      {
        Serial.println("Suscripcion root ok");
      }
      else
      {
        Serial.println("fallo Suscripciión");
      }
		} 
    else 
    {
			Serial.print("falló :( con error -> ");
			Serial.print(client.state());
			Serial.println(" Intentamos de nuevo en 5 segundos");
			delay(5000);
		}
	}
}


//*****************************
//***       CALLBACK        ***
//*****************************

void callback(char* topic, byte* payload, unsigned int length)
{
	String incoming = "";
	Serial.print("Topico -> ");
	Serial.print(topic);
	Serial.println("");
	for (int i = 0; i < length; i++) 
  {
		incoming += (char)payload[i];
	}
	incoming.trim();
	Serial.println("Mensaje -> " + incoming);
  if((char)payload[0] == '0')
  {
    digitalWrite(LED_BUILTIN,LOW);
    Serial.println("Led Encendido");
  }
  else
  {
    digitalWrite(LED_BUILTIN,HIGH);
    Serial.println("Led Apagado");
  }
  
}