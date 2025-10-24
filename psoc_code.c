 #include "project.h"
#include <string.h> // For strcmp, memset
#define MAX_LENGTH 10
 // Max password length + null terminator
 char receivedPassword[MAX_LENGTH];
 const char correctPassword[] = "1234"; // The correct password
 uint8 pwIndex = 0;
 // Current index in receivedPassword buffer
 // Function to check the entered password against the correct one
void CheckPassword()
{
LCD_ClearDisplay();
// Compare received string with the correct password
if (strcmp(receivedPassword, correctPassword) == 0)
{
LCD_Position(0, 0);
LCD_PrintString("Correct Password");
}
else
{
LCD_Position(0, 0);
LCD_PrintString("Wrong Password");
}
// Null-terminate just in case (already done before calling, but safe)
receivedPassword[pwIndex] = ’\0’;
}

 // Function to reset the LCD display for new entry
void ResetLCD()
{
LCD_ClearDisplay();
LCD_Position(0, 0);
LCD_PrintString("Enter password:");
LCD_Position(1, 0);
LCD_PrintString(" "); // Clear line 2 thoroughly
LCD_Position(1, 0); // Reset cursor to start of line 2
}

 int main(void)
 {
   CyGlobalIntEnable; /* Enable global interrupts. */

 // Start Components
 LCD_Start();
 USBUART_Start(0, USBUART_5V_OPERATION); // Device 0, relevant for USB VBUS check

 ResetLCD(); // Initial display setup

 for(;;) // Main application loop
 {
 // Wait for USB enumeration and configuration
 if (USBUART_IsConfigurationChanged())
 {
 // Initialize CDC Buffers (Required after configuration change)
 if (USBUART_GetConfiguration())
 {
 USBUART_CDC_Init();
 }
 }

 // Process incoming data only when USB is configured and ready
 if (USBUART_GetConfiguration())
 {
 if (USBUART_DataIsReady())
 {
 uint8 ch = USBUART_GetChar(); // Read one character

 // Check for Enter key (newline or carriage return)
 if (ch == ’\n’ || ch == ’\r’)
 {
 receivedPassword[pwIndex] = ’\0’; // Null-terminate the string
CheckPassword(); // Check if it’s correct
CyDelay(2000); // Display result for 2 seconds

 // Reset for next password entry
pwIndex = 0;
 memset(receivedPassword, 0, MAX_LENGTH); // Clear passwordbuffer
ResetLCD(); // Reset display
}
// Check for Backspace character (ASCII 8)
else if (ch == 8)
{
if (pwIndex > 0) // Only backspace if there are characters
{
pwIndex--; // Move index back
receivedPassword[pwIndex] = ’\0’; // Erase last character position

 // Update LCD display
 LCD_Position(1, 0);
 LCD_PrintString(" "); // Clear the line
 LCD_Position(1, 0);
 LCD_PrintString(receivedPassword); // Print updated string
 }
 }
 // Handle printable characters (Digits in this case)
 // Check buffer bounds and basic ASCII range for safety
 else if (pwIndex < (MAX_LENGTH- 1) && ch >= 32 && ch <= 126)
  {
 receivedPassword[pwIndex++] = ch; // Add character to buffer, increment index
 receivedPassword[pwIndex] = ’\0’; // Keep string null terminated
 // Update LCD- Overwrite previous content on line 2
 // Clearing first might cause flicker, direct print often ok
 LCD_Position(1, 0);
 LCD_PrintString("
 first
 "); // Clear the line
 } // end main
 LCD_Position(1, 0);
 LCD_PrintString(receivedPassword); // Print current buffer
 content
 }
 // Ignore characters if buffer is full or character is invalid
 }
 } // end if(USBUART_GetConfiguration())
 } // end for(;;)