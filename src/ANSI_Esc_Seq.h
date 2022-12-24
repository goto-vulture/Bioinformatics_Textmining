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
#ifndef ANSI_FONT_EFFECT_RESET
#define ANSI_FONT_EFFECT_RESET 0
#else
#error "The macro \"ANSI_FONT_EFFECT_RESET\" is already defined !"
#endif /* ANSI_FONT_EFFECT_RESET */
#ifndef ANSI_FONT_EFFECT_BOLD
#define ANSI_FONT_EFFECT_BOLD 1
#else
#error "The macro \"ANSI_FONT_EFFECT_RESET\" is already defined !"
#endif /* ANSI_FONT_EFFECT_RESET */
#ifndef ANSI_FONT_EFFECT_FAINT
#define ANSI_FONT_EFFECT_FAINT 2
#else
#error "The macro \"ANSI_FONT_EFFECT_FAINT\" is already defined !"
#endif /* ANSI_FONT_EFFECT_FAINT */
#ifndef ANSI_FONT_EFFECT_ITALIC
#define ANSI_FONT_EFFECT_ITALIC 3
#else
#error "The macro \"ANSI_FONT_EFFECT_ITALIC\" is already defined !"
#endif /* ANSI_FONT_EFFECT_ITALIC */
#ifndef ANSI_FONT_EFFECT_UNDERLINE
#define ANSI_FONT_EFFECT_UNDERLINE 4
#else
#error "The macro \"ANSI_FONT_EFFECT_UNDERLINE\" is already defined !"
#endif /* ANSI_FONT_EFFECT_UNDERLINE */
#ifndef ANSI_FONT_EFFECT_SLOW_BLINK
#define ANSI_FONT_EFFECT_SLOW_BLINK 5
#else
#error "The macro \"ANSI_FONT_EFFECT_SLOW_BLINK\" is already defined !"
#endif /* ANSI_FONT_EFFECT_SLOW_BLINK */
#ifndef ANSI_FONT_EFFECT_RAPID_BLINK
#define ANSI_FONT_EFFECT_RAPID_BLINK 6
#else
#error "The macro \"ANSI_FONT_EFFECT_RAPID_BLINK\" is already defined !"
#endif /* ANSI_FONT_EFFECT_RAPID_BLINK */
#ifndef ANSI_FONT_EFFECT_REVERSE_VIDEO
#define ANSI_FONT_EFFECT_REVERSE_VIDEO 7
#else
#error "The macro \"ANSI_FONT_EFFECT_REVERSE_VIDEO\" is already defined !"
#endif /* ANSI_FONT_EFFECT_REVERSE_VIDEO */
#ifndef ANSI_FONT_EFFECT_CONCEAL
#define ANSI_FONT_EFFECT_CONCEAL 8
#else
#error "The macro \"ANSI_FONT_EFFECT_CONCEAL\" is already defined !"
#endif /* ANSI_FONT_EFFECT_CONCEAL */
#ifndef ANSI_FONT_EFFECT_CROSSED_OUT
#define ANSI_FONT_EFFECT_CROSSED_OUT 9
#else
#error "The macro \"ANSI_FONT_EFFECT_CROSSED_OUT\" is already defined !"
#endif /* ANSI_FONT_EFFECT_CROSSED_OUT */

