#ifndef general_io
#define general_io

#include "Arduino.h"
#include <SoftwareSerial.h> 
#include "DHT.h"
#include <GeneralBasics.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SD.h>
#include <ExtendedEEPROM.h>


#define ANALOGREADPERIOD 100
#define DHTREADPERIOD 2000
#define KEYPAD4X4READPERIOD 50


// *** TimedValue *********************
class TimedValue {
	byte nextvalue;
	unsigned long lastRead;
public:
	int* time;
	int* value;
	boolean stringType;
	String string; //only one string
	byte numOfValue;
	TimedValue();
	~TimedValue();
	TimedValue(int);
	void set(TimedValue& in);
	void append(TimedValue& in);
	void print(String&);
	String contentToString();
	void read(int&); //return 0 if cleared the list
	void purge();
	void SDSave(File,byte);
	boolean SDLoad(File& f, String&);
};

// *** IOs *********************
// abstract
class G_IOBase {
public:
	bool change;
	byte index;
	G_IOBase();
	G_IOBase(byte idx);
	virtual ~G_IOBase();
	virtual void print(String&, String ident = "") = 0;
	virtual String contentToString()=0;
	boolean changed();
	virtual void read();
	virtual void trigger();
	virtual void set(TimedValue*);
	virtual void write();
	virtual int get();
	virtual int getnew();
	virtual String valueToString();
	virtual void SDSave(File,byte)=0;
};
// I & O
class G_Variable : public G_IOBase {
	int value;
	int newvalue;
	TimedValue comingValue;
public:
	byte scale;
	byte hexaPrint;

	G_Variable();
	G_Variable(byte);
	~G_Variable();
	void setup();
	virtual void print(String&, String ident = "");
	virtual String contentToString();
	virtual void read();
	virtual void set(TimedValue*);
	virtual int get() {return value;};
	virtual int getnew() {return newvalue;};
	virtual String valueToString();
	virtual void SDSave(File,byte);
	boolean SDLoad(File& f, String&);
};
// I
class G_AnalogSensor : public G_IOBase {
	unsigned long lastRead;
	byte pin;
	
	unsigned short value;
	unsigned short newvalue;
public:
	G_AnalogSensor(byte);
	void setup();
	virtual void print(String&,String ident="");
	virtual String contentToString();
	virtual void read();
	virtual int get() {return value;};
	virtual int getnew() {return newvalue;};
	virtual String valueToString();
	virtual void SDSave(File,byte);
	boolean SDLoad(File& f, String&);
};
// I
class G_DigitalSensor : public G_IOBase {
	byte pin;

	boolean value;
	boolean newvalue;
public:
	G_DigitalSensor(byte);
	void setup();
	virtual void print(String&, String ident = "");
	virtual String contentToString();
	virtual void read();
	virtual int get() {return value;};
	virtual int getnew() {return newvalue;};
	virtual String valueToString();
	virtual void SDSave(File,byte);
	boolean SDLoad(File& f, String&);
};
//     O
class G_AnalogActuator : public G_IOBase {
	byte pin;
	unsigned short value;
	TimedValue outputvalue;
public:
	G_AnalogActuator(byte);
	void setup();
	virtual void print(String&, String ident = "");
	virtual String contentToString();
	virtual void set(TimedValue*);
	virtual void write();
	virtual int get() { return value; };
	virtual int getnew() { return value; };
	virtual String valueToString();
	virtual void SDSave(File,byte);
	boolean SDLoad(File& f, String&);
};

//     O
class G_DigitalActuator : public G_IOBase {
	byte pin;
	boolean value;
	TimedValue outputvalue;
public:
	G_DigitalActuator(byte);
	void setup();
	virtual void print(String&, String ident = "");
	virtual String contentToString();
	virtual void set(TimedValue*);
	virtual void write();
	virtual int get() { return value; };
	virtual int getnew() { return value; };
	virtual String valueToString();
	virtual void SDSave(File,byte);
	boolean SDLoad(File& f, String&);
};
// I & O
class G_RFID : public G_IOBase {
	SoftwareSerial* serial;
	byte tpin;
	byte rpin;

	String newvalue;
	TimedValue outputvalue;

	G_Variable* authz;
	void authorize();
public:
	G_RFID(byte);
	~G_RFID();
	void setup();
	virtual void print(String&, String ident = "");
	virtual String contentToString();
	virtual void read();
	virtual void set(TimedValue*);
	virtual void write();
	virtual String valueToString();
	virtual void SDSave(File,byte);
	boolean SDLoad(File& f, String&);
	
	String setAuthorization(String serial, byte sector, String key, String content);
};
// I
class G_DHTSensor : public G_IOBase {
	DHT dht;
	unsigned long lastRead;
	byte pin;
	
	unsigned short temp;
	unsigned short humi;
	unsigned short newtemp;
	unsigned short newhumi;

	G_Variable* rh;
	G_Variable* t;
public:
	G_DHTSensor(byte);
	void setup();
	virtual void print(String&, String ident = "");
	virtual String contentToString();
	virtual void read();
	virtual String valueToString();
	virtual void SDSave(File,byte);
	boolean SDLoad(File& f, String&);
};
// I
class G_Keypad4x4 : public G_IOBase {
	unsigned long lastRead;
	byte rowpin[4];
	byte colpin[4];
	
