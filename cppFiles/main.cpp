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
#define CONFIG_FILE_MESSAGE "Error: ca_config missing! Are you in the right folder?"
#define NO_CPP_MESSAGE "Error: only available in c++ environment"
#define CLASS_THERE_MESSAGE "Error: there is already a class with this name"

#define FINISHED_SORTING "Finished sorting! Be sure to check the #includes"

#define INIT_C "Initializing C environment..."
#define INIT_CPP "Initializing C++ environment..."

#define NO_ERROR 0
#define INVALID_COMMAND_ERROR 1
#define USAGE_ERROR 2
#define OPTIONS_AMOUNT_ERROR 3
#define PROJECT_NAME_USED_ERROR 4
#define CONFIG_FILE_ERROR 5
#define NO_CPP_ERROR 6
#define CLASS_THERE_ERROR 7

#define CPP "c++"
#define CPEPE "cpp"
#define C "c"



using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::fstream;
using std::ifstream;

unsigned int class_number = 1;

void processIncludes(string file_name)
{
  cout << "processIncludes of " << file_name << endl;

  ifstream file(file_name);

  string file_string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()); // make dynamic ?!

  // now the whole file is in the string

  size_t position = 0;

  // search for includes
  while((position = file_string.find("#include \"", position)) != string::npos)
  {
    // now adjust the include
    file_string.insert(position + 10, "../hFiles/");
    position++;
  }

  fstream new_file;
  new_file.open(file_name, fstream::trunc | fstream::out);

  if(!new_file.is_open())
  {
    return;
  }

  new_file << file_string;

  new_file.close();
}

void createMakefile_c(string name)
{
  fstream make_file;
  make_file.open("Makefile", fstream::out); // needs errorhandling ?!

  // write a standard C Makefile

  make_file << "CC = gcc" << endl;
  make_file << "CFLAGS = -std=c99 -Wall" << endl;
  make_file << "LDFLAGS = " << endl;
  make_file << "SRC = *.c" << endl;
  make_file << "BIN = " << name << endl << endl;
  make_file << "all: CFLAGS += -O2 -fomit-frame-pointer" << endl;
  make_file << "all: binary" << endl << endl;
  make_file << "binary: $(SRC) ; $(CC) $(CFLAGS) -o $(BIN) $(SRC) $(LDFLAGS)" 
  << endl << endl;
  make_file << "clean: ;-rm -f $(BIN)" << endl << endl;
  make_file << "valgrind: ; valgrind --tool=memcheck --leak-check=full --show-reachable=yes ./$(BIN)" << endl;

  make_file.close();
}

void createMakefile_cpp_undependant(string name)
{
  fstream make_file;
  make_file.open("Makefile", fstream::out); // needs errorhandling ?!

  make_file << "SUBMISSION = " << name << endl;
  make_file << "EXECUTABLE = $(SUBMISSION)" << endl;
  make_file << "SOURCES    = $(wildcard *.cpp)" << endl;
  make_file << "OBJECTS    = $(patsubst \%,\%,${SOURCES:.cpp=.o})" << endl;
  make_file << "CXX        = g++" << endl;
  make_file << "CXXFLAGS   = -Wall -g -c -std=c++11 -o" << endl;
  make_file << "LDFLAGS    =" << endl << endl;
  make_file << "all: $(EXECUTABLE)" 
  << endl << endl;
  make_file << "\%.o: \%.cpp ; $(CXX) $(CXXFLAGS) $@ $< -MMD -MF ./$@.d" << endl << endl;
  make_file << "$(EXECUTABLE) : $(OBJECTS) ; $(CXX) -o $@ $^ $(LDFLAGS)" << endl << endl;
  make_file << "clean: ; rm -f ./*.o ; rm -f ./*.o.d ; rm -f $(EXECUTABLE)" << endl << endl;
  make_file << "valgrind: ; valgrind --tool=memcheck --leak-check=full ./$(EXECUTABLE)" << endl << endl;
  make_file << "run: ; ./$(SUBMISSION)" << endl;

  make_file.close();
}

