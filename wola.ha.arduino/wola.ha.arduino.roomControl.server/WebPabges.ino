
String ContentIndex() {
	long mil = millis() / 1000;
	String content;
	content = F("<!DOCTYPE HTML>\r\n<html>");
	content += F("<meta http-equiv=\"refresh\" content=\"10\">  ");
	content += F("</p><form method='get' action='saveTemp'>");
	content += "<p><strong><a href=\"http://" + WiFi.localIP().toString() + "/ustawienia\">Ustawienia</a></strong></p>";
	content += "<p align='right'>Czas od uruchomienia: " + time(mil) + "</p>";
	content += "<p align='right'> pamięć: " + String(ESP.getFreeHeap()) + "</p>";
	content += F("</br>");


	if(!digitalRead(settings.SelectedRelay))
		content += F("<p>Stan pieca: <span style=\"color:#008000; \"><strong>PODTRZYMANIE</strong></span></p>");
	else
		content += F("<p>Stan pieca: <strong><span style=\"color:#FF0000; \">PRACA</span></strong></p>");

	
	content += F("<table align='center' border='1' cellpadding='1' cellspacing='1' style='width:600px;'>");
	content += F("	<caption>");
	content += F("	<h2>Czujniki</h2>");
	content += F("	</caption>");
	content += F("	<thead>");
	content += F("		<tr>");
	content += F("			<th scope='col' style='width:200px;'>Nazwa</th>");
	content += F("			<th scope='col' style='width:200px;'>Temperatura</th>");
	content += F("			<th scope='col' style='width:200px;'>Ustawiona temperatura</th>");
	content += F("			<th scope='col' style='width:200px;'>Czas pomiaru</th>");
//	content += "			<th scope='col' style='width:200px;'>Zewnetrzny</th>";
	content += F("		</tr>");
	content += F("	</thead>");
	content += F("	<tbody>");
	char buffer[25];
	for (int i = 0; i<settings.CountOfSensors; i++)
	{
		String color = CheckTempSensor(i) ? "red" : "green";
		content += F("<tr>");
		content += F("	<td align='center' valign='middle'>");
		content += "		<a href=\"http://" + String(Sensors[i].Address) + "\">" + String(Sensors[i].Name) + "</a>";
		content += F("	</td>");
		
		
		content += F("	<td align='center' fb=\"white\" bgcolor=\"");
		content += color;
		
		content += F("\"valign='middle'>");
		content += F("<strong><span style=\"color:white; \"><span style=\"background - color:"); 
		content += color+"; \">";
		content += floatToString(buffer, Sensors[i].Temp, 2);
		content += F("	</span></span></strong></td>");



		content += F("	<td align='center' valign='middle'>");
		content += F("<input id = '");
		content += String(i);
		content += F("' maxlength = '10' name = 'SetTemp_");
		content += String(i);
		content += F("' size = '10' type = 'text' value = '");
		content += floatToString(buffer, settings.sens[i].Temp, 2);
		content += F("' />");
		content += F("	</td>");
		content += F("	<td align='center' valign='middle'>");
		content += String((millis() - Sensors[i].Time)/1000);
		content += F("	</td>");

		content += F("	</td>");
		/*content += "	<td align='center' valign='middle'>";
		content += String(Sensors[i].IsOutside);
		content += "	</td>";*/
		content += F("</tr>");
	}

	content += F("	</tbody>");
	content += F("</table>");
	content += F("<p><input name = 'Save' type = 'submit' value = 'Zapisz temperature'/>​</p>");
	content += F("<p>&nbsp;</p>");
	content += F("</form>");
	content += F("</html>");

	return content;
}