// Font selection
#ifndef ANSI_FONT_EFFECT_DEFAULT_FONT
#define ANSI_FONT_EFFECT_DEFAULT_FONT 10
#else
#error "The macro \"ANSI_FONT_EFFECT_DEFAULT_FONT\" is already defined !"
#endif /* ANSI_FONT_EFFECT_DEFAULT_FONT */
#ifndef ANSI_FONT_EFFECT_ALTERNATE_FONT_1
#define ANSI_FONT_EFFECT_ALTERNATE_FONT_1 11
#else
#error "The macro \"ANSI_FONT_EFFECT_ALTERNATE_FONT_1\" is already defined !"
#endif /* ANSI_FONT_EFFECT_ALTERNATE_FONT_1 */
#ifndef ANSI_FONT_EFFECT_ALTERNATE_FONT_2
#define ANSI_FONT_EFFECT_ALTERNATE_FONT_2 12
#else
#error "The macro \"ANSI_FONT_EFFECT_ALTERNATE_FONT_2\" is already defined !"
#endif /* ANSI_FONT_EFFECT_ALTERNATE_FONT_2 */
#ifndef ANSI_FONT_EFFECT_ALTERNATE_FONT_3
#define ANSI_FONT_EFFECT_ALTERNATE_FONT_3 13
#else
#error "The macro \"ANSI_FONT_EFFECT_ALTERNATE_FONT_3\" is already defined !"
#endif /* ANSI_FONT_EFFECT_ALTERNATE_FONT_3 */
#ifndef ANSI_FONT_EFFECT_ALTERNATE_FONT_4
#define ANSI_FONT_EFFECT_ALTERNATE_FONT_4 14
#else
#error "The macro \"ANSI_FONT_EFFECT_ALTERNATE_FONT_4\" is already defined !"
#endif /* ANSI_FONT_EFFECT_ALTERNATE_FONT_4 */
#ifndef ANSI_FONT_EFFECT_ALTERNATE_FONT_5
#define ANSI_FONT_EFFECT_ALTERNATE_FONT_5 15
#else
#error "The macro \"ANSI_FONT_EFFECT_ALTERNATE_FONT_5\" is already defined !"
#endif /* ANSI_FONT_EFFECT_ALTERNATE_FONT_5 */
#ifndef ANSI_FONT_EFFECT_ALTERNATE_FONT_6
#define ANSI_FONT_EFFECT_ALTERNATE_FONT_6 16
#else
#error "The macro \"ANSI_FONT_EFFECT_ALTERNATE_FONT_6\" is already defined !"
#endif /* ANSI_FONT_EFFECT_ALTERNATE_FONT_6 */
#ifndef ANSI_FONT_EFFECT_ALTERNATE_FONT_7
#define ANSI_FONT_EFFECT_ALTERNATE_FONT_7 17
#else
#error "The macro \"ANSI_FONT_EFFECT_ALTERNATE_FONT_7\" is already defined !"
#endif /* ANSI_FONT_EFFECT_ALTERNATE_FONT_7 */
#ifndef ANSI_FONT_EFFECT_ALTERNATE_FONT_8
#define ANSI_FONT_EFFECT_ALTERNATE_FONT_8 18
#else
#error "The macro \"ANSI_FONT_EFFECT_ALTERNATE_FONT_8\" is already defined !"
#endif /* ANSI_FONT_EFFECT_ALTERNATE_FONT_8 */
#ifndef ANSI_FONT_EFFECT_ALTERNATE_FONT_9
#define ANSI_FONT_EFFECT_ALTERNATE_FONT_9 19
#else
#error "The macro \"ANSI_FONT_EFFECT_ALTERNATE_FONT_9\" is already defined !"
#endif /* ANSI_FONT_EFFECT_ALTERNATE_FONT_9 */

