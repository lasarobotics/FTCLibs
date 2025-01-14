/**********************************************************

File Logging
logging.h

Allows rapidly writing to files on the NXT.

**********************************************************/
#ifndef _LOGGING_H
#define _LOGGING_H

/**** Globals ****/
bool log_enabled = false; //enable logging
bool log_setup = false;

/**** File OPERATIONS ****/
static string filename = "";
static TFileHandle filehandle= NULL;
static TFileIOResult ioresult = SUCCESS;

static int filesize = 6000; // will store our file size

static bool log_failure()
{
  if (ioresult == SUCCESS) { return true; }
  return false;
}

/**
* File Setup
* Setup File Handles for Logging
*/
bool log_init(const string file,int size)
{
  filesize = size;
  if (!log_enabled) { return true; }
  //Delete Previous File
   Delete(file, ioresult);
  //open the file for writing (creates the file if it does not exist)
  OpenWrite(filehandle, ioresult, file, filesize);

  filename = file;
  log_setup = true;
  return log_failure();
}


/**
* File Setup
* Setup File Handles for Logging
*/
bool log_init(const string file)
{
  if (!log_enabled) { return true; }
  //Delete Previous File
   Delete(file, ioresult);
  //open the file for writing (creates the file if it does not exist)
  OpenWrite(filehandle, ioresult, file, filesize);

  filename = file;
  log_setup = true;
  return log_failure();
}

/**
* Write a string to a log file
**/
bool log_write(const string tag,const string text)
{
  if (!log_enabled) { return true; }
  if (!log_setup){
    log_init("default.txt");
  }
  float time = ((float)time1[T4]/(float)1000);
  string s = "";
  StringFormat(s,"%1.2f:[%s]%s",time,tag,text);
  WriteText(filehandle, ioresult, s);
  WriteText(filehandle, ioresult, "\r\n");
  return log_failure();
}

/**
* Safely Stop Logging
**/
void log_stop()
{
  if (!log_enabled) { return; }
  log_enabled = false;
  Close(filehandle, ioresult);
}

#endif
