#include <stdio.h>
#include <string.h>

// Define a structure with function pointers for actions
typedef struct {
    int id;
    char name[20];
    void (*action)(int);       // Main action function pointer
    void (*timedAction)(int);  // Timed action function pointer
    int runtimeHours;          // Assigned runtime in hours
} Device;

// Functions for different actions
void turnOn(int id) { printf("Device %d: Turned ON\n", id); }
void turnOff(int id) { printf("Device %d: Turned OFF\n", id); }
void reset(int id) { printf("Device %d: Resetting...\n", id); }
void standby(int id) { printf("Device %d: Entering Standby Mode\n", id); }

// Timed action functions
void runFor10Hours(int id) { printf("Device %d is running for 10 hours.\n", id); }
void runFor20Hours(int id) { printf("Device %d is running for 20 hours.\n", id); }
void runFor30Hours(int id) { printf("Device %d is running for 30 hours.\n", id); }
void runFor40Hours(int id) { printf("Device %d is running for 40 hours.\n", id); }

// Function to update a device's main action
void updateDeviceAction(Device *device, void (*newAction)(int)) {
    if (device) {
        device->action = newAction;
        printf("Device %d (%s) action updated.\n", device->id, device->name);
    }
}

// Function to update a device's timed action
void updateDeviceTimedAction(Device *device, void (*newTimedAction)(int), int runtime) {
    if (device) {
        device->timedAction = newTimedAction;
        device->runtimeHours = runtime;
        printf("Device %d (%s) timed action updated to %d hours.\n", device->id, device->name, runtime);
    }
}

// Function to update device ID
void updateDeviceID(Device *device) {
    if (device) {
        int newID;
        printf("Current ID of %s is %d.\n", device->name, device->id);
        printf("Enter a new ID for %s (or enter -1 to keep the old ID): ", device->name);
        scanf("%d", &newID);

        if (newID != -1) {
            printf("Device ID changed from %d to %d.\n", device->id, newID);
            device->id = newID;
        } else {
            printf("Device ID remains unchanged.\n");
        }
    }
}

// Function to find a device by ID
Device* findDeviceByID(Device devices[], int numDevices, int id) {
    for (int i = 0; i < numDevices; i++) {
        if (devices[i].id == id) {
            return &devices[i];
        }
    }
    return NULL;
}

// Function to display the list of devices
void displayDevices(Device devices[], int numDevices) {
    printf("\n---- Device List ----\n");
    for (int i = 0; i < numDevices; i++) {
        printf("Device ID: %d, Name: %s, Runtime: %d hours -> ", 
               devices[i].id, devices[i].name, devices[i].runtimeHours);
        devices[i].action(devices[i].id);
        devices[i].timedAction(devices[i].id);
    }
}

int main() {
    // Initialize devices with function pointers
    Device devices[] = {
        {101, "Fan", turnOn, runFor10Hours, 10},
        {102, "Light", turnOff, runFor20Hours, 20},
        {103, "Thermostat", reset, runFor30Hours, 30},
        {104, "Router", standby, runFor40Hours, 40},
        {105, "TV", turnOn, runFor10Hours, 10}
    };

    int numDevices = sizeof(devices) / sizeof(devices[0]);

    // Display initial device states
    displayDevices(devices, numDevices);

    // Update TV ID
    updateDeviceID(&devices[4]); // TV is at index 4

    // Allow user to change device actions and timed actions
    int deviceChoice;
    printf("\nSelect a device to change its action and runtime (Enter ID): ");
    scanf("%d", &deviceChoice);

    Device *selectedDevice = findDeviceByID(devices, numDevices, deviceChoice);

    if (selectedDevice) {
        int actionChoice, timeChoice;

        // Update main action
        printf("\nSelect new **main action** for %s (ID %d):\n", selectedDevice->name, selectedDevice->id);
        printf("1. Turn On\n2. Turn Off\n3. Reset\n4. Standby\nEnter your choice: ");
        scanf("%d", &actionChoice);

        switch (actionChoice) {
            case 1: updateDeviceAction(selectedDevice, turnOn); break;
            case 2: updateDeviceAction(selectedDevice, turnOff); break;
            case 3: updateDeviceAction(selectedDevice, reset); break;
            case 4: updateDeviceAction(selectedDevice, standby); break;
            default: printf("Invalid action choice!\n");
        }

        // Update timed action
        printf("\nSelect new **timed action** for %s (ID %d):\n", selectedDevice->name, selectedDevice->id);
        printf("1. Run for 10 hours\n2. Run for 20 hours\n3. Run for 30 hours\n4. Run for 40 hours\nEnter your choice: ");
        scanf("%d", &timeChoice);

        switch (timeChoice) {
            case 1: updateDeviceTimedAction(selectedDevice, runFor10Hours, 10); break;
            case 2: updateDeviceTimedAction(selectedDevice, runFor20Hours, 20); break;
            case 3: updateDeviceTimedAction(selectedDevice, runFor30Hours, 30); break;
            case 4: updateDeviceTimedAction(selectedDevice, runFor40Hours, 40); break;
            default: printf("Invalid time choice!\n");
        }
    } else {
        printf("Device with ID %d not found!\n", deviceChoice);
    }

    // Display updated device states
    displayDevices(devices, numDevices);

    return 0;
}
