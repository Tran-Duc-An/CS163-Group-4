# Dictionary Application

Welcome to the **Dictionary Application**, a graphical desktop application developed as a group project for the **CS163 - Data Structures** course at the University of Science, Vietnam National University - Ho Chi Minh City, under the Advanced Program in Computer Science. This project implements a multi-lingual dictionary supporting English-Vietnamese, English-English, and Vietnamese-English translations, built using a Trie data structure and enhanced with features like SFML-based UI, search history, favorites, and interactive quizzes.

## Table of Contents
- [Project Overview](#project-overview)
- [Features](#features)
- [Directory Structure](#directory-structure)

## Project Overview
The Dictionary Application leverages the **Trie data structure** to efficiently store and manage three datasets: English-Vietnamese (EVTrie), English-English (EETrie), and Vietnamese-English (VTrie), with additional support for emoji searching. The project was developed by a team of five students to explore advanced data structures, algorithms (e.g., KMP for substring search), and graphical user interface design using SFML. It handles large datasets—103,868 words (EV), 113,477 words (EE), and 145,921 words (VE)—and provides fast operations for searching, adding, updating, and deleting words, with a loading time of approximately 18-40 seconds depending on the system.

This project emphasizes efficient data management, user interaction, and support for Vietnamese special characters using `wstring`, alongside features like randomized word quizzes and favorite word tracking.

## Features
### Core Features
- **Multi-Dataset Support**: Switch between English-Vietnamese, English-English, and Vietnamese-English dictionaries.
- **Word Management**:
  - Search for a word and its definition(s).
  - Add new words and definitions.
  - Edit existing definitions.
  - Delete words from the dictionary.
- **Search Enhancements**:
  - Search by keyword or substring in definitions (using KMP algorithm).
  - Emoji search using a hash table.
- **User Interaction**:
  - View search history with clickable redirection to words.
  - Add/remove words to/from a favorites list and view the list.
  - Reset dictionaries to their original state (individually or all).
- **Randomized Features**:
  - View a random word and its definition.
  - Quiz mode: Guess a word from four definitions or a definition from four words.
- **Example Sentences**: View usage of English words in example sentences (EETrie only).

### Technical Highlights
- **Data Structure**: Trie with vectors of children for efficient prefix-based operations.
- **Time Complexity**: Most operations (search, insert, delete) are O(N), where N is the word length (typically < 20 characters), making them near-instantaneous.
- **Special Handling**: Vietnamese characters supported via `wstring` and hashmap mapping.
- **UI**: Implemented using SFML for a graphical interface.

## Directory Structure
The project organizes its data and source files as follows (based on typical structure inferred from the report):
- **Datasets/**:
  - `EV_dataset.csv`: English-Vietnamese dictionary data (103,868 words).
  - `EE_dataset.csv`: English-English dictionary data (113,477 words).
  - `VE_dataset.csv`: Vietnamese-English dictionary data (145,921 words).
  - `emoji_dataset.csv`: Emoji data (1,816 icons).
- **Storage/**:
  - `search_history.csv`: Stores user search history (word, definition, timestamp).
  - `favorite_words.csv`: Stores user’s favorite words and definitions.
  - `EVTrie.bin`, `EETrie.bin`, `VTrie.bin`: Binary files for saving/loading Trie structures.
