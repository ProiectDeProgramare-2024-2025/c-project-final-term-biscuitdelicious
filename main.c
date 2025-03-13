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
    system("clear");
#elif  __APPLE__
    system("clear");
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


// Show Specialties Menu
void specialtiesMenu(char specialties[][STR_LEN], int size_list) {
    clearScreen();
    printf("Vizualizaza specialitatile disponibile\n");
    printf("_________________________\n");
    sleep(1);

    for(int i = 0; i < size_list; i++) {
        printf("%d. %s\n", i + 1, specialties[i]);
    }
    printf("\nPentru a alege medicii disponibili, alege una din specialitati.");
    printf("\n> ");
}


void removeEndLine(char *target) {
    char* newline = strchr(target, '\n');
    if(newline) {
        *newline = '\0';
    }
}


void saveDoctorsToFile(Doctor doctors[], int count) {
    FILE *file = fopen("doctors.dat", "wb");
    if(file == NULL) {
        printf("A aparut o eroare la salvarea doctorilor in fisier!\n");
        return;
    }

    fwrite(&count, sizeof(int), 1, file);
    fwrite(doctors, sizeof(Doctor), count, file);
    fclose(file);
    sleep(1);
    printf("Doctori salvati cu success!\n");
}

int loadDoctorsFromFile(Doctor doctors[], int maxDoctors) {
    FILE *file = fopen("doctors.dat", "rb");
    if(file == NULL) {
        printf("Nu s-a gasit fisierul cu doctori\n");
        return 0;
    }

    int count;
    fread(&count, sizeof(int), 1, file);
    if(count > maxDoctors) {
        count = maxDoctors;
    }
    fread(doctors, sizeof(Doctor), count, file);
    fclose(file);
    printf("S-au incarcat %d doctori din fisier.\n", count);
    return count;
}

void savePatientsToFile(Pacient patients[], int count) {
    FILE *file = fopen("patients.dat", "wb");
    if(file == NULL) {
        printf("A aparut o eroare la salvarea pacientilor in fisier! \n");
        return;
    }

    fwrite(&count, sizeof(int), 1, file);
    fwrite(patients, sizeof(Pacient), count, file);
    fclose(file);
    sleep(1);
    printf("Pacienti salvati cu success!\n");

}

int loadPatientsFromFile(Pacient patients[], int maxPatients) {
    FILE *file = fopen("patients.dat", "rb");
    if(file == NULL) {
        printf("Nu s-a gasit fisierul pacienti\n");
        return 0;
    }

    int count;
    fread(&count, sizeof(int), 1, file);
    if(count > maxPatients) {
        count = maxPatients;
    }

    fread(patients, sizeof(Pacient), count, file);
    fclose(file);
    sleep(1);
    printf("S-au incarcat %d pacienti din fisier \n, count);
    return count;
}

void saveAppointmentsToFile(Appointment appointments[], int count) {
    FILE *file = fopen("appointments.dat", "wb");

    if(file == NULL) {
        printf("Nu s-a gasit fisierul cu programari\n");
        return;
    }

    fwrite(&count, sizeof(int), 1, file);
    fwrite(appointments, sizeof(Appointment), count, file);
    fclose(file);
    sleep(1);
    printf("Appointment salvati cu success!\n");
}

int loadAppointmentsFromFile(Appointment appointments[], int maxAppointments) {
    FILE *file = fopen("appointments.dat", "rb");
    if(file == NULL) {
        printf("Nu s-a gasit fisierul cu programari. \n");
        return 0;
    }

    int count;
    fread(&count, sizeof(int), 1, file);

    if(count > maxAppointments) {
        count = maxAppointments;
    }
    fread(appointments, sizeof(Appointment), count, file);
    fclose(file);
    printf("S-au incarcat %d programari din fisier.\n", count);
    return count;
}




int main() {
    const char* timeSlots[MAX_SLOTS] = {
        "09:00-10:00", "10:00-11:00", "11:00-12:00", "14:00-15:00", "15:00-16:00"
    };

    const char* dayNames[MAX_DAYS] = {
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

    Doctor doctors[] = {
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

    Pacient pacienti = {};
    Appointment appointment[MAX_APPOINTMENTS];
    int sizeAppointment = sizeof(appointment) / sizeof(appointment[0]);
    int doctorsCount = sizeof(doctors) / sizeof(doctors[0]);
    int sizeListaSpecialitati = sizeof(listaSpecialitati) / sizeof(listaSpecialitati[0]);
    patientCount = loadPatients(appointment, MAX_PATIENTS);
    appointmentCount = loadAppointmentsFromFile(appointment, MAX_APPOINTMENTS);
    saveAppointmentsToFile(doctors, doctorsCount);


    int userOption;
    char userChoice;

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
            char* selectedSpecialty = listaSpecialitati[userOption - 1];
            int count = 0;

            printf("Medici disponibili pentru specialitatea %s:\n", selectedSpecialty);
            printf("_________________________\n");

            for (int i = 0; i < doctorsCount; i++) {
                for (int j = 0; j < MAX_SPECIALTIES; j++) {
                    if (doctors[i].specialties[j][0] != '\0' &&
                        strcmp(selectedSpecialty, doctors[i].specialties[j]) == 0) {
                        printf("%d. %s %s - Tel: %s\n", ++count,
                               doctors[i].name, doctors[i].surname, doctors[i].phoneNumber);
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

                    printf("Numele tau: ");
                    getchar();
                    fgets(pacienti.name, STR_LEN, stdin);
                    removeEndLine(pacienti.name);

                    printf("\nPrenumele tau: ");
                    fgets(pacienti.surname, STR_LEN, stdin);
                    removeEndLine(pacienti.surname);

                    printf("\nNumarul tau de telefon: ");
                    fgets(pacienti.phoneNumber, STR_LEN, stdin);
                    removeEndLine(pacienti.phoneNumber);
                    printf("\n\n");
                    sleep(2);

                    printf("\n Te rog sa alegi si o zi in care ai fi disponibil: ");
                    fgets(appointment->day,)
                }

            }

            printf("\nApasa [ENTER] pentru a continua\n");
            getchar();
            if (userChoice != 'Y' && userChoice != 'y') {
                getchar();

            }
            clearScreen();
        } else if (userOption == 2) {
            printf("La revedere!\n");
            break;
        } else {
            printf("Optiune invalida. Te rog sa alegi din nou.\n");
            sleep(1);
        }
    }

    return 0;
}
/*
*Utilizatorul aplicației poate vizualiza medicii și specialitățile disponibile și poate programa o consultație. DONE
Aplicația permite adăugarea/ștergerea unei programări pentru un anumit pacient cu nume, căutarea unui medic după nume, specialitate și program, programarea consultației și anularea consultației.
Pentru a efectua programarea pentru o consultație utilizatorul trebuie sa specifice numele, prenumele, telefonul, specialitatea, intervalul orar, ziua și data.

 */