String ContentZapamietanoUstawienia() {
	String content;
	content = F("<!DOCTYPE HTML>\r\n<html>");
	content += F("<h2><strong>Zapamiętano ustawienia</strong></h2>");
	content += F("<p><strong>​<a href=\"http://");
	content += WiFi.localIP().toString();
	content += F("\">Powrót</a></strong></p>");
	content += F("</html>");

	return content;
}
String ContentUstawienia() {
	String content = F("<!DOCTYPE HTML>\r\n<html>");
	content += F("</p><form method='get' action='setting'>");
	content += F("<h3><strong>Adres serwera:&nbsp;<input maxlength=\"16\" name=\"ServerAddress\" size=\"16\" type=\"text\" value=\"_SERVER_ADDRESS_\" />​</strong></h3>");
	content += F("<p><h3>Ilość czujników: <select name='CountSensors' size='1'>");
	for (int i = 0; i < 10; i++) {
		content += F("<option ");
		if(i == settings.CountOfSensors)
			content += F("selected = 'selected' ");
		content += "value = '"+String(i)+"'>"+String(i)+" </option> ";
	}

	content += F("</select></h3></p>");

	content += F("<p><h3>Załaczenie pieca: <select name='SelectedRelay' size='1'>");
	content += F("<option ");
	if (4 == settings.SelectedRelay)
		content += F("selected = 'selected' ");
	content += F("value = '4'>Przekaźnik 1</option> ");
	content += F("<option ");
	if (5 == settings.SelectedRelay)
		content += F("selected = 'selected' ");
	content += F("value = '5'>Przekaźnik 2</option> ");

	content += F("</select></h3></p>");


	content += F("<h3><strong>Konfiguracja czujnik&oacute;w </strong></h3>");

	content += F("<table align=\"left\" border=\"2\" cellpadding=\"2\" cellspacing=\"2\" dir=\"ltr\" style=\"width:200px;\">");
	content += F("	<caption><strong>Czujnik 1</strong></caption>");
	content += F("	<tbody>");
	content += F("		<tr>");
	content += F("			<td><span style=\"display: none;\">&nbsp;</span><b>Aktywny:</b></td>");
	content += F("			<td><span style=\"font-weight: bold;\">&nbsp;</span><input name=\"Temp1_Enable\" type=\"checkbox\" ");
	if (settings.Temp1.Enable == 1)
		content += F("checked");
	content += F("/>​</td>");
	content += F("		</tr>");
	content += F("		<tr>");
	content += F("			<td><strong>Id:</strong></td>");
	content += F("			<td><input maxlength=\"4\" name=\"Temp1_Id\" size=\"4\" type=\"text\" value=\"_TEMP1_ID_\" /> &nbsp;</td>");
	content += F("		</tr>");
	content += F("		<tr>");
	content += F("			<td><strong>Nazwa:</strong>&nbsp;</td>");
	content += F("			<td><br />");
	content += F("			<input maxlength=\"20\" name=\"Temp1_Name\" size=\"20\" type=\"text\" value=\"_TEMP1_NAME_\" /><br />");
	content += F("			&nbsp;</td>");
	content += F("		</tr>");
	content += F("		<tr>");
	content += F("			<td><strong>Typ czujnika</strong></td>");
	content += F("			<td><br />");
	content += F("			&nbsp;<select name=\"Temp1_Type\" size=\"2\"><option _TEMP1_TYPE1_ value=\"1\">DS18B20</option><option _TEMP1_TYPE0_ value=\"0\">DHT</option></select></td>");
	content += F("		</tr>");

	content += F("		<tr>");
	content += F("			<td><strong>Czujnik zewnętrzny</strong></td>");
	
	content += F("			<td><span style=\"font-weight: bold;\">&nbsp;</span><input name=\"Temp1_IsOutside\" type=\"checkbox\" ");
	if (settings.Temp1.IsOutside == 1)
		content += F("checked");
	content += F("/>​</td>");
	content += F("		</tr>");

	content += F("	</tbody>");
	content += F("</table>");
	
	content += F("<table align=\"left\" border=\"2\" cellpadding=\"2\" cellspacing=\"2\" dir=\"ltr\" style=\"width:200px;\">");
	content += F("	<caption><strong>Czujnik 2 </strong></caption>");
	content += F("	<tbody>");
	content += F("		<tr>");
	content += F("			<td><span style=\"display: none;\">&nbsp;</span><b>Aktywny:</b></td>");
	content += F("			<td><span style=\"font-weight: bold;\">&nbsp;</span><input name=\"Temp2_Enable\" type=\"checkbox\" ");
	if (settings.Temp2.Enable == 1)
		content += F("checked");
	content += F("/>​</td>");
	content += F("		</tr>");
	content += F("		<tr>");
	content += F("			<td><strong>Id:</strong></td>");
	content += F("			<td><input maxlength=\"4\" name=\"Temp2_Id\" size=\"4\" type=\"text\" value=\"_TEMP2_ID_\" /> &nbsp;</td>");
	content += F("		</tr>");
	content += F("		<tr>");
	content += F("			<td><strong>Nazwa:</strong>&nbsp;</td>");
	content += F("			<td><br />");
	content += F("			<input maxlength=\"20\" name=\"Temp2_Name\" size=\"20\" type=\"text\" value=\"_TEMP2_NAME_\" /><br />");
	content += F("			&nbsp;</td>");
	content += F("		</tr>");
	content += F("		<tr>");
	content += F("			<td><strong>Typ czujnika</strong></td>");
	content += F("			<td><br />");
	content += F("			&nbsp;<select name=\"Temp2_Type\" size=\"2\"><option _TEMP2_TYPE1_ value=\"1\">DS18B20</option><option _TEMP2_TYPE0_ value=\"0\">DHT</option></select></td>");
	content += F("		</tr>");

	content += F("		<tr>");
	content += F("			<td><strong>Czujnik zewnętrzny</strong></td>");

	content += F("			<td><span style=\"font-weight: bold;\">&nbsp;</span><input name=\"Temp2_IsOutside\" type=\"checkbox\" ");
	if (settings.Temp2.IsOutside == 1)
		content += F("checked");
	content += F("/>​</td>");
	content += F("		</tr>");

	content += F("	</tbody>");
	content += F("</table>");
	content += F("<p>​​</p>");
	content += F("<p>&nbsp;</p>");
	content += F("<p>&nbsp;</p>");
	content += F("<p>&nbsp;</p>");
	content += F("<p>&nbsp;</p>");
	content += F("<p>&nbsp;</p>");
	content += F("<p>&nbsp;</p>");
	
	content += F("<p></br><input type=\"submit\" value=\"Zapisz\" /></p>");
	content += "<p></br><strong><a href=\"http://" + WiFi.localIP().toString() + "\">Powrót</a>";
	content += F("</form>");
	content += F("</html>");

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