#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>


#define PAIRS 20
#define STR_LEN 40
#define MAX_DAYS 7
#define MAX_SLOTS 5
#define MAX_SPECIALTIES 5
#define MAX_DOCTORS 50
#define MAX_PATIENTS 100
#define MAX_APPOINTMENTS 100

// Hard code un dictionar
struct Dictionary {
    char key[STR_LEN];
    char value[STR_LEN][STR_LEN];
};


typedef struct {
    char name[STR_LEN];
    char surname[STR_LEN];
    char specialties[MAX_SPECIALTIES][STR_LEN];
    char phoneNumber[STR_LEN];

    // True - available, else False
    bool schedule[MAX_DAYS][MAX_SLOTS];

    // "09:00 - 10:00"
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
    char date[STR_LEN];
} Appointment;

// Clear the terminal screen, still doesn't work..
void clearScreen() {
#ifdef _WIN32
    system("cls");
#elif __linux__
    system("clear && printf '\e[3J'");
#elif  __APPLE__
    system("clear && printf '\e[3J'");
#endif
}


// Show Welcome Menu
void menuOne() {
    printf("Bun venit la MediConsult.\n");
    sleep(1);
    printf("_________________________"); // len 25
    printf("\nTe rog sa alegi din urmatoarele optiuni: \n\n");
    printf("(1) Vizualizeaza specialitatile disponibile \n(2) Iesi din aplicatie\n\n> ");
}


/*
 * @param specialties[][] - Array of available medical specialties
 * @param size_list - Number of specialties in the array
 */
// Displays all available medical specialties for user selection
void specialtiesMenu(char specialties[][STR_LEN], int size_list) {
    clearScreen();
    printf("Vizualizaza specialitatile disponibile\n");
    printf("_________________________\n");
    sleep(1);

    for (int i = 0; i < size_list; i++) {
        printf("%d. %s\n", i + 1, specialties[i]);
    }
    printf("\nPentru a alege medicii disponibili, alege una din specialitati.");
    printf("\n> ");
}


/*
 * @param target - String to be processed
 */
// Removes newline character from the end of a string (for input processing)
void removeEndLine(char *target) {
    char *newline = strchr(target, '\n');
    if (newline) {
        *newline = '\0';
    }
}


/*
 * @param doctors[] - Array of Doctor structures
 * @param count - Number of doctors to save
 */
// Saves doctor data to a binary file for persistent storage
void saveDoctorsToFile(Doctor doctors[], int count) {
    FILE *file = fopen("doctors.dat", "wb");
    if (file == NULL) {
        printf("A aparut o eroare la salvarea doctorilor in fisier!\n");
        return;
    }

    fwrite(&count, sizeof(int), 1, file);
    fwrite(doctors, sizeof(Doctor), count, file);
    fclose(file);
    sleep(1);
    printf("Doctori salvati cu success!\n");
}


/*
 * @param doctors[] - Array where doctor data will be loaded
 * @param maxDoctors - Maximum number of doctors that can be loaded
 */
// Loads doctor data from the file and returns the count of doctors loaded
int loadDoctorsFromFile(Doctor doctors[], int maxDoctors) {
    FILE *file = fopen("doctors.dat", "rb");
    if (file == NULL) {
        printf("Nu s-a gasit fisierul cu doctori\n");
        return 0;
    }

    int count;
    fread(&count, sizeof(int), 1, file);
    if (count > maxDoctors) {
        count = maxDoctors;
    }
    fread(doctors, sizeof(Doctor), count, file);
    fclose(file);
    printf("S-au incarcat %d doctori din fisier.\n", count);
    return count;
}


/*
 * @param appointments[] - Array of Appointment structures
 * @param count - Number of appointments to save
 */
// Saves appointment data to the file
void savePatientsToFile(Pacient patients[], int count) {
    FILE *file = fopen("patients.dat", "wb");
    if (file == NULL) {
        printf("A aparut o eroare la salvarea pacientilor in fisier! \n");
        return;
    }

    fwrite(&count, sizeof(int), 1, file);
    fwrite(patients, sizeof(Pacient), count, file);
    fclose(file);
    sleep(1);
    printf("Pacienti salvati cu success!\n");
}


/*
 * @param Pacient patients[] - struct for patients
 * @param maxPatients - Maximum number for patients
 */
// Load the patients from the files and return the number of patients loaded
int loadPatientsFromFile(Pacient patients[], int maxPatients) {
    FILE *file = fopen("patients.dat", "rb");
    if (file == NULL) {
        printf("Nu s-a gasit fisierul pacienti\n");
        return 0;
    }

    int count;
    fread(&count, sizeof(int), 1, file);
    if (count > maxPatients) {
        count = maxPatients;
    }

    fread(patients, sizeof(Pacient), count, file);
    fclose(file);
    sleep(1);
    printf("S-au incarcat %d pacienti din fisier \n", count);
    return count;
}


