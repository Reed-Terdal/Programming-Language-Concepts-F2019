# Programming-Language-Concepts-F2019
Repository for Programming Language Concepts and the Jott Interpreter

## How to run the Jott Interpreter:
Pre-req: Must have [GLibc](https://developer.gnome.org/glib/2.62/index.html) installed. 
> sudo apt-get install glib-2.0
1. Make sure the working directory is where the unzipped files are, should be same level as the CMakeLists.txt
2. Run cmake: "cmake ."
3. Run make: "make"
4. This will produce a binary called `jott`
5. Programs can then be run like: "./jott /path/to/programs/prog5.j"
6. To generate a JSON debug dump, add the '-d' flag to the end of the parameters: "./jott /path/to/programs/prog5.j -d"
