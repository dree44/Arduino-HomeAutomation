#include "GeneralIO.h"
#include <ExtendedEEPROM.h>


G_IOList gNames;
G_SensorList gSensors;
G_ActuatorList gActuators;
G_TriggerList gTriggers;
G_EventList gEvents;
ExtendedEEPROMClass eeprom;

// -----------TimedValue-----------
TimedValue::TimedValue() {
	lastRead=-1;
	stringType=false;
	string="";
	numOfValue=0;
	nextvalue=0;
	value=0;
	time=0;
}

TimedValue::~TimedValue() {
	if(value) delete value;
	if(time) delete time;
}

void TimedValue::set(TimedValue& in) {
	lastRead=in.lastRead;
    stringType=in.stringType;
    numOfValue=in.numOfValue;
    nextvalue=in.nextvalue;
 	if(numOfValue>0) value=new int[numOfValue];
	if(numOfValue>1) time=new int[numOfValue-1];
    int i;
    for(i=0;i<numOfValue;++i) {
      value[i]=in.value[i];
      if(i<numOfValue-1) time[i]=in.time[i];
    }
    string=in.string;
 }

TimedValue::TimedValue(int v) {
	lastRead = -1;
	stringType = false;
	string = "";
	numOfValue = 1;
	nextvalue = 0;
	value = new int[numOfValue];
	time = 0;
	value[0] = v;
}

void TimedValue::append(TimedValue& in) {
    if(stringType || in.stringType) return;
    int* newvalue=0;
	if(numOfValue+in.numOfValue>0) newvalue=new int[numOfValue+in.numOfValue];
    int* newtime=0;
	if(numOfValue+in.numOfValue>1) newtime=new int[numOfValue+in.numOfValue-1];
    for(int i=0;i<numOfValue;++i) {
      newvalue[i]=value[i];
      if(i<numOfValue-1) newtime[i]=time[i]; else newtime[i]=0;
    }
    for(int i=0;i<in.numOfValue;++i) {
      newvalue[numOfValue+i]=in.value[i];
      if(i<in.numOfValue-1) newtime[numOfValue+i]=in.time[i];
    }
	numOfValue+=in.numOfValue;
	if(time) delete time;
	if(value) delete value;
	time=newtime;
	value=newvalue;
/*	Serial.print("nextv ");
	Serial.print(nextvalue);
	Serial.print(" numV ");
	Serial.println(numOfValue);
	Serial.println(contentToString());
	Serial.println(lastRead);
	Serial.println(millis());
	*/
}

void TimedValue::print(String& result) {
	if(stringType) result+=("\""+string+"\"");
	else for(int i=0;i<numOfValue;++i) {
		result += value[i];
		if (numOfValue>1 && i<numOfValue - 1) {
			result+="<";
			result+=time[i];
			result+=">";
		}
	}
}

String TimedValue::contentToString() {
	String ret;
	print(ret);
	return ret;
}

void TimedValue::read(int& result) {
	if(stringType) {
		if(string=="!") {
			if(result>0) result=0;
			else result=1;
		}
		stringType=false;
		string="";
		return;
	}
	if(numOfValue==0) return;
	if(numOfValue==nextvalue) {
		purge();
		return;
	}
	result=value[nextvalue];
	unsigned long now=millis();
	if (now<lastRead) lastRead = now;
	if (lastRead + time[nextvalue] <= now) {
		lastRead+=time[nextvalue];
		nextvalue++;
	}
}

void TimedValue::purge() {
	if(nextvalue<numOfValue) return;
	if(value) delete value;
	if(time) delete time;
	lastRead = -1;
	value = 0;
	time=0;
	numOfValue=0;
	nextvalue=0;
}

void TimedValue::SDSave(File f,byte tabs) {
	if(stringType) {
		for(int i=0;i<tabs;++i) f.print("\t");
		f.print("\"string\" : \"");
		f.print(string);
		f.print("\"");
	}
	if(numOfValue>0) {
		for(int i=0;i<tabs;++i) f.print("\t");
		f.print("\"values\" : [");
		for(int x=0;x<numOfValue;++x) {
			f.print(value[x]);
			if(x<numOfValue-1) f.print(",");
		}
		f.print("]");
		if(numOfValue>1) {
			f.print(",\n");
			for(int i=0;i<tabs;++i) f.print("\t");
			f.print("\"times\" : [");
			for(int x=0;x<numOfValue-1;++x) {
				f.print(time[x]);
				if(x<numOfValue-2) f.print(",");
			}
			f.print("]");
		}
	}
}

boolean TimedValue::SDLoad(File& f, String& error) {
	String str = "";
	for (char c = f.read(); f.available() && c != '\"'; c = f.read());
	if (!f.available()) return true;
	for (char c = f.read(); f.available() && c != '\"'; c = f.read()) str += c;
	if (str == F("string")) {
		str = "";
		for (char c = f.read(); f.available() && c != '\"'; c = f.read());
		if (!f.available()) return false;
		for (char c = f.read(); f.available() && c != '\"'; c = f.read()) str += c;
		this->stringType = true;
		this->string = str;
		return true;
	}
	if (str != F("values")) { error = F("Invalid I/O description: >\""); error += str; error += F("\" insdead of \"values\""); return false; }
	str = "";
	for (char c = f.read(); f.available() && c != '['; c = f.read());
	if (!f.available()) return true;
	for (char c = f.read(); f.available() && c != ']'; c = f.read()) str += c;
	byte n=1;
	for (byte i = 0; i < str.length(); ++i) if (str[i] == ',') n++;
	this->numOfValue = n;
	if (numOfValue>0) value = new int[numOfValue];
	if (numOfValue>1) time = new int[numOfValue - 1];
	String s;
	for (byte i = 0, j = 0; j<str.length(); ++i, ++j) {
		s = "";
		for (; str[j] != ',' && j < str.length(); ++j) if (IsNumber(str[j])) s += str[j];
		this->value[i] = s.toInt();
	}
	if (numOfValue > 1) {
		str = "";
		for (char c = f.read(); f.available() && c != '\"'; c = f.read());
		if (!f.available()) return true;
		for (char c = f.read(); f.available() && c != '\"'; c = f.read()) str += c;
		if (str != F("times")) { error = F("Invalid I/O description: >\""); error += str; error += F("\" insdead of \"times\""); return false; }
		str = "";
		for (char c = f.read(); f.available() && c != '['; c = f.read());
		if (!f.available()) return true;
		for (char c = f.read(); f.available() && c != ']'; c = f.read()) str += c;
		for (byte i = 0, j = 0; j < str.length(); ++i, ++j) {
			s = "";
			for (; str[j] != ',' && j < str.length(); ++j) if (IsNumber(str[j])) s += str[j];
			this->time[i] = s.toInt();
		}
	}
	return true;
}
// -----------IOBase-----------
G_IOBase::G_IOBase() {
	index=0xff;
}

G_IOBase::G_IOBase(byte idx) {
	index = idx;
}

G_IOBase::~G_IOBase() {
	index = 0xff;
}

boolean G_IOBase::changed() {
	return change;
}

void G_IOBase::read() {}
void G_IOBase::trigger() {}
void G_IOBase::set(TimedValue* tv) {}
void G_IOBase::write() {}
int G_IOBase::get() {return -1;}
int G_IOBase::getnew() {return -1;}
String G_IOBase::valueToString() {return "x";}

void G_IOBase::SDSave(File f,byte tabs) {
	for(int i=0;i<tabs;++i) f.print("\t");
	f.print("\"name\" : \"");
	f.print(((G_Name*)gNames.find(index))->name);
	f.print("\"");
}

// -----------Variable-----------
G_Variable::G_Variable(byte idx) : G_IOBase(idx) {
	scale = 0;
	hexaPrint = 0xff;
}

G_Variable::~G_Variable() {
}

void G_Variable::setup() {
	value = 0;
	newvalue = 0;
}

void G_Variable::print(String& result,String ident) {
}

String G_Variable::contentToString() {
	return "";
}

void G_Variable::read() {
	change=false;
	value=newvalue;
	comingValue.read(newvalue);
	if (value != newvalue) change = true;
	
/*	if(numOfNewValues>0) {
		boolean clean=comingValues[0]->read(newvalue);
		if(clean) { // remove timedvalue
			numOfNewValues--;
			TimedValue** newTV=0;
			if(numOfNewValues>0) newTV=new TimedValue*[numOfNewValues];
			for(int i=0;i<numOfNewValues;++i) {
				newTV[i]=comingValues[i+1];
				delete comingValues[i+1];
			}
			delete comingValues;
			comingValues=newTV;
		}
		if(value!=newvalue) change=true;
	}*/
}

void G_Variable::set(TimedValue* tv) {
	comingValue.append(*tv);
}

String G_Variable::valueToString() {
	String str="";
	if (hexaPrint!=0xff) {
		if (hexaPrint>3) str += DigitToHex((newvalue >> 12) & 0xF);
		if (hexaPrint>2) str += DigitToHex((newvalue >> 8) & 0xF);
		if (hexaPrint>1) str += DigitToHex((newvalue >> 4) & 0xF);
		if (hexaPrint>0) str += DigitToHex(newvalue & 0xF);
		for (byte i = hexaPrint; i < 4; ++i) str += ' ';
		return str;
	}
	if (scale == 0){
		str += newvalue;
	}
	else {
		str += doubleToString((double)newvalue / pow(10, scale), scale);
	}
	return str;
}

