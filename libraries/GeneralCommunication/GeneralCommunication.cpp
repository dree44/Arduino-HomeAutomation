#include "GeneralCommunication.h"
#include <GeneralBasics.h>

String serialInputString = "";

void PrintCommands(String& result) {
  result=F("*** Usable Commands: \nhelp\n    usable command list (this printout)\nlist\n    listing pin settings, available events and applied triggers\nset name:stirng type:(analog|digital|swserial|humtemp) direction:(in|out) pin_id:byte\n    set up a pin\nclear (pin_id:byte | name:string)\n    remove a pin setting\nname\n    get the value of the sensor\nname TIMEDVALUE | @event\n    set the value of or apply event on the actuator\nsetevent @event:string TIMEDVALUE\n    set up an event.\n    TIMEDVALUE: value<time>value...32x | \"string\"\n        describe a value list or a value represented by a string\n        special strings: \"!\" means digital negalt\n                         \"PRINT\" means write out actual value on serial\nclearevent @event:string\n    clears the event settings\nsettrigger ~triggername:string {$periodictime:millisec | name((<-|->|=|<|>)value:byte)} name @event\n    set up triggering condition. periodictime=0 means allways\ncleartrigger ~triggername\n");
}

void PrintPinSettings(WRITING w) {
  SerialPrintAllIO();
  FREEMEM();
}

bool ParseSerial(String serialString,String& errorString,struct COMMAND& command) {
FREEMEM();

  String tokens[16];
  for(int k=0;k<16;++k) tokens[k]="";
  for(int i=0,k=0;i<serialString.length();++i) {
    if(serialString[i]==' ') {
      k++;
    } else {
      if(serialString[i]!=13) {
        tokens[k]+=serialString[i];
      }
    }
  }
  for(int k=0;k<16;++k) {
    if(tokens[k]==C_LIST) {
      if(command.command==C_INVALID) {
        command.command=LIST;
      } else {
	    errorString+=F("multiple command is not allowed: ");
        errorString+=C_LIST;
        return false;
      }
    } else if(tokens[k]==C_HELP) {
      if(command.command==C_INVALID) {
        command.command=HELP;
      } else {
	    errorString+=F("multiple command is not allowed: ");
        errorString+=C_HELP;
        return false;
      }
    } else if(tokens[k]==C_STATUS) {
      if(command.command==C_INVALID) {
        command.command=STATUS;
      } else {
	    errorString+=F("multiple command is not allowed: ");
        errorString+=C_STATUS;
        return false;
      }
    } else if(tokens[k]==C_DOWNLOAD) {
      if(command.command==C_INVALID) {
        command.command=DOWNLOAD;
      } else {
	    errorString+=F("multiple command is not allowed: ");
        errorString+=C_DOWNLOAD;
        return false;
      }
    } else if(tokens[k]==C_UPLOAD) {
      if(command.command==C_INVALID) {
        command.command=UPLOAD;
      } else {
	    errorString+=F("multiple command is not allowed: ");
        errorString+=C_UPLOAD;
        return false;
      }
    } else if(tokens[k]==C_LOADSETTINGS) {
      if(command.command==C_INVALID) {
        command.command=LOADSETTINGS;
      } else {
	    errorString+=F("multiple command is not allowed: ");
        errorString+=C_LOADSETTINGS;
        return false;
      }
    } else if(tokens[k].length()>0 && tokens[k][0]=='"' && tokens[k][tokens[k].length()-1]=='"') { //"string value"
      command.value.stringType=true;
      command.value.numOfValue=0;
      int m;
      for(m=0;m<tokens[k].length()-2;++m) {
        command.value.string+=tokens[k][m+1];
      }
    } else if(tokens[k].length()>0 && tokens[k][0]>='0' && tokens[k][0]<='9' || tokens[k].length()>1 && tokens[k][1]>='0' &&tokens[k][1]<='9' && tokens[k][0]!='@') { //value
      String valuetokens[TIMEDVALUELENGTH];
      String timetokens[TIMEDVALUELENGTH];
      for(int l=0;l<TIMEDVALUELENGTH;++l) {
        valuetokens[l]="";
        timetokens[l]="";
      }
      bool val=true;
      int lv=0,lt=0;
      for(int j=0;j<tokens[k].length();++j) {
        if(val && tokens[k][j]=='<') {
          val=false;
          lv++;
        } else if(!val && tokens[k][j]=='>') {
          val=true;
          lt++;
        } else {
          if(val) {
            valuetokens[lv]+=tokens[k][j];
          } else {
            timetokens[lt]+=tokens[k][j];
          }
        }
      }
      command.value.numOfValue=lv+1;
      if(command.value.numOfValue>0) command.value.value=new int[command.value.numOfValue];
      if(command.value.numOfValue>1) command.value.time=new int[command.value.numOfValue-1];
      for(lv=0;valuetokens[lv].length()>0;++lv) {
        if(valuetokens[lv].length()>2 && valuetokens[lv][0]=='0' && valuetokens[lv][1]=='x') { //hex
          command.value.value[lv]=0;
          for(int q=2;q<valuetokens[lv].length();++q) {
            char x=CharToHex(valuetokens[lv][q]);
            if(x==-1) {
			  errorString+=F("wrong hexa in value ");
              errorString+=valuetokens[lv];
              return false;
            }
            command.value.value[lv]=command.value.value[lv]*256+x;
          }
        } else { //not hex
          String intcandidate=valuetokens[lv];
          int value=intcandidate.toInt();
          if(value==0 && intcandidate!="0") {
		    errorString+=F("wrong number in value ");
            errorString+=valuetokens[lv];
            return false;
          }
          command.value.value[lv]=value;
        }
      }
      for(lt=0;timetokens[lt].length()>0;++lt) {
        command.value.time[lt]=0;
        if(timetokens[lt].toInt()==0 && timetokens[lt]!="0") {
		  errorString+=F("wrong number in value's time ");
          errorString+=timetokens[lt];
          return false;
        }
        command.value.time[lt]=timetokens[lt].toInt();
      }
    } else if(tokens[k].length()>1 && tokens[k][0]=='@') { //event name
      command.event=tokens[k];
    } else if(tokens[k].length()>0 && IsLetter(tokens[k][0]) ) { //name
      command.name=tokens[k];
    }
  }
  return true;
}

