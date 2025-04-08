
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SENSORS 3
#define MAX_VEHICLES 2

// Sensor Types
typedef enum {
    ENGINE_TEMP,
    OIL_PRESSURE,
    BATTERY_VOLTAGE
} SensorType;

// Sensor Structure
typedef struct {
    SensorType type;
    float value;
    char status[20];
} Sensor;

// Vehicle Structure
typedef struct {
    char id[10];
    Sensor sensors[MAX_SENSORS];
} Vehicle;

// Function Prototypes
void initializeVehicle(Vehicle *vehicle, const char *id);
void updateSensorData(Vehicle *vehicle);
void analyzeSensor(Sensor *sensor);
void performDiagnostics(Vehicle *vehicle);
void logAlert(Vehicle *vehicle, const char *message);

// Initialize Vehicle with Default Data
void initializeVehicle(Vehicle *vehicle, const char *id) {
    strcpy(vehicle->id, id);
    vehicle->sensors[ENGINE_TEMP] = (Sensor){ENGINE_TEMP, 90.0, "Normal"};
    vehicle->sensors[OIL_PRESSURE] = (Sensor){OIL_PRESSURE, 40.0, "Normal"};
    vehicle->sensors[BATTERY_VOLTAGE] = (Sensor){BATTERY_VOLTAGE, 12.6, "Normal"};
}

// Update Sensor Data (Simulation)
void updateSensorData(Vehicle *vehicle) {
    vehicle->sensors[ENGINE_TEMP].value = 80 + rand() % 40;         // 80 to 120
    vehicle->sensors[OIL_PRESSURE].value = 20 + rand() % 40;        // 20 to 60
    vehicle->sensors[BATTERY_VOLTAGE].value = 11 + ((rand() % 30) / 10.0); // 11.0 to 13.9
}

// Analyze One Sensor
void analyzeSensor(Sensor *sensor) {
    switch (sensor->type) {
        case ENGINE_TEMP:
            if (sensor->value > 110)
                strcpy(sensor->status, "Overheat");
            else
                strcpy(sensor->status, "Normal");
            break;

        case OIL_PRESSURE:
            if (sensor->value < 25)
                strcpy(sensor->status, "Low Pressure");
            else
                strcpy(sensor->status, "Normal");
            break;

        case BATTERY_VOLTAGE:
            if (sensor->value < 11.5)
                strcpy(sensor->status, "Low Battery");
            else
                strcpy(sensor->status, "Normal");
            break;
    }
}

// Diagnostics Function (uses analyzeSensor inside)
void performDiagnostics(Vehicle *vehicle) {
    printf("\nDiagnostics Report for Vehicle %s:\n", vehicle->id);
    for (int i = 0; i < MAX_SENSORS; i++) {
        analyzeSensor(&vehicle->sensors[i]);

        printf("Sensor %d | Value: %.2f | Status: %s\n", 
                vehicle->sensors[i].type,
                vehicle->sensors[i].value,
                vehicle->sensors[i].status);

        if (strcmp(vehicle->sensors[i].status, "Normal") != 0) {
            logAlert(vehicle, vehicle->sensors[i].status);
        }
    }
}

// Log Alert
void logAlert(Vehicle *vehicle, const char *message) {
    time_t now;
    time(&now);
    printf("ALERT: [%s] Vehicle %s - %s\n", ctime(&now), vehicle->id, message);
}

// Main Function
int main() {
    srand(time(0));
    Vehicle fleet[MAX_VEHICLES];

    initializeVehicle(&fleet[0], "V101");
    initializeVehicle(&fleet[1], "V102");

    for (int i = 0; i < MAX_VEHICLES; i++) {
        updateSensorData(&fleet[i]);
        performDiagnostics(&fleet[i]);
    }

    return 0;
}
