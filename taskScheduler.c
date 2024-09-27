#include <stdio.h>
#include <time.h>
#include <string.h>

#define MAX_TASKS 10

typedef struct {
    char description[100];
    time_t scheduled_time;
} Task;

// Array of tasks
Task tasks[MAX_TASKS];
int taskCount = 0;

//Starts the plugin and adds some example tasks
int initialize() {
    printf("Task Scheduler Plugin Initialized.\n");

    // Adding sample tasks
    if (taskCount < MAX_TASKS) {
        Task newTask;
        strcpy(newTask.description, "Complete programming assignment");
        newTask.scheduled_time = time(NULL) + 3600; // 1 hour from now
        tasks[taskCount++] = newTask;
    }

    if (taskCount < MAX_TASKS) {
        Task newTask;
        strcpy(newTask.description, "Study for debate");
        newTask.scheduled_time = time(NULL) + 7200; // 2 hours from now
        tasks[taskCount++] = newTask;
    }

    printf("%d tasks scheduled.\n", taskCount);
    return 0;
}

int run() {
    printf("Checking for due tasks...\n");

    time_t currentTime = time(NULL);

    for (int i = 0; i < taskCount; i++) {
        if (difftime(tasks[i].scheduled_time, currentTime) <= 0) {
            printf("Task due: %s\n", tasks[i].description);
        } else {
            printf("Task scheduled: %s (in %ld seconds)\n", tasks[i].description, (long)difftime(tasks[i].scheduled_time, currentTime));
        }
    }
    return 0;
}

int cleanup() {
    printf("Task Scheduler Plugin Cleaned Up.\n");
    return 0;
}