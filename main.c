#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h> // For isdigit

#define PAIRS 20
#define STR_LEN 40
#define MAX_DAYS 7
#define MAX_SLOTS 5
#define MAX_SPECIALTIES 5
#define MAX_DOCTORS 50
#define MAX_PATIENTS 100
#define MAX_APPOINTMENTS 100

// --- Color Definitions ---
void green() { printf("\033[1;32m"); }
void blue() { printf("\033[1;34m"); }
void cyan() { printf("\033[1;36m"); }
void red() { printf("\033[1;31m"); }
void yellow() { printf("\033[1;33m"); }
void purple() { printf("\033[1;35m"); }
void reset() { printf("\033[0m"); }

// --- Struct Definitions ---
typedef struct {
    char name[STR_LEN];
    char surname[STR_LEN];
    char specialties[MAX_SPECIALTIES][STR_LEN];
    char phoneNumber[STR_LEN];
    bool schedule[MAX_DAYS][MAX_SLOTS];
    char timeSlots[MAX_SLOTS][STR_LEN];
} Doctor;

typedef struct {
    char name[STR_LEN];
    char surname[STR_LEN];
    char phoneNumber[STR_LEN];
} Pacient;

typedef struct {
    Pacient pacient;
    Doctor doctor;
    int day;
    int timeSlot;
    char date[STR_LEN]; // Format: DD/MM/YYYY
} Appointment;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear"); // MacOS sau Linux
#endif
}

void waitForEnter() {
    printf("\nApasa [ENTER] pentru a continua...");
    // Consumam input buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    // Wait for the actual Enter press
    getchar();
}

void removeEndLine(char *target) {
    char *newline = strchr(target, '\n');
    if (newline) {
        *newline = '\0';
    }
}

// Helper to get validated integer input within a range
int getValidatedInt(const char *prompt, int min, int max) {
    int value;
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            // Check if input is purely numeric
            bool is_numeric = true;
            char *ptr = buffer;

            // Skip leading whitespace
            while (isspace((unsigned char)*ptr))
                ptr++;

            // Check for optional sign
            if (*ptr == '+' || *ptr == '-')
                ptr++;

            // Check digits
            if (!isdigit((unsigned char)*ptr)) {
                is_numeric = false;
            } else {
                while (isdigit((unsigned char)*ptr))
                    ptr++;
            }

            // Check trailing whitespace and newline
            while (isspace((unsigned char)*ptr) && *ptr != '\n') ptr++;
            if (*ptr != '\n' && *ptr != '\0') { // Check if there's non-numeric, non-whitespace chars left
                 is_numeric = false;
            }

            if (is_numeric && sscanf(buffer, "%d", &value) == 1 && value >= min && value <= max) {
                return value;
            }
        }
        red();
        printf("Input invalid. Va rugam introduceti un numar intre %d si %d.\n", min, max);
        reset();
    }
}

// Helper to get validated non-empty string input
void getValidatedString(const char *prompt, char *dest, int maxLength) {
    while (1) {
        printf("%s", prompt);
        if (fgets(dest, maxLength, stdin) != NULL) {
            removeEndLine(dest);
            // Check if the string is not empty or just whitespace
            char *ptr = dest;
            while (isspace((unsigned char)*ptr)) {
                ptr++;
            }
            if (*ptr != '\0') { // If it's not the end of the string, it's not empty
                return;
            }
        }
        red();
        printf("Input invalid. Campul nu poate fi gol.\n");
        reset();
    }
}

// Get valid Y/N input
bool validateYesNo(const char *prompt) {
    char choice;
    while (1) {
        printf("%s", prompt);
        if (scanf(" %c", &choice) == 1) { // Note the space before %c to consume leftover whitespace
            getchar();
            choice = toupper(choice);
            if (choice == 'Y' || choice == 'N') {
                return (choice == 'Y');
            }
        } else {
            // Scapam de input invalid
            while (getchar() != '\n');
        }
        red();
        printf("Input invalid. Va rugam introduceti Y sau N.\n");
        reset();
    }
}


// --- Functii cu fisierele binare ---
void saveDoctorsToFile(Doctor doctors[], int count) {
    FILE *file = fopen("doctors.dat", "wb");
    if (file == NULL) {
        red();
        printf("A aparut o eroare la salvarea doctorilor in fisier!\n");
        reset();
        return;
    }
    fwrite(&count, sizeof(int), 1, file);
    fwrite(doctors, sizeof(Doctor), count, file);
    fclose(file);
    green();
    printf("Doctori salvati cu success!\n");
    reset();
}