#ifndef ANSI_FONT_EFFECT_FRAKTUR
#define ANSI_FONT_EFFECT_FRAKTUR 20
#else
#error "The macro \"ANSI_FONT_EFFECT_FRAKTUR\" is already defined !"
#endif /* ANSI_FONT_EFFECT_FRAKTUR */
#ifndef ANSI_FONT_EFFECT_DOUBLE_UNDERLINE
#define ANSI_FONT_EFFECT_DOUBLE_UNDERLINE 21
#else
#error "The macro \"ANSI_FONT_EFFECT_DOUBLE_UNDERLINE\" is already defined !"
#endif /* ANSI_FONT_EFFECT_DOUBLE_UNDERLINE */
#ifndef ANSI_FONT_EFFECT_NORMAL_COLOR_OR_INTENSITY
#define ANSI_FONT_EFFECT_NORMAL_COLOR_OR_INTENSITY 22
#else
#error "The macro \"ANSI_FONT_EFFECT_NORMAL_COLOR_OR_INTENSITY\" is already defined !"
#endif /* ANSI_FONT_EFFECT_NORMAL_COLOR_OR_INTENSITY */
#ifndef ANSI_FONT_EFFECT_NOT_ITALIC_NOT_FRAKTUR
#define ANSI_FONT_EFFECT_NOT_ITALIC_NOT_FRAKTUR 23
#else
#error "The macro \"ANSI_FONT_EFFECT_NOT_ITALIC_NOT_FRAKTUR\" is already defined !"
#endif /* ANSI_FONT_EFFECT_NOT_ITALIC_NOT_FRAKTUR */
#ifndef ANSI_FONT_EFFECT_UNDERLINE_OFF
#define ANSI_FONT_EFFECT_UNDERLINE_OFF 24
#else
#error "The macro \"ANSI_FONT_EFFECT_UNDERLINE_OFF\" is already defined !"
#endif /* ANSI_FONT_EFFECT_UNDERLINE_OFF */

#ifndef ANSI_FONT_EFFECT_BLINK_OFF
#define ANSI_FONT_EFFECT_BLINK_OFF 25
#else
#error "The macro \"ANSI_FONT_EFFECT_BLINK_OFF\" is already defined !"
#endif /* ANSI_FONT_EFFECT_BLINK_OFF */
// #define ANSI_FONT_EFFECT_ 26 NOT IN USE !
#ifndef ANSI_FONT_EFFECT_INVERSE_OFF
#define ANSI_FONT_EFFECT_INVERSE_OFF 27
#else
#error "The macro \"ANSI_FONT_EFFECT_INVERSE_OFF\" is already defined !"
#endif /* ANSI_FONT_EFFECT_INVERSE_OFF */
#ifndef ANSI_FONT_EFFECT_REVEAL
#define ANSI_FONT_EFFECT_REVEAL 28
#else
#error "The macro \"ANSI_FONT_EFFECT_REVEAL\" is already defined !"
#endif /* ANSI_FONT_EFFECT_REVEAL */
#ifndef ANSI_FONT_EFFECT_NOT_CROSSED_OUT
#define ANSI_FONT_EFFECT_NOT_CROSSED_OUT 29
#else
#error "The macro \"ANSI_FONT_EFFECT_NOT_CROSSED_OUT\" is already defined !"
#endif /* ANSI_FONT_EFFECT_NOT_CROSSED_OUT */

