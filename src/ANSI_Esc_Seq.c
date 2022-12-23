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

    puts("Text color");
    printf(ANSI_TEXT_COLOR_RED "T" ANSI_TEXT_COLOR_GREEN "E" ANSI_TEXT_COLOR_YELLOW "S" ANSI_TEXT_COLOR_BLUE "T ");
    puts(ANSI_TEXT_COLOR_MAGENTA "T" ANSI_TEXT_COLOR_CYAN "E" ANSI_TEXT_COLOR_WHITE "X" ANSI_TEXT_COLOR_BLACK "T");

    puts(ANSI_TEXT_COLOR_RED "RED");
    puts(ANSI_TEXT_COLOR_GREEN "GREEN");
    puts(ANSI_TEXT_COLOR_YELLOW "YELLOW");
    puts(ANSI_TEXT_COLOR_BLUE "BLUE");
    puts(ANSI_TEXT_COLOR_MAGENTA "MAGENTA");
    puts(ANSI_TEXT_COLOR_CYAN "CYAN");
    puts(ANSI_TEXT_COLOR_WHITE "WHITE");
    puts(ANSI_TEXT_COLOR_BLACK "BLACK");
    puts(ANSI_RESET_ALL "DEFAULT");
    puts("");

    puts(ANSI_TEXT_BOLD "BOLD");
    puts(ANSI_TEXT_ITALIC "ITALIC");
    puts(ANSI_TEXT_UNDERLINE "UNDERLINE" ANSI_TEXT_UNDERLINE_OFF);
    puts(ANSI_TEXT_DOUBLE_UNDERLINE "DOUBLE UNDERLINE" ANSI_TEXT_UNDERLINE_OFF);
    // On many systems slow and fast blink have the same blink time
    puts(ANSI_TEXT_SLOW_BLINK "SLOW BLINK");
    puts(ANSI_TEXT_FAST_BLINK "FAST BLINK");

    puts(ANSI_RESET_ALL);
    printf("> End: %s <\n", __func__);
    fflush(stdout);
    return;
}