void ParseTextCommand(String string,String& response,String& errorString) {
      digitalWrite(21, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(10);               // wait for a second
      digitalWrite(21, LOW);    // turn the LED off by making the voltage LOW
//      Serial.println(serialString);
      struct COMMAND command;
      command.command=C_INVALID;
      command.name="";
      command.event="";
      command.trigger="";
//      String errorString="";
      if(!ParseSerial(string,errorString,command)) {
//        Serial.print("ERROR: ");
//        Serial.println(errorString);
      } else {
        if(command.command==LIST) {
          PrintPinSettings(GW_SERIAL);
        } else if(command.command==HELP) {
          PrintCommands(response);
        } else if(command.command==STATUS) {
        } else if(command.command==UPLOAD) {
        } else if(command.command==DOWNLOAD) {
        } else if(command.command==C_INVALID && command.name!="" && (command.event!="" || command.value.numOfValue>0 || command.value.stringType)) {
          command.command=SETVALUE;
		  G_IOBase* name = gNames.find(command.name);
          G_IOBase* act=0;
          if(name) act=gActuators.find(name->index);
		  G_IOBase* evName = gNames.find(command.event);
          if(act==0) {
            response=command.name;
            response+=F(" is not an actuator.");
          } else if(command.event!="" && evName==0) {
            response=command.event;
            response+=F(" is not existing.");
          } else {
            if(command.event!="") {
              G_Event* ev=((G_Event*)gEvents.list.find(evName->index));
			  act->set(&(ev->value));
            } else {
              act->set(&(command.value));
            }
            response=F("OK");
          }
        } else if(command.command==C_INVALID && command.name!="" && command.value.numOfValue==0) {
          command.command=GETVALUE;
          response=command.name;
          G_IOBase* n=gNames.find(command.name);
          if(n) {
            G_IOBase* io=gSensors.find(n->index);
            if(io==0) io=gActuators.find(n->index);
            if(io) {
              response+=F(": ");
              response+=io->valueToString();
            }
          }
        }          
      }
}

