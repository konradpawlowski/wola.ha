#include "floatToString.h"


void createWebServer()
{

	server.on("/", []() {
		content = "<!DOCTYPE HTML>\r\n<html>";
		content += "<meta http-equiv=\"refresh\" content=\"10\">  ";
		content += "<table align='center' border='1' cellpadding='1' cellspacing='1' style='width:300px;'>";
		content += "	<caption>";
		content += "	<h2>Czujniki</h2>";
		content += "	</caption>";
		content += "	<thead>";
		content += "		<tr>";
		content += "			<th scope='col'>Nazwa</th>";
		content += "			<th scope='col'>Temperatura</th>";
		if(settings.Temp1.Typ == Dht || settings.Temp2.Typ == Dht)
			content += "			<th scope='col'>Wilgotność</th>";
		
		content += "		</tr>";
		content += "	</thead>";
		content += "	<tbody>";
		if (settings.Temp1.Enable) {
			content += "		<tr>";
			content += "			<td align='center' valign='middle'>_TEMP1_NAME_</td>";
			content += "			<td align='center' valign='middle'>_TEMP1_TEMPC_VAL_</td>";
			if (settings.Temp1.Typ == Dht || settings.Temp2.Typ == Dht)
				content += "			<td align='center' valign='middle'>_TEMP1_HUMI_VAL_</td>";
			content += "		</tr>";
		}
		if (settings.Temp2.Enable) {
			content += "		<tr>";
			content += "			<td align='center' valign='middle'>_TEMP2_NAME_</td>";
			content += "			<td align='center' valign='middle'>_TEMP2_TEMPC_VAL_</td>";
			if (settings.Temp1.Typ == Dht || settings.Temp2.Typ == Dht)
				content += "			<td align='center' valign='middle'>_TEMP2_HUMI_VAL_</td>";
			content += "		</tr>";
		}
		content += "	</tbody>";
		content += "</table>";

		content += "<p>&nbsp;</p>";
		content += "</html>";

		char buffer[25];

		content.replace("_TEMP1_NAME_", settings.Temp1.Name);
		content.replace("_TEMP2_NAME_", settings.Temp2.Name);

		content.replace("_TEMP1_TEMPC_VAL_", floatToString(buffer, Temp1Value.Temp, 2));
		content.replace("_TEMP2_TEMPC_VAL_", floatToString(buffer, Temp2Value.Temp, 2));
		if (settings.Temp1.Typ == Dht || settings.Temp2.Typ == Dht) {
			content.replace("_TEMP1_HUMI_VAL_", floatToString(buffer, Temp1Value.Humi, 2));
			content.replace("_TEMP2_HUMI_VAL_", floatToString(buffer, Temp2Value.Humi, 2));
		}


		server.send(200, "text/html", content);
	});

	server.on("/ustawienia", []() {
		content = "<!DOCTYPE HTML>\r\n<html>";
		content += "</p><form method='get' action='setting'>";
		content += "<h3><strong>Adres serwera:&nbsp;<input maxlength=\"16\" name=\"ServerAddress\" size=\"16\" type=\"text\" value=\"_SERVER_ADDRESS_\" />​</strong></h3>";
		content += "";
		content += "<h3><strong>Konfiguracja czujnik&oacute;w </strong></h3>";
		content += "";
		content += "<table align=\"left\" border=\"2\" cellpadding=\"2\" cellspacing=\"2\" dir=\"ltr\" style=\"width:200px;\">";
		content += "	<caption><strong>Czujnik 1</strong></caption>";
		content += "	<tbody>";
		content += "		<tr>";
		content += "			<td><span style=\"display: none;\">&nbsp;</span><b>Aktywny:</b></td>";
		content += "			<td><span style=\"font-weight: bold;\">&nbsp;</span><input name=\"Temp1_Enable\" type=\"checkbox\" ";
		if (settings.Temp1.Enable == 1)
			content += "checked";
		content += "/>​</td>";
		content += "		</tr>";
		content += "		<tr>";
		content += "			<td><strong>Id:</strong></td>";
		content += "			<td><input maxlength=\"4\" name=\"Temp1_Id\" size=\"4\" type=\"text\" value=\"_TEMP1_ID_\" /> &nbsp;</td>";
		content += "		</tr>";
		content += "		<tr>";
		content += "			<td><strong>Nazwa:</strong>&nbsp;</td>";
		content += "			<td><br />";
		content += "			<input maxlength=\"20\" name=\"Temp1_Name\" size=\"20\" type=\"text\" value=\"_TEMP1_NAME_\" /><br />";
		content += "			&nbsp;</td>";
		content += "		</tr>";
		content += "		<tr>";
		content += "			<td><strong>Typ czujnika</strong></td>";
		content += "			<td><br />";
		content += "			&nbsp;<select name=\"Temp1_Type\" size=\"2\"><option _TEMP1_TYPE1_ value=\"1\">DS18B20</option><option _TEMP1_TYPE0_ value=\"0\">DHT</option></select></td>";
		content += "		</tr>";
		content += "	</tbody>";
		content += "</table>";
		content += "";
		content += "<table align=\"left\" border=\"2\" cellpadding=\"2\" cellspacing=\"2\" dir=\"ltr\" style=\"width:200px;\">";
		content += "	<caption><strong>Czujnik 2 </strong></caption>";
		content += "	<tbody>";
		content += "		<tr>";
		content += "			<td><span style=\"display: none;\">&nbsp;</span><b>Aktywny:</b></td>";
		content += "			<td><span style=\"font-weight: bold;\">&nbsp;</span><input name=\"Temp2_Enable\" type=\"checkbox\" " ;
		if (settings.Temp2.Enable == 1)
			content += "checked";
		content += "/>​</td>";
		content += "		</tr>";
		content += "		<tr>";
		content += "			<td><strong>Id:</strong></td>";
		content += "			<td><input maxlength=\"4\" name=\"Temp2_Id\" size=\"4\" type=\"text\" value=\"_TEMP2_ID_\" /> &nbsp;</td>";
		content += "		</tr>";
		content += "		<tr>";
		content += "			<td><strong>Nazwa:</strong>&nbsp;</td>";
		content += "			<td><br />";
		content += "			<input maxlength=\"20\" name=\"Temp2_Name\" size=\"20\" type=\"text\" value=\"_TEMP2_NAME_\" /><br />";
		content += "			&nbsp;</td>";
		content += "		</tr>";
		content += "		<tr>";
		content += "			<td><strong>Typ czujnika</strong></td>";
		content += "			<td><br />";
		content += "			&nbsp;<select name=\"Temp2_Type\" size=\"2\"><option _TEMP2_TYPE1_ value=\"1\">DS18B20</option><option _TEMP2_TYPE0_ value=\"0\">DHT</option></select></td>";
		content += "		</tr>";
		content += "	</tbody>";
		content += "</table>";
		content += "";
		content += "<p>​​</p>";
		content += "";
		content += "<p>&nbsp;</p>";
		content += "";
		content += "<p>&nbsp;</p>";
		content += "";
		content += "<p>&nbsp;</p>";
		content += "";
		content += "<p>&nbsp;</p>";
		content += "";
		content += "<p>&nbsp;</p>";
		content += "";
		content += "<p>&nbsp;</p>";
		content += "";
		content += "<p><input type=\"submit\" value=\"Zapisz\" /></p>";
		content += "</form>";
		content += "</html>";

		content.replace("_SERVER_ADDRESS_", settings.ServerAddress);
		content.replace("_TEMP1_ID_", String(settings.Temp1.Id));
		content.replace("_TEMP1_NAME_", settings.Temp1.Name);
		
		switch (settings.Temp1.Typ)
		{
		case Ds18b20:
			content.replace("_TEMP1_TYPE1_", "selected");
			content.replace("_TEMP1_TYPE0_", "");
			break;
		
		case Dht:
			content.replace("_TEMP1_TYPE1_", "");
			content.replace("_TEMP1_TYPE0_", "selected");
			break;
		default:
			break;
		}


		content.replace("_TEMP2_ID_", String(settings.Temp2.Id));
		content.replace("_TEMP2_NAME_", settings.Temp2.Name);


		switch (settings.Temp2.Typ)
		{
		case Ds18b20:
			content.replace("_TEMP2_TYPE1_", "selected");
			content.replace("_TEMP2_TYPE0_", "");
			break;

		case Dht:
			content.replace("_TEMP2_TYPE1_", "");
			content.replace("_TEMP2_TYPE0_", "selected");
			
			break;
		default:
			break;
		}

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