/*
 * @param appointments[] - Array where appointment data will be loaded
 * @param maxAppointments - Maximum number of appointments
 */
// Loads appointment data from the file and returns the count
void saveAppointmentsToFile(Appointment appointments[], int count) {
    FILE *file = fopen("appointments.dat", "wb");

    if (file == NULL) {
        printf("Nu s-a gasit fisierul cu programari\n");
        return;
    }

    fwrite(&count, sizeof(int), 1, file);
    fwrite(appointments, sizeof(Appointment), count, file);
    fclose(file);
    sleep(1);
    printf("Appointment salvati cu success!\n");
    return;
}

int loadAppointmentsFromFile(Appointment appointments[], int maxAppointments) {
    FILE *file = fopen("appointments.dat", "rb");
    if (file == NULL) {
        printf("Nu s-a gasit fisierul cu programari. \n");
        return 0;
    }

    int count;
    fread(&count, sizeof(int), 1, file);

    if (count > maxAppointments) {
        count = maxAppointments;
    }
    fread(appointments, sizeof(Appointment), count, file);
    fclose(file);
    printf("S-au incarcat %d programari din fisier.\n", count);
    return count;
}


int main() {
    /*
 * Initialize essential arrays for program operation:
 * - timeSlots: Available appointment times
 * - dayNames: Days of the week
 * - listaSpecialitati: Available medical specialties
 */

    const char *timeSlots[MAX_SLOTS] = {
        "09:00-10:00", "10:00-11:00", "11:00-12:00", "14:00-15:00", "15:00-16:00"
    };

    const char *dayNames[MAX_DAYS] = {
        "Luni", "Marti", "Miercuri", "Joi", "Vineri", "Sambata", "Duminica"
    };

    char listaSpecialitati[][STR_LEN] = {
        "Cardiologie",
        "Pediatrie",
        "Chirurgie",
        "Dietetica",
        "Psihiatrie",
        "Urologie"
    };

    Doctor doctors[MAX_DOCTORS];
    Pacient patients[MAX_PATIENTS];
    Appointment appointments[MAX_APPOINTMENTS];

    int doctorsCount = 0, patientCount = 0, appointmentCount = 0;
    int sizeAppointment = sizeof(appointments) / sizeof(appointments[0]);
    int sizeListaSpecialitati = sizeof(listaSpecialitati) / sizeof(listaSpecialitati[0]);

    patientCount = loadPatientsFromFile(patients, MAX_PATIENTS);
    appointmentCount = loadAppointmentsFromFile(appointments, MAX_APPOINTMENTS);

    doctorsCount = loadDoctorsFromFile(doctors, MAX_DOCTORS);
    int userOption;
    char userChoice;

    if (doctorsCount == 0) {
        printf("Nu s-au gasit doctori. Se adauga doctori... \n");

        Doctor defaultDrs[] = {
            {
                .name = "Dana",
                .surname = "Cristiana",
                .phoneNumber = "0766-901-234",
                .specialties = {"Cardiologie", "Pediatrie"},
                .schedule = {
                    {true, true, false, true, false},
                    {true, true, false, false, false},
                    {false, false, true, true, false},
                    {true, false, false, true, true},
                    {false, false, false, true, true},
                    {false, false, false, false, false},
                    {false, false, false, false, false}
                },
                .timeSlots = {"09:00-10:00", "10:00-11:00", "11:00-12:00", "14:00-15:00", "15:00-16:00"}
            },
            {
                .name = "Vlad",
                .surname = "Pedrescu",
                .phoneNumber = "0755-345-678",
                .specialties = {"Urologie", "Psihiatrie", "Chirurgie"},
                .schedule = {
                    {false, false, true, true, true},
                    {true, true, true, false, false},
                    {false, false, false, true, true},
                    {true, true, false, false, false},
                    {false, false, true, true, false},
                    {true, true, false, false, false},
                    {false, false, false, false, false}
                },
                .timeSlots = {"09:00-10:00", "10:00-11:00", "11:00-12:00", "14:00-15:00", "15:00-16:00"}
            },
            {
                .name = "Daniel",
                .surname = "Bordas",
                .phoneNumber = "0744-789-012",
                .specialties = {"Urologie"},
                .schedule = {
                    {true, false, false, true, true},
                    {false, false, false, false, false},
                    {true, true, true, false, false},
                    {false, false, true, true, true},
                    {true, true, false, false, false},
                    {false, false, false, true, true},
                    {false, false, false, false, false}
                },
                .timeSlots = {"09:00-10:00", "10:00-11:00", "11:00-12:00", "14:00-15:00", "15:00-16:00"}
            },
            {
                .name = "Ionut",
                .surname = "Leahu",
                .phoneNumber = "0733-123-456",
                .specialties = {"Cardiologie"},
                .schedule = {
                    {false, false, false, false, false},
                    {true, true, true, false, false},
                    {false, false, false, true, true},
                    {true, true, false, false, false},
                    {false, false, true, true, true},
                    {false, false, false, false, false},
                    {true, true, false, false, false}
                },
                .timeSlots = {"09:00-10:00", "10:00-11:00", "11:00-12:00", "14:00-15:00", "15:00-16:00"}
            },
            {
                .name = "Romeo",
                .surname = "Micu",
                .phoneNumber = "0722-456-789",
                .specialties = {"Chirurgie"},
                .schedule = {
                    {true, true, true, false, false},
                    {false, false, false, true, true},
                    {true, true, false, false, false},
                    {false, false, true, true, false},
                    {true, false, false, true, true},
                    {true, true, false, false, false},
                    {false, false, false, false, false}
                },
                .timeSlots = {"09:00-10:00", "10:00-11:00", "11:00-12:00", "14:00-15:00", "15:00-16:00"}
            }
        };

        memcpy(doctors, defaultDrs, sizeof(defaultDrs));
        doctorsCount = sizeof(defaultDrs) / sizeof(defaultDrs[0]);

        for (int i = 0; i < doctorsCount; i++) {
            for (int j = 0; j < MAX_SLOTS; j++) {
                strcpy(doctors[i].timeSlots[j], timeSlots[j]);
            }
        }

        saveDoctorsToFile(doctors, doctorsCount);
    }


    while (true) {
        menuOne();
        scanf("%d", &userOption);
        getchar();

        if (userOption == 1) {
            specialtiesMenu(listaSpecialitati, sizeListaSpecialitati);
            scanf("%d", &userOption);
            getchar();

            while (userOption < 1 || userOption > sizeListaSpecialitati) {
                printf("Optiunea aleasa nu exista. Te rog sa alegi din nou.\n");
                sleep(1);
                clearScreen();
                specialtiesMenu(listaSpecialitati, sizeListaSpecialitati);
                scanf("%d", &userOption);
                getchar();
            }

            clearScreen();
            char *selectedSpecialty = listaSpecialitati[userOption - 1];
            int count = 0;
            int doctorMap[MAX_DOCTORS]; // Maps displayed number to doctor index
            printf("Medici disponibili pentru specialitatea %s:\n", selectedSpecialty);
            printf("_________________________\n");

            for (int i = 0; i < doctorsCount; i++) {
                for (int j = 0; j < MAX_SPECIALTIES; j++) {
                    if (doctors[i].specialties[j][0] != '\0' &&
                        strcmp(selectedSpecialty, doctors[i].specialties[j]) == 0) {
                        doctorMap[count] = i;
                        printf("%d. %s %s - Tel: %s\n", count + 1,
                               doctors[i].name, doctors[i].surname, doctors[i].phoneNumber);
                        count++;
                        break;
                    }
                }
            }

            if (count == 0) {
                printf("Nu exista medici disponibili pentru aceasta specializare.\n");
                sleep(1);
                printf("Doresti sa selectezi din nou o specialitate? (Y/N) ");
                scanf("%c", &userChoice);

                if (userChoice == 'Y' || userChoice == 'y') {
                    continue;
                }
            } else {
                printf("\nDoresti sa programezi o consultatie? (Y/N) ");
                scanf("%c", &userChoice);
                if (userChoice == 'Y' || userChoice == 'y') {
                    printf("Pentru a programa o consultatie, \nam nevoie de urmatoarele date de la tine:\n");
                    sleep(1);

                    Pacient newPatient;
                    printf("Numele tau: ");
                    getchar();
                    fgets(newPatient.name, STR_LEN, stdin);
                    removeEndLine(newPatient.name);

                    printf("\nPrenumele tau: ");
                    fgets(newPatient.surname, STR_LEN, stdin);
                    removeEndLine(newPatient.surname);

                    printf("\nNumarul tau de telefon: ");
                    fgets(newPatient.phoneNumber, STR_LEN, stdin);
                    removeEndLine(newPatient.phoneNumber);

                    // Add patient to array
                    patients[patientCount] = newPatient;
                    patientCount++;

                    // Select doctor by number
                    printf("\nAlege un doctor (numar): ");
                    int doctorChoice;
                    scanf("%d", &doctorChoice);
                    getchar();

                    if (doctorChoice < 1 || doctorChoice > count) {
                        printf("Doctorul selectat nu exista!\n");
                        printf("\nApasa [ENTER] pentru a continua\n");
                        getchar();
                        continue;
                    }

                    int doctorIndex = doctorMap[doctorChoice - 1];

                    // Select day
                    printf("\nAlege o zi (1-7):\n");
                    for (int i = 0; i < MAX_DAYS; i++) {
                        printf("%d. %s\n", i + 1, dayNames[i]);
                    }
                    int dayChoice;
                    scanf("%d", &dayChoice);
                    getchar();
                    int dayIndex = dayChoice - 1;

                    printf("\nAlege un interval orar:\n");
                    for (int i = 0; i < MAX_SLOTS; i++) {
                        printf("%d. %s\n", i + 1, timeSlots[i]);
                    }
                    int slotChoice;
                    scanf("%d", &slotChoice);
                    getchar();
                    int slotIndex = slotChoice - 1;

                    printf("\nIntroduceti data (ZZ/LL/AAAA): ");
                    char date[STR_LEN];
                    fgets(date, STR_LEN, stdin);
                    removeEndLine(date);

                    if (!doctors[doctorIndex].schedule[dayIndex][slotIndex]) {
                        printf("Acest interval nu este disponibil pentru doctorul selectat!\n");
                        printf("\nApasa [ENTER] pentru a continua\n");
                        getchar();
                        continue;
                    }
                    doctors[doctorIndex].schedule[dayIndex][slotIndex] = false;

                    Appointment newAppointment;
                    newAppointment.pacient = patients[patientCount - 1];
                    newAppointment.doctor = doctors[doctorIndex];
                    newAppointment.day = dayIndex;
                    newAppointment.timeSlot = slotIndex;
                    strcpy(newAppointment.date, date);

                    appointments[appointmentCount] = newAppointment;
                    appointmentCount++;


                    savePatientsToFile(patients, patientCount);
                    saveAppointmentsToFile(appointments, appointmentCount);
                }
            }

            printf("\nApasa [ENTER] pentru a continua\n");
            getchar();
            if (userChoice != 'Y' && userChoice != 'y') {
                getchar();
            }
            clearScreen();
        } else if (userOption == 2) {
            saveDoctorsToFile(doctors, doctorsCount);
            savePatientsToFile(patients, patientCount);
            saveAppointmentsToFile(appointments, appointmentCount);
            printf("La revedere!\n");
            break;
        } else if (userOption == 3) {
            printf("\n----- FOR TESTING ONLY -----\n");
            printf("1. Show doctors\n");
            printf("2. Show patients\n");
            printf("3. Show appointments\n");
            printf("4. Salveaza datele manual \n");
            printf("> ");
            int debugOption;
            scanf("%d", &debugOption);
            getchar();

            if (debugOption == 1) {
                printf("\nMedici (%d):\n", doctorsCount);
                for (int i = 0; i < doctorsCount; i++) {
                    printf("%d. %s %s - Tel: %s\n", i + 1,
                           doctors[i].name, doctors[i].surname, doctors[i].phoneNumber);
                    printf("   Specialitati: ");
                    for (int j = 0; j < MAX_SPECIALTIES && doctors[i].specialties[j][0] != '\0'; j++) {
                        printf("%s", doctors[i].specialties[j]);
                        if (doctors[i].specialties[j + 1][0] != '\0') printf(", ");
                    }
                    printf("\n");
                }
            } else if (debugOption == 2) {
                printf("\nPacienti (%d):\n", patientCount);
                for (int i = 0; i < patientCount; i++) {
                    printf("%d. %s %s - Tel: %s\n", i + 1,
                           patients[i].name, patients[i].surname, patients[i].phoneNumber);
                }
            } else if (debugOption == 3) {
                printf("\nProgramari (%d):\n", appointmentCount);
                for (int i = 0; i < appointmentCount; i++) {
                    printf("%d. Pacient: %s %s\n", i + 1,
                           appointments[i].pacient.name, appointments[i].pacient.surname);
                    printf("   Doctor: %s %s\n",
                           appointments[i].doctor.name, appointments[i].doctor.surname);
                    printf("   Zi: %s, Interval: %s, Data: %s\n",
                           dayNames[appointments[i].day],
                           appointments[i].doctor.timeSlots[appointments[i].timeSlot],
                           appointments[i].date);
                }
            } else if (debugOption == 4) {
                saveDoctorsToFile(doctors, doctorsCount);
                savePatientsToFile(patients, patientCount);
                saveAppointmentsToFile(appointments, appointmentCount);
                printf("Date salvate manual cu succes!\n");
            }

            printf("\nApasa [ENTER] pentru a continua");
            getchar();
        } else {
            printf("Optiune invalida. Te rog sa alegi din nou.\n");
            sleep(1);
        }
    }
    return 0;
}
