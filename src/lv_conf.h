/**
 * @file lv_conf.h
 * Minimale LVGL 9 Konfiguration
 * ACHTUNG: Die meisten Einstellungen werden direkt über die platformio.ini gesteuert!
 */

#ifndef LV_CONF_H
#define LV_CONF_H

#ifndef __ASSEMBLER__
#include <stdint.h>
#endif

/*====================
   COLOR SETTINGS
 *====================*/
#define LV_COLOR_DEPTH 16

/*====================
   HAL SETTINGS
 *====================*/
/* Default display refresh, input device read and animation step period. */
#define LV_DEF_REFR_PERIOD 33 /*[ms]*/

/* Wir nutzen die Arduino millis() Funktion als internen LVGL-Timer */
#define LV_TICK_CUSTOM               1
#define LV_TICK_CUSTOM_INCLUDE       "Arduino.h"
#define LV_TICK_CUSTOM_SYS_TIME_EXPR (millis())

/*=======================
 * FEATURE CONFIGURATION
 *=======================*/
#define LV_USE_LOG 1
#if LV_USE_LOG
#define LV_LOG_LEVEL  LV_LOG_LEVEL_WARN
#define LV_LOG_PRINTF 1
#endif

/* Asserts (nur für Debugging sinnvoll, im Betrieb eher störend) */
#define LV_USE_ASSERT_NULL          1
#define LV_USE_ASSERT_MALLOC        1
#define LV_USE_ASSERT_STYLE         0
#define LV_USE_ASSERT_MEM_INTEGRITY 0
#define LV_USE_ASSERT_OBJ           0

/*==================
 * FONT USAGE
 *===================*/
/* * WICHTIG: Die Aktivierung der Schriften (LV_FONT_MONTSERRAT_24, etc.)
 * erfolgt EXKLUSIV über die platformio.ini, um Konflikte zu vermeiden!
 */
#ifndef __ASSEMBLER__
#define LV_FONT_DEFAULT &lv_font_montserrat_14
#endif

#define LV_USE_FONT_COMPRESSED  0
#define LV_USE_FONT_PLACEHOLDER 1

/*=================
 * TEXT SETTINGS
 *=================*/
#define LV_TXT_ENC         LV_TXT_ENC_UTF8
#define LV_TXT_BREAK_CHARS " ,.;:-_)]}"
#define LV_USE_BIDI        0

/*==================
 * WIDGETS
 *================*/
#define LV_USE_LABEL 1
#define LV_USE_IMAGE 1

/*==================
 * THEMES
 *==================*/
/*A simple, impressive and very complete theme*/
#define LV_USE_THEME_DEFAULT 1
#if LV_USE_THEME_DEFAULT
#define LV_THEME_DEFAULT_DARK            1
#define LV_THEME_DEFAULT_GROW            1
#define LV_THEME_DEFAULT_TRANSITION_TIME 80
#endif

/*==================
 * OPTIMIZATIONS (ESP32-S3 Specific)
 *==================*/
#define LV_USE_DRAW_SW_HELIUM 0
#define LV_USE_DRAW_SW_NEON   0
#define LV_USE_DRAW_SW_CUSTOM 0

#endif /*LV_CONF_H*/
