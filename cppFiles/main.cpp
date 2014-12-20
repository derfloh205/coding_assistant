// ca, the plain text <coding assistant> for linux
// V. alpha 0.1
// by Schneider Florian



#include <iostream>
#include <string>
#include <vector>
#include <map>

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

int add(vector<string> options)
{
  cout << "ADD COMMAND" << endl;
  return NO_ERROR;
}

int init(vector<string> options)
{
  // Check if command has enough options given | 1 option -> size must be 2 or more ?!
  // the options vector looks like this:
  // {command, option, option, ...}
  //     0       1        2    ...

  // Init command has to look like this
  // ./smart_init init <project_name> <language>

  if(options.size() < 2)
  {
    return OPTIONS_AMOUNT_ERROR;
  }

  cout << "INIT COMMAND" << endl;



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