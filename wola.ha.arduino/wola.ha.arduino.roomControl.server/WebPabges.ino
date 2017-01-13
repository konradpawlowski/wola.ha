
String ContentIndex() {
	String content;
	content = "<!DOCTYPE HTML>\r\n<html>";
	content += "<meta http-equiv=\"refresh\" content=\"60\">  ";
	content += "</p><form method='get' action='saveTemp'>";
	content += "<table align='center' border='1' cellpadding='1' cellspacing='1' style='width:600px;'>";
	content += "	<caption>";
	content += "	<h2>Czujniki</h2>";
	content += "	</caption>";
	content += "	<thead>";
	content += "		<tr>";
	content += "			<th scope='col' style='width:200px;'>Nazwa</th>";
	content += "			<th scope='col' style='width:200px;'>Temperatura</th>";
	content += "			<th scope='col' style='width:200px;'>Ustawiona temperatura</th>";
	content += "			<th scope='col' style='width:200px;'>Czas pomiaru</th>";
	
	content += "		</tr>";
	content += "	</thead>";
	content += "	<tbody>";
	char buffer[25];
	for (int i = 0; i<settings.CountOfSensors; i++)
	{
		content += "<tr>";
		content += "	<td align='center' valign='middle'>";
		content += "		<a href=\"http://" + String(Sensors[i].Address) + "\">" + String(Sensors[i].Name) + "</a>";
		content += "	</td>";
		content += "	<td align='center' valign='middle'>";
		content += floatToString(buffer, Sensors[i].Temp, 2);
		content += "	</td>";
		content += "	<td align='center' valign='middle'>";
		content += "<input id = '";
		content += String(i);
		content += "' maxlength = '10' name = 'SetTemp_";
		content += String(i);
		content += "' size = '10' type = 'text' value = '";
		content += floatToString(buffer, settings.sens[i].Temp, 2);
		content += "' / > ";
		content += "	</td>";
		content += "	<td align='center' valign='middle'>";
		content += String((millis() - Sensors[i].Time)/1000);
		content += "	</td>";
		content += "</tr>";
	}

	content += "	</tbody>";
	content += "</table>";
	content += "<p><input name = 'Save' type = 'submit' value = 'Zapisz temperature'/>​</p>";
	content += "<p>&nbsp;</p>";
	content += "</form>";
	content += "</html>";

	return content;
}

String ContentZapamietanoUstawienia() {
	String content;
	content = "<!DOCTYPE HTML>\r\n<html>";
	content += "<h2><strong>Zapamiętano ustawienia</strong></h2>";
	content += "<p><strong>​<a href=\"http://";
	content += WiFi.localIP().toString();
	content += "\">Powrót</a></strong></p>";
	content += "</html>";

	return content;
}
String ContentUstawienia() {
	String content = "<!DOCTYPE HTML>\r\n<html>";
	content += "</p><form method='get' action='setting'>";
	content += "<h3><strong>Adres serwera:&nbsp;<input maxlength=\"16\" name=\"ServerAddress\" size=\"16\" type=\"text\" value=\"_SERVER_ADDRESS_\" />​</strong></h3>";
	content += "<p><h3>Ilość czujników: <select name='CountSensors' size='1'>";
	for (int i = 0; i < 10; i++) {
		content += "<option ";
		if(i == settings.CountOfSensors)
			content += "selected = 'selected' ";
		content += "value = '"+String(i)+"'>"+String(i)+" </option> ";
	}

	content += "</select></h3></p>";
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
	content += "			<td><span style=\"font-weight: bold;\">&nbsp;</span><input name=\"Temp2_Enable\" type=\"checkbox\" ";
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
	return content;
}