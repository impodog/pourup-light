# PUPL - PourUP-Light

PUPL - A library that packs data and processes it.



PourUP-Light(pupl) is a light library for parsing PUPL language files and operating PUPL data structure.

Puppy-Light(ppyl) compiler is also given in this repository. It can compile json and ppyl source files into pupl format.

See [Language Tutorial](#Language Tutorial) for more details.

## Quick Start

To use this library, compile it with CMake. Here is one possible method:

```shell
# Run these commands under the library directory

mkdir build
# If necessary, specifiy generator in the following command using "-G <Generator>"
# <Generator> examples(Use one of them with quotes!): "Unix Makefiles", "MinGW Makefiles", "Ninja"
cmake -S . -B build
cmake --build build
```

Then you should get a dynamic library under "build/".

With the library, you can build your projects. You need to `#include "pupl.h"` and link to the library. Such as(using CMake):

In "CMakeLists.txt"

```cmake
project(my_project)
set(PUPL_PATH "/path/to/pupl/directory")
include_directories(${PUPL_PATH})
link_directories(${PUPL_PATH}/build)
add_executable(my_project sample.c)
target_link_libraries(my_project pourup_light)
```

In "sample.c":

```c
#include "pupl.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    /* Initialize PUPL library.*/
    PUPL_Init();
    
    /* Parse the desired PUPL source */
    PUPL_CallStack call_stack = PUPL_parse("sample.pupl");
    /* When NULL is returned, error happened. */
    if (call_stack == NULL) {
        /*Show the error by using PUPL_get_error(). */
        printf("Error: %s\n", PUPL_get_error());
        return 1;
    }
    
    /* Showing the environ(known as "object" in json, or "map" in pups) at the bottom in debug format
    The "bottom" is the top level of environ, or the parent to all data. */
    PUPL_Environ_show(PUPL_CallStack_bottom(call_stack));
    
    /* Last, clean up the call stack */
    PUPL_CallStack_free(call_stack);
    
    /*Quit PUPL library*/
    PUPL_quit();
    return 0;
}
```

In "sample.pupl":

```pupl
<$Hello, world!
>msg
```

Last, build the project with:

```shell
# Only do these two commands when you first build your project!
mkdir build
cmake -S . -B build
# Do this to build the project, whether you are building for the first time or the following times
cmake --build build
```

Run the project executable:

```shell
# Windows:
.\build\my_project.exe

# Other:
./build/my_project
```

Sample output:

```
{
'msg': "Hello, world!"
}
```



## Library Usage

Here we introduce the commonly used parts and functions in the PUPL library.

### Item

Item is the basic unit of PUPL operations.

#### Types

The enumeration types of all PUPL items. It is listed as follows:

```c
enum PUPL_ItemType {
    PUPL_INT_T,
    PUPL_PTR_T,
    PUPL_FLOAT_T,
    PUPL_BOOL_T,
    PUPL_NULL_T,
    PUPL_STRING_T,
    PUPL_ARRAY_T,
    PUPL_ENVIRON_PTR_T
};
```

#### Create

To create a new PUPL item, use:

```c
PUPL_Item PUPL_Item_new_int(PUPL_Int value);

PUPL_Item PUPL_Item_new_ptr(PUPL_Ptr value);

PUPL_Item PUPL_Item_new_float(PUPL_Float value);

PUPL_Item PUPL_Item_new_bool(PUPL_Bool value);

PUPL_Item PUPL_Item_new_null();

PUPL_Item PUPL_Item_new_string(PUPL_ConstString value);

/* IMPORTANT: The array DOES manage the given pointer. You do NOT need to free "value" again. */
PUPL_Item PUPL_Item_new_array(PUPL_Array value);

/* This one is special. It requires you to own a clone of PUPL_Environ and convert it into PUPL_Ptr.
The environ will be freed along with the item. */
PUPL_Item PUPL_Item_new_environ_ptr(PUPL_Ptr value);

/* You may also obtain a fresh copy of an item by using: */
PUPL_Item PUPL_Item_copy(PUPL_Item item);
```

#### Access

To access a item(actually a pointer to `struct PUPL_Item_`), use:

```c
/* Here lists the types of items again, and the associated values with the type.
Do tasks as you wish with the values. However, to make a copy, it is better to use PUPL_Item_copy. */
switch (item->type) {
    case PUPL_INT_T:
        item->value.int_v;
        break;
    case PUPL_PTR_T:
        item->value.ptr_v;
        break;
    case PUPL_FLOAT_T:
        item->value.float_v;
        break;
    case PUPL_BOOL_T:
        item->value.bool_v;
        break;
    case PUPL_NULL_T:
        /* NULL types does not store any meanful data here */
        break;
    case PUPL_STRING_T:
        item->value.string_v;
        break;
    case PUPL_ARRAY_T:
        item->value.array_v;
        break;
    case PUPL_ENVIRON_PTR_T:
        item->value.environ_ptr_v;
        break;
}
```



#### Free

To free a manually created item that is not managed by any environ(rarely happens), use:

```c
void PUPL_Item_free(PUPL_Item item);
```

#### Show

To print a item in debug format onto `stdout`, use:

```c
/* Note that this function doesn't include line breaks unless it is a string containing '\n' */
void PUPL_Item_show(PUPL_Item item);
```

#### Other

There are also other utilities in "item.h", here are common ones:

```c
/* This creates an array for storing data, or initializing an array item. */
PUPL_Array PUPL_Array_new();

/* Pushes some item into the array. The item IS managed by the array. To avoid conflicts, use PUPL_Item_copy. */
void PUPL_Array_push(PUPL_Array *array, PUPL_Item item);

/* Free the manually create array. You do NOT need to free the array after passing it to PUPL_Item_new_array. */
void PUPL_Array_free(PUPL_Array array);
```

### Environ

Environ(known as "object" in json, or "map" in pups) is the major way to store key-value data.

#### Create

You can simply create an empty, independent environ using:

```c
PUPL_Environ PUPL_Environ_new();
```

But most of the time, we would want the environ to be a child of other environs, so you can use:

```c
/* Creates a sub-environment and returns it.
 * If the key already exists and a sub-environment is already created, then it returns the sub-environment.
 * If the key already exists and the value is not a sub-environment, then it returns NULL.
 * If the key does not exist, then it creates a sub-environment and returns it. */
PUPL_Environ PUPL_Environ_sub(PUPL_Environ env, PUPL_ConstString key);
```

Or, you can copy the environ and storing data recursively using:

```c
/* Copies the environment recursively. 
This creates a fresh, independent copy. */
PUPL_Environ PUPL_Environ_new_copy(PUPL_Environ src);

/* Copies the environment to a sub-environment recursively.
This does the copy operation based on the result of PUPL_Environ_sub.
If the key already exists and not a sub-environment, it returns NULL */
PUPL_Environ PUPL_Environ_sub_copy(PUPL_Environ env, PUPL_Environ src, PUPL_ConstString key);
```

#### Access

##### Read

We provide two ways to read an item:

```c
/*This function may return NULL when the key does not exist.
 * Remember that this does NOT return a copy.*/
PUPL_Item PUPL_Environ_find(PUPL_Environ env, PUPL_ConstString key);

/*This function may return NULL when the key does not exist.
 * Remember that this DOES return a copy of the value*/
PUPL_Item PUPL_Environ_find_value(PUPL_Environ env, PUPL_ConstString key);
```

`PUPL_Environ_find` can only access names, but does NOT copy;

`PUPL_Environ_find_value` also supports literals, but DOES copy.

##### Write

You may write into two parts of an environ, `items` and `stack`.

```c
/* This is a typical access to "items".
This does NOT copy the value. You may want to copy the value before calling this function.
This function will free the previous value if the key already exists. */
void PUPL_Environ_set(PUPL_Environ env, PUPL_ConstString key, PUPL_Item value);


/* These following two functions controls values in the stack. See language tutorial for details. */
/* This does NOT copy the value. You may want to copy the value before calling this function. */
void PUPL_Environ_push_stack(PUPL_Environ env, PUPL_Item item);

/* This may return NULL when the stack is empty. */
PUPL_Item PUPL_Environ_pull_stack(PUPL_Environ env);
```

#### Free

Freeing an environ, along with all the items and sub-environs is simple:

```c
void PUPL_Environ_free(PUPL_Environ env);
```

#### Show

To show an environ in debug format, use:

```c
void PUPL_Environ_show(PUPL_Environ env);
```

#### Other

There are some rarely used functions for environ. See [Language Tutorial](#Language Tutorial) for more details.

```c
/* This does NOT copy the value. You may want to copy the value before calling this function. */
void PUPL_Environ_set_result(PUPL_Environ env, PUPL_Item item);

PUPL_Item PUPL_Environ_get_result(PUPL_Environ env);

/* This does NOT copy the value. You may want to copy the value before calling this function. */
void PUPL_Environ_set_left_bracket(PUPL_Environ env, PUPL_Item item);
```

### Call Stack

Call stacks are stacks of environs. You can easily push, or pop from it to simulate structured data parsing. 

#### Create

You can create an empty call stack using:

```c
PUPL_CallStack PUPL_CallStack_new();
```

But most of the time, you would want the simplest way to parse a PUPL file:

```c
PUPL_CallStack PUPL_parse(const char *filename);
```

This does everything for you, and if nothing goes wrong, you will get a new call stack sized 1, otherwise, you will get NULL.

#### Access

##### Read

You can read the top(or the current environ), the bottom(or the global environ), or any other positions by using:

```c
/* Get an environ at index. out-of-range checking is NOT included. */
PUPL_Environ PUPL_CallStack_get(PUPL_CallStack call_stack, size_t index);

PUPL_Environ PUPL_CallStack_top(PUPL_CallStack call_stack);

PUPL_Environ PUPL_CallStack_bottom(PUPL_CallStack call_stack);

size_t PUPL_CallStack_size(PUPL_CallStack call_stack);
```

##### Write

You can push/pop the stack by using:

```c
/* This does NOT copy the env. You do NOT need to free the env again after calling this function. */
void PUPL_CallStack_push(PUPL_CallStack call_stack, PUPL_Environ env);

PUPL_Bool PUPL_CallStack_pop(PUPL_CallStack call_stack);
```

#### Free

You can free a call stack simply:

```c
void PUPL_CallStack_free(PUPL_CallStack call_stack);
```

### Operation

There are not much to customize in the PUPL core. However, PUPL supports functions, which is a visitor to the call stack. This makes developing PUPL extensions possible. Here list utilities for this.

#### Function

The PUPL function type alias:

```c
typedef PUPL_Bool (*PUPL_Function)(PUPL_CallStack call_stack);
```

You can add you own custom functions into a global hash map, or access to a existing function.

```c
/* Returns some pointer to the original function if the function exists, or NULL if not */
PUPL_Function PUPL_FunctionMap_add(PUPL_ConstString name, PUPL_Function function);

/* Returns NULL on failure(function not found) */
PUPL_Function PUPL_FunctionMap_find(PUPL_ConstString name);
```

It is recommend to call these functions in your dynamic libraries, for example:

```c
/* EXAMPLE CODE in a custom library */
#include "pupl.h"
PUPL_Bool MyAwesomeLibrary_show_depth(PUPL_CallStack call_stack) {
	printf("The depth is %lld.\n", PUPL_CallStack_size(call_stack));
    return 0;
}

PUPL_Bool PUPL_Init_MyAwesomeLibrary() {
    if (PUPL_FunctionMap_add("show_depth", MyAwesomeLibrary_show_depth)) {
        printf("Oh no! Some name overriding happened...\n");
        return 1;
    }
    return 0;
}
```

#### Error

PUPL library is able to handle one error message once. You can either set, or get the message.

```c
/* Set the error message stored. The message IS duplicated. */
void PUPL_set_error(const char *msg);

/* Get the error message stored. The message is NOT duplicated */
const char *PUPL_get_error();
```

### System

PUPL contains quick functions for initializing, parsing, and quitting.

```c
/* ALWAYS call this BEFORE any other functions. */
void PUPL_init();

/*This returns NULL when the file does not exist, pupl cannot parse the file, or the file causes an error.\
 * It is recommended to use this to parse a .pupl file*/
PUPL_CallStack PUPL_parse(const char *filename);

/* ALWAYS call this AFTER any other functions. */
void PUPL_quit();
```

## Language Tutorial

### Storing

#### Data Structure

In PUPL,  value are stored in `PUPL_Item`. This can have different types, but can all be copied or freed using one function each.

There are two special types of value: Array and Environ.

Array is an ordered data type. In an array, you can store items of any types. You may also add/remove the last element of the array.

Environ is an unordered key-value data type. In an environ, you can only have one value to each key, similar to object in json, unordered_map in C++.

#### Stack

The PUPL system is able to do all work with the help of the stack.

If you don't know what a stack is, just imagine a "stack" of dishes. Each time, you may only put a dish on top, or take the top dish away.

Same here in PUPL. You can either "push" a copy of some item on top of the stack, or "pull" the top item of the stack and save it in the environ.

Did you notice that we mentioned "copy"? Yes, everything that goes into the stack should be copied.

### Syntax

#### Command

To speed up processing, PUPL has a very simple, but not very easy-to-understand syntax.

PUPL can be seen as a list of commands. Each command comes in form: `<CMD><NAME>`

Where `<CMD>` is just an ASCII character, and `<NAME>` is a string without line breaks. Note that there is NO SPACE between the two.

#### Block

The parser reads each line, and interprets it as a command.

For example, this is a valid PUPL file:

```pupl
<$Hello, world
>my name
Smy name
```

Each line, the first non-space character is `<CMD>` and the rest(including space characters) is `<NAME>`.

#### Constants

PUPL supports a variety of constants. They can be used when pushing into stack instead of names.

|  Name  |                            Syntax                            |           Examples           |
| :----: | :----------------------------------------------------------: | :--------------------------: |
|  Int   | A list of decimal digits, just like integers in any other languages |      12, -3, +99, -131       |
| Float  |        A list of decimal digits with a decimal point         |  1.2, +3.14, -0.147258, 4.0  |
| String | A dollar symbol('$') followed by the string contents. May also use \n, \r, and \t | $Hello, world!\nI like dogs! |
|  Bool  |              "#T" for true, and "#F" for false               |            #T, #F            |
|  Ptr   |      A ampersand('&') followed by a hexadecimal integer      |     &1, &abcdef, &7c6e60     |
|  Null  |                              #N                              |              #N              |

### Command

PUPL has the following built-in commands to describe a data structure.

| CMD  | NAME          | ACCEPTS                        | DOES                                                         |
| ---- | ------------- | ------------------------------ | ------------------------------------------------------------ |
| <    | Push          | A key name, or a constant      | Push the given value into stack.                             |
| >    | Pull          | A key name                     | Take the top value of the stack a assign it to the name. The top is removed after pulling. |
| {    | Environ Start | A key name                     | Create/Enter environ with the name in the call stack. Until the parser meets Environ End, all changes will be done inside the given environ. |
| }    | Environ End   | Nothing                        | End the started environ.                                     |
| [    | Array Length  | A key name, or an Int constant | Define the array length and save it temporarily. When the given value is not Int, an error is reported. |
| ]    | Array Create  | A key name                     | After defining the array length, pull length amount of values from the stack and create an array. Then, assign the key name with the created array. Note that array creation sequence is reversed from pushing sequence. |
| .    | Call          | A function name                | Get a user-defined function and call it with the current call stack. |
| =    | Return        | A key name                     | Save the return value temporarily in current environ. After ending, push the value into the outer stack. |
| S    | Show          | A key name                     | Find the value of the name, and show it in `stdout`.         |

Now, you can create a test PUPL file:

```pupl
{myDog
	<$Choco
	>name

	<3
	>age

	<$Swimming
	<$Eating
	<$Walking Around
	[3
	]hobbies
	
	Shobbies
	
	=age
}
>myDog_age
SmyDog_age
```

## PPYL Compiler

PPYL compiler is written in C++, and can handle json and ppyl sources, converting them into pupl format.

### Building

It is simple to build with CMake. Try this under "/ppyl/":

See [Quick Start](#Quick Start) for the meaning for these commands.

```shell
mkdir build
cmake -S . -B build
cmake --build build
```

After building, you shall get a executable named "ppyl" under "/ppyl/build/". Run it for command line help.

### PPYL Syntax

Before reading this section, please see [Language Tutorial](#Language Tutorial) first.

ppyl does not have much difference to pupl. The major differences are as follows:

#### Strings

In ppyl, strings are not in form `$<content>`, but are like other languages, using double quotes("). You can also include line breaks if you want.

For example:

```ppyl
<"Hello, world!
Welcome to PPYL!"
>msg
```

You can also escape double quote usages with `\"`.

#### Blank Handling

After and before commands, you may insert as many blanks(not line break though!)as you want. For example:

```ppyl
<    3.14
>    pi
```

#### Key Names with Blanks

In ppyl, if you want a key name with blanks, you must wrap it in single quotes('). For example:

```
< "Lars"
> 'My Dog\'s Name'
```

You can also escape single quote usages with `\'`.

#### Mixed Commands

The most important is that you can use mixed commands, which are multiple commands in pupl, but just one line in ppyl. For example:

```ppyl
<"win32"> required_system
```

This makes it kind of look like sharp brackets, but you know it isn't.

For array initialization:

```ppyl
<1<2<3<4<5 [5] five_digits
```

Isn't that much clearer?

#### Macros

Macros are ways to simplify repeated syntax, such as array creation.

Macros all come in one syntax: `@<MACRO NAME> : arg1, arg2, arg3...`. Remember that there is an at symbol(@)

Here lists some useful macros:

| Macro Name | Arguments                | Does                                                         |
| ---------- | ------------------------ | ------------------------------------------------------------ |
| PUSH       | args...                  | Push the arguments by order onto the stack.                  |
| QUEUE      | args...                  | Push the arguments reversely onto the stack. This is useful for array-like stack pushing. |
| PULL       | args...                  | Pull the argument from stack to the names given. If you want the order to match, use `QUEUE` for pushing, and `PULL` for pulling. |
| ARRAY      | elements... , array_name | Create an array with the name containing elements. Simplifies pushing and sizing. |
| SHOW       | args...                  | Show the args one by one. This actually calls "S" command many times. |

Usage example:

```ppyl
@QUEUE : 1, 2, 3
@PULL : a, b, c
@SHOW : a, b, c
@ARRAY : a, b, c, 'My Array'
@SHOW : 'My Array'
```

### JSON Compiling

This compiler also supports converting json to ppyl then ppyl to pupl. It is simple to use. Just pass the compiler a json file path!

To make json more flexible, you can add additional commands to key "_ppyl". Such as:

```json
{
    "House": {
        "Color": "Red",
        "Size": 100
    },
    "_ppyl": [
        "@SHOW : House",
        "@ARRAY : 10, 10, 5, Volume"
    ]
}
```

These commands will be compiled with the json file, and are executed at the end of each environ(or object).
