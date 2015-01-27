#include "GeneralIO.h"
#include <ExtendedEEPROM.h>


G_IOList gNames;
G_SensorList gSensors;
G_ActuatorList gActuators;
G_TriggerList gTriggers;
G_EventList gEvents;

// -----------TimedValue-----------
TimedValue::TimedValue() {
	lastRead=0;
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
 }
  
void TimedValue::EEPROMRead(unsigned int& pos) {
    byte r;
    EEPROMReadByte(pos,r);  
    stringType=r;
    if(stringType) {
      numOfValue=1;
      nextvalue=0;
      EEPROMReadString(pos,string);
    } else {
      EEPROMReadByte(pos,numOfValue);
      nextvalue=0;
      for(int j=0;j<numOfValue;++j) {
        EEPROMReadInt(pos,value[j]);
      }
      for(int j=0;j<numOfValue-1;++j) {
        EEPROMReadInt(pos,time[j]);
      }
    }
}

void TimedValue::EEPROMWrite(unsigned int& pos) {
	EEPROMWriteByte(pos,stringType);  
	if(stringType) {
	  EEPROMWriteString(pos,string);
	} else {
	  EEPROMWriteByte(pos,numOfValue);
	  for(int j=0;j<numOfValue;++j) {
		EEPROMWriteInt(pos,value[j]);
	  }
	  for(int j=0;j<numOfValue-1;++j) {
		EEPROMWriteInt(pos,time[j]);
	  }
	}
}

