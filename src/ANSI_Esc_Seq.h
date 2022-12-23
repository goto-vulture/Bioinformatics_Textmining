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

#ifndef ANSI_ESC_SEQ_H
#define ANSI_ESC_SEQ_H ///< Include-Guard

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#ifndef ANSI_MAIN_ESC_SEQ
#define ANSI_MAIN_ESC_SEQ(...) "\033[" __VA_ARGS__ "m"  ///< Place the prae- and postfix char for a ANSI color escape sequence
#else
#error "The macro \"ANSI_MAIN_ESC_SEQ\" is already defined !"
#endif /* ANSI_MAIN_ESC_SEQ */

// There are two steps necessary for the stringification of a macro expansion. Without the first step the name of the
// macro (and not the replacement (!)) will be stringified !
#ifndef STRFICATION_STEP1
#define STRFICATION_STEP1(str) STRFICATION_STEP2(str)   ///< First step for the stringification of a macro expansion
#else
#error "The macro \"STRFICATION_STEP1\" is already defined !"
#endif /* STRFICATION_STEP1 */
#ifndef STRFICATION_STEP2
#define STRFICATION_STEP2(str) #str                     ///< Second step for the stringification of a macro expansion
#else
#error "The macro \"STRFICATION_STEP2\" is already defined !"
#endif /* STRFICATION_STEP2 */

// ===== ===== ===== ===== ===== ===== ===== ===== BEGIN Font effects ===== ===== ===== ===== ===== ===== ===== =====
/**
 * Source: https://readforlearn.com/list-of-ansi-color-escape-sequences/
 */
// Main effects
#define ANSI_FONT_EFFECT_RESET 0
#define ANSI_FONT_EFFECT_BOLD 1
#define ANSI_FONT_EFFECT_FAINT 2
#define ANSI_FONT_EFFECT_ITALIC 3
#define ANSI_FONT_EFFECT_UNDERLINE 4
#define ANSI_FONT_EFFECT_SLOW_BLINK 5
#define ANSI_FONT_EFFECT_RAPID_BLINK 6
#define ANSI_FONT_EFFECT_REVERSE_VIDEO 7
#define ANSI_FONT_EFFECT_CONCEAL 8
#define ANSI_FONT_EFFECT_CROSSED_OUT 9

// Font selection
#define ANSI_FONT_EFFECT_DEFAULT_FONT 10
#define ANSI_FONT_EFFECT_ALTERNATE_FONT_1 11
#define ANSI_FONT_EFFECT_ALTERNATE_FONT_2 12
#define ANSI_FONT_EFFECT_ALTERNATE_FONT_3 13
#define ANSI_FONT_EFFECT_ALTERNATE_FONT_4 14
#define ANSI_FONT_EFFECT_ALTERNATE_FONT_5 15
#define ANSI_FONT_EFFECT_ALTERNATE_FONT_6 16
#define ANSI_FONT_EFFECT_ALTERNATE_FONT_7 17
#define ANSI_FONT_EFFECT_ALTERNATE_FONT_8 18
#define ANSI_FONT_EFFECT_ALTERNATE_FONT_9 19

#define ANSI_FONT_EFFECT_FRAKTUR 20
#define ANSI_FONT_EFFECT_DOUBLE_UNDERLINE 21
#define ANSI_FONT_EFFECT_NORMAL_COLOR_OR_INTENSITY 22
#define ANSI_FONT_EFFECT_NOT_ITALIC_NOT_FRAKTUR 23
#define ANSI_FONT_EFFECT_UNDERLINE_OFF 24

#define ANSI_FONT_EFFECT_BLINK_OFF 25
// #define ANSI_FONT_EFFECT_ 26 NOT IN USE !
#define ANSI_FONT_EFFECT_INVERSE_OFF 27
#define ANSI_FONT_EFFECT_REVEAL 28
#define ANSI_FONT_EFFECT_NOT_CROSSED_OUT 29

#define ANSI_FONT_EFFECT_FOREGROUND_COLOR_1 30
#define ANSI_FONT_EFFECT_FOREGROUND_COLOR_2 31
#define ANSI_FONT_EFFECT_FOREGROUND_COLOR_3 32
#define ANSI_FONT_EFFECT_FOREGROUND_COLOR_4 33
#define ANSI_FONT_EFFECT_FOREGROUND_COLOR_5 34
#define ANSI_FONT_EFFECT_FOREGROUND_COLOR_6 35
#define ANSI_FONT_EFFECT_FOREGROUND_COLOR_7 36
#define ANSI_FONT_EFFECT_FOREGROUND_COLOR_8 37

