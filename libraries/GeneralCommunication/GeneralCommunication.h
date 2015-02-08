#ifndef generalcommunication
#define generalcommunication

#include "Arduino.h"
#include <GeneralIO.h>
#include <GeneralLog.h>

#define C_LIST "list"
#define C_HELP "help"
#define C_STATUS "status"
#define C_UPLOAD "upload"
#define C_DOWNLOAD "download"
#define C_LOADSETTINGS "loadsettings"

#define TIMEDVALUELENGTH 10

typedef enum {C_INVALID,LIST,GETVALUE,SETVALUE,HELP,STATUS,UPLOAD,DOWNLOAD,LOADSETTINGS} CommandType;

struct COMMAND {
  CommandType command;
  String name;
  String event;
  String trigger;
  TimedValue value;
};

void PrintCommands(String& result);
void PrintPinSettings(WRITING);
bool ParseSerial(String serialString,String& errorString,struct COMMAND& command);
void ParseTextCommand(String string,String& response,String& errorString);

extern String serialInputString;

#endif