int loadDoctorsFromFile(Doctor doctors[], int maxDoctors) {
    FILE *file = fopen("doctors.dat", "rb");
    if (file == NULL) {
        yellow();
        printf("Nu s-a gasit fisierul cu doctori (doctors.dat). Se incepe cu o lista goala.\n");
        reset();
        return 0;
    }
    int count = 0;
    if (fread(&count, sizeof(int), 1, file) != 1) {
         yellow();
         printf("Fisierul doctors.dat este gol. Se incepe cu o lista goala.\n");
         reset();
         fclose(file);
         return 0;
    }

    if (count < 0) {
        yellow();
        printf("Numar invalid de doctori in fisier. Se incepe cu o lista goala.\n");
        reset();
        fclose(file);
        return 0;
    }
    if (count > maxDoctors) {
        yellow();
        printf("Atentie: Fisierul contine %d doctori, dar se pot incarca maxim %d.\n", count, maxDoctors);
        reset();
        count = maxDoctors;
    }
    int loadedCount = fread(doctors, sizeof(Doctor), count, file);
    fclose(file);
    if (loadedCount != count) {
        red();
        printf("Eroare la citirea datelor doctorilor din fisier. S-au incarcat %d din %d.\n", loadedCount, count);
        reset();
        return loadedCount;
    }
    green();
    printf("S-au incarcat %d doctori din fisier.\n", loadedCount);
    reset();
    return loadedCount;
}

void savePatientsToFile(Pacient patients[], int count) {
    FILE *file = fopen("patients.dat", "wb");
    if (file == NULL) {
        red();
        printf("A aparut o eroare la salvarea pacientilor in fisier!\n");
        reset();
        return;
    }
    fwrite(&count, sizeof(int), 1, file);
    fwrite(patients, sizeof(Pacient), count, file);
    fclose(file);
    green();
    printf("Pacienti salvati cu success!\n");
    reset();
}

int loadPatientsFromFile(Pacient patients[], int maxPatients) {
    FILE *file = fopen("patients.dat", "rb");
    if (file == NULL) {
        yellow();
        printf("Nu s-a gasit fisierul cu pacienti (patients.dat). Se incepe cu o lista goala.\n");
        reset();
        return 0;
    }
     int count = 0;
    if (fread(&count, sizeof(int), 1, file) != 1) {
         yellow();
         printf("Fisierul patients.dat este gol. Se incepe cu o lista goala.\n");
         reset();
         fclose(file);
         return 0;
    }
     if (count < 0) { // Basic sanity check
        yellow();
        printf("Numar invalid de pacienti in fisier. Se incepe cu o lista goala.\n");
        reset();
        fclose(file);
        return 0;
    }

if (count > maxPatients) {
        yellow();
        printf("Atentie: Fisierul contine %d pacienti, dar se pot incarca maxim %d.\n", count, maxPatients);
        reset();
        count = maxPatients;
    }
    int loadedCount = fread(patients, sizeof(Pacient), count, file);
    fclose(file);
    if (loadedCount != count) {
        red();
        printf("Eroare la citirea datelor pacientilor din fisier. S-au incarcat %d din %d.\n", loadedCount, count);
        reset();
        return loadedCount;
    }
    green();
    printf("S-au incarcat %d pacienti din fisier.\n", loadedCount);
    reset();
    return loadedCount;
}

void saveAppointmentsToFile(Appointment appointments[], int count) {
    FILE *file = fopen("appointments.dat", "wb");
    if (file == NULL) {
        red();
        printf("A aparut o eroare la salvarea programarilor in fisier!\n");
        reset();
        return;
    }
    fwrite(&count, sizeof(int), 1, file);
    fwrite(appointments, sizeof(Appointment), count, file);
    fclose(file);
    green();
    printf("Programari salvate cu success!\n");
    reset();
}