#ifndef ANSI_FONT_EFFECT_FOREGROUND_COLOR_1
#define ANSI_FONT_EFFECT_FOREGROUND_COLOR_1 30
#else
#error "The macro \"ANSI_FONT_EFFECT_FOREGROUND_COLOR_1\" is already defined !"
#endif /* ANSI_FONT_EFFECT_FOREGROUND_COLOR_1 */
#ifndef ANSI_FONT_EFFECT_FOREGROUND_COLOR_2
#define ANSI_FONT_EFFECT_FOREGROUND_COLOR_2 31
#else
#error "The macro \"ANSI_FONT_EFFECT_FOREGROUND_COLOR_2\" is already defined !"
#endif /* ANSI_FONT_EFFECT_FOREGROUND_COLOR_2 */
#ifndef ANSI_FONT_EFFECT_FOREGROUND_COLOR_3
#define ANSI_FONT_EFFECT_FOREGROUND_COLOR_3 32
#else
#error "The macro \"ANSI_FONT_EFFECT_FOREGROUND_COLOR_3\" is already defined !"
#endif /* ANSI_FONT_EFFECT_FOREGROUND_COLOR_3 */
#ifndef ANSI_FONT_EFFECT_FOREGROUND_COLOR_4
#define ANSI_FONT_EFFECT_FOREGROUND_COLOR_4 33
#else
#error "The macro \"ANSI_FONT_EFFECT_FOREGROUND_COLOR_4\" is already defined !"
#endif /* ANSI_FONT_EFFECT_FOREGROUND_COLOR_4 */
#ifndef ANSI_FONT_EFFECT_FOREGROUND_COLOR_5
#define ANSI_FONT_EFFECT_FOREGROUND_COLOR_5 34
#else
#error "The macro \"ANSI_FONT_EFFECT_FOREGROUND_COLOR_5\" is already defined !"
#endif /* ANSI_FONT_EFFECT_FOREGROUND_COLOR_5 */
#ifndef ANSI_FONT_EFFECT_FOREGROUND_COLOR_6
#define ANSI_FONT_EFFECT_FOREGROUND_COLOR_6 35
#else
#error "The macro \"ANSI_FONT_EFFECT_FOREGROUND_COLOR_6\" is already defined !"
#endif /* ANSI_FONT_EFFECT_FOREGROUND_COLOR_6 */
#ifndef ANSI_FONT_EFFECT_FOREGROUND_COLOR_7
#define ANSI_FONT_EFFECT_FOREGROUND_COLOR_7 36
#else
#error "The macro \"ANSI_FONT_EFFECT_FOREGROUND_COLOR_7\" is already defined !"
#endif /* ANSI_FONT_EFFECT_FOREGROUND_COLOR_7 */
#ifndef ANSI_FONT_EFFECT_FOREGROUND_COLOR_8
#define ANSI_FONT_EFFECT_FOREGROUND_COLOR_8 37
#else
#error "The macro \"ANSI_FONT_EFFECT_FOREGROUND_COLOR_8\" is already defined !"
#endif /* ANSI_FONT_EFFECT_FOREGROUND_COLOR_8 */

#ifndef ANSI_FONT_EFFECT_SET_FOREGROUND_COLOR
#define ANSI_FONT_EFFECT_SET_FOREGROUND_COLOR 38
#else
#error "The macro \"ANSI_FONT_EFFECT_SET_FOREGROUND_COLOR\" is already defined !"
#endif /* ANSI_FONT_EFFECT_SET_FOREGROUND_COLOR */
#ifndef ANSI_FONT_EFFECT_DEFAULT_FOREGROUND_COLOR
#define ANSI_FONT_EFFECT_DEFAULT_FOREGROUND_COLOR 39
#else
#error "The macro \"ANSI_FONT_EFFECT_DEFAULT_FOREGROUND_COLOR\" is already defined !"
#endif /* ANSI_FONT_EFFECT_DEFAULT_FOREGROUND_COLOR */