void G_Variable::SDSave(File f,byte tabs) {
	G_IOBase::SDSave(f,tabs);
	f.print(",\n");
}

boolean G_Variable::SDLoad(File& f, String& error) {
	gSensors.variables.add(this);
	setup();
	return true;
}

// -----------AnalogSensor-----------
G_AnalogSensor::G_AnalogSensor(byte idx) : G_IOBase(idx) {
	lastRead=-1;
};

void G_AnalogSensor::setup() {
	value = 0;
	newvalue = 0;
	//setup
	pinMode(pin, INPUT);
}

void G_AnalogSensor::print(String& result,String ident) {
	result+="analog in pin ";
	result+=String(pin);
}

String G_AnalogSensor::contentToString() {
	return "pin " + String(pin);
}

void G_AnalogSensor::read() {
	change=false;
	value=newvalue;
	unsigned long now=millis();
	if (now<lastRead) lastRead = now;
	if (lastRead + ANALOGREADPERIOD <= now) {
		lastRead+=ANALOGREADPERIOD;
		newvalue=analogRead(pin);
		if(value!=newvalue) change=true;
	}
}

String G_AnalogSensor::valueToString() {
	String str;
	str+=value;
	return str;
}

void G_AnalogSensor::SDSave(File f,byte tabs) {
	G_IOBase::SDSave(f,tabs);
	f.print(",\n");
	for(int i=0;i<tabs;++i) f.print("\t");
	f.print("\"pin\" : ");
	f.print(pin);
	f.print("\n");
}

boolean G_AnalogSensor::SDLoad(File& f, String& error) {
	String str = "";
	for (char c = f.read(); f.available() && c != '\"'; c = f.read());
	if (!f.available()) return true;
	for (char c = f.read(); f.available() && c != '\"'; c = f.read()) str += c;
	if (str != F("pin")) { error = F("Invalid I/O description: >\""); error += str; error += F("\" insdead of \"pin\""); return false; }
	str = "";
	for (char c = f.read(); f.available() && c != '}'; c = f.read()) if(IsNumber(c)) str += c;
	byte pinid = (byte)str.toInt();
	this->pin = pinid;
	gSensors.analogsensors.add(this);
	setup();
	return true;
}

// -----------DigitalSensor-----------
G_DigitalSensor::G_DigitalSensor(byte idx) : G_IOBase(idx) {
};

void G_DigitalSensor::setup() {
	value = 0;
	newvalue = 0;
	//setup
	pinMode(pin, INPUT);
}

void G_DigitalSensor::print(String& result,String ident) {
	result+="digital in pin ";
	result+=String(pin);
}

String G_DigitalSensor::contentToString() {
	return "pin " + String(pin);
}

void G_DigitalSensor::read() {
	change=false;
	value=newvalue;
	newvalue=digitalRead(pin);
	if(value!=newvalue) change=true;
}

String G_DigitalSensor::valueToString() {
	String str;
	str+=value;
	return str;
}

void G_DigitalSensor::SDSave(File f,byte tabs) {
	G_IOBase::SDSave(f,tabs);
	f.print(",\n");
	for(int i=0;i<tabs;++i) f.print("\t");
	f.print("\"pin\" : ");
	f.print(pin);
	f.print("\n");
}

boolean G_DigitalSensor::SDLoad(File& f, String& error) {
	String str = "";
	for (char c = f.read(); f.available() && c != '\"'; c = f.read());
	if (!f.available()) return true;
	for (char c = f.read(); f.available() && c != '\"'; c = f.read()) str += c;
	if (str != F("pin")) { error = F("Invalid I/O description: >\""); error += str; error += F("\" insdead of \"pin\""); return false; }
	str = "";
	for (char c = f.read(); f.available() && c != '}'; c = f.read()) if (IsNumber(c)) str += c;
	byte pinid = (byte)str.toInt();
	this->pin = pinid;
	gSensors.digitalsensors.add(this);
	setup();
	return true;
}
// -----------AnalogActuator-----------
G_AnalogActuator::G_AnalogActuator(byte idx) : G_IOBase(idx) {
};

void G_AnalogActuator::setup() {
	value = 0;
	//setup
	pinMode(pin, OUTPUT);
}
	
void G_AnalogActuator::print(String& result,String ident) {
	result+="analog out pin ";
	result+=String(pin);
}

String G_AnalogActuator::contentToString() {
	return "pin " + String(pin);
}

void G_AnalogActuator::set(TimedValue* tv) {
	outputvalue.set(*tv);
}

void G_AnalogActuator::write() {
	int out=value;
	outputvalue.read(out);
	if((unsigned short)out!=value) {
		value=(unsigned short)out;
		analogWrite(pin,value);
	}
}

String G_AnalogActuator::valueToString() {
	String str;
	str+=value;
	return str;
}

void G_AnalogActuator::SDSave(File f,byte tabs) {
	G_IOBase::SDSave(f,tabs);
	f.print(",\n");
	for(int i=0;i<tabs;++i) f.print("\t");
	f.print("\"pin\" : ");
	f.print(pin);
	f.print("\n");
}

boolean G_AnalogActuator::SDLoad(File& f, String& error) {
	String str = "";
	for (char c = f.read(); f.available() && c != '\"'; c = f.read());
	if (!f.available()) return true;
	for (char c = f.read(); f.available() && c != '\"'; c = f.read()) str += c;
	if (str != F("pin")) { error = F("Invalid I/O description: >\""); error += str; error += F("\" insdead of \"pin\""); return false; }
	str = "";
	for (char c = f.read(); f.available() && c != '}'; c = f.read()) if (IsNumber(c)) str += c;
	byte pinid = (byte)str.toInt();
	this->pin = pinid;
	gActuators.analogactuators.add(this);
	setup();
	return true;
}
// -----------DigitalActuator-----------
G_DigitalActuator::G_DigitalActuator(byte idx) : G_IOBase(idx) {
};

void G_DigitalActuator::setup() {
	value = 0;
	//setup
	pinMode(pin, OUTPUT);
}

void G_DigitalActuator::print(String& result,String ident) {
	result+="digital out pin ";
	result+=String(pin);
}

String G_DigitalActuator::contentToString() {
	return "pin " + String(pin);
}

void G_DigitalActuator::set(TimedValue* tv) {
	outputvalue.set(*tv);
}

void G_DigitalActuator::write() {
	int out=value;
	outputvalue.read(out);
	if((boolean)out!=value) {
		value=(boolean)out;
		digitalWrite(pin,value);
	}
}

String G_DigitalActuator::valueToString() {
	String str;
	str+=value;
	return str;
}

void G_DigitalActuator::SDSave(File f,byte tabs) {
	G_IOBase::SDSave(f,tabs);
	f.print(",\n");
	for(int i=0;i<tabs;++i) f.print("\t");
	f.print("\"pin\" : ");
	f.print(pin);
	f.print("\n");
}

boolean G_DigitalActuator::SDLoad(File& f, String& error) {
	String str = "";
	for (char c = f.read(); f.available() && c != '\"'; c = f.read());
	if (!f.available()) return true;
	for (char c = f.read(); f.available() && c != '\"'; c = f.read()) str += c;
	if (str != F("pin")) { error = F("Invalid I/O description: >\""); error += str; error += F("\" insdead of \"pin\""); return false; }
	str = "";
	for (char c = f.read(); f.available() && c != '}'; c = f.read()) if (IsNumber(c)) str += c;
	byte pinid = (byte)str.toInt();
	this->pin = pinid;
	gActuators.digitalactuators.add(this);
	setup();
	return true;
}
// -----------RFID-----------
G_RFID::G_RFID(byte idx) : G_IOBase(idx) {
	authz = 0;
};
	
G_RFID::~G_RFID() {
	//delete serial;
};
	
void G_RFID::setup() {
	newvalue = "";
	//setup
	pinMode(rpin, INPUT);
	pinMode(tpin, OUTPUT);
	serial = new SoftwareSerial(rpin, tpin);//Rx, Tx on board
	serial->listen();
	serial->begin(9600);
	serial->write(0x02);

	String sensorName = ((G_Name*)(gNames.find(this->index)))->name;
	G_Name* authzName = new G_Name(sensorName + ".authz");
	gNames.add(authzName);
	authz = new G_Variable(authzName->index);
	gSensors.variables.add(authz);
	authz->setup();
}

void G_RFID::print(String& result, String ident) {
	result+="rfid tx ";
	result+=String(tpin);
	result+=" rx ";
	result+=String(rpin);
}

String G_RFID::contentToString() {
	return "tx " + String(tpin) + " rx " + String(rpin);
}

void G_RFID::read() {
	change=false;
	String readbuffer;
	boolean rfidEvent=false;
    while (serial->available()) { 
      byte C = serial->read(); 
      readbuffer+=ByteToHex(C);
      rfidEvent = true; 
    } 
    if (rfidEvent) { 
		newvalue=readbuffer;
		change=true;
		authorize();
	}
	else {
//		TimedValue* v;
//		v = new TimedValue(0);
//		authz->set(v);
//		delete v;
	}
}

