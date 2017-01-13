


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

			  if (server.hasArg("TEMP1_IsOutside"))
				  param.Temp1.IsOutside = server.arg("TEMP1_IsOutside").toInt();
			  else
				  param.Temp1.IsOutside = 0;

			  if (server.hasArg("TEMP2_IsOutside"))
				  param.Temp2.IsOutside = server.arg("TEMP2_IsOutside").toInt();
			  else
				  param.Temp2.IsOutside = 0;


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

		

			  printTParameters(param);
			  SaveSettings(param);
			  settings = param;
			  content = ContentZapamietanoUstawienia();
			  statusCode = 200;
		} else {
			  content = "<html><h2><strong>Zapisu ustawieñ</strong></h2></html>";
		statusCode = 404;
		Serial.println("Sending 404");

		}
	
		  server.send(statusCode, "text/html", content);
	});

}