void createMakefile_cpp_dependant(string name)
{
  fstream make_file;
  make_file.open("Makefile", fstream::out); // needs errorhandling ?!

  make_file << "SUBMISSION = " << name << endl;
  make_file << "EXECUTABLE = $(SUBMISSION)" << endl;
  make_file << "SOURCES    = $(wildcard cppFiles/*.cpp)" << endl;
  make_file << "OBJECTS    = $(patsubst \%,\%,${SOURCES:.cpp=.o})" << endl;
  make_file << "CXX        = g++" << endl;
  make_file << "CXXFLAGS   = -Wall -g -c -std=c++11 -o" << endl;
  make_file << "LDFLAGS    =" << endl << endl;
  make_file << "all: $(EXECUTABLE)" 
  << endl << endl;
  make_file << "\%.o: \%.cpp ; $(CXX) $(CXXFLAGS) $@ $< -MMD -MF ./$@.d" << endl << endl;
  make_file << "$(EXECUTABLE) : $(OBJECTS) ; $(CXX) -o $@ $^ $(LDFLAGS)" << endl << endl;
  make_file << "clean: ; rm -f ./cppFiles/*.o ; rm -f ./cppFiles/*.o.d ; rm -f $(EXECUTABLE)" << endl << endl;
  make_file << "valgrind: ; valgrind --tool=memcheck --leak-check=full ./$(EXECUTABLE)" << endl << endl;
  make_file << "run: ; ./$(SUBMISSION)" << endl;

  make_file.close();
}

vector<string> getCommandOptions(vector<string> &all_options, int position)
{
  vector<string> command_options;
  for(unsigned int count = position; count < all_options.size(); count++)
  {
    command_options.push_back(all_options[count]);
  }

  return command_options;
}


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

void init_c(string name)
{
  cout << INIT_C << endl;

  string file = name + ".c";

  // open it
  fstream c_file;
  c_file.open(file, fstream::out); // needs errorhandling ?!

  // write a c framework into it

  c_file << "#include <stdio.h>" << endl << endl;
  c_file << "#define NO_ERROR 0" << endl;
  c_file << "#define TRUE 1" << endl;
  c_file << "#define FALSE 0" << endl << endl;
  c_file << "int main(int argc, char** argv)" << endl;
  c_file << "{" << endl;
  c_file << "  printf(\"*Thank you for using Coding Assistant by derfloh205*\\n\");" << endl;
  c_file << "  return NO_ERROR;" << endl;
  c_file << "}";

  // close
  c_file.close();

  // create c makefile
  createMakefile_c(name);

  // create the config file

  fstream config_file;
  config_file.open("ca_config", fstream::out);

  // for now only write the name of the language

  config_file << "c" << endl;

  config_file.close();
}