#ifndef ANSI_FONT_EFFECT_BACKGROUND_COLOR_1
#define ANSI_FONT_EFFECT_BACKGROUND_COLOR_1 40
#else
#error "The macro \"ANSI_FONT_EFFECT_BACKGROUND_COLOR_1\" is already defined !"
#endif /* ANSI_FONT_EFFECT_BACKGROUND_COLOR_1 */
#ifndef ANSI_FONT_EFFECT_BACKGROUND_COLOR_2
#define ANSI_FONT_EFFECT_BACKGROUND_COLOR_2 41
#else
#error "The macro \"ANSI_FONT_EFFECT_BACKGROUND_COLOR_2\" is already defined !"
#endif /* ANSI_FONT_EFFECT_BACKGROUND_COLOR_2 */
#ifndef ANSI_FONT_EFFECT_BACKGROUND_COLOR_3
#define ANSI_FONT_EFFECT_BACKGROUND_COLOR_3 42
#else
#error "The macro \"ANSI_FONT_EFFECT_BACKGROUND_COLOR_3\" is already defined !"
#endif /* ANSI_FONT_EFFECT_BACKGROUND_COLOR_3 */
#ifndef ANSI_FONT_EFFECT_BACKGROUND_COLOR_4
#define ANSI_FONT_EFFECT_BACKGROUND_COLOR_4 43
#else
#error "The macro \"ANSI_FONT_EFFECT_BACKGROUND_COLOR_4\" is already defined !"
#endif /* ANSI_FONT_EFFECT_BACKGROUND_COLOR_4 */
#ifndef ANSI_FONT_EFFECT_BACKGROUND_COLOR_5
#define ANSI_FONT_EFFECT_BACKGROUND_COLOR_5 44
#else
#error "The macro \"ANSI_FONT_EFFECT_BACKGROUND_COLOR_5\" is already defined !"
#endif /* ANSI_FONT_EFFECT_BACKGROUND_COLOR_5 */
#ifndef ANSI_FONT_EFFECT_BACKGROUND_COLOR_6
#define ANSI_FONT_EFFECT_BACKGROUND_COLOR_6 45
#else
#error "The macro \"ANSI_FONT_EFFECT_BACKGROUND_COLOR_6\" is already defined !"
#endif /* ANSI_FONT_EFFECT_BACKGROUND_COLOR_6 */
#ifndef ANSI_FONT_EFFECT_BACKGROUND_COLOR_7
#define ANSI_FONT_EFFECT_BACKGROUND_COLOR_7 46
#else
#error "The macro \"ANSI_FONT_EFFECT_BACKGROUND_COLOR_7\" is already defined !"
#endif /* ANSI_FONT_EFFECT_BACKGROUND_COLOR_7 */
#ifndef ANSI_FONT_EFFECT_BACKGROUND_COLOR_8
#define ANSI_FONT_EFFECT_BACKGROUND_COLOR_8 47
#else
#error "The macro \"ANSI_FONT_EFFECT_BACKGROUND_COLOR_8\" is already defined !"
#endif /* ANSI_FONT_EFFECT_BACKGROUND_COLOR_8 */

#ifndef ANSI_FONT_EFFECT_SET_BACKGROUND_COLOR
#define ANSI_FONT_EFFECT_SET_BACKGROUND_COLOR 48
#else
#error "The macro \"ANSI_FONT_EFFECT_SET_BACKGROUND_COLOR\" is already defined !"
#endif /* ANSI_FONT_EFFECT_SET_BACKGROUND_COLOR */
#ifndef ANSI_FONT_EFFECT_DEFAULT_BACKGROUND_COLOR
#define ANSI_FONT_EFFECT_DEFAULT_BACKGROUND_COLOR 49
#else
#error "The macro \"ANSI_FONT_EFFECT_DEFAULT_BACKGROUND_COLOR\" is already defined !"
#endif /* ANSI_FONT_EFFECT_DEFAULT_BACKGROUND_COLOR */

// #define ANSI_FONT_EFFECT_ 50 NOT IN USE !
#ifndef ANSI_FONT_EFFECT_FRAMED
#define ANSI_FONT_EFFECT_FRAMED 51
#else
#error "The macro \"ANSI_FONT_EFFECT_FRAMED\" is already defined !"
#endif /* ANSI_FONT_EFFECT_FRAMED */
#ifndef ANSI_FONT_EFFECT_ENCIRCLED
#define ANSI_FONT_EFFECT_ENCIRCLED 52
#else
#error "The macro \"ANSI_FONT_EFFECT_ENCIRCLED\" is already defined !"
#endif /* ANSI_FONT_EFFECT_ENCIRCLED */
#ifndef ANSI_FONT_EFFECT_OVERLINED
#define ANSI_FONT_EFFECT_OVERLINED 53
#else
#error "The macro \"ANSI_FONT_EFFECT_OVERLINED\" is already defined !"
#endif /* ANSI_FONT_EFFECT_OVERLINED */
#ifndef ANSI_FONT_EFFECT_NOT_FRAMED_OR_ENCIRCLED
#define ANSI_FONT_EFFECT_NOT_FRAMED_OR_ENCIRCLED 54
#else
#error "The macro \"ANSI_FONT_EFFECT_NOT_FRAMED_OR_ENCIRCLED\" is already defined !"
#endif /* ANSI_FONT_EFFECT_NOT_FRAMED_OR_ENCIRCLED */

