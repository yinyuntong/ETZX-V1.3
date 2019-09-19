#ifndef _DISPALY_H_
#define _DISPLAY_H_


#ifndef _IN_DISPLAY_
extern uchar xdata Display_data[64];
extern void LCD_WRITE_DATA(uchar write_data,uchar R_W);
extern void LCD_page(uchar page);
extern void read_code_number(uchar code_data,uchar positive);
extern void read_code_chinese(uchar code_data,uchar positive);
extern void show_number(uchar page,uchar y,uchar number,uchar positive);
extern void show_chinese(uchar page,uchar y,uchar number,uchar positive);
extern void clear_screen(void);
extern void clear_screen_line(uchar page,uchar positive);
extern void InitLCD(void);
extern void OpenLCDBLK(void);
extern void CloseLCDBLK(void);
extern void DisplayWelcome(void);
extern void DisplayMainMenu1(void);
extern void DisplayMainMenu2(void);
extern void DisplayMainMenu3(void);
extern void DisplayMainMenu4(void);
extern void DisplayMainMenu5(void);
extern void DisplayMainMenu6(void);
extern void DisplayMainMenu7(void);
extern void DisplayCallNumberMenu(uchar a,uchar b);

extern void DisplayFinger_HaveEmpty(void);
extern void DisplayEnter_OldPassword(void);
extern void DisplayEnter_NewPassword(void);
extern void DisplayEnter_Password(void);
extern void DisplayCheckPasswordError(void);
extern void DisplayCheckPasswordOK(void);
extern void DisplayModifyPasswordOK(void);
extern void DisplayEnterPhoneNumber(void);
extern void DisplayManagerFineger(void);
extern void DisplayFirstEnterFinger(void);
extern void DisplaySecondEnterFinger(void);
extern void DisplayEnterFingerOK(void);
extern void DisplayEnterFingerError(void);
extern void DisplayCheckFingerOK(void);
extern void DisplayCheckFingerError(void);
extern void DisplayRenewPassword(void);
extern void DisplayRenewFunction(void);
extern void DisplayCancelFunction(void);
extern void DisplayEnterPhoneNumberOK(void);
extern void DisplayNetOK(void);
extern void DisplayNetError(void);
extern void DisplayInputSec(void);
extern void DisplayInputRoom(void);
extern void DisplayInputBed(void);
extern void DisplayInputNumber(void);
extern void ClearInputSec(void);
#else
void LCD_WRITE_DATA(uchar write_data,uchar R_W);
void LCD_page(uchar page);
void read_code_number(uchar code_data,uchar positive);
void read_code_chinese(uchar code_data,uchar positive);
void show_number(uchar page,uchar y,uchar number,uchar positive);
void show_chinese(uchar page,uchar y,uchar number,uchar positive);
void clear_screen(void);
void clear_screen_line(uchar page,uchar positive);
void InitLCD(void);
void OpenLCDBLK(void);
void CloseLCDBLK(void);
void DisplayWelcome(void);
void DisplayMainMenu1(void);
void DisplayMainMenu2(void);
void DisplayMainMenu3(void);
void DisplayMainMenu4(void);
void DisplayMainMenu5(void);
void DisplayMainMenu6(void);
void DisplayMainMenu7(void);
void DisplayCallNumberMenu(uchar a,uchar b);

void DisplayFinger_HaveEmpty(void);
void DisplayEnter_OldPassword(void);
void DisplayEnter_NewPassword(void);
void DisplayEnter_Password(void);
void DisplayCheckPasswordError(void);
void DisplayCheckPasswordOK(void);
void DisplayModifyPasswordOK(void);
void DisplayEnterPhoneNumber(void);
void DisplayManagerFineger(void);
void DisplayFirstEnterFinger(void);
void DisplaySecondEnterFinger(void);
void DisplayEnterFingerOK(void);
void DisplayEnterFingerError(void);
void DisplayCheckFingerOK(void);
void DisplayCheckFingerError(void);
void DisplayRenewPassword(void);
void DisplayRenewFunction(void);
void DisplayCancelFunction(void);
void DisplayEnterPhoneNumberOK(void);
void DisplayNetOK(void);
void DisplayNetError(void);
void DisplayInputSec(void);
void DisplayInputRoom(void);
void DisplayInputBed(void);
void DisplayInputNumber(void);
#endif

#endif