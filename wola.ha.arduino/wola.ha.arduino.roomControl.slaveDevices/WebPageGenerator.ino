


void createWebServer()
{

	server.on("/", []() {
		//IPAddress ip = WiFi.softAPIP();
		//String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
		settings = ReadSettings();
		content = "<!DOCTYPE HTML>\r\n<html> <h1>Temperatura</h1> ";
		content += "<p><Strong>" + String(settings.Id) + " - " + settings.Name + "</strong></p>";
		content += "<p>";
		content += st;
		content += temp;
		content += "</html>";
		server.send(200, "text/html", content);
	});

	server.on("/ustawienia", []() {
		IPAddress ip = WiFi.softAPIP();
		String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
		content = "<!DOCTYPE HTML>\r\n<html>Ustawienia ";
		content += ipStr;
		content += "<p>";
		content += st;
		content += "</p><form method='get' action='setting'>";
		content += "<label>ID: </label><input name='id' length=10> \r\n";
		content += "<label>Name: </label><input name='name' length=20>";
		content += "<input type='submit'> \r\n";

		content += "<label>Adres serwera: </label><input name='serverAddress' length=16>";
		content += "<input type='submit'> \r\n";



		content += "</form>";
		content += "</html>";
		server.send(200, "text/html", content);
	});
	server.on("/setting", []() {
		
		TParameters param;
		  if (server.arg("id").length() > 0 && server.arg("name").length() > 0) {
			  param.Id = server.arg("id").toInt();
			  server.arg("name").toCharArray(param.Name, 10);
			  server.arg("serverAddress").toCharArray(param.ServerAddress, 16);

			  printTParameters(param);
			  SaveSettings(param);
			  settings = param;
			  content = "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}";
			  statusCode = 200;
		} else {
		content = "{\"Error\":\"404 not found\"}";
		statusCode = 404;
		Serial.println("Sending 404");

		}
	
		server.send(statusCode, "application/json", content);
	});

}
