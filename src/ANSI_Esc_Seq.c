/**
 * @brief ANSI_Esc_Seq.h
 *
 * Here are may macros to alter the terminal behavior with ANSI escape sequences.
 *
 * The main source for this sequences: https://readforlearn.com/list-of-ansi-color-escape-sequences/
 *
 * @date Dec 22, 2022
 * @author: goto-vulture
 */

#include "ANSI_Esc_Seq.h"
#include <stdio.h>



/**
 * @brief Small test whether the ANSI escape sequences are working.
 */
extern void Test_ANSI_Esc_Seq()
{
    puts(ANSI_RESET_ALL);
    printf("> Begin: %s <\n", __func__);

    puts("Test 1: text color");
    printf(ANSI_TEXT_COLOR_RED "T" ANSI_TEXT_COLOR_GREEN "E" ANSI_TEXT_COLOR_YELLOW "S" ANSI_TEXT_COLOR_BLUE "T ");
    puts(ANSI_TEXT_COLOR_MAGENTA "T" ANSI_TEXT_COLOR_CYAN "E" ANSI_TEXT_COLOR_WHITE "X" ANSI_TEXT_COLOR_BLACK "T");

    puts(ANSI_RESET_ALL);
    printf("> End: %s <\n", __func__);
    fflush(stdout);
    return;
}