#ifndef ANSI_FONT_EFFECT_NOT_OVERLINED
#define ANSI_FONT_EFFECT_NOT_OVERLINED 55
#else
#error "The macro \"ANSI_FONT_EFFECT_NOT_OVERLINED\" is already defined !"
#endif /* ANSI_FONT_EFFECT_NOT_OVERLINED */
// #define ANSI_FONT_EFFECT_ 56 NOT IN USE !
// #define ANSI_FONT_EFFECT_ 57 NOT IN USE !
// #define ANSI_FONT_EFFECT_ 58 NOT IN USE !
// #define ANSI_FONT_EFFECT_ 59 NOT IN USE !

#ifndef ANSI_FONT_EFFECT_IDEOGRAM_UNDERLINE
#define ANSI_FONT_EFFECT_IDEOGRAM_UNDERLINE 60
#else
#error "The macro \"ANSI_FONT_EFFECT_IDEOGRAM_UNDERLINE\" is already defined !"
#endif /* ANSI_FONT_EFFECT_IDEOGRAM_UNDERLINE */
#ifndef ANSI_FONT_EFFECT_IDEOGRAM_DOUBLE_UNDERLINE
#define ANSI_FONT_EFFECT_IDEOGRAM_DOUBLE_UNDERLINE 61
#else
#error "The macro \"ANSI_FONT_EFFECT_IDEOGRAM_DOUBLE_UNDERLINE\" is already defined !"
#endif /* ANSI_FONT_EFFECT_IDEOGRAM_DOUBLE_UNDERLINE */
#ifndef ANSI_FONT_EFFECT_IDEOGRAM_OVERLINE
#define ANSI_FONT_EFFECT_IDEOGRAM_OVERLINE 62
#else
#error "The macro \"ANSI_FONT_EFFECT_IDEOGRAM_OVERLINE\" is already defined !"
#endif /* ANSI_FONT_EFFECT_IDEOGRAM_OVERLINE */
#ifndef ANSI_FONT_EFFECT_IDEOGRAM_DOUBLE_OVERLINE
#define ANSI_FONT_EFFECT_IDEOGRAM_DOUBLE_OVERLINE 63
#else
#error "The macro \"ANSI_FONT_EFFECT_IDEOGRAM_DOUBLE_OVERLINE\" is already defined !"
#endif /* ANSI_FONT_EFFECT_IDEOGRAM_DOUBLE_OVERLINE */
#ifndef ANSI_FONT_EFFECT_IDEOGRAM_STRESS_MARKING
#define ANSI_FONT_EFFECT_IDEOGRAM_STRESS_MARKING 64
#else
#error "The macro \"ANSI_FONT_EFFECT_IDEOGRAM_STRESS_MARKING\" is already defined !"
#endif /* ANSI_FONT_EFFECT_IDEOGRAM_STRESS_MARKING */
#ifndef ANSI_FONT_EFFECT_IDEOGRAM_ATTRIBUTES_OFF
#define ANSI_FONT_EFFECT_IDEOGRAM_ATTRIBUTES_OFF 65
#else
#error "The macro \"ANSI_FONT_EFFECT_IDEOGRAM_ATTRIBUTES_OFF\" is already defined !"
#endif /* ANSI_FONT_EFFECT_IDEOGRAM_ATTRIBUTES_OFF */
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

