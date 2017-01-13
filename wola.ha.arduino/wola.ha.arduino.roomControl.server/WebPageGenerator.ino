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
		
		TParameters param;
		  if (server.arg("Temp1_Id").length() > 0 && server.arg("Temp1_Name").length() > 0) {
			  param.Temp1.Id = server.arg("Temp1_Id").toInt();
			  server.arg("Temp1_Name").toCharArray(param.Temp1.Name, 20);
			  
			  server.arg("ServerAddress").toCharArray(param.ServerAddress, 16);

			  if (server.arg("Temp1_Enable").length() > 0)
			  {
				  param.Temp1.Enable = 1;
			  }
			  else
			  {
				  param.Temp1.Enable = 0;
			  }
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

			  if (server.arg("Temp2_Enable").length() > 0)
				  param.Temp2.Enable = 1;
			  else
				  param.Temp2.Enable = 0;

			  param.Temp2.Id = server.arg("Temp2_Id").toInt();
			  server.arg("Temp2_Name").toCharArray(param.Temp2.Name, 20);
			  param.Temp2.Pin = (int)Temp2;
			  param.Temp2.Typ = server.arg("Temp2_Type").toInt();

			  if (server.hasArg("CountSensors"))
				  param.CountOfSensors = server.arg("CountSensors").toInt();
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
		
		Sensors[id].Time = millis();
			Serial.println("Id: ");
			Serial.println(id);
			

			Serial.println("Temp: ");
			Serial.println(temp);
			
			content = ContentZapamietanoUstawienia();
			statusCode = 200;
		
		
		

		server.send(statusCode, "text/html", content);
	});

	server.on("/temp2", []() {
	
		for (int i = 0; i < 10; i++)
		{

			printValuesOnOff(settings.sens[i]);
		}


		content = ContentZapamietanoUstawienia();
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