#define ANSI_FONT_EFFECT_SET_FOREGROUND_COLOR 38
#define ANSI_FONT_EFFECT_DEFAULT_FOREGROUND_COLOR 39

#define ANSI_FONT_EFFECT_BACKGROUND_COLOR_1 40
#define ANSI_FONT_EFFECT_BACKGROUND_COLOR_2 41
#define ANSI_FONT_EFFECT_BACKGROUND_COLOR_3 42
#define ANSI_FONT_EFFECT_BACKGROUND_COLOR_4 43
#define ANSI_FONT_EFFECT_BACKGROUND_COLOR_5 44
#define ANSI_FONT_EFFECT_BACKGROUND_COLOR_6 45
#define ANSI_FONT_EFFECT_BACKGROUND_COLOR_7 46
#define ANSI_FONT_EFFECT_BACKGROUND_COLOR_8 47

#define ANSI_FONT_EFFECT_SET_BACKGROUND_COLOR 48
#define ANSI_FONT_EFFECT_DEFAULT_BACKGROUND_COLOR 49

// #define ANSI_FONT_EFFECT_ 50 NOT IN USE !
#define ANSI_FONT_EFFECT_FRAMED 51
#define ANSI_FONT_EFFECT_ENCIRCLED 52
#define ANSI_FONT_EFFECT_OVERLINED 53
#define ANSI_FONT_EFFECT_NOT_FRAMED_OR_ENCIRCLED 54

#define ANSI_FONT_EFFECT_NOT_OVERLINED 55
// #define ANSI_FONT_EFFECT_ 56 NOT IN USE !
// #define ANSI_FONT_EFFECT_ 57 NOT IN USE !
// #define ANSI_FONT_EFFECT_ 58 NOT IN USE !
// #define ANSI_FONT_EFFECT_ 59 NOT IN USE !

#define ANSI_FONT_EFFECT_IDEOGRAM_UNDERLINE 60
#define ANSI_FONT_EFFECT_IDEOGRAM_DOUBLE_UNDERLINE 61
#define ANSI_FONT_EFFECT_IDEOGRAM_OVERLINE 62
#define ANSI_FONT_EFFECT_IDEOGRAM_DOUBLE_OVERLINE 63
#define ANSI_FONT_EFFECT_IDEOGRAM_STRESS_MARKING 64

#define ANSI_FONT_EFFECT_IDEOGRAM_ATTRIBUTES_OFF 65
// ===== ===== ===== ===== ===== ===== ===== ===== END Font effects ===== ===== ===== ===== ===== ===== ===== =====


// ===== ===== ===== ===== ===== ===== ===== ===== BEGIN Color set macros ===== ===== ===== ===== ===== ===== ===== =====
#ifndef ANSI_TEXT_COLOR_BLACK
#define ANSI_TEXT_COLOR_BLACK       ANSI_MAIN_ESC_SEQ(STRFICATION_STEP1(ANSI_FONT_EFFECT_FOREGROUND_COLOR_1))   ///< Black terminal text
#else
#error "The macro \"ANSI_TEXT_COLOR_BLACK\" is already defined !"
#endif /* ANSI_TEXT_COLOR_BLACK */
#ifndef ANSI_TEXT_COLOR_RED
#define ANSI_TEXT_COLOR_RED         ANSI_MAIN_ESC_SEQ(STRFICATION_STEP1(ANSI_FONT_EFFECT_FOREGROUND_COLOR_2))   ///< Red terminal text
#else
#error "The macro \"ANSI_TEXT_COLOR_RED\" is already defined !"
#endif /* ANSI_TEXT_COLOR_RED */
#ifndef ANSI_TEXT_COLOR_GREEN
#define ANSI_TEXT_COLOR_GREEN       ANSI_MAIN_ESC_SEQ(STRFICATION_STEP1(ANSI_FONT_EFFECT_FOREGROUND_COLOR_3))   ///< Green terminal text
#else
#error "The macro \"ANSI_TEXT_COLOR_GREEN\" is already defined !"
#endif /* ANSI_TEXT_COLOR_GREEN */
#ifndef ANSI_TEXT_COLOR_YELLOW
#define ANSI_TEXT_COLOR_YELLOW      ANSI_MAIN_ESC_SEQ(STRFICATION_STEP1(ANSI_FONT_EFFECT_FOREGROUND_COLOR_4))   ///< Yellow terminal text
#else
#error "The macro \"ANSI_TEXT_COLOR_YELLOW\" is already defined !"
#endif /* ANSI_TEXT_COLOR_YELLOW */

