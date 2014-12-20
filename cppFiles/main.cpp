// ca, the plain text <coding assistant> for linux
// V. alpha 0.1
// by Schneider Florian

#include <iostream>
#include <string>
#include <vector>
#include <dirent.h>
#include <unistd.h>
#include <fstream>


#define USAGE_MESSAGE "Usage: ca <command> <command_options>"
#define INVALID_COMMAND "Error: command not found"
#define UNKNOWN_ERROR "Error: unknown error"
#define OPTIONS_AMOUNT_MESSAGE "Error: not enough or invalid command options"
#define PROJECT_NAME_USED_MESSAGE "Error: there is already a file named <project_name>"

#define INIT_C "Initializing C environment..."
#define INIT_CPP "Initializing C++ environment..."

#define NO_ERROR 0
#define INVALID_COMMAND_ERROR 1
#define USAGE_ERROR 2
#define OPTIONS_AMOUNT_ERROR 3
#define PROJECT_NAME_USED_ERROR 4

#define CPP "c++"
#define CPEPE "cpp"
#define C "c"



using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::fstream;

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

void init_c()
{
  cout << INIT_C << endl;
  // create a programm.c
  system("touch programm.c");

  // open it
  fstream c_file;
  c_file.open("programm.c", fstream::out); // needs errorhandling ?!

  // write a c framework into it

  c_file << "#include <stdio.h>" << endl << endl;
  c_file << "#define NO_ERROR 0" << endl;
  c_file << "#define TRUE 1" << endl;
  c_file << "#define FALSE 0" << endl << endl;
  c_file << "int main(int argc, char** argv)" << endl;
  c_file << "{" << endl;
  c_file << " " << endl;
  c_file << "  return NO_ERROR;" << endl;
  c_file << "}";

  // close
  c_file.close();

  // create a Makefile
  system("touch Makefile");

  // open it
  fstream make_file;
  make_file.open("Makefile", fstream::out); // needs errorhandling ?!

  // write a standard C Makefile

  make_file << "CC = gcc" << endl;
  make_file << "CFLAGS = -std=c99 -Wall" << endl;
  make_file << "LDFLAGS = " << endl;
  make_file << "SRC = *.c" << endl;
  make_file << "BIN = a.out" << endl << endl;
  make_file << "all: CFLAGS += -O2 -fomit-frame-pointer" << endl;
  make_file << "all: binary" << endl << endl;
  make_file << "binary: $(SRC)" << endl;
  make_file << 0xB;
  make_file << "$(CC) $(CFLAGS) -o $(BIN) $(SRC) $(LDFLAGS)" << endl << endl;
  make_file << "clean:" << endl;
  make_file << "  -rm -f $(BIN)" << endl << endl;
  make_file << "valgrind:" << endl;
  make_file << "  valgrind --tool=memcheck --leak-check=full --show-reachable=yes ./$(BIN)";

  make_file.close();
}

void init_cpp()
{
  cout << INIT_CPP << endl;
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

  // check if project_name is already there
  bool found = false;

  for(auto actual_file: files_here)
  {
    if(actual_file == project_name)
    {
      found = true;
      // check if its not a folder
      if(!opendir(actual_file.c_str()))
      {
        return PROJECT_NAME_USED_ERROR;
      }
    }
  }
  // else check if we found a directory that is named like that
  if(found)
  {
    // then go into it
    chdir(project_name.c_str());
  }
  else
  {
    // make one and then go into it
    string syscom = "mkdir " + project_name;
    system(syscom.c_str());
    chdir(project_name.c_str());
  }

  // now we have to differ between the languages
  // for now without errorhandling for the init functions

  if(language == C)
  {
    init_c();
  }
  else if(language == CPP || language == CPEPE)
  {
    init_cpp();
  }


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

      case PROJECT_NAME_USED_ERROR:
      {
        cout << PROJECT_NAME_USED_MESSAGE << endl;
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