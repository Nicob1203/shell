# Shell Program with Plugin Support

This project implements a custom shell (`msh.c`) with support for dynamically loading and executing plugins. The shell allows users to run shell commands and extend functionality through plugins.

## Features

- Custom shell interface
- Support for loading and running plugins at runtime
- Example task scheduler plugin included

## Project Files

- **msh.c**: The main shell program that initializes and runs plugins, that can also execute regular shell commands.
- **testplugin.c**: An example plugin that just confirms the plugin was initialized and functions after use in shell. 
- **taskScheduler.c**: Contains the example plugin functionality which just initiates a few scheduled tasks based on time.

## Usage

### Compiling the Program

1. **Compile the shell program**:
    ```bash
    gcc -o msh msh.c
    ```

2. **Compile a plugin** (example: `testplugin.c`):
    ```bash
    gcc -fPIC -shared -o testplugin.so testplugin.c
    ```

### Running the Shell

1. Run the `msh` shell program:
    ```bash
    ./msh
    ```

2. From within the shell, you can load plugins and execute commands. This shell supports dynamic plugin loading, which enables users to extend the shell's functionality at runtime.

### Loading and Running Plugins

Once the shell is running, you can load plugins to enhance its capabilities. For example, to load the compiled `testplugin.so`, use the following command:

1. In the shell, type:
    ```
    load testplugin
    ```

   This will dynamically load the plugin and apply its functionality to the shell.
2. Once the plugin is initialized, type:
    ```
    testplugin
    ```
   This will run the loaded plugin.

### Example Plugin (`testplugin.c`)

The `testplugin.c` file demonstrates how to create a basic plugin for the shell. It serves as a template for developing additional plugins. By following the structure and conventions in this example, you can create your own plugins that interact with the shell and provide new functionality.

## Creating Your Own Plugins

To develop your own plugin:

1. Create a `.c` file that follows the structure of `testplugin.c`.
2. Compile it into a shared object (`.so`) file using the provided compilation commands.
3. Load it into the shell using the `load <plugin_name>` command.
