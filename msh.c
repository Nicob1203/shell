#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct Plugin {                                                             //Plugin Struct that holds its name, the address to initialize and run and then the handle for closing later
    char plugName[21];
    int (*init)();
    int (*run)();
    void *handle;
} plugin;

plugin pluginLoader(char *pluginPath){                                            //Function to open the plugin, takes the plugins path
    plugin plug;
    plug.handle = dlopen(pluginPath, RTLD_LAZY);                      //Opens the plugin, lazily during runtime
    if(!plug.handle){                                                             //Invalid handle just gets returned
        return plug;
    }
    dlerror();                                                                   //Clearing any previous errors
    plug.init = dlsym(plug.handle, "initialize");                         //Requesting a Symbol
    char * error = dlerror();
    if(error!=NULL){                                                             //Checking if any error occured
        printf("ERROR: Failed to retrieve symbol!\n");
        exit(0);
    }
    dlerror();
    plug.run = dlsym(plug.handle, "run");
    error = dlerror();
    if(error!=NULL){
        printf("ERROR: Failed to retrieve symbol!\n");
        exit(0);
    }
    return plug;
}

int main(int argc, char ** argv){
    char command[201] = "";                                                     //Char array of 201 to store the max length 200 command with the null terminator
    plugin loadedPlugs[10];                                                     //Array of plugin structs to store the currently loaded ones
    char *firstComm = "";                                                       //Char pointer to the first command which is just the first word in the command
    int numPlugs = 0;                                                           //This saves the number of plugins loaded
    do{
        int numArgs = 0;                                                        //Tracks the number of arguments for each command
        printf("> ");
        fgets(command, 200, stdin);
        command[strcspn(command, "\n")] = 0;                         //Stripping the command of newline characters
        char *args[21];                                                         //Array of pointer to strings to store the arguments, 20 arguments plus 1 for the NULL needed for the run command and exec
        char* comm = strtok(command, " ");                             //Seperates the first command by spaces
        firstComm = comm;
        while (comm != NULL) {                                                  //Iterates through each word by the spaces until NULL
            args[numArgs] = comm;                                               //Puts each arg into the array
            comm = strtok(NULL, " ");                                  //Gets next arg
            numArgs++;                                                          //Keeps track of numArgs
        }
        args[numArgs] = NULL;                                                   //Makes sure its null terminated
        numArgs--;                                                              //Subtracts 1 so the null terminator isnt counted as an argument
        bool alrLoaded = false;                                                 //Boolean to save whether a plugin has already been loaded
        for(int x = 0; x<numPlugs; x++)
        {
            if(strcmp(args[0], loadedPlugs[x].plugName)==0){                    //Iterates through each loaded plugin and checks if it got called
                alrLoaded = true;
                break;
            }
            else if(numArgs>0 && strcmp(args[1], loadedPlugs[x].plugName)==0)   //Checks to see if this plugin has already been loaded
            {
                printf("Error: Plugin %s initialization failed!\n", args[1]);
                alrLoaded = true;
                break;
            }
        }
        if(strcmp(firstComm, "exit")==0)                                        //If exit just continues which will break the loop
            continue;
        else if(strcmp(firstComm, "load")==0 && numArgs==0)                     //Ensures load has arguments after
        {
            printf("Error: Plugin %s initialization failed!\n", args[1]);

        }
        else if(strcmp(firstComm, "load") == 0)
        {
            if(!alrLoaded){                                                     //Makes sure it hasn't been loaded already
                char plugName[26] = "./";
                strcat(plugName, args[1]);
                strcat(plugName, ".so");                                        //Formats the plugin name with the path requirements
                plugin plug = pluginLoader(plugName);
                strncpy(plug.plugName, args[1], 20);                            //Copies the name of the plugin into the struct
                if(!plug.handle || plug.init()!=0)                              //If it failed to open or failed to initialize prints the required message
                {
                    printf("Error: Plugin %s initialization failed!\n", args[1]);
                }
                else{                                                           //Otherwise puts the new plugin into the struct and increments numPlugs
                    loadedPlugs[numPlugs] = plug;
                    numPlugs++;
                }
            }
        }
        else if(alrLoaded){                                                     //If the plugins already been loaded
            for(int x = 0; x<numPlugs; x++)
            {
                if(strcmp(firstComm, loadedPlugs[x].plugName)==0){              //Finds the plugin it matches and calls run with all arguments
                    loadedPlugs[x].run(args);
                }
            }}
        else{                                                                   //Otherwise assumes its an exec command
            pid_t pid = fork();
            if (pid == 0) {
                execvp(firstComm, args);                              //Runs the command then it needed to exit because otherwise I kept getting odd errors where variables would be changed
                exit(0);
            }
            else
                wait(NULL);
        }
    }while(strcmp(firstComm, "exit"));
    for(int x = 0; x<numPlugs; x++)
    {
        dlclose(loadedPlugs[x].handle);                                         //Makes sure to close all the open plugins
    }
    return 0;
}

