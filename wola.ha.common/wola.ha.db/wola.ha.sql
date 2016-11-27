--
-- Plik wygenerowany przez SQLiteStudio v3.1.1 dnia niedz. lis 27 19:36:19 2016
--
-- U¿yte kodowanie tekstu: System
--
PRAGMA foreign_keys = off;
BEGIN TRANSACTION;

-- Tabela: DataBusType
CREATE TABLE DataBusType (Id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, Type VARCHAR (10) NOT NULL DEFAULT Brak CHECK (Type == 'Brak' or Type == 'I2C' or Type == 'Raspberry' or Type == 'SPI'), Name VARCHAR (50), Descript VARCHAR (100), Address CHAR (10));
INSERT INTO DataBusType (Id, Type, Name, Descript, Address) VALUES (1, 'I2C', 'Arduino 1', 'Arduino w kot³ownu', '0x40');
INSERT INTO DataBusType (Id, Type, Name, Descript, Address) VALUES (2, 'I2C', 'Arduino 2', 'Arduino strycg', '0x41');
INSERT INTO DataBusType (Id, Type, Name, Descript, Address) VALUES (3, 'I2C', 'BMP180', 'Czujnik ciœnienia', NULL);
INSERT INTO DataBusType (Id, Type, Name, Descript, Address) VALUES (4, 'I2C', 'Expander In', 'Rozszerzenie wejœæ', '0x27');
INSERT INTO DataBusType (Id, Type, Name, Descript, Address) VALUES (5, 'I2C', 'Expander OUT', 'Rozszerzenie wyjœæ', '0x20');
INSERT INTO DataBusType (Id, Type, Name, Descript, Address) VALUES (6, 'Raspberry', 'Raspberry', 'Raspberry', NULL);

-- Tabela: SensorHumidityValues
CREATE TABLE SensorHumidityValues (Date DATETIME PRIMARY KEY DESC NOT NULL DEFAULT (CURRENT_TIMESTAMP), Sensor INTEGER REFERENCES Sensors (Id) NOT NULL, Value DECIMAL (6, 2) DEFAULT (0));

-- Tabela: SensorOnOffValue
CREATE TABLE SensorOnOffValue (Date DATETIME PRIMARY KEY DESC NOT NULL DEFAULT (CURRENT_TIMESTAMP), Sensor INTEGER REFERENCES Sensors (Id) NOT NULL, Value INTEGER NOT NULL DEFAULT (0) CHECK (Value == 0 or Value == 1));

-- Tabela: Sensors
CREATE TABLE Sensors (Id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, SensorType VARCHAR (10) REFERENCES SensorType (Type) ON UPDATE CASCADE MATCH FULL, DateBusType INTEGER REFERENCES DataBusType (Id), Name VARCHAR (100), Descript VARCHAR (100), Pin CHAR (10));
INSERT INTO Sensors (Id, SensorType, DateBusType, Name, Descript, Pin) VALUES (1, 'DHT22', 1, 'Zewnetrzny', 'Temperatura na zewnatrz', NULL);

-- Tabela: SensorTemperatureValues
CREATE TABLE SensorTemperatureValues (Date DATETIME PRIMARY KEY DESC NOT NULL DEFAULT (CURRENT_TIMESTAMP), Sensor INTEGER REFERENCES Sensors (Id) NOT NULL, value DECIMAL (6, 2) DEFAULT (0));

-- Tabela: SensorType
CREATE TABLE SensorType (Type STRING (10) PRIMARY KEY UNIQUE NOT NULL, Descript STRING (100) NOT NULL);
INSERT INTO SensorType (Type, Descript) VALUES ('DS18B20', 'Temperatura');
INSERT INTO SensorType (Type, Descript) VALUES ('DHT11', 'Temperatura i wilgotnoœæ');
INSERT INTO SensorType (Type, Descript) VALUES ('DHT22', 'Temperatura i wilgotnoœæ');
INSERT INTO SensorType (Type, Descript) VALUES ('BMP180', 'Temperatura i ciœnienie');
INSERT INTO SensorType (Type, Descript) VALUES ('ACS712', 'Czujnik przep³ywu pr¹du');
INSERT INTO SensorType (Type, Descript) VALUES ('OnOff', 'Czujnik On / Off');

-- Indeks: idx_DataBusType_Id
CREATE UNIQUE INDEX idx_DataBusType_Id ON DataBusType (Id);

-- Indeks: idx_SensorOnOffValues_Date
CREATE UNIQUE INDEX idx_SensorOnOffValues_Date ON SensorOnOffValue (Date ASC);

-- Indeks: idx_Sensors_Id
CREATE UNIQUE INDEX idx_Sensors_Id ON Sensors (Id ASC);

-- Indeks: idx_SensorsHumidityValues_Date
CREATE UNIQUE INDEX idx_SensorsHumidityValues_Date ON SensorHumidityValues (Date ASC);

-- Indeks: idx_SensorTemperatureValue_Date
CREATE UNIQUE INDEX idx_SensorTemperatureValue_Date ON SensorTemperatureValues (Date ASC);

-- Indeks: idx_SensorType_Type
CREATE UNIQUE INDEX idx_SensorType_Type ON SensorType (Type ASC);

COMMIT TRANSACTION;
PRAGMA foreign_keys = on;
