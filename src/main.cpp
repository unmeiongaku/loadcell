#include <Arduino.h>
#include "active_object.h"


typedef struct{
  uint8_t settingbutton;
  uint8_t backbutton;
  uint8_t increasebutton;
  uint8_t decreasebutton;
  uint8_t buttonglobal;
}button_t;

static button_t button;


void reset_button_value(button_t *button);
static uint8_t process_button_pad_value(uint8_t btn_pad_value);
static void proobject_event_dispatcher(proobject_t *const mobj,event_t const *const e);
static void dispatch_setting_sig(proobject_user_event_t *ue, uint8_t button);

static proobject_t A0s;

void setup(){
  proobject_init(&A0s);
}

void loop(){
  // put your main code here, to run repeatedly:uint16_t proevent;
  uint16_t proevent = 0;
  static uint32_t current_time = millis();
  static proobject_tick_event_t te;
  // put your main code here, to run repeatedly:
  proobject_user_event_t ue;

  button.settingbutton = digitalRead(BUTTON_SETTING_SIG);
  button.backbutton = digitalRead(BUTTON_BACK_SIG);
  button.increasebutton = digitalRead(BUTTON_INCREASE_SIG);
  button.decreasebutton = digitalRead(BUTTON_DECREASE_SIG);

  button.buttonglobal = button.decreasebutton << 3 | button.increasebutton <<2 | button.backbutton<<1 | button.settingbutton;

  button.buttonglobal = process_button_pad_value(button.buttonglobal);
  proevent = button.buttonglobal;

  /*Make Event*/
  switch(proevent){
    case SETTING_SIG_DEFINE:
    {
      /*dispatch settting sig*/
      dispatch_setting_sig(&ue,proevent);
      proobject_event_dispatcher(&A0s,&ue.super);
      reset_internal_signal();
    }
      break;
    case BACK_SIG_DEFINE:
    {
      ue.super.sig = BACK_SIG;
      proobject_event_dispatcher(&A0s,&ue.super);
    }
      break;
    case CW_SIG_DEFINE:
    {
      ue.super.sig = CW_SIG;
      proobject_event_dispatcher(&A0s,&ue.super);
    }
      break;
    case CCW_SIG_DEFINE:
    {
      ue.super.sig = CCW_SIG;
        proobject_event_dispatcher(&A0s,&ue.super);
    }
      break;
  }

  reset_button_value(&button);

  //4. dispatch the time tick event for every  100ms
  if(millis() - current_time  >= TIME_TICK_CYCLE_DEFINE){
#if ENABLE_TICK_LED == 1
    digitalWrite(LED_TICK_CALLBACK, !digitalRead(LED_TICK_CALLBACK));
#endif
    //100ms has passed
    current_time = millis();
    te.super.sig = TIME_TICK_SIG;
    if(++te.ss > 10) te.ss = 1;
      proobject_event_dispatcher(&A0s,&te.super);
  }
}


void reset_button_value(button_t *button){
  button->settingbutton = 0;
  button->backbutton = 0;
  button->increasebutton = 0;
  button->decreasebutton = 0;
}

static void proobject_event_dispatcher(proobject_t *const mobj,event_t const *const e){
  
  event_status_t status;
  proobject_state_t source, target;
  
  source = mobj->active_state;
  status = proobject_state_machine(mobj,e);

  if(status == EVENT_TRANSITION){
    target = mobj->active_state;
    event_t ee;
    //1. run the exit action for the source state
    ee.sig = EXIT;
    mobj->active_state = source;
    proobject_state_machine(mobj,&ee);

    //2. run the entry action for the target state
    ee.sig = ENTRY;
    mobj->active_state = target;
    proobject_state_machine(mobj,&ee);
  }
} 

static void dispatch_setting_sig(proobject_user_event_t *ue, uint8_t button){
   uint16_t internal_sig = (get_internal_signal()|button);
   switch(internal_sig){
    case SETTING_SIG_DEFINE:
        ue->super.sig = SETTING_SIG;
      break;
    case SPINT_DATA_SIG_DEFINE:
        ue->super.sig = SPRINT_DATA_SIG;
      break;
    case DISPLAY_DATA_SIG_DEFINE:
        ue->super.sig = DISPLAY_DATA_SIG;
      break;
    case CALIB_HX711_SIG_DEFINE:
        ue->super.sig = CALIB_HX711_SIG;
      break;
    case POSITIVE_PROPELLER_SIG_DEFINE:
        ue->super.sig = POSITIVE_PROPELLER_SIG;
      break;
    case NEGATIVE_PROPELLER_SIG_DEFINE:
        ue->super.sig = NEGATIVE_PROPELLER_SIG;
      break;
    case SET_SERIAL_BAUDRATE_SIG_DEFINE:
        ue->super.sig = SET_SERIAL_BAUDRATE_SIG;
      break;
   }
}


 static uint8_t process_button_pad_value(uint8_t btn_pad_value){
  static button_state_t btn_sm_state = NOT_PRESSED;
  static uint32_t curr_time = millis();

  switch(btn_sm_state){
    case NOT_PRESSED:
    {
      if(btn_pad_value){
        btn_sm_state = BOUNCE;
        curr_time = millis();
      }
    break;
    }
    case BOUNCE:{
    if(millis() - curr_time >= 40 ){
      //50ms has passed 
    if(btn_pad_value){
        btn_sm_state = PRESSED;
        return btn_pad_value;
    }
    else
          btn_sm_state = NOT_PRESSED;
    }
      break;
    case PRESSED:{
      if(!btn_pad_value){
        btn_sm_state = BOUNCE;
        curr_time = millis();
      }
      break;
    }
    }
  }
  return 0;
}