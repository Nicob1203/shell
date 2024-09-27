#include <stdio.h>

int initialize() {
    printf("Initializing test plugin\n");
    return 0; // Return 0 to indicate successful initialization
}

int run() {
    printf("Running test plugin\n");
    return 0; // Return 0 to indicate successful run
}

int cleanup() {
    printf("Cleaning up test plugin\n");
    return 0; // Return 0 to indicate successful cleanup
}