void init_cpp(string name)
{
  cout << INIT_CPP << endl;
  
  // create file folders
  system("mkdir cppFiles");
  system("mkdir hFiles");
  
  // write a standard CPP Makefile, folder dependant

  createMakefile_cpp_dependant(name);

  // create the config file

  fstream config_file;
  config_file.open("ca_config", fstream::out);

  // for now only write the name of the language

  config_file << "cpp" << endl;

  config_file.close();

  // go into the cppFiles folder
  chdir("cppFiles");

  // create a main.cpp including the dummyclass

  fstream main_file;
  main_file.open("main.cpp", fstream::out);

  main_file << "#include <iostream>" << endl << endl;
  main_file << "#include \"../hFiles/CLASS.h\"" << endl << endl;
  main_file << "#define NO_ERROR 0" << endl << endl;
  main_file << "using std::cout;" << endl;
  main_file << "using std::endl;" << endl << endl;
  main_file << "int main(int argc, char** argv)" << endl;
  main_file << "{" << endl;
  main_file << "  cout << \"*Thank you for using Coding Assistant by derfloh205*\" << endl;" << endl;
  main_file << "  return NO_ERROR;" << endl;
  main_file << "}" << endl;

  main_file.close();

  // create cpp of dummyclass

  fstream dummy_cpp;
  dummy_cpp.open("CLASS.cpp", fstream::out);

  dummy_cpp << "#include \"../hFiles/CLASS.h\"" << endl << endl;
  dummy_cpp << "CLASS::CLASS(int value) : value_(value)" << endl;
  dummy_cpp << "{" << endl;
  dummy_cpp << "  " << endl;
  dummy_cpp << "}" << endl << endl;
  dummy_cpp << 
  "//--------------------------------- GETTER -------------------------------------"
  << endl;
  dummy_cpp << "int CLASS::getValue()" << endl;
  dummy_cpp << "{" << endl;
  dummy_cpp << "  return value_;" << endl;
  dummy_cpp << "}" << endl << endl;
  dummy_cpp <<
  "//--------------------------------- SETTER -------------------------------------"
  << endl;
  dummy_cpp << "void CLASS::setValue(int newValue)" << endl;
  dummy_cpp << "{" << endl;
  dummy_cpp << "  value_ = newValue;" << endl;
  dummy_cpp << "}" << endl << endl;
  dummy_cpp <<
  "//--------------------------------- OTHER --------------------------------------"
  << endl;
  dummy_cpp << "void CLASS::method()" << endl;
  dummy_cpp << "{" << endl;
  dummy_cpp << "  " << endl;
  dummy_cpp << "}";

  dummy_cpp.close();

  // get into hFiles
  chdir("../hFiles");

  // create header for CLASS
  fstream dummy_h;
  dummy_h.open("CLASS.h", fstream::out);

  dummy_h << "#ifndef CLASS_H" << endl;
  dummy_h << "#define CLASS_H" << endl << endl;
  dummy_h << "class CLASS" << endl;
  dummy_h << "{" << endl;
  dummy_h << "  private:" << endl << endl;
  dummy_h << "  int value_;" << endl << endl;
  dummy_h << "  public:" << endl << endl;
  dummy_h << "  CLASS(int value);" << endl;
  dummy_h << "  int getValue();" << endl;
  dummy_h << "  void setValue(int newValue);" << endl;
  dummy_h << "  void method();" << endl;
  dummy_h << "};" << endl;
  dummy_h << "#endif" << endl;

  dummy_h.close();
}

int install(vector<string> options)
{
  if(options.size() != 1)
  {
    return OPTIONS_AMOUNT_ERROR;
  }

  cout << "Installing Code Assistant by derfloh205..." << endl;
  
  system("sudo cp ca /bin/ca");

  return NO_ERROR; 
}

int sort(vector<string> options) // used to sort the actual folder into hFiles and cppFiles
{
  if(options.size() >= 2)
  {
    if(options[1] == "-m")
    {
      // also create a makefile when the -m flag was set
      string tmp_exec_name = "a.out";
      createMakefile_cpp_dependant(tmp_exec_name);
    }
    else
    {
      return OPTIONS_AMOUNT_ERROR;
    }
  }

  // make two folders
  system("mkdir hFiles");
  system("mkdir cppFiles");

  // sort
  system("mv *.cpp cppFiles/");
  system("mv *.h hFiles/");

  // create the config file

  fstream config_file;
  config_file.open("ca_config", fstream::out);

  // for now only write the name of the language

  config_file << "cpp" << endl;

  config_file.close();

  // change the includes in the files

  chdir("cppFiles");

  vector<string> files_here = getFileList();

  for(auto actual_file_name: files_here)
  {
    processIncludes(actual_file_name);
  }

  //

  cout << FINISHED_SORTING << endl;

  return NO_ERROR;
}

