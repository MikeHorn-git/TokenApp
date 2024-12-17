# Description

Retrieves and displays information about the current process's security token, such as the Session ID and Owner Security Identifier (SID).

# Features

* Retrieves and displays the Session ID of the current process
* Extracts and prints the Owner SID in a human-readable format

# Requirements

* [Msbuild](https://visualstudio.microsoft.com/downloads/)

# Compilation

```batch
cl /EHsc TokenApp.cpp
```