void G_RFID::authorize() {
	boolean authorized = false;
	TimedValue* v;
	unsigned int recordpos = EEPROM_RFID_START;
	unsigned int pos;
	for (; recordpos < EEPROM_RFID_RECORDSIZE*EEPROM_RFID_COUNT; recordpos += EEPROM_RFID_RECORDSIZE) {
		pos = recordpos;
		byte lead;
		eeprom.ReadByte(pos, lead);
		if (lead == 0x11) {
			String str = "";
			eeprom.ReadString(pos,str);
			if (str == newvalue) { //matching serial number
				str = "";
				byte block;
				eeprom.ReadByte(pos, block);
				eeprom.ReadString(pos, str);
				String readstring = F("AB0A03");
				readstring += ByteToHex(block);
				readstring += F("00");
				readstring += str;
				str = "";
				eeprom.ReadString(pos, str);
				String contentstring = F("AB1203");
				contentstring += str;
				//reading
				outputvalue.stringType = true;
				outputvalue.string = readstring;
				write();
				delay(50);
				String readbuffer = "";
				while (serial->available()) {
					byte C = serial->read();
					readbuffer += ByteToHex(C);
				}
				if (readbuffer==contentstring) authorized = true;
				serial->write(0x02);
				break;
			}
		}
	}
	if (authorized) {
		v = new TimedValue(1);
	}
	else {
		v = new TimedValue(2);
	}
	authz->set(v);
	delete v;
	v = new TimedValue(0);
	authz->set(v);
	delete v;

}

String G_RFID::setAuthorization(String serial4, byte sector, String key6, String content16) {
	unsigned int pos = eeprom.getFreeAuthzRecord();
	if (pos == -1) return F("No place for new card.");
	eeprom.WriteByte(pos, 0x11);
	eeprom.WriteString(pos, serial4);
	eeprom.WriteByte(pos, sector);
	eeprom.WriteString(pos, key6);
	eeprom.WriteString(pos, content16);
	return F("Ok.");
}

void G_RFID::set(TimedValue* tv) {
	outputvalue.set(*tv);
}

void G_RFID::write() {
	if(!outputvalue.stringType) return;
	unsigned int k = 0;
	byte b=0;
	while(k<outputvalue.string.length()) {
		if(k%2==0) b=CharToHex(outputvalue.string[k]);
		if(k%2==1) {
			b*=16;
			b+=CharToHex(outputvalue.string[k]);
			serial->write(b);
		}
		++k;
	}
	outputvalue.stringType = false;
}

String G_RFID::valueToString() {
	String str;
	str+=newvalue;
	return str;
}

void G_RFID::SDSave(File f,byte tabs) {
	G_IOBase::SDSave(f,tabs);
	f.print(",\n");
	for(int i=0;i<tabs;++i) f.print("\t");
	f.print("\"tx\" : ");
	f.print(tpin);
	f.print(",\n");
	for(int i=0;i<tabs;++i) f.print("\t");
	f.print("\"rx\" : ");
	f.print(rpin);
	f.print("\n");
}

boolean G_RFID::SDLoad(File& f, String& error) {
	String str = "";
	for (char c = f.read(); f.available() && c != '\"'; c = f.read());
	if (!f.available()) return true;
	for (char c = f.read(); f.available() && c != '\"'; c = f.read()) str += c;
	if (str != F("tx")) { error = F("Invalid rfid I/O description: >\""); error += str; error += F("\" insdead of \"tx\""); return false; }
	str = "";
	for (char c = f.read(); f.available() && c != ','; c = f.read()) if (IsNumber(c)) str += c;
	this->tpin = (byte)str.toInt();
	str = "";
	for (char c = f.read(); f.available() && c != '\"'; c = f.read());
	if (!f.available()) return true;
	for (char c = f.read(); f.available() && c != '\"'; c = f.read()) str += c;
	if (str != F("rx")) { error = F("Invalid rfid I/O description: >\""); error += str; error += F("\" insdead of \"rx\""); return false; }
	str = "";
	for (char c = f.read(); f.available() && c != '}'; c = f.read()) if (IsNumber(c)) str += c;
	this->rpin = (byte)str.toInt();
	gSensors.rfids.add(this);
	setup();
	return true;
}
// -----------DHTSensor-----------
G_DHTSensor::G_DHTSensor(byte idx) : G_IOBase(idx) {
	t = 0;
	rh = 0;
};
	
void G_DHTSensor::setup() {
	temp=0;
	humi=0;
	newtemp=0;
	newhumi=0;
	//setup
	pinMode(pin, INPUT);
	dht.setup(pin);

	String sensorName = ((G_Name*)(gNames.find(this->index)))->name;
	G_Name* rhName = new G_Name(sensorName + ".rh");
	gNames.add(rhName);
	rh = new G_Variable(rhName->index);
	gSensors.variables.add(rh);
	rh->scale = 1;
	rh->setup();
	G_Name* tName = new G_Name(sensorName + ".t");
	gNames.add(tName);
	t = new G_Variable(tName->index);
	gSensors.variables.add(t);
	t->scale = 1;
	t->setup();
}

void G_DHTSensor::print(String& result, String ident) {
	result+="DHT Sensor pin ";
	result+=String(pin);
}

String G_DHTSensor::contentToString() {
	return "pin " + String(pin);
}

void G_DHTSensor::read() {
	change=false;
	humi=newhumi;
	temp=newtemp;
	unsigned long now=millis();
	if (now<lastRead) lastRead = now;
	if (lastRead + DHTREADPERIOD <= now) {
		lastRead += DHTREADPERIOD;
		newhumi=10*dht.getHumidity();
		newtemp=10*dht.getTemperature();
		if (humi != newhumi || temp != newtemp) change = true;
		TimedValue* htv = new TimedValue(newhumi);
		TimedValue* ttv = new TimedValue(newtemp);
		rh->set(htv);
		t->set(ttv);
		delete htv;
		delete ttv;
	}
}

String G_DHTSensor::valueToString() {
	String str;
	str += F("RH="); str += doubleToString((double)humi / 10, 1);
	str += F("% T="); str += doubleToString((double)temp / 10, 1);
	str+=F("C");
	return str;
}

void G_DHTSensor::SDSave(File f,byte tabs) {
	G_IOBase::SDSave(f,tabs);
	f.print(",\n");
	for(int i=0;i<tabs;++i) f.print("\t");
	f.print("\"pin\" : ");
	f.print(pin);
	f.print("\n");
}

boolean G_DHTSensor::SDLoad(File& f, String& error) {
	String str = "";
	for (char c = f.read(); f.available() && c != '\"'; c = f.read());
	if (!f.available()) return true;
	for (char c = f.read(); f.available() && c != '\"'; c = f.read()) str += c;
	if (str != F("pin")) { error = F("Invalid I/O description: >\""); error += str; error += F("\" insdead of \"pin\""); return false; }
	str = "";
	for (char c = f.read(); f.available() && c != '}'; c = f.read()) if (IsNumber(c)) str += c;
	byte pinid = (byte)str.toInt();
	this->pin = pinid;
	gSensors.dhts.add(this);
	setup();
	return true;
}
// -----------G_Keypad4x4-----------
G_Keypad4x4::G_Keypad4x4(byte idx) : G_IOBase(idx) {
};
	
void G_Keypad4x4::setup() {
	key = 32;
	newkey = 32;
	//setup
	pinMode(rowpin[0], INPUT_PULLUP);
	pinMode(rowpin[1], INPUT_PULLUP);
	pinMode(rowpin[2], INPUT_PULLUP);
	pinMode(rowpin[3], INPUT_PULLUP);
	pinMode(colpin[0], OUTPUT);
	pinMode(colpin[1], OUTPUT);
	pinMode(colpin[2], OUTPUT);
	pinMode(colpin[3], OUTPUT);

	String sensorName = ((G_Name*)(gNames.find(this->index)))->name;
	G_Name* pName = new G_Name(sensorName + ".pin");
	gNames.add(pName);
	p = new G_Variable(pName->index);
	gSensors.variables.add(p);
	p->hexaPrint = 0;
	p->setup();
}

void G_Keypad4x4::print(String& result, String ident) {
	result+="Keypad pins row ";
	result+=String(rowpin[0]);
	result+=" ";
	result+=String(rowpin[1]);
	result+=" ";
	result+=String(rowpin[2]);
	result+=" ";
	result+=String(rowpin[3]);
	result+=" col ";
	result+=String(colpin[0]);
	result+=" ";
	result+=String(colpin[1]);
	result+=" ";
	result+=String(colpin[2]);
	result+=" ";
	result+=String(colpin[3]);
}

String G_Keypad4x4::contentToString() {
	return "row pins " + String(rowpin[0]) + " " + String(rowpin[1]) + " " + String(rowpin[2]) + " " + String(rowpin[3]) + " col pins " + String(colpin[0]) + " " + String(colpin[1]) + " " + String(colpin[2]) + " " + String(colpin[3]);
}

