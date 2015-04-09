/* 
 * File:   lcd_display.h
 * Author: Matt Mongeon
 *
 * Created on April 9, 2015, 3:28 PM
 */

#ifndef INCLUDED_LCD_DISPLAY_H
#define	INCLUDED_LCD_DISPLAY_H


// Writes the parameter string to the display.
//
// Params:
// const char* str - the null-teriminated string to write.
void display_write_string(const char* str, int row);

#endif	/* INCLUDED_LCD_DISPLAY_H */