// ===== ===== ===== ===== ===== ===== ===== ===== BEGIN Main ANSI set macros ===== ===== ===== ===== ===== ===== ===== =====
#ifndef ANSI_TEXT_BOLD
#define ANSI_TEXT_BOLD              ANSI_MAIN_ESC_SEQ(STRFICATION_STEP1(ANSI_FONT_EFFECT_BOLD))                 ///< Bold the text
#else
#error "The macro \"ANSI_TEXT_BOLD\" is already defined !"
#endif /* ANSI_TEXT_BOLD */
#ifndef ANSI_TEXT_ITALIC
#define ANSI_TEXT_ITALIC            ANSI_MAIN_ESC_SEQ(STRFICATION_STEP1(ANSI_FONT_EFFECT_ITALIC))               ///< Use an italic effect for the text
#else
#error "The macro \"ANSI_TEXT_ITALIC\" is already defined !"
#endif /* ANSI_TEXT_ITALIC */
#ifndef ANSI_TEXT_UNDERLINE
#define ANSI_TEXT_UNDERLINE         ANSI_MAIN_ESC_SEQ(STRFICATION_STEP1(ANSI_FONT_EFFECT_UNDERLINE))            ///< Single underline the text
#else
#error "The macro \"ANSI_TEXT_UNDERLINE\" is already defined !"
#endif /* ANSI_TEXT_UNDERLINE */
#ifndef ANSI_TEXT_DOUBLE_UNDERLINE
#define ANSI_TEXT_DOUBLE_UNDERLINE  ANSI_MAIN_ESC_SEQ(STRFICATION_STEP1(ANSI_FONT_EFFECT_DOUBLE_UNDERLINE))     ///< Double underline the text
#else
#error "The macro \"ANSI_TEXT_DOUBLE_UNDERLINE\" is already defined !"
#endif /* ANSI_TEXT_DOUBLE_UNDERLINE */
#ifndef ANSI_TEXT_SLOW_BLINK
#define ANSI_TEXT_SLOW_BLINK        ANSI_MAIN_ESC_SEQ(STRFICATION_STEP1(ANSI_FONT_EFFECT_SLOW_BLINK))           ///< Let the text slow blink
#else
#error "The macro \"ANSI_TEXT_SLOW_BLINK\" is already defined !"
#endif /* ANSI_TEXT_SLOW_BLINK */
#ifndef ANSI_TEXT_FAST_BLINK
#define ANSI_TEXT_FAST_BLINK        ANSI_MAIN_ESC_SEQ(STRFICATION_STEP1(ANSI_FONT_EFFECT_RAPID_BLINK))          ///< Let the text rapid blink
#else
#error "The macro \"ANSI_TEXT_FAST_BLINK\" is already defined !"
#endif /* ANSI_TEXT_FAST_BLINK */
// ===== ===== ===== ===== ===== ===== ===== ===== END Main ANSI set macros ===== ===== ===== ===== ===== ===== ===== ====

// ===== ===== ===== ===== ===== ===== ===== ===== BEGIN Reset set macros ===== ===== ===== ===== ===== ===== ===== =====
/**
 * @brief Disable all attributes.
 *
 * This is the normal way to reset attributes. The other macros can only reset specific attributes.
 */
#ifndef ANSI_RESET_ALL
#define ANSI_RESET_ALL ANSI_MAIN_ESC_SEQ(STRFICATION_STEP1(ANSI_FONT_EFFECT_RESET))
#else
#error "The macro \"ANSI_RESET_ALL\" is already defined !"
#endif /* ANSI_RESET_ALL */