void G_Keypad4x4::read() {
	change=false;
	key=newkey;
	unsigned long now=millis();
	if (now<lastRead) lastRead = now;
	if (lastRead + KEYPAD4X4READPERIOD <= now) {
		byte num = 0;
		lastRead+=KEYPAD4X4READPERIOD;
		digitalWrite(colpin[0],HIGH);
		digitalWrite(colpin[1],HIGH);
		digitalWrite(colpin[2],HIGH);
		digitalWrite(colpin[3],HIGH);
		newkey=32; //nopush
		for (int c = 0, r = 0; c<4 || r<4;) {
			digitalWrite(colpin[c],LOW);
			boolean pushed=!digitalRead(rowpin[r]);
			digitalWrite(colpin[c],HIGH);
			if(pushed) {
				if(r==0 && c==0) {newkey='1'; num=1;} else
				if(r==0 && c==1) {newkey='2'; num=2;} else
				if(r==0 && c==2) {newkey='3'; num=3;} else
				if(r==0 && c==3) {newkey='a'; num=10;} else
				if(r==1 && c==0) {newkey='4'; num=4;} else
				if(r==1 && c==1) {newkey='5'; num=5;} else
				if(r==1 && c==2) {newkey='6'; num=6;} else
				if(r==1 && c==3) {newkey='b'; num=11;} else
				if(r==2 && c==0) {newkey='7'; num=7;} else
				if(r==2 && c==1) {newkey='8'; num=8;} else
				if(r==2 && c==2) {newkey='9'; num=9;}else
				if(r==2 && c==3) {newkey='c'; num=12;} else
				if(r==3 && c==0) {newkey='*'; num=14;} else
				if(r==3 && c==1) {newkey='0'; num=0;}else
				if(r==3 && c==2) {newkey='#'; num=15;} else
				if(r==3 && c==3) {newkey='d'; num=13;} else
				break;
			}
			++r;
			if(c<4 && r==4) { r=0;++c; }
		}
		if (key != newkey) {
			change = true;
			//handling pin4:
			if (newkey != 32) {
				if (num < 14) {
					if (p->hexaPrint == 4) p->hexaPrint = 0;
					if (p->hexaPrint == 0) { pin4 = 0; }
					else pin4 *= 16;
					pin4 += num;
					p->hexaPrint++;
				}
				else if (num == 15) {
					p->hexaPrint = 0;
					pin4 = 0;
				}
				TimedValue* ptv = new TimedValue(pin4);
				p->set(ptv);
				delete ptv;
			}
		}
	}
}

String G_Keypad4x4::valueToString() {
	String str;
	str+=newkey;
	return str;
}

void G_Keypad4x4::SDSave(File f,byte tabs) {
	G_IOBase::SDSave(f,tabs);
	f.print(",\n");
	for(int i=0;i<tabs;++i) f.print("\t");
	f.print("\"col pins\" : [");
	f.print(colpin[0]); f.print(",");
	f.print(colpin[1]); f.print(",");
	f.print(colpin[2]); f.print(",");
	f.print(colpin[3]);
	f.print("],\n");
	for(int i=0;i<tabs;++i) f.print("\t");
	f.print("\"row pins\" : [");
	f.print(rowpin[0]); f.print(",");
	f.print(rowpin[1]); f.print(",");
	f.print(rowpin[2]); f.print(",");
	f.print(rowpin[3]);
	f.print("]\n");
}

boolean G_Keypad4x4::SDLoad(File& f, String& error) {
	String str = "";
	for (char c = f.read(); f.available() && c != '\"'; c = f.read());
	if (!f.available()) return true;
	for (char c = f.read(); f.available() && c != '\"'; c = f.read()) str += c;
	if (str != F("rows")) { error = F("Invalid I/O description: >\""); error += str; error += F("\" insdead of \"rows\""); return false; }
	str = "";
	for (char c = f.read(); f.available() && c != '['; c = f.read());
	if (!f.available()) return true;
	for (char c = f.read(); f.available() && c != ']'; c = f.read()) str += c;
	String s;
	for (byte i = 0, j = 0; i < 4; ++i, ++j) {
		s = "";
		for (; str[j] != ',' && j < str.length(); ++j) if (IsNumber(str[j])) s += str[j];
		this->rowpin[i] = (byte)s.toInt();
	}
	str = "";
	for (char c = f.read(); f.available() && c != '\"'; c = f.read());
	if (!f.available()) return true;
	for (char c = f.read(); f.available() && c != '\"'; c = f.read()) str += c;
	if (str != F("columns")) { error = F("Invalid I/O description: >\""); error += str; error += F("\" insdead of \"columns\""); return false; }
	str = "";
	for (char c = f.read(); f.available() && c != '['; c = f.read());
	if (!f.available()) return true;
	for (char c = f.read(); f.available() && c != ']'; c = f.read()) str += c;
	for (byte i = 0, j = 0; i < 4; ++i,++j) {
		s = "";
		for (; str[j] != ',' && j < str.length(); ++j) if (IsNumber(str[j])) s += str[j];
		this->colpin[i] = (byte)s.toInt();
	}
	for (char c = f.read(); f.available() && c != '}'; c = f.read());
	gSensors.keypads4x4.add(this);
	setup();
	return true;
}
// -----------I2CLCD-----------
G_I2CLCD::G_I2CLCD(byte idx) : G_IOBase(idx) {
	i2cAddress = 0;
	rows = 2;
	columns = 16;
};
	
G_I2CLCD::~G_I2CLCD() {
	//delete lcd;
}	
	
void G_I2CLCD::setup() {
	lcd = new LiquidCrystal_I2C(i2cAddress, columns, rows);
	lcd->init();
	lcd->setCursor(0, 0);
	lcd->backlight();
}

void G_I2CLCD::print(String& result, String ident) {
	result+="I2C-LCD sdapin ";
	result+=String(sdapin);
	result+=" sclpin ";
	result+=String(sclpin);
	result += " address ";
	result += String(i2cAddress);
	result += " rows ";
	result += String(rows);
	result += " columns ";
	result += String(columns);
}

String G_I2CLCD::contentToString() {
	return "sdapin " + String(sdapin) + " sclpin " + String(sclpin) + " address " + String(i2cAddress) + " rows " + String(rows) + " columns " + String(columns);
}

void G_I2CLCD::set(TimedValue* tv) {
	outputvalue.set(*tv);
}

void G_I2CLCD::write() {
	if(!outputvalue.stringType) return;
	// [l..][c..][on][off]text
	// |value|
	boolean command=false;
	boolean val = false;
	String commandString="";
	String valueString = "";
	String putString="";
	byte l=0;
	byte c=0;
	for(byte i=0;i<outputvalue.string.length();++i) {
		if (!command && outputvalue.string[i] == '[') {
			command = true;
			continue;
		}
		if (!val && outputvalue.string[i] == '|') {
			val = true;
			continue;
		}
		if (command && outputvalue.string[i] == ']') {
			if(commandString=="on") lcd->backlight();
			else if(commandString=="off") lcd->noBacklight();
			else if(commandString.length()>0 && commandString[0]=='l') {
				l=commandString.substring(1,commandString.length()).toInt();
			} 
			else if(commandString.length()>0 && commandString[0]=='c') {
				c=commandString.substring(1,commandString.length()).toInt();
			}
			command=false;
			commandString="";
		}
		else if (val && outputvalue.string[i] == '|') {
			G_IOBase* n = gNames.find(valueString);
			if (n) {
				G_IOBase* act = gActuators.find(n->index);
				if (!act) act = gSensors.find(n->index);
				if (act) putString += act->valueToString();
			}
			val = false;
			valueString = "";
		}
		else if (command) {
			commandString += outputvalue.string[i];
		}
		else if (val) {
			valueString += outputvalue.string[i];
		}
		else if (!command && !val) putString += outputvalue.string[i];
	}
	lcd->setCursor(c,l);
	lcd->print(putString);
	outputvalue.stringType=false;
}

void G_I2CLCD::SDSave(File f,byte tabs) {
	G_IOBase::SDSave(f,tabs);
	f.print(",\n");
	for(int i=0;i<tabs;++i) f.print("\t");
	f.print("\"sdapin\" : ");
	f.print(sdapin);
	f.print(",\n");
	for(int i=0;i<tabs;++i) f.print("\t");
	f.print("\"sclpin\" : ");
	f.print(sclpin);
	f.print("\n");
}

boolean G_I2CLCD::SDLoad(File& f, String& error) {
	String str = "";
	for (char c = f.read(); f.available() && c != '\"'; c = f.read());
	if (!f.available()) return true;
	for (char c = f.read(); f.available() && c != '\"'; c = f.read()) str += c;
	if (str != F("sda")) { error = F("Invalid rfid I/O description: >\""); error += str; error += F("\" insdead of \"sda\""); return false; }
	str = "";
	for (char c = f.read(); f.available() && c != ','; c = f.read()) if (IsNumber(c)) str += c;
	this->sdapin = (byte)str.toInt();
	str = "";
	for (char c = f.read(); f.available() && c != '\"'; c = f.read());
	if (!f.available()) return true;
	for (char c = f.read(); f.available() && c != '\"'; c = f.read()) str += c;
	if (str != F("scl")) { error = F("Invalid rfid I/O description: >\""); error += str; error += F("\" insdead of \"scl\""); return false; }
	str = "";
	for (char c = f.read(); f.available() && c != ','; c = f.read()) if (IsNumber(c)) str += c;
	this->sclpin = (byte)str.toInt();
	str = "";
	for (char c = f.read(); f.available() && c != '\"'; c = f.read());
	if (!f.available()) return true;
	for (char c = f.read(); f.available() && c != '\"'; c = f.read()) str += c;
	if (str != F("address")) { error = F("Invalid rfid I/O description: >\""); error += str; error += F("\" insdead of \"address\""); return false; }
	str = "";
	for (char c = f.read(); f.available() && c != ','; c = f.read()) if (IsNumber(c)) str += c;
	this->i2cAddress = (byte)str.toInt();
	str = "";
	for (char c = f.read(); f.available() && c != '\"'; c = f.read());
	if (!f.available()) return true;
	for (char c = f.read(); f.available() && c != '\"'; c = f.read()) str += c;
	if (str != F("rows")) { error = F("Invalid rfid I/O description: >\""); error += str; error += F("\" insdead of \"rows\""); return false; }
	str = "";
	for (char c = f.read(); f.available() && c != ','; c = f.read()) if (IsNumber(c)) str += c;
	this->rows = (byte)str.toInt();
	str = "";
	for (char c = f.read(); f.available() && c != '\"'; c = f.read());
	if (!f.available()) return true;
	for (char c = f.read(); f.available() && c != '\"'; c = f.read()) str += c;
	if (str != F("columns")) { error = F("Invalid rfid I/O description: >\""); error += str; error += F("\" insdead of \"columns\""); return false; }
	str = "";
	for (char c = f.read(); f.available() && c != '}'; c = f.read()) if (IsNumber(c)) str += c;
	this->columns = (byte)str.toInt();
	gActuators.i2clcds.add(this);
	setup();
	return true;
}
// -----------Clock-----------
G_Clock::G_Clock(byte idx) : G_IOBase(idx) {
};

