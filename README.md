# TGE: Text Game Engine

TGE is a minimalist, high-performance engine for text-based RPGs written in pure C. It utilizes lexical analysis and symbol table concepts to bridge the gap between human language and structured game logic.

## Motivation

I like to write small horror stories on my free time. But since I'm too shy to publish them, I thought about creating a minimalist Text RPG Engine, inspired on Zork, to create small games with my stories and maybe publish them.

**PS: I created this project without AI. Used AI just to generate this cool Readme.** 

## Project Status and Future Improvements

This engine was developed as a short-term hobby project over the course of a few days. While it is fully functional for small-scale horror narratives, it was built at a personal pace with an emphasis on simplicity over exhaustive feature sets.

As an engineering exercise, I recognize several areas where the engine could be evolved to reach "production-grade" stability:

**PS: I know the demo game have broken use of pronouns when you choose Option 1, and I plan to fix in the future. I designed the demo thinking in my character that uses Option 2, and this was a big oversight from my part. Sorry!**

### 1. Shift from Global to Contextual State

Currently, the engine relies on global variables for the player and command structures. A more robust implementation would involve passing a `GameContext` pointer through functions to support reentrancy and potentially multiple simultaneous game instances.

### 2. From Keyword Matching to Formal Grammar

The current parser acts as a sophisticated keyword matcher. Moving toward a formal Lexer/Parser architecture with token sequencing would allow for more complex command strings (e.g., "take the key then unlock the door") and reduce ambiguity in natural language processing.

### 3. Dynamic Memory for Inventories

The current inventory system uses fixed-size buffers for simplicity. Transitioning to a linked-list or dynamic array approach for items would optimize memory usage and remove the current 16-character limit on item names.

### 4. Serialization for Save/Load Systems

Because room states are managed via `static` variables within individual `.c` files, they are currently difficult to serialize. Moving room-specific state into a unified, heap-allocated struct would be the first step toward implementing a persistent Save/Load feature.

### 5. Enhanced Error Handling

While the engine uses `errno` and basic null checks, a more comprehensive safety layer would include defensive checks across all library boundaries to prevent segmentation faults during malformed initialization or unexpected user input.

## Sensitive Content Disclaimer

The included demo game, **Damp Cave**, contains dark themes, descriptive horror, and references to suicide. It is intended for mature audiences. This content reflects the developer's interest in the horror genre and is used to demonstrate the engine's ability to handle atmospheric, reactive storytelling.

## How the Engine Works

The engine is built on a "Simple-First" architecture that treats the game world as a collection of isolated state machines.

### 1. Lexical Command Parsing

Instead of simple string comparison, TGE processes input into a `tge_command` structure. The engine compares user input against a provided context of verbs, nouns, and prepositions to extract intent. This allows for complex actions like `USE [item] ON [object]` without manual string splitting in every room.

### 2. Functional Decoupling

The core engine (`tge.c`) provides the "physics" of the game: memory management, item transfers, and string utilities. The game logic is injected into the engine via function pointers within the `tge_room` struct:

* `description()`: A function that prints the current state of the room.
* `func()`: A logic handler that receives a parsed command and decides the outcome.

### 3. State-Driven Narrative

By defining rooms in individual `.c` files, developers can use static variables to track local states (e.g., if a light is on, if a key has been taken). This keeps the global namespace clean while allowing the world to react dynamically to player choices.

## Demo Game

The whole codebase of `Damp Cave` (the demo I created) is inside the folder `/demo`. You can use it as an example on how to build with this engine.

## Building a Game with TGE

To create a new game using the engine, follow these steps:

1. **Define the Vocabulary:** In a central command file (like `commands.c`), populate the `tge_command_special_words` struct with the verbs and nouns your game understands.
2. **Implement Rooms:** Create a `.c` file for each location. Define a `setup` function to link the room's description and action functions.
3. **Initialize the Player:** Use `tge_malloc()` to allocate the global player state and set up initial attributes like name and pronouns.
4. **Game Loop:** In your `main.c`, use a `while` loop to call `run_action()`, passing the user's input and the current room's function pointer.

## Compilation and Multi-Platform Support

The project includes a multi-target Makefile designed for cross-compilation. This allows you to generate binaries for various operating systems and architectures from a single host.

### Build Targets

The Makefile supports the following variables:

* `PLATFORM`: `linux` (default), `windows`, or `mac`.
* `ARCH`: `x86_64` (default) or `arm64`.

### Examples

To build for the local machine:

```bash
make

```

To cross-compile for Windows (requires `mingw-w64`):

```bash
make PLATFORM=windows

```

To build for macOS on ARM64 (Apple Silicon):

```bash
make PLATFORM=mac ARCH=arm64

```

### Output

The build process generates a shared library (`.so`, `.dll`, or `.dylib`) and a demo executable within a platform-specific subdirectory in the `build/` folder. This keeps object files and binaries isolated by architecture.

## License

Distributed under the GNU General Public License v3.0 or later. See [COPYING](https://github.com/ariasdiniz/text_game_engine/blob/main/COPYING) for more information.