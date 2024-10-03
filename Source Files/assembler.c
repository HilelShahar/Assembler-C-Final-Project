#include <stdio.h>
#include "../Header Files/errors.h"
#include "../Header Files/utility.h"
#include "../Header Files/pre_proc.h"
#include "../Header Files/first_pass.h"
#include "../Header Files/definitions.h"

/**
 * This is the main function that receives assembly input files (written in a specific language defined by the project's requirements).
 * The function then passes them over to the analysis of the "Three Steps Assembler".
 * @param argc The number of command-line arguments.
 * @param argv An array of strings containing the command-line arguments.
 * @return Returns 0 on successful completion.
 */
int main(int argc, char *argv[]) {
    int i = 1;
    FILE *file;
    char *file_name;

    if (argc < TWO) {  /* Checking if no files were entered */
        print_system_error(Error_0);
        return 1;  /* Indicates faliure */
    }
    /* Scanning files */
    for (; i < argc; i++) {
        file_name = valid_file_name(argv[i]);  /* Validating the input file name */
        if (file_name == NULL)
            continue;  /* Skipping to the next file */

        file = search_file(file_name);
        if (file == NULL)
            continue;  /* Skipping to the next file */

        printf("\nProcessing file: \"%s\"\n",file_name);

        /* Starting pre_processing */
        if (pre_processing(file_name) != 0) {
            printf("Process terminated\n");
            continue;  /* Skipping to the next file */
        }
        /* Starting first pass */
        if (first_pass(file_name) != 0) {
            printf("Process terminated\n");
            continue;  /* Skipping to the next file */
        }
        printf("Process ended\n");
        free_all_memory();
    }
    return 0;  /* Indicates success */
}