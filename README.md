# Spell-Checker---Red-Black-Tree-based

## Overview

The "Spell Checker" is an application that uses a Red-Black Tree data structure to efficiently check the correctness of words and provide suggestions for misspelled words. It loads a dictionary of words from a file and allows users to enter words to check if they are spelled correctly. If a word is misspelled, the application suggests corrections based on Levenshtein distance, a measure of similarity between two strings.

## Features

  1. Check if a word is spelled correctly
  2. Suggest corrections for misspelled words based on Levenshtein distance
  3. Load a dictionary of words from a file


## Usage

  1. Compile and run the "Spell Checker" application.
  2. Enter a word to check if it is spelled correctly.
  3. If the word is misspelled, the application will suggest corrections.
  4. To exit the application, enter "-1" when prompted for a word.

## Implementation Details

The "Spell Checker" uses a Red-Black Tree to efficiently store and search for words in the loaded dictionary. The Red-Black Tree ensures balanced and fast operations, making it suitable for managing large dictionaries.


To find suggestions for misspelled words, the application calculates the Levenshtein distance between the misspelled word and each word in the dictionary. Levenshtein distance measures the number of single-character edits (insertions, deletions, or substitutions) required to transform one string into another. Words with a Levenshtein distance within a specified threshold are suggested as corrections.

## Dictionary File

The dictionary of correctly spelled words is loaded from a file named "file_name.txt." The file should contain one word per line, with no leading or trailing whitespaces.
