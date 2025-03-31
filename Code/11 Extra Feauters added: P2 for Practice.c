#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>   // For sleep()
#include <stdlib.h>   // For rand()
#include <time.h>     // For timestamp in logging

#define MAX_VEHICLES 5
#define MAX_SENSORS 5

// Sensor Types
typedef enum {
    TEMPERATURE_SENSOR,
    SPEED_SENSOR,
    FUEL_SENSOR,
    BRAKE_PRESSURE_SENSOR,
    BATTERY_VOLTAGE_SENSOR
} SensorType;

// Sensor Data Structure
typedef struct {
    SensorType type;
    uint16_t value;
    uint8_t is_faulty; // 0 = Normal, 1 = Faulty
} SensorData;

// Vehicle Structure
typedef struct {
    char id[10];
    SensorData sensors[MAX_SENSORS];
    uint8_t sensor_count;
    uint8_t status;
} Vehicle;

// Fleet Management
typedef struct {
    Vehicle vehicles[MAX_VEHICLES];
    uint8_t count;
} FleetManager;

// Global Fleet Object
FleetManager fleet;

// Function Prototypes
void addVehicle(char *id, uint8_t status);
void assignRandomSensors(Vehicle *vehicle);
void updateSensorData(Vehicle *vehicle);
void processFleet();
void processTemperature(SensorData *sensor);
void processSpeed(SensorData *sensor);
void processFuel(SensorData *sensor);
void processBrakePressure(SensorData *sensor);
void processBatteryVoltage(SensorData *sensor);
void logSensorData(Vehicle *vehicle);
void checkSensorFault(SensorData *sensor);
void sendToCANBus(SensorData *sensor);

// Function Pointer for Sensor Processing
typedef void (*SensorProcessor)(SensorData *);

// Function Pointer Array for Sensor Processing
SensorProcessor sensorHandlers[] = {
    processTemperature,
    processSpeed,
    processFuel,
    processBrakePressure,
    processBatteryVoltage
};

// Function to Add a Vehicle
void addVehicle(char *id, uint8_t status) {
    if (fleet.count < MAX_VEHICLES) {
        strcpy(fleet.vehicles[fleet.count].id, id);
        fleet.vehicles[fleet.count].status = status;
        fleet.vehicles[fleet.count].sensor_count = 0; // Initialize with 0 sensors
        fleet.count++;
    }
}

// Assign Random Sensors Dynamically
void assignRandomSensors(Vehicle *vehicle) {
    int num_sensors = (rand() % MAX_SENSORS) + 1; // 1 to MAX_SENSORS
    vehicle->sensor_count = num_sensors;

    for (int i = 0; i < num_sensors; i++) {
        vehicle->sensors[i].type = (SensorType)(rand() % MAX_SENSORS); // Random sensor type
        vehicle->sensors[i].value = (rand() % 50) + 10; // Random sensor values between 10-59
        vehicle->sensors[i].is_faulty = 0; // No faults initially
    }
}

// Update Sensor Data in Real Time

// Update Sensor Data in Real Time and Randomly Add More Sensors
void updateSensorData(Vehicle *vehicle) {
    for (int i = 0; i < vehicle->sensor_count; i++) {
        int change = (rand() % 5) - 2; // Random fluctuation between -2 and +2
        vehicle->sensors[i].value += change;
        checkSensorFault(&vehicle->sensors[i]); // Check for faults
    }

    // Randomly add a new sensor if vehicle has space
    if (vehicle->sensor_count < MAX_SENSORS) {
        int add_new_sensor = rand() % 10; // 30% chance to add a new sensor
        if (add_new_sensor < 3) {
            int new_index = vehicle->sensor_count;
            vehicle->sensors[new_index].type = (SensorType)(rand() % MAX_SENSORS); // Random sensor type
            vehicle->sensors[new_index].value = (rand() % 50) + 10; // Assign random value
            vehicle->sensors[new_index].is_faulty = 0; // Initially not faulty
            vehicle->sensor_count++;
            printf("🚗 Vehicle %s added new sensor type %d!\n", vehicle->id, vehicle->sensors[new_index].type);
        }
    }
}


