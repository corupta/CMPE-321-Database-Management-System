# CMPE 321 DBMS Implementation


## List of Classes:

* ### Helpers:
    A helper module created for handling and debugging cpp binary read/write file streams.

* ### Scheme:
    A class that knows and manages the definition of a type. 
    (type name, field count, field names)
    It can read and write its type definition from/to the system catalog when 
    given a file stream of the system catalog.
    
* ### RWHandler:
    A class that manages the records and indexes of a type.
    Has a scheme instance for type definitions.
    It is responsible for DML operations and creating/opening/reading/writing 
    data table & index files of a type.
    
* ### SystemCatalog:
    A class that knows and manages the system catalog.
    It has a collection of the schemes of all the types. (Has a map of Scheme instances by type names)
    It is responsible for DDL operations and creating/opening/reading/writing system catalog file.
    
* ### System:
    A singleton facade class responsible for reading & parsing queries from the user, 
    redirecting them to respective classes/functions and printing their output.


* #### main:
    Not a class, but the main function. Creates & validates the input/output streams from the program arguments.
    Creates a System class instance and starts it.
    
    
## Important Note

Although fstream handles the read / write pointers as the same single pointer, I've though otherwise and implemented separate functions for them. Yet, when I found about this situation I just fixed the duplicate changes of the pointers and used them as is when I fixed the problems within them.