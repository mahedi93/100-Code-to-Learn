#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Sensor Types
typedef enum {
    TEMP_SENSOR,
    VOLTAGE_SENSOR,
    BRAKE_SENSOR,
    TIRE_SENSOR,
    DISTANCE_SENSOR
} SensorType;

// Sensor Data
typedef struct {
    SensorType type;
    float value;
    char status[20];
} Sensor;

// Diagnostic Info
typedef struct {
    float avgTemp;
    float minVoltage;
    int criticalSensors;
} DiagnosticInfo;

// Vehicle Info
typedef struct {
    char id[10];
    Sensor sensors[5];
    int healthStatus;
} Vehicle;

// Safety Module
typedef struct {
    int obstacleAlert;
    int brakeFailure;
    int tirePressureIssue;
} SafetyModule;

// Prototypes
void initializeVehicle(Vehicle *v, const char *id);
void simulateSensorInput(Vehicle *v);
void analyzeSensors(Vehicle *v, DiagnosticInfo *diag);
void checkSafety(Vehicle *v, SafetyModule *safety);
void logHealthStatus(Vehicle *v, DiagnosticInfo *diag, SafetyModule *safety);
void updateSensorStatus(Sensor *s, float value, const char *okMessage, const char *warnMessage, float threshold, int *criticalCount);

// Implementation

void initializeVehicle(Vehicle *v, const char *id) {
    strcpy(v->id, id);
    for (int i = 0; i < 5; i++) {
        v->sensors[i].type = i;
        v->sensors[i].value = 0;
        strcpy(v->sensors[i].status, "Unknown");
    }
    v->healthStatus = 1;
}

void simulateSensorInput(Vehicle *v) {
    v->sensors[TEMP_SENSOR].value = 75 + rand() % 50;      // 75-125
    v->sensors[VOLTAGE_SENSOR].value = 11.5 + (rand() % 20) / 10.0;  // 11.5–13.5
    v->sensors[BRAKE_SENSOR].value = rand() % 2;
    v->sensors[TIRE_SENSOR].value = 28 + rand() % 10;      // 28–38
    v->sensors[DISTANCE_SENSOR].value = rand() % 100;      // 0–100 cm
}

void analyzeSensors(Vehicle *v, DiagnosticInfo *diag) {
    diag->avgTemp = 0;
    diag->minVoltage = 99;
    diag->criticalSensors = 0;

    updateSensorStatus(&v->sensors[TEMP_SENSOR], v->sensors[TEMP_SENSOR].value, "OK", "OVERHEAT", 100, &diag->criticalSensors);
    diag->avgTemp = v->sensors[TEMP_SENSOR].value;

    updateSensorStatus(&v->sensors[VOLTAGE_SENSOR], v->sensors[VOLTAGE_SENSOR].value, "OK", "LOW VOLTAGE", 12.0, &diag->criticalSensors);
    diag->minVoltage = v->sensors[VOLTAGE_SENSOR].value;

    updateSensorStatus(&v->sensors[B-RAKE_SENSOR], v->sensors[BRAKE_SENSOR].value, "OK", "BRAKE FAIL", 1, &diag->criticalSensors);
    updateSensorStatus(&v->sensors[TIRE_SENSOR], v->sensors[TIRE_SENSOR].value, "OK", "LOW PRESSURE", 30, &diag->criticalSensors);
}

void updateSensorStatus(Sensor *s, float value, const char *okMessage, const char *warnMessage, float threshold, int *criticalCount) {
    if (value < threshold) {
        strcpy(s->status, warnMessage);
        (*criticalCount)++;
    } else {
        strcpy(s->status, okMessage);
    }
}

void checkSafety(Vehicle *v, SafetyModule *safety) {
    safety->obstacleAlert = (v->sensors[DISTANCE_SENSOR].value < 20);
    safety->brakeFailure = strcmp(v->sensors[BRAKE_SENSOR].status, "BRAKE FAIL") == 0;
    safety->tirePressureIssue = strcmp(v->sensors[TIRE_SENSOR].status, "LOW PRESSURE") == 0;
}

void logHealthStatus(Vehicle *v, DiagnosticInfo *diag, SafetyModule *safety) {
    time_t now;
    time(&now);

    printf("\n--- Vehicle ID: %s ---\n", v->id);
    printf("Time: %s", ctime(&now));
    printf("Temp: %.1f (%s)\n", v->sensors[TEMP_SENSOR].value, v->sensors[TEMP_SENSOR].status);
    printf("Voltage: %.1f (%s)\n", v->sensors[VOLTAGE_SENSOR].value, v->sensors[VOLTAGE_SENSOR].status);
    printf("Brake: %s\n", v->sensors[BRAKE_SENSOR].status);
    printf("Tire Pressure: %.1f (%s)\n", v->sensors[TIRE_SENSOR].value, v->sensors[TIRE_SENSOR].status);
    printf("Distance: %.1fcm\n", v->sensors[DISTANCE_SENSOR].value);

    if (safety->obstacleAlert)
        printf("!!! ALERT: Obstacle too close\n");
    if (safety->brakeFailure)
        printf("!!! ALERT: Brake Failure\n");
    if (safety->tirePressureIssue)
        printf("!!! ALERT: Tire Pressure Issue\n");

    printf("Health Score: %s\n", diag->criticalSensors > 0 ? "UNSAFE" : "SAFE");
    printf("-------------------------\n");
}

// Main
int main() {
    srand(time(NULL));
    Vehicle v;
    DiagnosticInfo diag;
    SafetyModule safety;

    initializeVehicle(&v, "VH700");

   while(1){
    for (int cycle = 0; cycle < 5; cycle++) {
        simulateSensorInput(&v);
        analyzeSensors(&v, &diag);
        checkSafety(&v, &safety);
        logHealthStatus(&v, &diag, &safety);
        sleep(2);
    }
    }

    return 0;
}
