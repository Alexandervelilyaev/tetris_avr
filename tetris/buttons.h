/*
 * buttons.h
 *
 * Created: 26.09.2018 22:03:07
 *  Author: Alexander Velilyaev
 */ 


#ifndef BUTTONS_H_
#define BUTTONS_H_

#define BTN_UP 0
#define BTN_LEFT 1
#define BTN_RIGHT 2
#define BTN_DOWN 3

void OnButtonUp_Press();
void OnButtonUp_Unpress();
void OnButtonDown_Press();
void OnButtonDown_Unpress();
void OnButtonLeft_Press();
void OnButtonLeft_Unpress();
void OnButtonRight_Press();
void OnButtonRight_Unpress();

void InitButtons();

void PollButtons();

#endif /* BUTTONS_H_ */