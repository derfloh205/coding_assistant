// ca, the plain text <coding assistant> for linux
// V. alpha 0.1
// by Schneider Florian

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <dirent.h>

#define USAGE_MESSAGE "Usage: ca <command> <command_options>"
#define INVALID_COMMAND "Error: command not found"
#define UNKNOWN_ERROR "Error: unknown error"
#define OPTIONS_AMOUNT_MESSAGE "Error: not enough or invalid command options"

#define INIT_C "Initializing C environment..."
#define INIT_CPP "Initializing C++ environment..."

#define NO_ERROR 0
#define INVALID_COMMAND_ERROR 1
#define USAGE_ERROR 2
#define OPTIONS_AMOUNT_ERROR 3



using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::map;

bool ignored(std::string name) // used to ignore hidden .name files
{
  // ignore "."

  if(name[0] == '.')
  {
    return true;
  }

  return false;
}

vector<string> getFileList() // used to get the files in the actual folder as string vector
{
  vector<string> files_here;
  // get list of all files within

  DIR *dpdf;
  struct dirent *epdf;

  dpdf = opendir(".");
  if (dpdf != NULL)
  {
     while (epdf = readdir(dpdf))
     {
        
        if(!ignored(epdf->d_name))
        {
          files_here.push_back(epdf->d_name);
        }     
     }
  }

  return files_here;
}

int add(vector<string> options) // used to add classes, makefiles ect.
{
  cout << "ADD COMMAND" << endl;
  return NO_ERROR;
}

int init(vector<string> options) // used to initialize a prepared projectfolder
{
  // Check if command has enough options given
  // the options vector looks like this:
  // {command, option, option, ...}
  //     0       1        2    ...

  // Init command has to look like this
  // ./smart_init init <project_name> <language>

  if(options.size() < 3)
  {
    return OPTIONS_AMOUNT_ERROR;
  }

  cout << "INIT COMMAND" << endl;

  // if yes then extract the two options as strings

  string project_name = options[1];
  string language = options[2];

  // get file list
  vector<string> files_here = getFileList();


  return NO_ERROR;
}

int main(int argc, char** argv)
{

  int errorcode = 0;

  try
  {
    // handle command
    if(argc < 2)
    {
      errorcode = USAGE_ERROR;
      throw errorcode;
    }
    
    // write commands and their options into vector

    vector<string> argument_vector;

    for(int count = 1; count < argc; count++)
    {
      string argument = argv[count];
      argument_vector.push_back(argument);
    }

    /*
    Commands are:
    new
    add
    */
  
    if(argument_vector[0] == "init")
    {
      errorcode = init(argument_vector);
      throw errorcode;
    }
    else if( argument_vector[0] == "add")
    {
      errorcode = add(argument_vector);
      throw errorcode;
    }
    else
    {
      errorcode = INVALID_COMMAND_ERROR;
      throw errorcode;
    }
  }

  catch(int errorcode)
  {
    switch(errorcode)
    {
      case INVALID_COMMAND_ERROR:
      {
        cout << INVALID_COMMAND << endl;
        break;
      }

      case USAGE_ERROR:
      {
        cout << USAGE_MESSAGE << endl;
        break;
      }

      case OPTIONS_AMOUNT_ERROR:
      {
        cout << OPTIONS_AMOUNT_MESSAGE << endl;
        break;
      }

      case NO_ERROR:
      {
        return NO_ERROR;
      }

      default:
      {
        cout << UNKNOWN_ERROR << endl;
        break;
      }
    }
  }
  

  return NO_ERROR;
}