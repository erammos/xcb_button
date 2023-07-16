/**
 * @file gui.h
 * @brief Demo - Functions prototypes for the gui module
 *
 * Contains functions for window creation, drawing text and button
 *
 * @author Elias Rammos <rammoselias@gmail.com>
 * @bug Quits if 6x13 font, not found
 * @version 0.0.1
 */

#ifndef GUI_H
#define GUI_H

#ifdef gui_IMPORT
#define EXTERN
#else
#define EXTERN extern
#endif

#include <stdbool.h>
#include <stdint.h>

typedef struct gui_window gui_window;
typedef struct gui_event
{
  uint32_t type;
} gui_event;

/** @brief Destroy window
 *
 *  Disconnect from Xorg server and free resources
 *
 *  @param * gui_window
 *  @return void
 */
EXTERN void gui_dispose_window (void *);

/** @brief Create window
 *
 *  Connect to Xorg server, allocates gui_window
 *  and creates Xcb graphic context for the window;
 *
 *  @param x x position of the window relative to parent
 *  @param y y position of the window relative to parent
 *  @param width width of the window
 *  @param height height of the window
 *
 *  @return * gui_window
 */
EXTERN gui_window *gui_create_window (uint32_t x, uint32_t y, uint32_t width,
                                      uint32_t height);
/** @brief Returns true, if ESC pressed
 *
 *  XCB Event Polling for XCB_KEY_RELEASE
 *
 *  @param * gui_window
 *
 *  @return bool
 */
EXTERN bool gui_quit (void *);

/** @brief Flushes the connection with the Xorg server
 *
 *  @param * gui_window
 *
 *  @return void
 */
EXTERN void gui_update_window (void *);

/** @brief Draws a button with a text and returns true/false if clicked
 *
 *  Listen to xcb button (pressed/released )events
 *  and changes the size of the button accordingly
 *  Creates a text with a fixed font (6x13) in the center of the button
 *
 *  @param * gui_window
 *  @param x x position relative to window
 *  @param y y position relative to window
 *  @param width width of the button
 *  @param height height of the button
 *  @param text centered in the button
 *
 *  @return bool true if it's pressed
 */
EXTERN bool gui_button (void *p, uint32_t x, uint32_t y, uint32_t width,
                        uint32_t height, const char *text);
/** @brief Draws a text
 *
 *  Creates a text with the specified font
 *  with white background
 *
 *  @param * gui_window
 *  @param x x position relative to window
 *  @param y y position relative to window
 *  @param font the name of the font
 *  @param text the drawing text
 *
 *  @return void
 */
EXTERN void gui_text (void *p, uint32_t x, uint32_t y, const char *font,
                      const char *text);

#undef EXTERN
#endif