int loadAppointmentsFromFile(Appointment appointments[], int maxAppointments) {
    FILE *file = fopen("appointments.dat", "rb");
    if (file == NULL) {
        yellow();
        printf("Nu s-a gasit fisierul cu programari (appointments.dat). Se incepe cu o lista goala.\n");
        reset();
        return 0;
    }
     int count = 0;
    if (fread(&count, sizeof(int), 1, file) != 1) {
         yellow();
         printf("Fisierul appointments.dat este gol. Se incepe cu o lista goala.\n");
         reset();
         fclose(file);
         return 0;
    }
     if (count < 0) {
        yellow();
        printf("Numar invalid de programari in fisier. Se incepe cu o lista goala.\n");
        reset();
        fclose(file);
        return 0;
    }
    if (count > maxAppointments) {
        yellow();
        printf("Atentie: Fisierul contine %d programari, dar se pot incarca maxim %d.\n", count, maxAppointments);
        reset();
        count = maxAppointments;
    }
    int loadedCount = fread(appointments, sizeof(Appointment), count, file);
    fclose(file);
     if (loadedCount != count) {
        red();
        printf("Eroare la citirea datelor programarilor din fisier. S-au incarcat %d din %d.\n", loadedCount, count);
        reset();
        return loadedCount;
    }
    green();
    printf("S-au incarcat %d programari din fisier.\n", loadedCount);
    reset();
    return loadedCount;
}

// --- Core Logic Functions ---

int deleteAppointment(Doctor doctors[], int doctorsCount, Appointment appointments[], int *appointmentCount, int index) {
    if (index < 0 || index >= *appointmentCount) {
        red();
        printf("Index invalid pentru stergere.\n");
        reset();
        return 0; // Index invalid
    }

    // Cautam doctorul si marcam slotul available
    int dayIndex = appointments[index].day;
    int slotIndex = appointments[index].timeSlot;
    bool doctorFound = false;
    for (int i = 0; i < doctorsCount; i++) {
        // Cautam doctorul dupa campurile relevante
        if (strcmp(doctors[i].name, appointments[index].doctor.name) == 0 &&
            strcmp(doctors[i].surname, appointments[index].doctor.surname) == 0 &&
            strcmp(doctors[i].phoneNumber, appointments[index].doctor.phoneNumber) == 0)
        {
            doctors[i].schedule[dayIndex][slotIndex] = true;
            doctorFound = true;
            break;
        }
    }

    if (!doctorFound) {
        yellow();
        printf("Atentie: Doctorul pentru programarea respectiva nu a fost gasit pentru actualizarea programului.\n");
        reset();
    }

    // Scoatem programarea prin shiftarea elementelor la stanga
    for (int i = index; i < (*appointmentCount - 1); i++) {
        appointments[i] = appointments[i + 1];
    }

    (*appointmentCount)--;
    return 1;
}

int addDoctor(Doctor doctors[], int *doctorsCount, const char *timeSlots[], const char *dayNames[],
             char availableSpecialties[][STR_LEN], int specialtiesCount) {
    if (*doctorsCount >= MAX_DOCTORS) {
        red();
        printf("Numarul maxim de doctori (%d) a fost atins!\n", MAX_DOCTORS);
        reset();
        return 0;
    }

    Doctor newDoctor;

    cyan(); printf("\n--- Adaugare Doctor Nou ---\n"); reset();
    getValidatedString("Introduceti numele doctorului: ", newDoctor.name, STR_LEN);
    getValidatedString("Introduceti prenumele doctorului: ", newDoctor.surname, STR_LEN);
    getValidatedString("Introduceti numarul de telefon: ", newDoctor.phoneNumber, STR_LEN);

    // Initializam specializarile
    for (int i = 0; i < MAX_SPECIALTIES; i++) {
        newDoctor.specialties[i][0] = '\0';
    }

    // Get specialties
    yellow(); printf("\nSpecialitati disponibile:\n"); reset();
    for (int i = 0; i < specialtiesCount; i++) {
        printf("%d. %s\n", i + 1, availableSpecialties[i]);
    }

    int numSpecialties = getValidatedInt("\nCate specialitati are doctorul? ", 1, MAX_SPECIALTIES);

    int addedSpecialtiesCount = 0;
    for (int i = 0; i < numSpecialties; i++) {
        int choice = getValidatedInt("Alegeti specialitatea (numar): ", 1, specialtiesCount);
        // Check pentru duplicate
        bool alreadyAdded = false;
        for(int j=0; j < addedSpecialtiesCount; ++j){
            if(strcmp(newDoctor.specialties[j], availableSpecialties[choice - 1]) == 0){
                alreadyAdded = true;
                break;
            }
        }
        if(alreadyAdded){
            yellow(); printf("Specialitatea '%s' a fost deja adaugata.\n", availableSpecialties[choice - 1]); reset();
            i--; // Lasam utilizatorul sa aleaga din nou
        } else {
            strcpy(newDoctor.specialties[addedSpecialtiesCount], availableSpecialties[choice - 1]);
            addedSpecialtiesCount++;
        }
    }

    // Confirmam programarea
    yellow(); printf("\nConfigurare program (1 = disponibil, 0 = indisponibil):\n"); reset();
    for (int day = 0; day < MAX_DAYS; day++) {
        printf("Pentru %s:\n", dayNames[day]);
        for (int slot = 0; slot < MAX_SLOTS; slot++) {
            char prompt[100];
            sprintf(prompt, "  Interval %s (1/0): ", timeSlots[slot]);
            int available = getValidatedInt(prompt, 0, 1);
            newDoctor.schedule[day][slot] = (available == 1);
        }
    }

    // Copiem slot-urile de timp pentru doctor
    for (int i = 0; i < MAX_SLOTS; i++) {
        strcpy(newDoctor.timeSlots[i], timeSlots[i]);
    }

    // Adaugam la sir
    doctors[*doctorsCount] = newDoctor;
    (*doctorsCount)++;

    green(); printf("Doctor adaugat cu succes!\n"); reset();
    return 1;
}