// Check if Sensor is Faulty
void checkSensorFault(SensorData *sensor) {
    switch (sensor->type) {
        case TEMPERATURE_SENSOR:
            sensor->is_faulty = (sensor->value > 90) ? 1 : 0;
            break;
        case SPEED_SENSOR:
            sensor->is_faulty = (sensor->value > 180) ? 1 : 0;
            break;
        case FUEL_SENSOR:
            sensor->is_faulty = (sensor->value < 10) ? 1 : 0;
            break;
        case BRAKE_PRESSURE_SENSOR:
            sensor->is_faulty = (sensor->value < 20) ? 1 : 0;
            break;
        case BATTERY_VOLTAGE_SENSOR:
            sensor->is_faulty = (sensor->value < 11) ? 1 : 0;
            break;
    }
}

// Process Temperature Sensor
void processTemperature(SensorData *sensor) {
    printf("Temperature: %d°C %s\n", sensor->value, sensor->is_faulty ? "[FAULTY]" : "");
}

// Process Speed Sensor
void processSpeed(SensorData *sensor) {
    printf("Speed: %d km/h %s\n", sensor->value, sensor->is_faulty ? "[FAULTY]" : "");
}

// Process Fuel Sensor
void processFuel(SensorData *sensor) {
    printf("Fuel Level: %d%% %s\n", sensor->value, sensor->is_faulty ? "[FAULTY]" : "");
}

// Process Brake Pressure Sensor
void processBrakePressure(SensorData *sensor) {
    printf("Brake Pressure: %d bar %s\n", sensor->value, sensor->is_faulty ? "[FAULTY]" : "");
}

// Process Battery Voltage Sensor
void processBatteryVoltage(SensorData *sensor) {
    printf("Battery Voltage: %dV %s\n", sensor->value, sensor->is_faulty ? "[FAULTY]" : "");
}

// Log Sensor Data to File
void logSensorData(Vehicle *vehicle) {
    FILE *logFile = fopen("sensor_log.txt", "a");
    if (logFile == NULL) return;

    time_t now;
    time(&now);
    fprintf(logFile, "\n[%s] Vehicle ID: %s\n", ctime(&now), vehicle->id);

    for (int i = 0; i < vehicle->sensor_count; i++) {
        fprintf(logFile, "Sensor Type: %d, Value: %d %s\n",
                vehicle->sensors[i].type,
                vehicle->sensors[i].value,
                vehicle->sensors[i].is_faulty ? "[FAULTY]" : "");
    }
    fclose(logFile);
}

// Simulated CAN Bus Transmission
void sendToCANBus(SensorData *sensor) {
    printf("Sending Sensor Type %d with Value %d to CAN Bus...\n", sensor->type, sensor->value);
}

// Process the Entire Fleet
void processFleet() {
    for (int i = 0; i < fleet.count; i++) {
        printf("\nVehicle ID: %s\n", fleet.vehicles[i].id);

        // Update sensor data dynamically
        updateSensorData(&fleet.vehicles[i]);

        // Process each sensor
        for (int j = 0; j < fleet.vehicles[i].sensor_count; j++) {
            sensorHandlers[fleet.vehicles[i].sensors[j].type](&fleet.vehicles[i].sensors[j]);
            sendToCANBus(&fleet.vehicles[i].sensors[j]); // Send data to CAN
        }

        logSensorData(&fleet.vehicles[i]); // Log to file
    }
}

int main() {
    fleet.count = 0;
    srand(time(0)); // Seed random values

    // Adding vehicles
    addVehicle("VH123", 1);
    assignRandomSensors(&fleet.vehicles[0]);
    addVehicle("VH456", 1);
    assignRandomSensors(&fleet.vehicles[1]);

    // Real-Time Simulation
    while (1) {
        processFleet();
        sleep(3); // Wait 3 seconds before updating again
    }

    return 0;
}
