Introduction of implementation:

Column: We use our array classes for different types of array to represent different types of Columns. Basically the methods in Column 
are the same as methods in array. 

Schema: We use array of string pointers to record column names and row names, and we use a character array to keep track of the types
of each column. In order to avoid capacity problem when adding new elements to these arrays, we created fields which corresponded to 
their number of elements and capacity so we can update these arrays to larger capacity when elements exceed the size of the array.   

Row: We use Column** to keep track of the data of a single row. It's basically an array of column with size 1. 

Dataframe: Basically we have fields of Column** to keep track of its data, a schema which defines the structure and size_t which records
the current number of rows and columns. We construct the dataframe by reading the fields of input schema and initialize the column** and 
schema correspondingly.

Overview of design choices:
The most difficult decision is how to implement class column. We decided to use our array class as the array has basically most of 
the functionality of column. By assigning array as field of column, we can simply call our array methods to get the result. To manage
capacity, we have capacity fields corresponded to every array feild that we have in order to solve the issue if adding new elements to 
an array with max storage. We set initial capacity as 10 to initialize array with size of 10 and We would check the capacity for each adding and expand the array if necessary. 

Use cases:
User can use our dataframe basically as an excel table which can record huge amount of data. The User can keep track of two attributes of
these data by giving a schema with column names and row names. Besides, the user can access the exact data by giving its (x, y) index in 
the table, and print out the whole dataframe in a table format. 

Use case of dataframe:
Imagine we want to view the functionalites of different vehicles:

// Creating a data frame with the right structure 
Schema scm(“FIIIFF”);       // the schema

//add column names

String* s1 = new String("mpg")

String* s2 = new String("cyl")

String* s3 = new String("disp")

...

scm.column_names = new String*[s1, s2, s3, ...];

//add row names 

String* s4 = new String("Mazda RX4")

String* s5 = new String("Mazda RX4 Wag")

String* s6 = new String("Datsun 710")

scm.row_names = new String*[s4, s5, s6, ...]

DataFrame df(scm);         // the data frame  


// Inputing data columns 

Column* c1 = new FloatColumn(21.0, 21.0, 22.8, ...)

Column* c2 = new IntColumn(6, 6, 4, ...)

Column* c3 = new IntColumn(160, 160, 108, ...)

...

df.add_column(c1);

df.add_column(c2);

df.add_column(c3);

...

df.print()

//we will get
                              
                              
                             mpg cyl disp  hp drat   wt ...

               Mazda RX4     21.0   6  160 110 3.90 2.62 ...

               Mazda RX4 Wag 21.0   6  160 110 3.90 2.88 ...

               Datsun 710    22.8   4  108  93 3.85 2.32 ...

                              ............