// --- Functii pentru meniu ---
void menuOne() {
    cyan(); printf("\nBun venit la MediConsult.\n"); reset();
    printf("_________________________\n"); // len 25
    printf("Te rog sa alegi din urmatoarele optiuni: \n\n");
    yellow();
    printf("(1) "); reset(); printf("Programeaza o consultatie\n");
    yellow();
    printf("(2) "); reset(); printf("Anuleaza o programare\n");
    yellow();
    printf("(3) "); reset(); printf("Iesi din aplicatie\n");
    yellow();
    printf("(4) "); reset(); printf("Meniu Debug\n");
    printf("\n> ");
}

void specialtiesMenu(char specialties[][STR_LEN], int size_list) {
    clearScreen();
    cyan(); printf("Vizualizaza specialitatile disponibile\n"); reset();
    printf("_________________________\n");

    for (int i = 0; i < size_list; i++) {
        yellow(); printf("%d. ", i + 1); reset(); printf("%s\n", specialties[i]);
    }
    printf("\nPentru a vedea medicii disponibili, alege numarul unei specialitati.");
    printf("\n> ");
}

void displayAppointments(Appointment appointments[], int appointmentCount, const char *dayNames[], const char *timeSlots[]) {
     if (appointmentCount == 0) {
        yellow(); printf("\nNu exista programari active.\n"); reset();
        return;
    }
    cyan(); printf("\n--- Programari Active (%d) ---\n", appointmentCount); reset();
    for (int i = 0; i < appointmentCount; i++) {
        printf("%d. ", i + 1);
        purple(); printf("Pacient: "); reset(); printf("%s %s, ", appointments[i].pacient.name, appointments[i].pacient.surname);
        purple(); printf("Doctor: "); reset(); printf("%s %s\n", appointments[i].doctor.name, appointments[i].doctor.surname);
        printf("   ");
        purple(); printf("Data: "); reset(); printf("%s, ", appointments[i].date);
        purple(); printf("Zi: "); reset(); printf("%s, ", dayNames[appointments[i].day]);
        purple(); printf("Ora: "); reset(); printf("%s\n", timeSlots[appointments[i].timeSlot]);
        printf("   ------------------------------------\n");
    }
}

void displayDoctors(Doctor doctors[], int doctorsCount) {
     if (doctorsCount == 0) {
        yellow(); printf("\nNu exista doctori in sistem.\n"); reset();
        return;
    }
    cyan(); printf("\n--- Lista Doctori (%d) ---\n", doctorsCount); reset();
    for (int i = 0; i < doctorsCount; i++) {
        printf("%d. ", i + 1);
        blue(); printf("Dr. %s %s", doctors[i].name, doctors[i].surname); reset();
        printf(" - Tel: %s\n", doctors[i].phoneNumber);
        printf("   Specialitati: ");
        bool firstSpec = true;
        for (int j = 0; j < MAX_SPECIALTIES && doctors[i].specialties[j][0] != '\0'; j++) {
            if (!firstSpec) printf(", ");
            green(); printf("%s", doctors[i].specialties[j]); reset();
            firstSpec = false;
        }
        if (firstSpec) { // Nu avem specialitati
            yellow(); printf("N/A"); reset();
        }
        printf("\n");
        printf("   ------------------------------------\n");
    }
}