#ifndef ANSI_TEXT_NORMAL_COLOR_OR_INTENSITY
#define ANSI_TEXT_NORMAL_COLOR_OR_INTENSITY ANSI_MAIN_ESC_SEQ(STRFICATION_STEP1(ANSI_FONT_EFFECT_NORMAL_COLOR_OR_INTENSITY))    ///< Disable user-defined color or intensity
#else
#error "The macro \"ANSI_TEXT_NORMAL_COLOR_OR_INTENSITY\" is already defined !"
#endif /* ANSI_TEXT_NORMAL_COLOR_OR_INTENSITY */
#ifndef ANSI_TEXT_NOT_ITALIC_NOT_FRAKTUR
#define ANSI_TEXT_NOT_ITALIC_NOT_FRAKTUR    ANSI_MAIN_ESC_SEQ(STRFICATION_STEP1(ANSI_FONT_EFFECT_NOT_ITALIC_NOT_FRAKTUR))       ///< Disable the fraktur property
#else
#error "The macro \"ANSI_TEXT_NOT_ITALIC_NOT_FRAKTUR\" is already defined !"
#endif /* ANSI_TEXT_NOT_ITALIC_NOT_FRAKTUR */
#ifndef ANSI_TEXT_INVERSE_OFF
#define ANSI_TEXT_INVERSE_OFF   ANSI_MAIN_ESC_SEQ(STRFICATION_STEP1(ANSI_FONT_EFFECT_INVERSE_OFF))                              ///< Disable the inverse property
#else
#error "The macro \"ANSI_TEXT_INVERSE_OFF\" is already defined !"
#endif /* ANSI_TEXT_INVERSE_OFF */
#ifndef ANSI_TEXT_NOT_CROSSED_OUT
#define ANSI_TEXT_NOT_CROSSED_OUT   ANSI_MAIN_ESC_SEQ(STRFICATION_STEP1(ANSI_FONT_EFFECT_NOT_CROSSED_OUT))                      ///< Disable the crossed out property
#else
#error "The macro \"ANSI_TEXT_NOT_CROSSED_OUT\" is already defined !"
#endif /* ANSI_TEXT_NOT_CROSSED_OUT */
#ifndef ANSI_TEXT_NOT_FRAMED_OR_ENCIRCLED
#define ANSI_TEXT_NOT_FRAMED_OR_ENCIRCLED   ANSI_MAIN_ESC_SEQ(STRFICATION_STEP1(ANSI_FONT_EFFECT_NOT_FRAMED_OR_ENCIRCLED))      ///< Disable the framed and the encircled property
#else
#error "The macro \"ANSI_TEXT_NOT_FRAMED_OR_ENCIRCLED\" is already defined !"
#endif /* ANSI_TEXT_NOT_FRAMED_OR_ENCIRCLED */
#ifndef ANSI_TEXT_NOT_OVERLINED
#define ANSI_TEXT_NOT_OVERLINED ANSI_MAIN_ESC_SEQ(STRFICATION_STEP1(ANSI_FONT_EFFECT_NOT_OVERLINED))                            ///< Disable the overline property
#else
#error "The macro \"ANSI_TEXT_NOT_OVERLINED\" is already defined !"
#endif /* ANSI_TEXT_NOT_OVERLINED */
#ifndef ANSI_TEXT_IDEOGRAM_ATTRIBUTES_OFF
#define ANSI_TEXT_IDEOGRAM_ATTRIBUTES_OFF   ANSI_MAIN_ESC_SEQ(STRFICATION_STEP1(ANSI_FONT_EFFECT_IDEOGRAM_ATTRIBUTES_OFF))      ///< Disable all ideogram attributes
#else
#error "The macro \"ANSI_TEXT_IDEOGRAM_ATTRIBUTES_OFF\" is already defined !"
#endif /* ANSI_TEXT_IDEOGRAM_ATTRIBUTES_OFF */
#ifndef ANSI_TEXT_UNDERLINE_OFF
#define ANSI_TEXT_UNDERLINE_OFF     ANSI_MAIN_ESC_SEQ(STRFICATION_STEP1(ANSI_FONT_EFFECT_UNDERLINE_OFF))                        ///< Disable any underline effects
#else
#error "The macro \"ANSI_TEXT_UNDERLINE_OFF\" is already defined !"
#endif /* ANSI_TEXT_UNDERLINE_OFF */
// ===== ===== ===== ===== ===== ===== ===== ===== END Reset set macros ===== ===== ===== ===== ===== ===== ===== =====



/**
 * @brief Small test whether the main ANSI escape sequences (different colors and font changes like bold and italic) are
 * working.
 */
extern void Test_ANSI_Main_Esc_Seq(void);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ANSI_ESC_SEQ_H */