int add(vector<string> options) // used to add classes, makefiles ect.
{
  // add <what>

  if(options.size() < 2)
  {
    return OPTIONS_AMOUNT_ERROR;
  }

  string to_add;
  to_add = options[1];

  if(to_add == "class")
  {

    // get config file language
    fstream config_file;
    config_file.open("ca_config", fstream::in);

    if(!config_file.is_open())
    {
      return CONFIG_FILE_ERROR;
    }

    string language;
    config_file >> language;

    if(language != "cpp")
    {
      return NO_CPP_ERROR;
    }
     // because add what name

    string class_name;
    if(options.size() == 3)
    {
      class_name = options[2];
    }
    else
    {
      class_name = "CLASS";
    }

    // test if there is already a class in the hFiles folder with this name

    chdir("hFiles");

    fstream tmp; 
    string tmp_name_h = class_name + ".h";
    string tmp_name_cpp = class_name + ".cpp";
    tmp.open(tmp_name_h, fstream::in);
    if(tmp.is_open())
    {
      tmp.close();
      return CLASS_THERE_ERROR;  
    }
    else
    {
      chdir("../cppFiles");
      tmp.open(tmp_name_cpp, fstream::in);
      if(tmp.is_open())
      {
        tmp.close();
        return CLASS_THERE_ERROR;
      }
    }

    chdir("../hFiles");

    fstream class_to_create;
    class_to_create.open(tmp_name_h, fstream::out);

    class_to_create << "#ifndef " << class_name << "_H" << endl;
    class_to_create << "#define " << class_name << "_H" << endl << endl;
    class_to_create << "class " << class_name << endl;
    class_to_create << "{" << endl;
    class_to_create << "  private:" << endl << endl;
    class_to_create << "  int value_;" << endl << endl;
    class_to_create << "  public:" << endl << endl;
    class_to_create << "  " << class_name << "(int value);" << endl;
    class_to_create << "  int getValue();" << endl;
    class_to_create << "  void setValue(int newValue);" << endl;
    class_to_create << "  void method();" << endl;
    class_to_create << "};" << endl;
    class_to_create << "#endif" << endl;

    class_to_create.close();

    // go into cppFiles
    chdir("../cppFiles");

    // make cpp file

    fstream class_to_create_cpp;
    class_to_create_cpp.open(tmp_name_cpp, fstream::out);

    class_to_create_cpp << "#include \"../hFiles/" << class_name << ".h\"" << endl << endl;
    class_to_create_cpp << class_name << "::" << class_name << "(int value) : value_(value)" << endl;
    class_to_create_cpp << "{" << endl;
    class_to_create_cpp << "  " << endl;
    class_to_create_cpp << "}" << endl << endl;
    class_to_create_cpp << 
    "//--------------------------------- GETTER -------------------------------------"
    << endl;
    class_to_create_cpp << "int " << class_name << "::getValue()" << endl;
    class_to_create_cpp << "{" << endl;
    class_to_create_cpp << "  return value_;" << endl;
    class_to_create_cpp << "}" << endl << endl;
    class_to_create_cpp <<
    "//--------------------------------- SETTER -------------------------------------"
    << endl;
    class_to_create_cpp << "void " << class_name << "::setValue(int newValue)" << endl;
    class_to_create_cpp << "{" << endl;
    class_to_create_cpp << "  value_ = newValue;" << endl;
    class_to_create_cpp << "}" << endl << endl;
    class_to_create_cpp <<
    "//--------------------------------- OTHER --------------------------------------"
    << endl;
    class_to_create_cpp << "void " << class_name << "::method()" << endl;
    class_to_create_cpp << "{" << endl;
    class_to_create_cpp << "  " << endl;
    class_to_create_cpp << "}";

    class_to_create_cpp.close();

    cout << "Added class \"" << class_name << "\"." << endl;
  }
  else if(to_add == "makefile")
  {
    if(options.size() < 3)
    {
      return OPTIONS_AMOUNT_ERROR;
    }

    string makefile_language = options[2];
   
    if(makefile_language == "c")
    {
      string tmp_exec_name = "a.out";
      createMakefile_c(tmp_exec_name);
    }
    else if(makefile_language == "cpp" || makefile_language == "c++")
    {
      if(options.size() >= 4)
      {
        // there could be a -f flag for folders
        if(options[3] == "-f")
        {
          // make folder dependent makefile
          string tmp_exec_name = "a.out";
          createMakefile_cpp_dependant(tmp_exec_name);
        }
        else
        {
          return OPTIONS_AMOUNT_ERROR;
        }
      }
      else
      {
        // make folder undependant makefile
        string tmp_exec_name = "a.out";
        createMakefile_cpp_undependant(tmp_exec_name);
      }
    }

    cout << "Added Makefile (" << makefile_language << ")" << endl;

  }
  else
  {
    return OPTIONS_AMOUNT_ERROR;
  }


  return NO_ERROR;
}