#ifndef ANSI_TEXT_COLOR_BLUE
#define ANSI_TEXT_COLOR_BLUE        ANSI_MAIN_ESC_SEQ(STRFICATION_STEP1(ANSI_FONT_EFFECT_FOREGROUND_COLOR_5))   ///< Blue terminal text
#else
#error "The macro \"ANSI_TEXT_COLOR_BLUE\" is already defined !"
#endif /* ANSI_TEXT_COLOR_BLUE */
#ifndef ANSI_TEXT_COLOR_MAGENTA
#define ANSI_TEXT_COLOR_MAGENTA     ANSI_MAIN_ESC_SEQ(STRFICATION_STEP1(ANSI_FONT_EFFECT_FOREGROUND_COLOR_6))   ///< Magenta terminal text
#else
#error "The macro \"ANSI_TEXT_COLOR_MAGENTA\" is already defined !"
#endif /* ANSI_TEXT_COLOR_MAGENTA */
#ifndef ANSI_TEXT_COLOR_CYAN
#define ANSI_TEXT_COLOR_CYAN        ANSI_MAIN_ESC_SEQ(STRFICATION_STEP1(ANSI_FONT_EFFECT_FOREGROUND_COLOR_7))   ///< Cyan terminal text
#else
#error "The macro \"ANSI_TEXT_COLOR_CYAN\" is already defined !"
#endif /* ANSI_TEXT_COLOR_CYAN */
#ifndef ANSI_TEXT_COLOR_WHITE
#define ANSI_TEXT_COLOR_WHITE       ANSI_MAIN_ESC_SEQ(STRFICATION_STEP1(ANSI_FONT_EFFECT_FOREGROUND_COLOR_8))   ///< White terminal text
#else
#error "The macro \"ANSI_TEXT_COLOR_WHITE\" is already defined !"
#endif /* ANSI_TEXT_COLOR_WHITE */
// ===== ===== ===== ===== ===== ===== ===== ===== END Color set macros ===== ===== ===== ===== ===== ===== ===== =====

#define ANSI_TEXT_BOLD              ANSI_MAIN_ESC_SEQ(STRFICATION_STEP1(ANSI_FONT_EFFECT_BOLD))
#define ANSI_TEXT_ITALIC            ANSI_MAIN_ESC_SEQ(STRFICATION_STEP1(ANSI_FONT_EFFECT_ITALIC))
#define ANSI_TEXT_UNDERLINE         ANSI_MAIN_ESC_SEQ(STRFICATION_STEP1(ANSI_FONT_EFFECT_UNDERLINE))
#define ANSI_TEXT_DOUBLE_UNDERLINE  ANSI_MAIN_ESC_SEQ(STRFICATION_STEP1(ANSI_FONT_EFFECT_DOUBLE_UNDERLINE))
#define ANSI_TEXT_SLOW_BLINK        ANSI_MAIN_ESC_SEQ(STRFICATION_STEP1(ANSI_FONT_EFFECT_SLOW_BLINK))
#define ANSI_TEXT_FAST_BLINK        ANSI_MAIN_ESC_SEQ(STRFICATION_STEP1(ANSI_FONT_EFFECT_RAPID_BLINK))

#define ANSI_TEXT_UNDERLINE_OFF     ANSI_MAIN_ESC_SEQ(STRFICATION_STEP1(ANSI_FONT_EFFECT_UNDERLINE_OFF))



#ifndef ANSI_RESET_ALL
#define ANSI_RESET_ALL ANSI_MAIN_ESC_SEQ(STRFICATION_STEP1(ANSI_FONT_EFFECT_RESET)) ///< Disable all attributes
#else
#error "The macro \"ANSI_RESET_ALL\" is already defined !"
#endif /* ANSI_RESET_ALL */



/**
 * @brief Small test whether the ANSI escape sequences are working.
 */
extern void Test_ANSI_Esc_Seq(void);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ANSI_ESC_SEQ_H */
