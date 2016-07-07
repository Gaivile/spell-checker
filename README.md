# Spell Checker
An example of trie data structure implementation used as a dictionary for spellcheck.

<b>CS50 Problem Set 5</b>

Usage: ./speller [dictionary] text

Reads data from dictionary file, loads it into memory creating trie data structure. Trie was chosen to optimize real-world running time as per task's specification the main goal was to minimize time for load(), check(), size() and unload() functions in dictionary.c.
After loading dictionary file, each word from text file is checked and printed if it is not found in dictionary file. The program also prints total number of words found in dictionary and text files; as well as benchmarks for the named functions. 


