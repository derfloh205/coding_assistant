<h1>Coding_Assistant</h1>

<h2>Introduction</h2>

Coding_Assistant should be an enhancement to everybody who likes to code plain text
in linux. Its a simple shell command, providing easy and fast usage.

<How do I use it?>

Simply write "ca", the command you want to use and its options afterward like this:

  ca command commandoptions

<h2>Commands</h2>

<h3>init</h3>

<h4>ca init project_name c</h4>

Creates a folder named project_name with a prepared .c file and a Makefile to
compile, valgrind or clean this folder.

<h4>ca init project_name cpp || ca init project_name c++</h4>

Creates a folder named project_name with a Makefile and two folders in it named
hFiles and cppFiles. The folders already contain a dummy class and a main.cpp
ready to be compiled by the Makefile.

<h4>ca add class (not yet implemented)</h4>

In a cpp environment, it creates a dummy class with header and cpp file within
the two folders.

<h4>ca ??? (further ideas incoming)</h4>


<h2>ca_config Specification:</h2>

The ca_config file is here to make it possible for the user to determine some
options.
It is created when a c or a cpp environment is initated.

It looks like this:

<em>language</em><br/>
<em>option1</em><br/>
<em>option2</em><br/>

For now there will only be the name of the 
actual environments coding language within!