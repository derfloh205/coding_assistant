<h1>Coding_Assistant</h1>

<h2>Introduction</h2>

Coding_Assistant should be an enhancement to everybody who likes to code plain text
in linux. It is a simple shell command, providing easy and fast usage.

<h2> SETUP </h2>

Just make it, and then type:

<b>ca install</b>

It will normally ask for superuser rights because it copies itself into the /bin folder

<h2>How do I use it?</h2>

Simply write "ca", the command you want to use and its options afterward like this:

<b>ca command commandoptions</b>

<h2>Commands</h2>

<h3>init</h3>

<h4>ca init project_name c</h4>

Creates a folder named project_name with a prepared .c file and a Makefile to
compile, valgrind or clean this folder.

<h4>ca init project_name cpp || ca init project_name c++</h4>

Creates a folder named project_name with a Makefile and two folders in it named
hFiles and cppFiles. The folders already contain a dummy class and a main.cpp
ready to be compiled by the Makefile.

<h4>ca add class</h4>

In a cpp environment, it creates a dummy class with header and cpp file within
the two folders.
If you give it a name like:

<b>ca add class name</b>

it will create a class.h and class.cpp with the given name

<h4>ca add makefile language</h4>

Adds a makefile for the specific given language
Also possible:

<b>ca add makefile language -f</b>

The -f flag only works with cpp makefile and makes a folder dependant makefile
that compiles h and cpp files out of folders.

<h4>ca cppsort || ca c++sort</h4>

Sorts the actual folder into hFiles and cppFiles
With the -m flag it also creates a folder dependant c++ makefile
It changes the includes of your files to keep it compileable

<h2>ca_config Specification:</h2>

The ca_config file is here to make it possible for the user to determine some
options.
It is created when a c or a cpp environment is initated.

It looks like this:

<em>language</em><br/>
<em>option1</em><br/>
<em>option2</em><br/>

<b>For now there will only be the name of the 
actual environments coding language within!</b>

<h2>Coming Soon...</h2>

<h4>Get Command</h4>

This could maybe help you retrieve Information for certain algorithms or other stuff 
from the net or locally

<h4>Icon Command</h4>

A own Command to change the icon of your executeable to anything you want ?
Why not?
Difficulty here is to save that there is an icon applied and apply it evertime you compile it with make
simple version i not complicated...

<h4>Search Command</h4>

Search all your directories from here for a specific word
but why? there is already a search option in various browsers..
.. maybe with advanced options? But i dont know what

<h4>Git Supply</h4>

<h5>ca git finish -commitname-</h5>

Does the following:
> git add --all
> git commit -a -m -commitname-
> git push --all