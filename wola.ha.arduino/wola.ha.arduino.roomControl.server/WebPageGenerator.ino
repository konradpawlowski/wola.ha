void createWebServer()
{

	server.on("/", []() {
		content = ContentIndex();
		server.send(200, "text/html", content);
	});

	server.on("/ustawienia", []() {
		content = ContentUstawienia();

		server.send(200, "text/html", content);
	});
	server.on("/setting", []() {
		
		TParameters param =settings;
		  if (server.arg("Temp1_Id").length() > 0 && server.arg("Temp1_Name").length() > 0) {
			  param.Temp1.Id = server.arg("Temp1_Id").toInt();
			  server.arg("Temp1_Name").toCharArray(param.Temp1.Name, 20);
			  
			  server.arg("ServerAddress").toCharArray(param.ServerAddress, 16);

			  if (server.hasArg("Temp1_Enable"))
				param.Temp1.Enable = 1;
			  else
				param.Temp1.Enable = 0;
			  

			  if (server.hasArg("Temp1_IsOutside"))
				  param.Temp1.IsOutside = 1;
			  else
				  param.Temp1.IsOutside = 0;

			  if (server.hasArg("Temp2_IsOutside"))
				  param.Temp2.IsOutside = 1;
			  else
				  param.Temp2.IsOutside = 0;

			  if (server.hasArg("Temp1_IsInBox"))
				  param.Temp1.IsInBox = 1;
			  else
				  param.Temp1.IsInBox = 0;

			  if (server.hasArg("Temp2_IsInBox"))
				  param.Temp2.IsInBox = 1;
			  else
				  param.Temp2.IsInBox = 0;




			  if (server.hasArg("SelectedRelay"))
				  param.SelectedRelay = server.arg("SelectedRelay").toInt();
			  Serial.println("temp1: ");
			  Serial.println(server.arg("Temp1_Enable"));
			  Serial.println(server.arg("Temp1_Type"));
			  Serial.println(server.arg("Temp1_Type").toInt());

			  Serial.println("temp2: ");
			  Serial.println(server.arg("Temp2_Enable"));
			  Serial.println(server.arg("Temp2_Type"));
			  Serial.println(server.arg("Temp2_Type").toInt());

			  param.Temp1.Pin = (int)Temp1;
			  param.Temp1.Typ = server.arg("Temp1_Type").toInt();

			  if (server.hasArg("Temp2_Enable"))
				  param.Temp2.Enable = 1;
			  else
				  param.Temp2.Enable = 0;

			  param.Temp2.Id = server.arg("Temp2_Id").toInt();
			  server.arg("Temp2_Name").toCharArray(param.Temp2.Name, 20);
			  param.Temp2.Pin = (int)Temp2;
			  param.Temp2.Typ = server.arg("Temp2_Type").toInt();

			  if (server.hasArg("CountSensors"))
				  param.CountOfSensors = server.arg("CountSensors").toInt();


			  if (server.hasArg("IsWifiAp"))
			  {
				  param.Wifi.IsWifiAp = 1;
				  server.arg("WifiSSID").toCharArray(param.Wifi.Ssid, 20);
				  server.arg("WifiHPass").toCharArray(param.Wifi.Pass, 20);
			  }
			  else
			  {
				  param.Wifi.IsWifiAp = 0;
				  server.arg("WifiSSID").toCharArray("", 20);
				  server.arg("WifiHPass").toCharArray("", 20);
			  }
			  printTParameters(param);
			  SaveSettings(param);

			

			  settings = param;
			  content = ContentZapamietanoUstawienia();
			  statusCode = 200;
		} else {
		content = "<html><h2><strong>Błąd zapisu ustawień</strong></h2></html>";
		statusCode = 404;
		Serial.println("Sending 404");

		}
	
		server.send(statusCode, "text/html", content);
		//Blink(2);
	});

	server.on("/temp", []() {
		int id = 0;
		float temp = 0;
		String name = "";
		TParameters param;
		String ipAddress = "";
		Serial.println("Weszło:");
		if(server.hasArg("Id"))
			id = server.arg("Id").toInt();
		else{
			content = "<html><h2><strong>Brak zdefiniowanego czujnika</strong></h2></html>";
			statusCode = 404;
			Serial.println("Sending 404");

		}

		if(server.hasArg("Temperatura"))
			Sensors[id].Temp = server.arg("Temperatura").toFloat();

		if(server.hasArg("ipAddress"))
			 server.arg("ipAddress").toCharArray(Sensors[id].Address,16);

		if (server.hasArg("Name"))
			server.arg("Name").toCharArray(Sensors[id].Name,20);

		if (server.hasArg("IsOutside"))
			Sensors[id].IsOutside = server.arg("IsOutside").toInt();
		
		Sensors[id].Time = millis();
			Serial.println("Id: ");
			Serial.println(id);
			

			Serial.println("Temp: ");
			Serial.println(temp);
			
			content = ContentZapamietanoUstawienia();
			statusCode = 200;
		
		
		

		server.send(statusCode, "text/html", content);
		//Blink();
	});

	server.on("/reset", []() {
	
		ESP.restart();

		content = ContentZapamietanoUstawienia();
		statusCode = 200;
		server.send(statusCode, "text/html", content);
	});
	server.on("/clear_wifi", []() {
		WiFiManager wifiManager;
		wifiManager.resetSettings();

		ESP.restart();
		content = ContentInfo("Wyczyszczono dane i ustawienia wifi");
		statusCode = 200;
		server.send(statusCode, "text/html", content);
	});
	server.on("/clear_eeprom", []() {

		ClearEeprom();
		ESP.restart();

		content = ContentInfo("Wyczyszczono dane i ustawienia wifi");
		statusCode = 200;
		server.send(statusCode, "text/html", content);
	});
	server.on("/clear_all", []() {
		WiFiManager wifiManager;
		wifiManager.resetSettings();
		ClearEeprom();
		ESP.restart();
		content = ContentInfo("Wyczyszczono dane i ustawienia wifi");
		statusCode = 200;
		server.send(statusCode, "text/html", content);
	});
	server.on("/saveTemp", []() {
	
		for (int i = 0; i < settings.CountOfSensors; i++) {
			String param = "SetTemp_" + String(i);
			if (server.hasArg(param)) {
				settings.sens[i].Temp = server.arg(param).toFloat();
			}
		}
		SaveSettings(settings);
		content = ContentZapamietanoUstawienia();
		statusCode = 200;
		server.send(statusCode, "text/html",content);
	
	});
}