void G_Clock::setup(unsigned int y, byte m, byte d, byte h, byte mi, byte s) {
	lastTick = millis();
	second = s;
	minute = mi;
	hour = h;
	day = d;
	month = m;
	year = y;
}

void G_Clock::print(String& result, String ident) {
	result+=F("Clock type ");
	result+=F("full");
}

String G_Clock::contentToString() {
	return F("type full");
}

void G_Clock::read() {
	change = false; 
	unsigned long delta;
	unsigned long now = millis();
	if (now < lastTick) {
		delta = (unsigned long)(-1) - lastTick + now;
	} else {
		delta = now - lastTick;
	}
	byte deltaSec = delta / 1000;
	if (deltaSec > 0) {
		lastTick = now;
		change = true;
		second += deltaSec;
		if (second > 59) {
			minute++;
			second -= 60;
			if (minute > 59) {
				hour++;
				minute = 0;
				if (hour > 23) {
					day++;
					hour = 0;
					if ((month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) && day>31 || (month==4||month==6||month==9||month==11) && day>30 || (month==2)&& year%4==0 && day>29 || (month==2) && day>28) {
						month++;
						day = 1;
						if (month > 12) {
							year++;
							month = 1;
						}
					}
				}
			}
		}
	}
}

String G_Clock::valueToString() {
	String str;
/*	str += year;
	str += '-';
	if (month < 10) str += '0';
	str += month;
	str += '-';
	if (day < 10) str += '0';
	str += day;
	str += ' ';
	*/
	if (hour < 10) str += '0';
	str += hour;
	str += ':';
	if (minute < 10) str += '0';
	str += minute;
	str += ':';
	if (second < 10) str += '0';
	str += second;
	return str;
}

void G_Clock::SDSave(File f,byte tabs) {
	G_IOBase::SDSave(f,tabs);
	f.print(",\n");
}

boolean G_Clock::SDLoad(File& f, String& error) {
	gSensors.clocks.add(this);
	setup();
	return true;
}

// -----------Name-------------
G_Name::G_Name() : G_IOBase() {
}

G_Name::G_Name(String n) : G_IOBase() {
	name=n;
}

String G_Name::contentToString() {
	return name;
}

void G_Name::print(String& result,String ident) {
	result+=name;
}

boolean G_Name::SDLoad(File& f, String& error, byte& idx) {
	String str = "";
	for (char c = f.read(); f.available() && c != '\"'; c = f.read()) if (c == ']') { f.seek(f.position() - 1);  return true; }
	if (!f.available()) return false;
	for (char c = f.read(); f.available() && c != '\"'; c = f.read()) str += c;
	if (str != F("name")) { error = F("Invalid I/O description: >\""); error += str; error += F("\" insdead of \"name\""); return false; }
	str = "";
	for (char c = f.read(); f.available() && c != '\"'; c = f.read());
	if (!f.available()) return false;
	for (char c = f.read(); f.available() && c != '\"'; c = f.read()) str += c;

	G_IOBase* n = gNames.find(str);
	if (n != 0) {
		error += F("Name \"");
		error += str;
		error += F("\" is already exist");
		return false;
	}
	this->name = str;
	gNames.add(this);
	idx = this->index;
	return true;
}

// -----------Event-------------
G_Event::G_Event(byte idx) : G_IOBase(idx) {
}

String G_Event::contentToString() {
	return value.contentToString();
}

void G_Event::print(String& result,String ident) {
	value.print(result);
}

void G_Event::SDSave(File f,byte tabs) {
	G_IOBase::SDSave(f,tabs);
	f.print(",\n");
	value.SDSave(f,tabs);
	f.print("\n");
}

boolean G_Event::SDLoad(File& f, String& error) {
	String str = "";
	this->value.SDLoad(f, error);
	for (char c = f.read(); f.available() && c != '}'; c = f.read());
	gEvents.list.add(this);
	return true;
}

// -----------Condtition-------------
G_Condition::G_Condition() {
}

String G_Condition::contentToString() {
	String ret;
	print(ret);
	return ret;
}

void G_Condition::print(String& result) {
	result+=((G_Name*)gNames.find(sensorIndex))->name;
	switch(relation) {
		case 0: result+="="; break;
		case 1: result+="->"; break;
		case 2: result+="<-"; break;
		case 3: result+=">"; break;
		case 4: result+="<"; break;
		case 5: result+=" changed"; break;
	}
	if(relation!=5) result+=value;
}

boolean G_Condition::execute() {
	if(relation==5) return true;
	G_IOBase* sensor=gSensors.find(sensorIndex);
	if(!sensor) return false;
	unsigned int newvalue=sensor->getnew(); //with attrib number
	unsigned int oldvalue=sensor->get();
	switch(relation) {
		case 0: return (newvalue==value);
		case 1: return (oldvalue<value && newvalue>=value);
		case 2: return (oldvalue>value && newvalue<=value);
		case 3: return (newvalue>value);
		case 4: return (newvalue<value);
	}
	return false;
}

boolean G_Condition::hasChange() {
	G_IOBase* s=gSensors.find(sensorIndex);
	if(s && s->change) return true;
	return false;
}

void G_Condition::SDSave(File f,byte tabs) {
	for(int i=0;i<tabs;++i) f.print("\t"); f.print("\"name\" : \"");
	f.print(((G_Name*)gNames.find(sensorIndex))->name);
	f.print("\",\n");
	for(int i=0;i<tabs;++i) f.print("\t"); f.print("\"relation\" : ");
	switch(relation) {
		case 0: f.print("\"=\",\n"); break;
		case 1: f.print("\"->\",\n"); break;
		case 2: f.print("\"<-\",\n"); break;
		case 3: f.print("\">\",\n"); break;
		case 4: f.print("\"<\",\n"); break;
		case 5: f.print("\"changed\"\n"); break;
	}
	if(relation!=5) {
		for(int i=0;i<tabs;++i) f.print("\t"); f.print("\"value\" : ");
		f.print(value);
		f.print("\n");
	}
}

boolean G_Condition::SDLoad(File& f, String& error) {
	String str = "";
	for (char c = f.read(); f.available() && c != '\"'; c = f.read());
	if (!f.available()) return true;
	for (char c = f.read(); f.available() && c != '\"'; c = f.read()) str += c;
	if (str != F("name")) { error = F("Invalid condition description: >\""); error += str; error += F("\" insdead of \"name\""); return false; }
	str = "";
	for (char c = f.read(); f.available() && c != '\"'; c = f.read());
	if (!f.available()) return false;
	for (char c = f.read(); f.available() && c != '\"'; c = f.read()) str += c;
	G_IOBase* n;
	if (!(n = gNames.find(str)) || !gSensors.find(n->index)) { error += str; error = F(" is not existing sensor in a condition.\""); return false; }
	sensorIndex = n->index;
	str = "";
	for (char c = f.read(); f.available() && c != '\"'; c = f.read());
	if (!f.available()) return true;
	for (char c = f.read(); f.available() && c != '\"'; c = f.read()) str += c;
	if (str != F("relation")) { error = F("Invalid condition description: >\""); error += str; error += F("\" insdead of \"relation\""); return false; }
	str = "";
	for (char c = f.read(); f.available() && c != '\"'; c = f.read());
	if (!f.available()) return false;
	for (char c = f.read(); f.available() && c != '\"'; c = f.read()) str += c;
	if (str == "=") relation = 0;
	if (str == "->") relation = 1;
	if (str == "<-") relation = 2;
	if (str == ">") relation = 3;
	if (str == "<") relation = 4;
	if (str == "changed") relation = 5;
	if (relation != 5) {
		str = "";
		for (char c = f.read(); f.available() && c != '\"'; c = f.read());
		if (!f.available()) return true;
		for (char c = f.read(); f.available() && c != '\"'; c = f.read()) str += c;
		if (str != F("value")) { error = F("Invalid condition description: >\""); error += str; error += F("\" insdead of \"value\""); return false; }
		str = "";
		for (char c = f.read(); f.available() && c != '}'; c = f.read()) if (IsNumber(c)) str += c;
		this->value = str.toInt();
	}
	return true;
}