void TimedValue::print(String& result) {
	if(stringType) result+=("\""+string+"\"");
	else for(int i=0;i<numOfValue;++i) {
		if(i>0) {
			result+="<";
			result+=time[i];
			result+=">";
		}
		result+=value[i];
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
	if(lastRead+time[nextvalue]<=now) {
		lastRead+=time[nextvalue];
		nextvalue++;
	}
	if(now<lastRead) lastRead=now;
}

void TimedValue::purge() {
	if(nextvalue<numOfValue) return;
	delete value;
	delete time;
	value=0;
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

// -----------IOBase-----------
G_IOBase::G_IOBase() {
	index=0xff;
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
G_Variable::G_Variable() {
}

G_Variable::~G_Variable() {
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
	if(value!=newvalue) change=true;
	
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
	String str;
	str+=value;
	return str;
}

void G_Variable::SDSave(File f,byte tabs) {
	G_IOBase::SDSave(f,tabs);
	f.print(",\n");
}

// -----------AnalogSensor-----------
G_AnalogSensor::G_AnalogSensor(byte x) {
	pin=x;
	lastRead=0;
	//setup
	pinMode(pin, INPUT);
};

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
	if(lastRead+ANALOGREADPERIOD<=now) {
		lastRead+=ANALOGREADPERIOD;
		newvalue=analogRead(pin);
		if(value!=newvalue) change=true;
	}
	if(now<lastRead) lastRead=now;
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

// -----------DigitalSensor-----------
G_DigitalSensor::G_DigitalSensor(byte x) {
	pin=x;
	//setup
	pinMode(pin, INPUT);
};

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

// -----------AnalogActuator-----------
G_AnalogActuator::G_AnalogActuator(byte x) {
	pin=x;
	value=0;
	//setup
	pinMode(pin, OUTPUT);
};
	
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

// -----------DigitalActuator-----------
G_DigitalActuator::G_DigitalActuator(byte x) {
	pin=x;
	//setup
	pinMode(pin, OUTPUT);
};
	
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

// -----------RFID-----------
G_RFID::G_RFID(byte x,byte y) {
	tpin=x;
	rpin=y;
	//setup
	pinMode(rpin, INPUT);
	pinMode(tpin, OUTPUT);
	serial=new SoftwareSerial(rpin, tpin);//Rx, Tx on board
    serial->begin(9600);
    serial->write(0x01);
};
	
G_RFID::~G_RFID() {
	//delete serial;
};
	
void G_RFID::print(String& result,String ident) {
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
    }
}

void G_RFID::set(TimedValue* tv) {
	outputvalue.set(*tv);
}

void G_RFID::write() {
	if(!outputvalue.stringType) return;
    unsigned int k=0;
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
	outputvalue.stringType=false;
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

// -----------DHTSensor-----------
G_DHTSensor::G_DHTSensor(byte x) {
	pin=x;
	//setup
	pinMode(pin, INPUT);
	dht.setup(pin);
};
	
void G_DHTSensor::print(String& result,String ident) {
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
	if(lastRead+DHTREADPERIOD<=now) {
		lastRead+=DHTREADPERIOD;
		newhumi=dht.getHumidity();
		newtemp=dht.getTemperature();
		if(humi!=newhumi || temp!=newtemp) change=true;
	}
	if(now<lastRead) lastRead=now;
}

String G_DHTSensor::valueToString() {
	String str;
    str+=F("RH="); str+=doubleToString(dht.getHumidity());
    str+=F("% T="); str+=doubleToString(dht.getTemperature());
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

// -----------G_Keypad4x4-----------
G_Keypad4x4::G_Keypad4x4(byte row[4],byte col[4]) {
	rowpin[0]=row[0];
	rowpin[1]=row[1];
	rowpin[2]=row[2];
	rowpin[3]=row[3];
	colpin[0]=col[0];
	colpin[1]=col[1];
	colpin[2]=col[2];
	colpin[3]=col[3];
	
	//setup
	pinMode(rowpin[0], INPUT_PULLUP);
	pinMode(rowpin[1], INPUT_PULLUP);
	pinMode(rowpin[2], INPUT_PULLUP);
	pinMode(rowpin[3], INPUT_PULLUP);
	pinMode(colpin[0], OUTPUT);
	pinMode(colpin[1], OUTPUT);
	pinMode(colpin[2], OUTPUT);
	pinMode(colpin[3], OUTPUT);
};
	
void G_Keypad4x4::print(String& result,String ident) {
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
	return "row pins " + String(rowpin[0]) + String(rowpin[1]) + String(rowpin[2]) + String(rowpin[3]) + "col pins " + String(colpin[0]) + String(colpin[1]) + String(colpin[2]) + String(colpin[3]);
}

void G_Keypad4x4::read() {
	change=false;
	key=newkey;
	unsigned long now=millis();
	if(lastRead+KEYPAD4X4READPERIOD<=now) {
		lastRead+=KEYPAD4X4READPERIOD;
		digitalWrite(colpin[0],HIGH);
		digitalWrite(colpin[1],HIGH);
		digitalWrite(colpin[2],HIGH);
		digitalWrite(colpin[3],HIGH);
		newkey=0; //nopush
		for(int c=0,r=0;c<4||r<4;) {
			digitalWrite(colpin[c],LOW);
			boolean pushed=!digitalRead(rowpin[r]);
			digitalWrite(colpin[c],HIGH);
			if(pushed) {
				if(r==0 && c==0) newkey='1'; else
				if(r==0 && c==1) newkey='2'; else
				if(r==0 && c==2) newkey='3'; else
				if(r==0 && c==3) newkey='a'; else
				if(r==1 && c==0) newkey='4'; else
				if(r==1 && c==1) newkey='5'; else
				if(r==1 && c==2) newkey='6'; else
				if(r==1 && c==3) newkey='b'; else
				if(r==2 && c==0) newkey='7'; else
				if(r==2 && c==1) newkey='8'; else
				if(r==2 && c==2) newkey='9'; else
				if(r==2 && c==3) newkey='c'; else
				if(r==3 && c==0) newkey='*'; else
				if(r==3 && c==1) newkey='0'; else
				if(r==3 && c==2) newkey='#'; else
				if(r==3 && c==3) newkey='d'; else
				break;
			}
			++r;
			if(r==4) { r=0;++c; }
		}
		if(key!=newkey) change=true;
	}
	if(now<lastRead) lastRead=now;
}

String G_Keypad4x4::valueToString() {
	String str;
	str+=key;
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

// -----------I2CLCD-----------
G_I2CLCD::G_I2CLCD(byte x,byte y) {
	dpin=x; //todo ?
	spin=y;
	lcd=new LiquidCrystal_I2C(0x27,20,4); //!!!
};
	
G_I2CLCD::~G_I2CLCD() {
	//delete lcd;
}	
	
void G_I2CLCD::print(String& result,String ident) {
	result+="I2C-LCD dpin ";
	result+=String(dpin);
	result+=" spin ";
	result+=String(spin);
}

String G_I2CLCD::contentToString() {
	return "dpin " + String(dpin) + " spin " + String(spin);
}

void G_I2CLCD::set(TimedValue* tv) {
	outputvalue.set(*tv);
}

void G_I2CLCD::write() {
	if(!outputvalue.stringType) return;
	// [l..,c..][on][off]text
	boolean command=false;
	String commandString="";
	String putString="";
	byte l=0;
	byte c=0;
	for(byte i=0;i<outputvalue.string.length();++i) {
		if(outputvalue.string[i]=='[') command=true;
		if(command && outputvalue.string[i]==']') {
			if(commandString=="on") lcd->backlight();
			else if(commandString=="off") lcd->noBacklight();
			else if(commandString.length()>0 && commandString[0]=='l') {
				l=commandString.substring(1,commandString.length()-1).toInt();
			} 
			else if(commandString.length()>0 && commandString[0]=='c') {
				c=commandString.substring(1,commandString.length()-1).toInt();
			}
			command=false;
			commandString="";
		} else if(command) commandString+=outputvalue.string[i];
		else if(!command) putString+=outputvalue.string[i];
	}
	lcd->setCursor(c,l);
	lcd->print(putString);
	outputvalue.stringType=false;
}

void G_I2CLCD::SDSave(File f,byte tabs) {
	G_IOBase::SDSave(f,tabs);
	f.print(",\n");
	for(int i=0;i<tabs;++i) f.print("\t");
	f.print("\"dpin\" : ");
	f.print(dpin);
	f.print(",\n");
	for(int i=0;i<tabs;++i) f.print("\t");
	f.print("\"spin\" : ");
	f.print(spin);
	f.print("\n");
}

// -----------Clock-----------
G_Clock::G_Clock(byte x) {
	pin=x;
	//setup
	// todo
};
	
void G_Clock::print(String& result,String ident) {
	result+="Clock pin ";
	result+=String(pin);
}

String G_Clock::contentToString() {
	return "pin " + String(pin);
}

void G_Clock::read() {
	change=false;
	//value=newvalue;
//todo
//	unsigned long now=millis();
//	if(lastRead+KEYPAD4X4READPERIOD<=now) {
//		lastRead+=KEYPAD4X4READPERIOD;
//		if() change=true;
//	}
//	if(now<lastRead) lastRead=now;
}

String G_Clock::valueToString() {
	String str;
	//todo
	return str;
}

void G_Clock::SDSave(File f,byte tabs) {
	G_IOBase::SDSave(f,tabs);
	f.print(",\n");
	for(int i=0;i<tabs;++i) f.print("\t");
	f.print("\"pin\" : ");
	f.print(pin);
	f.print("\n");
}

// -----------Name-------------
G_Name::G_Name(String n) {
	name=n;
}

String G_Name::contentToString() {
	return name;
}

void G_Name::print(String& result,String ident) {
	result+=name;
}

// -----------Event-------------
G_Event::G_Event(TimedValue& in) {
	value.set(in);
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

// -----------Trigger-------------
G_Trigger::G_Trigger() {
	conditions=0;
	nCondition=0;
	timed=false;
	triggeredEvent=0xff;
	triggeredValue=0;
}

G_Trigger::~G_Trigger() {
	if(conditions) delete [] conditions;
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
	result+=" when ";
	for(int i=0;i<nCondition;++i) {
		if(i>0) result+=" & ";
		conditions[i]->print(result);
	}
	result+=" apply ";
	G_Name* name=(G_Name*)gNames.find(triggeredActuator);
	if(name) { name->print(result); result+=" "; }
	if(triggeredEvent) {
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
	delete this->list;
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
	for(;f.available();) {
		for(char c=f.read();f.available() && c!='\"';c=f.read());
		if(!f.available()) break;
		for(char c=f.read();f.available() && c!='\"';c=f.read()) iotypename+=c;
		for(char c=f.read();f.available() && c!='[';c=f.read());
			Serial.println(iotypename);
		if(!f.available()) break;
		if(iotypename==F("variable")) {
			//if(!variables.SDLoad(f,error,iotypename)) break;
		} else if(iotypename==F("analog")) {
		} else if(iotypename==F("rfid")) {
		} else if(iotypename==F("dht")) {
		} else if(iotypename==F("4x4 keypad")) {
		} else if(iotypename==F("clock")) {
		} else ;//return false;
		iotypename="";
	}
	return true;
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
