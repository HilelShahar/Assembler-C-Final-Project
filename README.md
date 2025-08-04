# Assembler-C-Final-Project (Grade - 💯)
(The Open University 2024B Course 20465)
# Project Overview 📋
This is an assembler project written in C90 (ANSI) designed for a specific assembly language defined by the project's requirements.
The assembler's core feature is its unique approach to completing the assembly process in just two passes over the source code. This is achieved by efficiently flagging and resolving label addresses in the first pass.
In the first pass, when an operand is identified as a potential label, it is added to the labels list as type "operand". In the instruction array, its address is marked as uncoded by setting:
- Bit 0 → indicates external
- Bit 1 → indicates relocatable

Coded addresses will have both bits turned off.

This ensures a perfect match between the next uncoded label and the corresponding operand label.
During the second pass, the machine code is completed since all label addresses are now known. The process involves retrieving the next uncoded "word" from the instruction array, matching it with the next operand label, verifying that it was defined (checking if the label exists in the list not as type "operand"), and then updating the address accordingly.

This approach efficiently scans the source code, while handling all type of potential errors and creating the relevant output files.

# Stages Overview 🟢🟡🔴 
🔄 **Pre-processing:** 

The pre-processing stage reads the input file once.
- Macro Management: Handles macro declarations and calls.
- Error Handling: Identifies and reports errors related to macros.
- File Generation: Replaces all macro calls with their corresponding content and removes the original macro declarations.
- Single Pass: All the above tasks are completed in a single pass.

1️⃣ **First Pass:**

The file is read a second time.
- Error Detection: Detects errors in instructions and operations.
- Validation: Validates instructions against expected formats.
- Machine Coding: Generates initial machine code.
- Uncoded Words: Marks placeholders for unresolved labels to ensure the file is only read twice.

2️⃣ **Second Pass:**

Resolving uncoded words from the first pass.
- Label Matching: Matches placeholders with corresponding label addresses.
- Complete Machine Code: Updates and finalizes the machine code, ensuring all addresses are correctly resolved.
- File Generation: Creates the final object, entry, and extern files.

# 📝 Additional Notes
⚠️ **Error Handling:**

Some errors will not be detected in the earlier steps.
For example, undefined labels will only be detected during the second pass because labels can be used before they are defined, so for the error to be detected- the first pass must be successfully completed.

🧠 **Memory Management:**

The use of static variables was generally avoided, and so they were only used with dynamic data structures handled through linked lists for macros, labels, and memory nodes. 

This design:
- Promotes efficient memory usage
- Allows memory to be freed at any time
- Avoids the need to pass head pointers between functions

**Shahar Hilel ©**