// -----------Trigger-------------
G_Trigger::G_Trigger() : G_IOBase() {
	conditions=0;
	nCondition=0;
	timed=false;
	triggeredEvent=0xff;
	triggeredValue=0;
}

G_Trigger::~G_Trigger() {
	if(conditions) delete [] conditions;
}

void G_Trigger::addCondition(G_Condition* cond) {
	G_Condition** newList = new G_Condition*[this->nCondition + 1];
	int i;
	for (i=0; i < this->nCondition; ++i) {
		newList[i] = this->conditions[i];
	}
	newList[i]=cond;
	this->nCondition++;
	if (conditions) delete [] this->conditions;
	this->conditions = newList;
}

boolean G_Trigger::anyChange() {
	for(int i=0;i<nCondition;++i) {
		if(conditions[i]->hasChange()) return true;
	}
	return false;
}

String G_Trigger::contentToString() {
	String ret;
	print(ret);
	return ret;
}

void G_Trigger::print(String& result,String ident) {
	result+=F(" when ");
	if (timed) {
		result += F("elapsed ");
		result += timer;
	}
	else {
		for (int i = 0; i<nCondition; ++i) {
			if (i>0) result += " & ";
			conditions[i]->print(result);
		}
	}
	result+=F(" apply ");
	G_Name* name=(G_Name*)gNames.find(triggeredActuator);
	if(name) { name->print(result); result+=" "; }
	if(triggeredEvent!=0xff) {
		G_Name* eventname=(G_Name*)gNames.find(triggeredEvent);
		if(eventname) eventname->print(result);
	} else if(triggeredValue) {
		triggeredValue->print(result);
	}
}

void G_Trigger::trigger() {
	if(timed) {
		unsigned long now=millis();
		if(lastTick+timer<=now) {
			lastTick=lastTick+timer;
			fire();
		}
		if(now<lastTick) lastTick=now;
	} else if(anyChange()) {
		boolean apply=true;
		for(int i=0;i<nCondition;++i) {
			apply=apply&&conditions[i]->execute();
			if(!apply) return;
		}
		fire();
	}
}

void G_Trigger::fire() {
	if(triggeredEvent!=0xff) {
		G_Event* event=((G_Event*)gEvents.list.find(triggeredEvent));
		if(event==0) return;
		gActuators.set(triggeredActuator,&event->value);
	} else if(triggeredValue) {
		gActuators.set(triggeredActuator,triggeredValue);
	}	
}

void G_Trigger::SDSave(File f,byte tabs) {
	for(int i=0;i<tabs;++i) f.print("\t"); f.print("\"conditions\" :\n");
		tabs++;
		for(int i=0;i<tabs;++i) f.print("\t"); f.print("[\n");
			tabs++;
			for(int x=0;x<nCondition;++x) {
				for(int i=0;i<tabs;++i) f.print("\t"); f.print("{\n");
					tabs++;
					conditions[x]->SDSave(f,tabs);
					tabs--;
				for(int i=0;i<tabs;++i) f.print("\t"); f.print("}");
				if(x<nCondition-1) f.print(",\n"); else f.print("\n");
			}
			tabs--;
		for(int i=0;i<tabs;++i) f.print("\t"); f.print("],\n");
		tabs--;
	for(int i=0;i<tabs;++i) f.print("\t"); f.print("\"triggered actuator\" : \"");
	f.print(((G_Name*)gNames.find(triggeredActuator))->name);
	f.print("\",\n");
	if(triggeredEvent<0xff) {
		for(int i=0;i<tabs;++i) f.print("\t"); f.print("\"triggered event\" : \"");
		f.print(((G_Name*)gNames.find(triggeredEvent))->name);
		f.print("\"\n");
	}
	if(triggeredValue) {
		for(int i=0;i<tabs;++i) f.print("\t"); f.print("\"triggered value\" :\n");
		tabs++;
			for(int i=0;i<tabs;++i) f.print("\t"); f.print("{\n");
			tabs++;
				triggeredValue->SDSave(f,tabs);
			tabs--;
			for(int i=0;i<tabs;++i) f.print("\t"); f.print("}\n");
		tabs--;
	}
}

boolean G_Trigger::SDLoad(File& f, String& error) {
	String str = "";
	for (char c = f.read(); f.available() && c != '\"'; c = f.read());
	if (!f.available()) return true;
	for (char c = f.read(); f.available() && c != '\"'; c = f.read()) str += c;
	if (str != F("time") && str != F("conditions")) { error = F("Invalid trigger I/O description: >\""); error += str; error += F("\" insdead of \"time\" or \"conditions\""); return false; }
	if (str == F("time")) {
		str = "";
		for (char c = f.read(); f.available() && c != ','; c = f.read()) if (IsNumber(c)) str += c;
		timer = (unsigned int)str.toInt();
		timed = true;
	}
	else {
		for (char c = f.read(); f.available() && c != '['; c = f.read());
		for(boolean end=false;;) {
			G_Condition* cond = new G_Condition();
			if (!cond->SDLoad(f, error)) { delete cond; return false; }
			this->addCondition(cond);
			char c;
			for (c = f.read(); f.available() && c != ']' && c != ','; c = f.read());
			if (c == ']') end = true;
			String s; s += c;
			if (end) break;
		}
	}

	str = "";
	for (char c = f.read(); f.available() && c != '\"'; c = f.read());
	if (!f.available()) return true;
	for (char c = f.read(); f.available() && c != '\"'; c = f.read()) str += c;
	if (str != F("triggered actuator")) { error = F("Invalid trigger I/O description: >\""); error += str; error += F("\" insdead of \"triggered actuator\""); return false; }
	str = "";
	for (char c = f.read(); f.available() && c != '\"'; c = f.read());
	if (!f.available()) return false;
	for (char c = f.read(); f.available() && c != '\"'; c = f.read()) str += c;
	G_IOBase* n;
	if (!(n = gNames.find(str)) || !gActuators.find(n->index)) { error += str; error = F(" is not existing actuator in a trigger.\""); return false; }
	triggeredActuator = n->index;

	str = "";
	for (char c = f.read(); f.available() && c != '\"'; c = f.read());
	if (!f.available()) return true;
	for (char c = f.read(); f.available() && c != '\"'; c = f.read()) str += c;
	if (str != F("triggered event") && str != F("triggered value")) {error = F("Invalid trigger I/O description: >\""); error += str; error += F("\" insdead of \"triggered value\" or \"triggered event\""); return false;}
	if (str == F("triggered value")) {
		triggeredEvent = 0xff;
		triggeredValue = new TimedValue();
		if (!triggeredValue->SDLoad(f, error)) return false;
		for (char c = f.read(); f.available() && c != '}'; c = f.read());
	}
	else {
		str = "";
		for (char c = f.read(); f.available() && c != '\"'; c = f.read());
		if (!f.available()) return false;
		for (char c = f.read(); f.available() && c != '\"'; c = f.read()) str += c;
		G_IOBase* n;
		if (!(n = gNames.find(str)) || !gEvents.list.find(n->index)) { error += str; error = F(" is not existing event in a trigger.\""); return false; }
		triggeredEvent = n->index;
	}

	for (char c = f.read(); f.available() && c != '}'; c = f.read());
	gTriggers.list.add(this);
	return true;
}

// -----------IOList-----------
G_IOList::G_IOList() {
	list=0;
	listSize=0;
}

G_IOList::~G_IOList() {
	if(list!=0) delete [] list;
	//TODO delete the elements!
}

byte G_IOList::add(G_IOBase* elem) {
	byte potentialplace;
	if(elem->index==0xff) {
		potentialplace=this->listSize;
		if(this->listSize==0) elem->index=0;
		else elem->index=this->list[potentialplace-1]->index+1;
	} else {
		potentialplace=findplace(elem->index);
		if(potentialplace<this->listSize && this->list[potentialplace]->index==elem->index) return 0xff; // already contains this index
	}
	G_IOBase** newList=new G_IOBase*[this->listSize+1];

	int i;
	for(i=0;i<potentialplace;++i) {
		newList[i]=this->list[i];
	}
	newList[i]=elem;
	int resultId=i;
	++i;
	for(;i<=this->listSize;++i) {
		newList[i]=this->list[i-1];
	}
	this->listSize++;
	if(this->list) delete [] this->list;
	this->list=newList;
	return this->list[resultId]->index;
}

void G_IOList::remove(byte id) {
	if(id<this->listSize) {
		delete this->list[id];
		this->listSize--;
		for(int i=id;i<this->listSize-1;++i) {
			list[i]=list[i+1];
		}
	}
}

G_IOBase* G_IOList::find(byte idx) {
	byte i=0;
	for(;i<this->listSize;++i) {
		if(this->list[i]->index==idx) return this->list[i];
	}
	return 0;
}

byte G_IOList::find(G_IOBase* n) {
	for(int i=0;i<this->listSize;++i) {
		if(this->list[i]->contentToString()==n->contentToString()) return i;
	}
	return 0xff;
}

G_IOBase* G_IOList::find(String ref) {
	for(int i=0;i<this->listSize;++i) {
		if(this->list[i]->contentToString()==ref) return this->list[i];
	}
	return 0;
}

void G_IOList::write() {
	for(int i=0;i<this->listSize;++i) {
		this->list[i]->write();
	}
}

byte G_IOList::findplace(byte idx) {
	byte i=0;
	for(;i<this->listSize;++i) {
		if(this->list[i]->index>=idx) break;
	}
	return i;
}

