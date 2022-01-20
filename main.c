#include <stdio.h>
#include <string.h>

#define FILENAME "passwords.dat"
#define LENGTH 1000
#define MAX_PASSWORDS 100

/*
 * PASSWORD KEEPER
 *
 * Store passwords by site name
 * encrypt passwords
 * save passwords
 * encryption based on master key that is asked from the user
 * xor-operation between master key and the password
 * master key may not be stored anywhere
 * if the user forgets the master key --> passwords can’t be recovered
 * running the encryption twice (using the same master key) returns password to its origin
 * passwords are decrypted when displayed
 * plaintext password must be wiped from memory after display
 *
 * Print a list of sites and passwords
 */

void encryptDecrypt(char pass[LENGTH], char key[LENGTH]);

// structure for password
typedef struct Password {
    char site[LENGTH];
    char pass[LENGTH];
};

struct Password passwords[MAX_PASSWORDS] = {};

int main() {
    char master_key[LENGTH];
    int option;
    FILE *append_file;
    FILE *read_file;
    FILE *write_file;


    // Test that file exists
    read_file = fopen(FILENAME, "r");

    if (read_file == NULL)
    {
        printf("Unable to open %s file\n", FILENAME);

        return 0;
    }
    fclose(read_file);

    printf("PASSWORD KEEPER\n\n");
    printf("Enter master key: ");
    scanf("%s", master_key);

    /*
     * There are 4 options for user to choose
     * 1. Add password
     * 2. Retrieve password
     * 3. Clear them
     * 4. Exit program
     */

    while(1)
    {
        option = 0;

        printf("\n\n1. Add password\n2. Retrieve passwords\n3. Clear passwords\n4 Exit\n\n");
        scanf("%d", &option);

        if (option == 1)
        {
            // Add Password
            struct Password new_pass;
            printf("Site name: ");
            scanf("%s", new_pass.site);
            printf("Password: ");
            scanf("%s", new_pass.pass);

            encryptDecrypt(new_pass.pass, master_key);

            append_file = fopen(FILENAME, "ab");
            fwrite(&new_pass, sizeof(struct Password), 1, append_file);
            fclose(append_file);

            continue;
        }
        else if (option == 2)
        {
            // Print list of passwords
            read_file = fopen(FILENAME, "r");
            printf("\n%-30s | %-100s\n\n", "SITE", "PASSWORD" );
            while (fread(&passwords, sizeof(struct Password), 1, read_file))
            {
                encryptDecrypt(passwords->pass, master_key);
                printf("%-30s | %-100s\n", passwords->site, passwords->pass );
            }
            fclose(read_file);

            continue;
        }
        else if (option == 3)
        {
            // Clear passwords
            printf("\nClear Passwords");
            write_file = fopen(FILENAME, "w");
            fclose(write_file);

            continue;
        }
        else if (option == 4)
        {
            // Exit
            printf("\nExit");
            break;
        }
        else {
            printf("Try again");
            continue;
        }
    }

    return 0;
}

// Encryption function with xor-operation
void encryptDecrypt(char pass[LENGTH], char key[LENGTH])
{
    char xorKey[LENGTH];

    strcpy(xorKey, key);

    // calculate lengths of password and key
    int passLen = strlen(pass);
    int keyLen = strlen(xorKey);

    // Make the key at least the length of password
    while (passLen > keyLen)
    {
        strcat(xorKey, key);
        keyLen = strlen(xorKey);
    }

    for (int i = 0; i < passLen; ++i)
    {
        pass[i] = pass[i] ^ xorKey[i] - '0';
    }
}