void displayPatients(Pacient patients[], int patientCount) {
     if (patientCount == 0) {
        yellow(); printf("\nNu exista pacienti in sistem.\n"); reset();
        return;
    }
    cyan(); printf("\n--- Lista Pacienti (%d) ---\n", patientCount); reset();
    for (int i = 0; i < patientCount; i++) {
        printf("%d. ", i + 1);
        purple(); printf("%s %s", patients[i].name, patients[i].surname); reset();
        printf(" - Tel: %s\n", patients[i].phoneNumber);
        printf("   ------------------------------------\n");
    }
}


int main() {
    // --- Initializarea variabilelor ---
    const char *timeSlots[MAX_SLOTS] = {
        "09:00-10:00", "10:00-11:00", "11:00-12:00", "14:00-15:00", "15:00-16:00"
    };

    const char *dayNames[MAX_DAYS] = {
        "Luni", "Marti", "Miercuri", "Joi", "Vineri", "Sambata", "Duminica"
    };

    char listaSpecialitati[][STR_LEN] = {
        "Cardiologie", "Pediatrie", "Chirurgie", "Dietetica",
        "Psihiatrie", "Urologie", "Dermatologie", "Ortopedie" // Added more
    };
    int sizeListaSpecialitati = sizeof(listaSpecialitati) / sizeof(listaSpecialitati[0]);

    Doctor doctors[MAX_DOCTORS];
    Pacient patients[MAX_PATIENTS];
    Appointment appointments[MAX_APPOINTMENTS];

    int doctorsCount = 0;
    int patientCount = 0;
    int appointmentCount = 0;

    // --- Load data(fisiere mari) ---
    printf("Se incarca datele...\n");
    doctorsCount = loadDoctorsFromFile(doctors, MAX_DOCTORS);
    patientCount = loadPatientsFromFile(patients, MAX_PATIENTS);
    appointmentCount = loadAppointmentsFromFile(appointments, MAX_APPOINTMENTS);
    printf("Datele au fost incarcate.\n");
    sleep(1); // User experience - lasam utilizatorul sa vada incarcarea datelor

    // --- Main Loop ---
    while (true) {
        clearScreen();
        menuOne();
        int userOption = getValidatedInt("> ", 1, 4); // Validate main menu choice

        // --- Option 1: Programarea in sine ---
        if (userOption == 1) {
            specialtiesMenu(listaSpecialitati, sizeListaSpecialitati);
            int specialtyChoice = getValidatedInt("> ", 1, sizeListaSpecialitati);

            clearScreen();
            char *selectedSpecialty = listaSpecialitati[specialtyChoice - 1];
            int availableDoctorsCount = 0;
            int doctorMap[MAX_DOCTORS];

            cyan(); printf("Medici disponibili pentru specialitatea: ");
            green(); printf("%s\n", selectedSpecialty); reset();
            printf("_________________________\n");

            for (int i = 0; i < doctorsCount; i++) {
                bool hasSpecialty = false;
                for (int j = 0; j < MAX_SPECIALTIES; j++) {
                    if (doctors[i].specialties[j][0] != '\0' &&
                        strcmp(selectedSpecialty, doctors[i].specialties[j]) == 0) {
                        hasSpecialty = true;
                        break;
                        }
                }
                if (hasSpecialty) {
                    doctorMap[availableDoctorsCount] = i;
                    printf("%d. ", availableDoctorsCount + 1);
                    blue(); printf("Dr. %s %s", doctors[i].name, doctors[i].surname); reset();
                    printf(" - Tel: %s\n", doctors[i].phoneNumber);
                    availableDoctorsCount++;
                }
            }

            if (availableDoctorsCount == 0) {
                yellow(); printf("\nNu exista medici disponibili pentru aceasta specializare.\n"); reset();
            } else {
                if (validateYesNo("\nDoresti sa programezi o consultatie? (Y/N) ")) {
                    cyan(); printf("\n--- Detalii Pacient ---\n"); reset();
                    Pacient newPatient; // Temp struct pentru check
                    getValidatedString("Numele tau: ", newPatient.name, STR_LEN);
                    getValidatedString("Prenumele tau: ", newPatient.surname, STR_LEN);
                    getValidatedString("Numarul tau de telefon: ", newPatient.phoneNumber, STR_LEN);

                    // Verificam daca pacientul exista
                    int patientIndex = -1;
                    for(int i = 0; i < patientCount; ++i) {
                        if (strcmp(patients[i].name, newPatient.name) == 0 &&
                            strcmp(patients[i].surname, newPatient.surname) == 0 &&
                            strcmp(patients[i].phoneNumber, newPatient.phoneNumber) == 0) {
                            patientIndex = i;
                            yellow(); printf("Pacient gasit in sistem.\n"); reset();
                            break;
                            }
                    }

                    if (patientIndex == -1) { // Adaugam nou pacient
                        if (patientCount < MAX_PATIENTS) {
                            patients[patientCount] = newPatient;
                            patientIndex = patientCount;
                            patientCount++;
                            green(); printf("Pacient nou adaugat.\n"); reset();
                            savePatientsToFile(patients, patientCount); // Il salvam
                        } else {
                            red(); printf("Nu se pot adauga pacienti noi, limita atinsa.\n"); reset();
                            waitForEnter();
                            continue; // Main menu
                        }
                    }

                    // Selectam doctorul
                    int doctorChoice = getValidatedInt("\nAlege un doctor (numar): ", 1, availableDoctorsCount);
                    int doctorIndex = doctorMap[doctorChoice - 1]; // Luam index doctor

                    // Display la programul ales de doctor
                    cyan(); printf("\n--- Program Disponibil Dr. %s %s ---\n", doctors[doctorIndex].name, doctors[doctorIndex].surname); reset();
                    for (int day = 0; day < MAX_DAYS; day++) {
                        printf("%d. %s: ", day + 1, dayNames[day]);
                        bool dayHasSlots = false;
                        for (int slot = 0; slot < MAX_SLOTS; slot++) {
                            if (doctors[doctorIndex].schedule[day][slot]) {
                                printf("[%s] ", timeSlots[slot]);
                                dayHasSlots = true;
                            }
                        }
                        if (!dayHasSlots) {
                            yellow(); printf("Indisponibil"); reset();
                        }
                        printf("\n");
                    }

                    // Alegem ziua
                    int dayChoice = getValidatedInt("\nAlege o zi (numar): ", 1, MAX_DAYS);
                    int dayIndex = dayChoice - 1;

                    // Alegem intervalul pentru programare
                    printf("\nIntervale disponibile pentru %s:\n", dayNames[dayIndex]);
                    int availableSlotsCount = 0;
                    int slotMap[MAX_SLOTS];

                    for (int slot = 0; slot < MAX_SLOTS; slot++) {
                        if (doctors[doctorIndex].schedule[dayIndex][slot]) {
                            printf("%d. %s\n", availableSlotsCount + 1, timeSlots[slot]);
                            slotMap[availableSlotsCount] = slot;
                            availableSlotsCount++;
                        }
                    }

                    if (availableSlotsCount == 0) {
                        yellow(); printf("Nu exista intervale disponibile pentru ziua selectata.\n"); reset();
                    } else {
                        int slotChoiceInput = getValidatedInt("Alege un interval orar (numar): ", 1, availableSlotsCount);
                        int slotIndex = slotMap[slotChoiceInput - 1];

                        // Luam data
                        char date[STR_LEN];
                        getValidatedString("\nIntroduceti data (ZZ/LL/AAAA): ", date, STR_LEN);

                        // Final checks
                        if (doctors[doctorIndex].schedule[dayIndex][slotIndex]) {
                            if (appointmentCount < MAX_APPOINTMENTS) {
                                // Eliberam slot-ul pentru programari
                                doctors[doctorIndex].schedule[dayIndex][slotIndex] = false;

                                // Cream si adaugam o noua programare
                                Appointment newAppointment;
                                newAppointment.pacient = patients[patientIndex]; // Use the found/added patient
                                newAppointment.doctor = doctors[doctorIndex]; // Luam datele despre doctor
                                newAppointment.day = dayIndex;
                                newAppointment.timeSlot = slotIndex;
                                strcpy(newAppointment.date, date);

                                appointments[appointmentCount] = newAppointment;
                                appointmentCount++;

                                // Salvam modificarile
                                saveDoctorsToFile(doctors, doctorsCount);
                                saveAppointmentsToFile(appointments, appointmentCount);

                                green();
                                printf("\nProgramare efectuata cu succes!\n");
                                printf("Pacient: %s %s\n", newAppointment.pacient.name, newAppointment.pacient.surname);
                                printf("Doctor: Dr. %s %s\n", newAppointment.doctor.name, newAppointment.doctor.surname);
                                printf("Data: %s, Zi: %s, Ora: %s\n", newAppointment.date, dayNames[newAppointment.day], timeSlots[newAppointment.timeSlot]);
                                reset();
                            } else {
                                red(); printf("Nu se pot adauga programari noi, limita atinsa.\n"); reset();
                            }
                        } else {
                            // Safety check
                            red(); printf("Eroare: Intervalul selectat nu mai este disponibil.\n"); reset();
                        }
                    }
                }
                waitForEnter();
            }
            }

            // --- Option 2: Anulam programarea ---
            else if (userOption == 2) {
                clearScreen();
                cyan(); printf("--- Anulare Programare ---\n"); reset();

                if (appointmentCount == 0) {
                    yellow(); printf("\nNu exista programari care pot fi anulate!\n"); reset();
                } else {
                    displayAppointments(appointments, appointmentCount, dayNames, timeSlots); // Display la programari

                    int deleteChoice = getValidatedInt("\nIntroduceti numarul programarii de anulat: ", 1, appointmentCount);

                    if (deleteAppointment(doctors, doctorsCount, appointments, &appointmentCount, deleteChoice - 1)) {
                        green(); printf("\nProgramarea a fost anulata cu succes!\n"); reset();
                        // Salvam modificarile
                        saveDoctorsToFile(doctors, doctorsCount);
                        saveAppointmentsToFile(appointments, appointmentCount);
                    } else {
                        // Safety check
                        red(); printf("\nA aparut o eroare la anularea programarii!\n"); reset();
                    }
                }
                waitForEnter();
            }

            // --- Option 3: Iesim din aplicatie ---
            else if (userOption == 3) {
                clearScreen();
                cyan(); printf("Se salveaza datele...\n"); reset();
                saveDoctorsToFile(doctors, doctorsCount);
                savePatientsToFile(patients, patientCount);
                saveAppointmentsToFile(appointments, appointmentCount);
                green(); printf("Datele au fost salvate.\n"); reset();
                cyan(); printf("La revedere!\n"); reset();
                break; // Exit the while loop
            }

            // --- Option 4: Meniu Debug ---
            else if (userOption == 4) {
                clearScreen();
                cyan(); printf("\n--- MENIU DEBUG ---\n"); reset();
                printf("_____________________\n");
                yellow(); printf("1."); reset(); printf(" Afiseaza Doctori\n");
                yellow(); printf("2."); reset(); printf(" Afiseaza Pacienti\n");
                yellow(); printf("3."); reset(); printf(" Afiseaza Programari\n");
                yellow(); printf("4."); reset(); printf(" Salveaza Datele Manual\n");
                yellow(); printf("5."); reset(); printf(" Adauga Doctor Nou\n");
                yellow(); printf("6."); reset(); printf(" Inapoi la Meniul Principal\n");

                int debugOption = getValidatedInt("> ", 1, 6);

                switch (debugOption) {
                    case 1:
                        displayDoctors(doctors, doctorsCount);
                    break;
                    case 2:
                        displayPatients(patients, patientCount);
                    break;
                    case 3:
                        displayAppointments(appointments, appointmentCount, dayNames, timeSlots);
                    break;
                    case 4:
                        cyan(); printf("Salvare manuala date...\n"); reset();
                    saveDoctorsToFile(doctors, doctorsCount);
                    savePatientsToFile(patients, patientCount);
                    saveAppointmentsToFile(appointments, appointmentCount);
                    green(); printf("Date salvate manual cu succes!\n"); reset();
                    break;
                    case 5:
                        if (addDoctor(doctors, &doctorsCount, timeSlots, dayNames, listaSpecialitati, sizeListaSpecialitati)) {
                            saveDoctorsToFile(doctors, doctorsCount); // Salvam dupa ce adaugam doctor/-i noi
                        } else {
                            // Eroarea se printeaza in addDoctor()
                        }
                    break;
                    case 6:
                        // Back to meniul principal
                            break;
                }

                if (debugOption != 6) {
                    waitForEnter();
                }
            }

        }

        return 0;
    }