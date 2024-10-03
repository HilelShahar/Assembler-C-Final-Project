/**
 * This is the pre-processing header file.
 * Explanation of the process:
 * If the line is a comment or a regular line (not related to macros), it is directly copied into a new output file with an ".am" extension.
 * When a macro call is encountered, the file replaces the call with the corresponding macro content in the new output file.
 * For macro declarations, the file first validates the declaration. If the declaration is valid, the macro's content is stored for later use,
 * but the macro declaration and its content are not added to the output file.
 * The end result is a new file that has all macro declarations removed and all macro calls replaced with their corresponding content,
 * providing a simplified file ready for further processing by the assembler.
 */
#ifndef PRE_PROC_H
#define PRE_PROC_H


/**
 * Pre-processes the given assembly file to handle macros and generate a new file with expanded macros.
 * The function changes the file extension to ".am" and processes macro calls and declarations.
 * If errors are detected during macro handling, it frees the allocated resources and returns a failure status.
 * @param file_name The name of the input file containing the source code to be pre-processed.
 * @return Returns 0 on successful completion, or 1 if errors are detected during macro handling.
 */
int pre_processing(char *file_name);


/**
 * Processes the given assembly file to handle macros, expanding them into the output file.
 * Reads the input file line by line, processes macros, and writes the expanded content to the output file.
 * If any errors are detected during processing, the output file is deleted.
 * @param file_name The name of the input file containing the source code to be read and processed.
 * @param file_am_name The name of the output file where the processed content with expanded macros will be written.
 * @return Returns 0 if no errors were found, or 1 if errors were detected.
 */
int handle_macros(char *file_name,char *file_am_name);


/**
 * Validates a macro declaration by checking the first word and validating the macro name.
 * @param file_name The name of the file being processed.
 * @param decl The line containing the macro declaration.
 * @param line_count The current line number in the file.
 * @return Returns the macro name if valid, otherwise returns NULL indicating failure.
 */
char *valid_macro_decl(char *file_name,char *decl,int line_count);


#endif