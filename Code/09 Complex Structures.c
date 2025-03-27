#include <stdio.h>
#include <string.h>

#define MAX_VEHICLES 10
#define MAX_SENSORS 5

// Structure for vehicle sensors
typedef struct {
    char type[20];
    float value;
} Sensor;

// Structure for vehicle information
typedef struct {
    char make[50];
    char model[50];
    int year;
    float engine_capacity;
    int mileage;
    Sensor sensors[MAX_SENSORS];
    int sensor_count;
} Vehicle;

// Fleet management structure
typedef struct {
    Vehicle vehicles[MAX_VEHICLES];
    int count;
} VehicleFleet;

// Function prototypes
Vehicle createVehicle(char *make, char *model, int year, float engine_capacity, int mileage);
void addVehicle(VehicleFleet *fleet, Vehicle vehicle);
void addSensor(Vehicle *vehicle, char *type, float value);
void printVehicle(Vehicle vehicle);
void displayFleet(VehicleFleet *fleet);
int findVehicleIndex(VehicleFleet *fleet, char *make, char *model);
int removeVehicle(VehicleFleet *fleet, char *make, char *model);
void updateMileage(Vehicle *vehicle, int mileage);
void displaySensorData(Vehicle *vehicle);

int main() {
    VehicleFleet fleet = {.count = 0};

    // Creating and adding vehicles
    Vehicle v1 = createVehicle("Toyota", "Camry", 2021, 2.5, 15000);
    Vehicle v2 = createVehicle("Ford", "Mustang", 2020, 5.0, 22000);
    addVehicle(&fleet, v1);
    addVehicle(&fleet, v2);

    // Adding sensors to vehicles
    addSensor(&fleet.vehicles[1], "Temperature", 85.2);
    addSensor(&fleet.vehicles[0], "Oil Pressure", 30.5);
    addSensor(&fleet.vehicles[1], "Brake Fluid", 12.7);

    // Displaying fleet
    printf("Fleet of Vehicles:\n");
    displayFleet(&fleet);

    // Displaying sensor data

    for(int j=0; j<MAX_VEHICLES; j++){
    printf("\nSensor Data for %s %s:\n", fleet.vehicles[j].make, fleet.vehicles[j].model);
    displaySensorData(&fleet.vehicles[j]);

    }


    // Updating mileage
    updateMileage(&fleet.vehicles[0], 18000);
    printf("\nUpdated Mileage for %s %s: %d km\n", fleet.vehicles[0].make, fleet.vehicles[0].model, fleet.vehicles[0].mileage);

    return 0;
}

Vehicle createVehicle(char *make, char *model, int year, float engine_capacity, int mileage) {
    Vehicle vehicle;
    strcpy(vehicle.make, make);
    strcpy(vehicle.model, model);
    vehicle.year = year;
    vehicle.engine_capacity = engine_capacity;
    vehicle.mileage = mileage;
    vehicle.sensor_count = 0;
    return vehicle;
}

void addVehicle(VehicleFleet *fleet, Vehicle vehicle) {
    if (fleet->count < MAX_VEHICLES) {
        fleet->vehicles[fleet->count++] = vehicle;
    } else {
        printf("Error: Fleet is full, cannot add vehicle\n");
    }
}

void addSensor(Vehicle *vehicle, char *type, float value) {
    if (vehicle->sensor_count < MAX_SENSORS) {
        strcpy(vehicle->sensors[vehicle->sensor_count].type, type);
        vehicle->sensors[vehicle->sensor_count].value = value;
        vehicle->sensor_count++;
    } else {
        printf("Error: Sensor limit reached for vehicle %s %s\n", vehicle->make, vehicle->model);
    }
}

void printVehicle(Vehicle vehicle) {
    printf("Make: %s\n", vehicle.make);
    printf("Model: %s\n", vehicle.model);
    printf("Year: %d\n", vehicle.year);
    printf("Engine Capacity: %.2fL\n", vehicle.engine_capacity);
    printf("Mileage: %d km\n", vehicle.mileage);
}

void displayFleet(VehicleFleet *fleet) {
    for (int i = 0; i < fleet->count; i++) {
        printVehicle(fleet->vehicles[i]);
        printf("----------------------\n");
    }
}

void displaySensorData(Vehicle *vehicle) {
    for (int i = 0; i < vehicle->sensor_count; i++) {
        printf("%s: %.2f\n", vehicle->sensors[i].type, vehicle->sensors[i].value);
    }
}

void updateMileage(Vehicle *vehicle, int mileage) {
    vehicle->mileage = mileage;
}