int init(vector<string> options) // used to initialize a prepared projectfolder
{
  // Check if command has enough options given
  // the options vector looks like this:
  // {command, option, option, ...}
  //     0       1        2    ...

  if(options.size() < 3)
  {
    return OPTIONS_AMOUNT_ERROR;
  }

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
    init_c(project_name);
  }
  else if(language == CPP || language == CPEPE)
  {
    init_cpp(project_name);
  }


  return NO_ERROR;
}

int showHelp()
{
  cout << "Thank you for using coding assistant by derfloh205" << endl << endl;
  cout << "Command Overview" << endl << endl;
  cout << "init ............. is used to initialize a c or c++ working environment" << endl << endl;
  cout << "add .............. is used to add a new c++ class or c/c++ makefile" << endl << endl;
  cout << "cppsort/c++sort .. is used to initialize a already existing c++ working environment" << endl << endl;
  cout << ".................. this command also sorts your h and cpp files into folders and changes the includes within" << endl << endl;
  cout << "install .......... asking for sudo rights, this command will install ca into your bin folder on your linux system" << endl << endl;
  cout << ".................. so that you can use ca whereever you want" << endl << endl;
  cout << "help.............. you are using that just now." << endl << endl;

  return NO_ERROR;
}

int gitSupply(vector<string> options)
{
  if(options.size() < 3)
  {
    return OPTIONS_AMOUNT_ERROR;
  }


  if(options[1] == "finish") // I dont know if this name is good
  {

    system("git add --all");
    string commitstring = "git commit -a -m " + options[2]; 
    system(commitstring.c_str());
    system("git push --all");
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
    else if(argument_vector[0] == "cppsort" || argument_vector[0] == "c++sort")
    {
      errorcode = sort(argument_vector);
      throw errorcode;
    }
    else if(argument_vector[0] == "install")
    {
      errorcode = install(argument_vector);
      throw errorcode;
    }
    else if(argument_vector[0] == "help")
    {
      errorcode = showHelp();
      throw errorcode;
    }
    else if(argument_vector[0] == "git")
    {
      errorcode = gitSupply(argument_vector);
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
        exit(INVALID_COMMAND_ERROR);
      }

      case USAGE_ERROR:
      {
        cout << USAGE_MESSAGE << endl;
        exit(USAGE_ERROR);
      }

      case OPTIONS_AMOUNT_ERROR:
      {
        cout << OPTIONS_AMOUNT_MESSAGE << endl;
        exit(OPTIONS_AMOUNT_ERROR);
      }

      case PROJECT_NAME_USED_ERROR:
      {
        cout << PROJECT_NAME_USED_MESSAGE << endl;
        exit(PROJECT_NAME_USED_ERROR);
      }

      case CONFIG_FILE_ERROR:
      {
        cout << CONFIG_FILE_MESSAGE << endl;
        exit(CONFIG_FILE_ERROR);
      }

      case NO_CPP_ERROR:
      {
        cout << NO_CPP_MESSAGE << endl;
        exit(NO_CPP_ERROR);
      }

      case CLASS_THERE_ERROR:
      {
        cout << CLASS_THERE_MESSAGE << endl;
        exit(CLASS_THERE_ERROR);
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