void G_IOList::print(String& result) {
	for(int i=0;i<this->listSize;++i) {
		this->list[i]->print(result);
		result+="\n";
	}
}

void G_IOList::printWname(String& result,boolean printname) {
	for(int i=0;i<this->listSize;++i) {
		result+="(";
		result+=this->list[i]->index;
		result+=") ";
		if(printname) {
			G_Name* name=(G_Name*)gNames.find(this->list[i]->index);
			if(name) { name->print(result); result+=" "; }
		}
		this->list[i]->print(result);
		result+="\n";
	}
}

void G_IOList::toStringWname(String& result,String ident,boolean printname) {
	for(int i=0;i<this->listSize;++i) {
		result+=ident;
		result+="(";
		result+=this->list[i]->index;
		result+=") ";
		if(printname) {
			G_Name* name=(G_Name*)gNames.find(this->list[i]->index);
			if(name) { name->print(result); result+=" "; }
		}
		result+=this->list[i]->contentToString();
		result+="\n";
	}
}

void G_IOList::SDSave(File f,byte tabs) {
	for(int x=0;x<listSize;++x) {
		for(int i=0;i<tabs;++i) f.print("\t"); f.print("{\n");
			tabs++;
			list[x]->SDSave(f,tabs);
			tabs--;
		for(int i=0;i<tabs;++i) f.print("\t"); 	if(x<listSize-1) { f.print("},\n"); } else { f.print("}\n"); }
	}
}

boolean G_IOList::SDLoad(File& f, String& error, String iotypename) {
	String str;
	byte nameidx = 0xff;
	boolean end = false;
	for (;;) {
		if (iotypename == F("variable") || iotypename == F("analog") || iotypename == F("digital") || iotypename == F("rfid") || iotypename == F("dht") || iotypename == F("4x4 keypad") || iotypename == F("clock") || iotypename == F("analogA") || iotypename == F("digitalA") || iotypename == F("i2c lcdA") || iotypename == F("events")) {
			G_Name* name = new G_Name();
			if (!name->SDLoad(f, error, nameidx)) {
				delete name;
				return false;
			}
			else if (nameidx == 0xff) {
				delete name;
				return true;
			}
		}
		if (iotypename == F("variable")) {
			G_Variable* sensor = new G_Variable(nameidx);
			if (!sensor->SDLoad(f, error)) {
				delete sensor;
				return false;
			}
		}
		else if (iotypename == F("analog")) {
			G_AnalogSensor* sensor = new G_AnalogSensor(nameidx);
			if (!sensor->SDLoad(f, error)) {
				delete sensor;
				return false;
			}
		}
		else if (iotypename == F("digital")) {
			G_DigitalSensor* sensor = new G_DigitalSensor(nameidx);
			if (!sensor->SDLoad(f, error)) {
				delete sensor;
				return false;
			}
		}
		else if (iotypename == F("rfid")) {
			G_RFID* sensor = new G_RFID(nameidx);
			if (!sensor->SDLoad(f, error)) {
				delete sensor;
				return false;
			}
		}
		else if (iotypename == F("dht")) {
			G_DHTSensor* sensor = new G_DHTSensor(nameidx);
			if (!sensor->SDLoad(f, error)) {
				delete sensor;
				return false;
			}
		}
		else if (iotypename == F("4x4 keypad")) {
			G_Keypad4x4* sensor = new G_Keypad4x4(nameidx);
			if (!sensor->SDLoad(f, error)) {
				delete sensor;
				return false;
			}
		}
		else if (iotypename == F("clock")) {
			G_Clock* sensor = new G_Clock(nameidx);
			if (!sensor->SDLoad(f, error)) {
				delete sensor;
				return false;
			}
		}
		else if (iotypename == F("analogA")) {
			G_AnalogActuator* sensor = new G_AnalogActuator(nameidx);
			if (!sensor->SDLoad(f, error)) {
				delete sensor;
				return false;
			}
		}
		else if (iotypename == F("digitalA")) {
			G_DigitalActuator* sensor = new G_DigitalActuator(nameidx);
			if (!sensor->SDLoad(f, error)) {
				delete sensor;
				return false;
			}
		}
		else if (iotypename == F("i2c lcdA")) {
			G_I2CLCD* sensor = new G_I2CLCD(nameidx);
			if (!sensor->SDLoad(f, error)) {
				delete sensor;
				return false;
			}
		}
		else if (iotypename == F("events")) {
			G_Event* sensor = new G_Event(nameidx);
			if (!sensor->SDLoad(f, error)) {
				delete sensor;
				return false;
			}
		}
		else if (iotypename == F("triggers")) {
			G_Trigger* sensor = new G_Trigger();
			if (!sensor->SDLoad(f, error)) {
				delete sensor;
				return false;
			}
		}
		else return false;
		String r = "";
		char c;
		for (c = f.read(); f.available() && (c != ']' && c != ','); c = f.read());
		if (c == ']') { f.seek(f.position() - 1);  end = true; } 
		if (end) break;
	}
	return true;
}

// -----------SensorList-----------
void G_SensorList::read() {
	for(int i=0;i<variables.listSize;++i) variables.list[i]->read();
	for(int i=0;i<analogsensors.listSize;++i) analogsensors.list[i]->read();
	for(int i=0;i<digitalsensors.listSize;++i) digitalsensors.list[i]->read();
	for(int i=0;i<rfids.listSize;++i) rfids.list[i]->read();
	for(int i=0;i<dhts.listSize;++i) dhts.list[i]->read();
	for(int i=0;i<keypads4x4.listSize;++i) keypads4x4.list[i]->read();
	for(int i=0;i<clocks.listSize;++i) clocks.list[i]->read();
}

//obsolete
boolean G_SensorList::hasChange() {
	for(int i=0;i<variables.listSize;++i) if(variables.list[i]->changed()) return true;
	for(int i=0;i<analogsensors.listSize;++i) if(analogsensors.list[i]->changed()) return true;
	for(int i=0;i<digitalsensors.listSize;++i) if(digitalsensors.list[i]->changed()) return true;
	for(int i=0;i<rfids.listSize;++i) if(rfids.list[i]->changed()) return true;
	for(int i=0;i<dhts.listSize;++i) if(dhts.list[i]->changed()) return true;
	for(int i=0;i<keypads4x4.listSize;++i) if(keypads4x4.list[i]->changed()) return true;
	for(int i=0;i<clocks.listSize;++i) if(clocks.list[i]->changed()) return true;
	return false;
}

G_IOBase* G_SensorList::find(byte index) {
	G_IOBase* g=0;
	if((g=variables.find(index))) return g;
	if((g=analogsensors.find(index))) return g;
	if((g=digitalsensors.find(index))) return g;
	if((g=rfids.find(index))) return g;
	if((g=dhts.find(index))) return g;
	if((g=keypads4x4.find(index))) return g;
	if((g=clocks.find(index))) return g;
	return 0;
}

void G_SensorList::SDSave(File f,byte tabs) {
	for(int i=0;i<tabs;++i) f.print("\t"); f.print("\"sensors\" :\n");
		tabs++;
		for(int i=0;i<tabs;++i) f.print("\t"); f.print("{\n");
			tabs++;
			for(int i=0;i<tabs;++i) f.print("\t"); f.print("\"variable\" :\n");
				tabs++;
				for(int i=0;i<tabs;++i) f.print("\t"); f.print("[\n");
					tabs++;
					variables.SDSave(f,tabs);
					tabs--;
				for(int i=0;i<tabs;++i) f.print("\t"); f.print("],\n");
				tabs--;
			for(int i=0;i<tabs;++i) f.print("\t"); f.print("\"analog\" :\n");
				tabs++;
				for(int i=0;i<tabs;++i) f.print("\t"); f.print("[\n");
					tabs++;
					analogsensors.SDSave(f,tabs);
					tabs--;
				for(int i=0;i<tabs;++i) f.print("\t"); f.print("],\n");
				tabs--;
			for(int i=0;i<tabs;++i) f.print("\t"); f.print("\"digital\" :\n");
				tabs++;
				for(int i=0;i<tabs;++i) f.print("\t"); f.print("[\n");
					tabs++;
					digitalsensors.SDSave(f,tabs);
					tabs--;
				for(int i=0;i<tabs;++i) f.print("\t"); f.print("],\n");
				tabs--;
			for(int i=0;i<tabs;++i) f.print("\t"); f.print("\"rfid\" :\n");
				tabs++;
				for(int i=0;i<tabs;++i) f.print("\t"); f.print("[\n");
					tabs++;
					rfids.SDSave(f,tabs);
					tabs--;
				for(int i=0;i<tabs;++i) f.print("\t"); f.print("],\n");
				tabs--;
			for(int i=0;i<tabs;++i) f.print("\t"); f.print("\"dht\" :\n");
				tabs++;
				for(int i=0;i<tabs;++i) f.print("\t"); f.print("[\n");
					tabs++;
					dhts.SDSave(f,tabs);
					tabs--;
				for(int i=0;i<tabs;++i) f.print("\t"); f.print("],\n");
				tabs--;
			for(int i=0;i<tabs;++i) f.print("\t"); f.print("\"4x4 keypad\" :\n");
				tabs++;
				for(int i=0;i<tabs;++i) f.print("\t"); f.print("[\n");
					tabs++;
					keypads4x4.SDSave(f,tabs);
					tabs--;
				for(int i=0;i<tabs;++i) f.print("\t"); f.print("],\n");
				tabs--;
			for(int i=0;i<tabs;++i) f.print("\t"); f.print("\"clock\" :\n");
				tabs++;
				for(int i=0;i<tabs;++i) f.print("\t"); f.print("[\n");
					tabs++;
					clocks.SDSave(f,tabs);
					tabs--;
				for(int i=0;i<tabs;++i) f.print("\t"); f.print("]\n");
				tabs--;
			tabs--;
		for(int i=0;i<tabs;++i) f.print("\t"); f.print("}");
}

