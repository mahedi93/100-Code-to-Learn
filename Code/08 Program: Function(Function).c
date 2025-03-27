#include <stdio.h>
#include <string.h>

// Define a structure for storing vehicle information
typedef struct {
    char make[50];
    char model[50];
    int year;
    float engine_capacity;
    int mileage;
    int seat;
} Vehicle;

// Define a structure for managing a fleet of vehicles
#define MAX_VEHICLES 5  // Set a fixed maximum number of vehicles

typedef struct {
    Vehicle vehicles[MAX_VEHICLES];  // Static array for vehicles
    int count;  // To keep track of the number of vehicles in the fleet
} VehicleFleet;

// Function prototypes
Vehicle createVehicle(Vehicle vehicle);
void addVehicle(VehicleFleet *fleet, Vehicle vehicle);
void printVehicle(Vehicle vehicle);
void displayFleet(VehicleFleet *fleet);
int findVehicleIndex(VehicleFleet *fleet, char *make, char *model);
int removeVehicle(VehicleFleet *fleet, char *make, char *model);

int main() {
    // Initialize fleet
    VehicleFleet fleet = {.count = 0};

    // Array of vehicles
    Vehicle vehicle[] = {
        {"Toyota", "Corolla", 2020, 1.8, 15000, 4},
        {"Ford", "Focus", 2019, 2.0, 20000, 3},
        {"Honda", "Civic", 2021, 1.5, 5000, 2},
    };

    // Adding vehicles to the fleet
    addVehicle(&fleet, createVehicle(vehicle[0]));
    addVehicle(&fleet, createVehicle(vehicle[1]));
    addVehicle(&fleet, createVehicle(vehicle[2]));

    // Display the fleet
    printf("Fleet of Vehicles:\n");
    displayFleet(&fleet);

    // Searching for a vehicle in the fleet
    char searchMake[] = "Ford";
    char searchModel[] = "Focus";

    int index = findVehicleIndex(&fleet, searchMake, searchModel);
    if (index != -1) {
        printf("\nFound vehicle: %s %s\n", searchMake, searchModel);
        printVehicle(fleet.vehicles[index]);
    } else {
        printf("\nVehicle not found: %s %s\n", searchMake, searchModel);
    }

    // Remove a vehicle from the fleet
    if (removeVehicle(&fleet, "Toyota", "Corolla") == 0) {
        printf("\nVehicle removed successfully!\n");
    } else {
        printf("\nError: Vehicle not found for removal.\n");
    }

    // Display the updated fleet
    printf("\nUpdated Fleet of Vehicles:\n");
    displayFleet(&fleet);

    return 0;
}

// Function to create a new vehicle (Adjusted to remove `newVehicle`)
Vehicle createVehicle(Vehicle vehicle) {
    return vehicle;
}

// Function to add a vehicle to the fleet
void addVehicle(VehicleFleet *fleet, Vehicle vehicle) {
    if (fleet->count < MAX_VEHICLES) {
        fleet->vehicles[fleet->count++] = vehicle;
    } else {
        printf("Error: Fleet is full, cannot add vehicle\n");
    }
}

// Function to print details of a single vehicle
void printVehicle(Vehicle vehicle) {
    printf("Make: %s\n", vehicle.make);
    printf("Model: %s\n", vehicle.model);
    printf("Year: %d\n", vehicle.year);
    printf("Engine Capacity: %.2fL\n", vehicle.engine_capacity);
    printf("Mileage: %d km\n", vehicle.mileage);
    printf("Seat: %d \n", vehicle.seat);
}

// Function to display the entire fleet
void displayFleet(VehicleFleet *fleet) {
    for (int i = 0; i < fleet->count; i++) {
        printVehicle(fleet->vehicles[i]);
        printf("----------------------\n");
    }
}

// Function to search for a vehicle in the fleet by make and model
int findVehicleIndex(VehicleFleet *fleet, char *make, char *model) {
    for (int i = 0; i < fleet->count; i++) {
        if (strcmp(fleet->vehicles[i].make, make) == 0 && strcmp(fleet->vehicles[i].model, model) == 0) {
            return i; // Return the index of the found vehicle
        }
    }
    return -1; // Return -1 if not found
}

// Function to remove a vehicle from the fleet
int removeVehicle(VehicleFleet *fleet, char *make, char *model) {
    int index = findVehicleIndex(fleet, make, model);
    if (index == -1) {
        return -1; // Vehicle not found
    }

    // Shift the remaining vehicles to fill the gap
    for (int i = index; i < fleet->count - 1; i++) {
        fleet->vehicles[i] = fleet->vehicles[i + 1];
    }
    fleet->count--;
    return 0; // Vehicle removed successfully
}