	char key;
	char newkey;

	int pin4;
	G_Variable* p;
public:
	G_Keypad4x4(byte);
	void setup();
	virtual void print(String&, String ident = "");
	virtual String contentToString();
	virtual void read();
	virtual int get() {return key;};
	virtual int getnew() {return newkey;};
	virtual String valueToString();
	virtual void SDSave(File,byte);
	boolean SDLoad(File& f, String&);
};
//     O
class G_I2CLCD : public G_IOBase {
	byte sdapin;
	byte sclpin;
	byte i2cAddress;
	byte rows;
	byte columns;
	
	TimedValue outputvalue;
public:
	LiquidCrystal_I2C* lcd;
	G_I2CLCD(byte);
	~G_I2CLCD();
	void setup();
	virtual void print(String&, String ident = "");
	virtual String contentToString();
	virtual void set(TimedValue*);
	virtual void write();
	virtual void SDSave(File,byte);
	boolean SDLoad(File& f, String&);
};
// I
class G_Clock : public G_IOBase {
	unsigned int year;
	byte month;
	byte day;
	byte hour;
	byte minute;
	byte second;

	unsigned long lastTick;
public:
	G_Clock(byte);
	void setup(unsigned int y=1,byte m=1,byte d=1,byte h=0,byte mi=0,byte s=0);
	virtual void print(String&, String ident = "");
	virtual String contentToString();
	virtual void read();
//	virtual int get() {return value;};
//	virtual int getnew() {return newvalue;};
	virtual String valueToString();
	virtual void SDSave(File,byte);
	boolean SDLoad(File& f, String&);
};

// *** NAMES *********************

class G_Name : public G_IOBase {
public:
	String name;
	G_Name();
	G_Name(String);
	virtual void print(String&,String ident="");
	virtual String contentToString();
	virtual void SDSave(File,byte) {};
	boolean SDLoad(File& f, String&, byte&);
};

// *** Event *********************
class G_Event : public G_IOBase {
public:
	TimedValue value;
	G_Event(byte);
	virtual void print(String&,String ident="");
	virtual String contentToString();
	void SDSave(File,byte);
	boolean SDLoad(File& f, String&);
};

// *** Condition *********************
class G_Condition {
public:
	byte sensorIndex;
	byte relation;  // =  : 0
				   // -> : 1
				   // <- : 2
				   // >  : 3
				   // <  : 4
	unsigned short value;	
public:
	G_Condition();
	boolean execute();
	boolean hasChange();
	void print(String&);
	String contentToString();
	void SDSave(File,byte);
	boolean SDLoad(File&, String&);
};

// *** Trigger *********************
class G_Trigger : public G_IOBase {
	unsigned long lastTick;
	void fire();
	void addCondition(G_Condition*);
public:
	byte triggeredActuator;
	byte triggeredEvent; //0xff if not event
	TimedValue* triggeredValue;
	boolean timed;
	unsigned int timer;
	G_Condition** conditions;
	byte nCondition;

	G_Trigger();
	~G_Trigger();
	boolean anyChange();
	virtual void print(String&,String ident="");
	virtual String contentToString();
	virtual void trigger();
	void SDSave(File,byte);
	boolean SDLoad(File&, String&);
};

// *** I/O LISTS *********************

class G_IOList {
	byte findplace(byte index);
public:
	G_IOBase** list;
	byte listSize;
	G_IOList();
	~G_IOList();
	byte add(G_IOBase*);
	void remove(byte index);
	G_IOBase* find(byte index);
	byte find(G_IOBase*);
	G_IOBase* find(String);
	void write();
	void print(String&);
	void printWname(String&,boolean printname=true);
	void toStringWname(String&,String s="",boolean printname=true);
	void SDSave(File,byte);
	boolean SDLoad(File& f, String& error, String iotypename);
};

class G_SensorList {
public:
	G_IOList variables;
	G_IOList analogsensors;
	G_IOList digitalsensors;
	G_IOList rfids;
	G_IOList dhts;
	G_IOList keypads4x4;
	G_IOList clocks;
	
	void read();
	boolean hasChange();
	G_IOBase* find(byte index);
	void SDSave(File,byte);
	boolean SDLoad(File&,String&);
};

class G_ActuatorList {
//	G_IOBase* find(byte index);
public:
	G_IOList analogactuators;
	G_IOList digitalactuators;
	G_IOList i2clcds;
	
	void set(byte id,TimedValue*);
	void write();
	boolean hasChange();
	G_IOBase* find(byte index);
	void SDSave(File,byte);
	boolean SDLoad(File&, String&);
};

// *** Event List *********************
class G_EventList {
public:
	G_IOList list;
	
	void SDSave(File,byte);
	boolean SDLoad(File&, String&);
};

// *** Trigger List *********************
class G_TriggerList {
public:
	G_IOList list;
	
	void trigger();
	void SDSave(File,byte);
	boolean SDLoad(File&, String&);
};

// *** Global *********************
void PrintAllIO(String&);

extern G_IOList gNames;
extern G_SensorList gSensors;
extern G_ActuatorList gActuators;
extern G_TriggerList gTriggers;
extern G_EventList gEvents;
extern ExtendedEEPROMClass eeprom;


#endif