boolean G_SensorList::SDLoad(File& f,String& error) {
	String iotypename="";
	boolean end = false;
	boolean ok = true;
	for(;f.available();) {
		iotypename = "";
		for (char c = f.read(); f.available() && c != '\"'; c = f.read());
		if(!f.available()) break;
		for(char c=f.read();f.available() && c!='\"';c=f.read()) iotypename+=c;
		for(char c=f.read();f.available() && c!='[';c=f.read());
		if(!f.available()) break;
		else if (iotypename == F("variable"))	{ if (!(variables.SDLoad(f, error, iotypename))) { ok = false; break; } }
		else if (iotypename == F("analog"))		{ if (!(analogsensors.SDLoad(f, error, iotypename))) { ok = false; break; } }
		else if (iotypename == F("digital"))	{ if (!(digitalsensors.SDLoad(f, error, iotypename))) { ok = false; break; } }
		else if (iotypename == F("rfid"))		{ if (!(rfids.SDLoad(f, error, iotypename))) { ok = false; break; } }
		else if (iotypename == F("dht"))		{ if (!(dhts.SDLoad(f, error, iotypename))) { ok = false; break; } }
		else if (iotypename == F("4x4 keypad"))	{ if (!(keypads4x4.SDLoad(f, error, iotypename))) { ok = false; break; } }
		else if (iotypename == F("clock"))		{ if (!(clocks.SDLoad(f, error, iotypename))) { ok = false; break; } }
		else { error += F("Invalid IO type: \""); error += iotypename; error += F("\"");  return false; }
		for (char c = f.read(); f.available() && c != ']'; c = f.read());
		for (char c = f.read(); f.available() && c != ','; c = f.read()) if (c == '}') {
			end = true; break;
		}
		if (end) break;
	}
	return ok;
}

// -----------ActuatorList-----------
void G_ActuatorList::set(byte index,TimedValue* val) {
	G_IOBase* found=find(index);
	if(!found) return;
	found->set(val);
}

G_IOBase* G_ActuatorList::find(byte index) {
	G_IOBase* found=analogactuators.find(index);
	if(!found) found=digitalactuators.find(index);
	if(!found) found=i2clcds.find(index);
	if(!found) found=gSensors.variables.find(index);
	if(!found) found=gSensors.rfids.find(index);
	return found;
}

void G_ActuatorList::write() {
	analogactuators.write();
	digitalactuators.write();
	i2clcds.write();
	gSensors.variables.write();
	gSensors.rfids.write();
}

void G_ActuatorList::SDSave(File f,byte tabs) {
	for(int i=0;i<tabs;++i) f.print("\t"); f.print("\"actuators\" :\n");
		tabs++;
		for(int i=0;i<tabs;++i) f.print("\t"); f.print("{\n");
			tabs++;
			for(int i=0;i<tabs;++i) f.print("\t"); f.print("\"analog\" :\n");
				tabs++;
				for(int i=0;i<tabs;++i) f.print("\t"); f.print("[\n");
					tabs++;
					analogactuators.SDSave(f,tabs);
					tabs--;
				for(int i=0;i<tabs;++i) f.print("\t"); f.print("],\n");
				tabs--;
			for(int i=0;i<tabs;++i) f.print("\t"); f.print("\"digital\" :\n");
				tabs++;
				for(int i=0;i<tabs;++i) f.print("\t"); f.print("[\n");
					tabs++;
					digitalactuators.SDSave(f,tabs);
					tabs--;
				for(int i=0;i<tabs;++i) f.print("\t"); f.print("],\n");
				tabs--;
			for(int i=0;i<tabs;++i) f.print("\t"); f.print("\"i2c lcd\" :\n");
				tabs++;
				for(int i=0;i<tabs;++i) f.print("\t"); f.print("[\n");
					tabs++;
					i2clcds.SDSave(f,tabs);
					tabs--;
				for(int i=0;i<tabs;++i) f.print("\t"); f.print("]\n");
				tabs--;
			tabs--;
		for(int i=0;i<tabs;++i) f.print("\t"); f.print("}");
}

boolean G_ActuatorList::SDLoad(File& f, String& error) {
	String iotypename = "";
	boolean end = false;
	boolean ok = true;
	for (; f.available();) {
		iotypename = "";
		for (char c = f.read(); f.available() && c != '\"'; c = f.read());
		if (!f.available()) break;
		for (char c = f.read(); f.available() && c != '\"'; c = f.read()) iotypename += c;
		for (char c = f.read(); f.available() && c != '['; c = f.read());
		if (!f.available()) break;
		else if (iotypename == F("analog"))		{ if (!(analogactuators.SDLoad(f, error, iotypename+"A"))) { ok = false; break; } }
		else if (iotypename == F("digital"))	{ if (!(digitalactuators.SDLoad(f, error, iotypename+"A"))) { ok = false; break; } }
		else if (iotypename == F("i2c lcd"))	{ if (!(i2clcds.SDLoad(f, error, iotypename + "A"))) { ok = false; break; } }
		else { error += F("Invalid IO type: \""); error += iotypename; error += F("\"");  return false; }
		for (char c = f.read(); f.available() && c != ']'; c = f.read());
		for (char c = f.read(); f.available() && c != ','; c = f.read()) if (c == '}') {
			end = true; break;
		}
		if (end) break;
	}
	return ok;
}

// -----------EventList-----------
void G_EventList::SDSave(File f,byte tabs) {
	for(int i=0;i<tabs;++i) f.print("\t"); f.print("\"events\" :\n");
		tabs++;
		for(int i=0;i<tabs;++i) f.print("\t"); f.print("[\n");
			tabs++;
			list.SDSave(f,tabs);
			tabs--;
		for(int i=0;i<tabs;++i) f.print("\t"); f.print("]");
		tabs--;
}

boolean G_EventList::SDLoad(File& f, String& error) {
	String iotypename = "";
	if (!(list.SDLoad(f, error, F("events")))) { return false; } 
	return true;
}

// -----------TriggerList-----------
void G_TriggerList::trigger() {
	for(int i=0;i<list.listSize;++i) list.list[i]->trigger();
}

void G_TriggerList::SDSave(File f,byte tabs) {
	for(int i=0;i<tabs;++i) f.print("\t"); f.print("\"triggers\" :\n");
		tabs++;
		for(int i=0;i<tabs;++i) f.print("\t"); f.print("[\n");
			tabs++;
			list.SDSave(f,tabs);
			tabs--;
		for(int i=0;i<tabs;++i) f.print("\t"); f.print("]");
		tabs--;
}

boolean G_TriggerList::SDLoad(File& f, String& error) {
	String iotypename = "";
	if (!(list.SDLoad(f, error, F("triggers")))) { return false; }
	return true;
}

// -----------Global-----------
void SerialPrintAllIO() {
	String ident=F("   ");
	String str;
	Serial.println(F("\nIO LIST:"));
	Serial.println(F("Analog senrors:"));
	gSensors.analogsensors.toStringWname(str,ident);
	if(str!="") Serial.print(str);
	str="";
	Serial.println(F("Digital senrors:"));
	gSensors.digitalsensors.toStringWname(str,ident);
	if(str!="") Serial.print(str);
	str="";
	Serial.println(F("Analog actuators:"));
	gActuators.analogactuators.toStringWname(str,ident);
	if(str!="") Serial.print(str);
	str="";
	Serial.println(F("Digital actuators:"));
	gActuators.digitalactuators.toStringWname(str,ident);
	if(str!="") Serial.print(str);
	str="";
	Serial.println(F("DHT senrors:"));
	gSensors.dhts.toStringWname(str,ident);
	if(str!="") Serial.print(str);
	str="";
	Serial.println(F("4x4 Keypads:"));
	gSensors.keypads4x4.toStringWname(str,ident);
	if(str!="") Serial.print(str);
	str="";
	Serial.println(F("I2C LCDs:"));
	gActuators.i2clcds.toStringWname(str,ident);
	if(str!="") Serial.print(str);
	str="";
	Serial.println(F("RFIDs:"));
	gSensors.rfids.toStringWname(str,ident);
	if(str!="") Serial.print(str);
	str="";
	Serial.println(F("Clocks:"));
	gSensors.clocks.toStringWname(str,ident);
	if(str!="") Serial.print(str);
	str="";
	Serial.println(F("Variables:"));
	gSensors.variables.toStringWname(str,ident);
	if(str!="") Serial.print(str);
	str="";
	Serial.println(F("\nEVENT LIST:"));
	gEvents.list.toStringWname(str,ident);
	if(str!="") Serial.print(str);
	str="";
	Serial.println(F("\nTRIGGER LIST:"));
	gTriggers.list.toStringWname(str,ident,false);
	if(str!="") Serial.print(str);
}
