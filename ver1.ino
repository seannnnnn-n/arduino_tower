#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd_I2C_27(0x27,16,2);

/*notes
char(0) = 48
button4 = right
button7 = act
button8 = left
*/

/*---index declare---*/
bool buttonPress4 = false;
bool buttonPress7 = false;
bool buttonPress8 = false;
bool buttonAction4 = false;
bool buttonAction7 = false;
bool buttonAction8 = false;

char _pos1[] = {'1','2','3','4','5','_'};
char _pos2[] = {' ',' ',' ',' ',' ','_'};
char _pos3[] = {' ',' ',' ',' ',' ','_'};
char _total[3][6] = {{},{},{}};
char _handing = ' ';

int _select = 0;
int plateAmount = sizeof(_total)/sizeof(char)/3;
int _click = 0;
int _lastPos = 4;

String _arrow = "<-";
String _action = "down"; 

void showInt(int w,int h,int num);

void showString(int w,int h,String text);

void showChar(int w,int h,char text);

int FFN(char _pos[]); 

bool button(int number);

void setup(){ 

  lcd_I2C_27.init (); 
  lcd_I2C_27.backlight();
   
  pinMode(8,INPUT);
  pinMode(4,INPUT);
  pinMode(7,INPUT);

  showString(4*_select+3,1,_arrow);

  for(int i = 0; i < plateAmount;i++){
    _total[0][i] = _pos1[i];
    _total[1][i] = _pos2[i];
    _total[2][i] = _pos3[i];
  }

}

void loop(){ 
  /*---showing---*/
  if(_action != "win "){
    showChar(0,0,_handing);
    showChar(2,1,_total[0][FFN(_total[0])]);
    showChar(6,1,_total[1][FFN(_total[1])]);
    showChar(10,1,_total[2][FFN(_total[2])]);

    for(int i = 0;i < plateAmount-1;i++){
      showChar(5+i,0,_total[_select][i]);
    }
  }

  /*---finish---*/
  if(_total[1][0] == '1' || _total[2][0] == '1'){
    _action = "win ";
    showString(0,0,"    you win!    ");
    showString(0,1,"    click:");
    showInt(10,1,_click);
    showString(12,1,"    ");
    return 0;
  }

  /*---index operating---*/
  if(_handing == ' ' && _action != "win ") {
    _action = "up  ";
    showString(12,0,_action);
  }

  else if(_handing != ' ' && _action != "win ") {
    _action = "down";
    showString(12,0,_action);
  }

  if(button(4) == false && buttonPress4 == true) buttonAction4 = true;

  if(button(7) == false && buttonPress7 == true) buttonAction7 = true;

  if(button(8) == false && buttonPress8 == true) buttonAction8 = true;

  buttonPress4 = button(4);
  buttonPress7 = button(7);
  buttonPress8 = button(8);

  /*---action control---*/
  if(buttonAction4){
    buttonAction4 = false;
    if(_select != 2){
      showString(4*_select+3,1,"  ");
      _select++;
      showString(4*_select+3,1,_arrow);
    }
  }

  else if(buttonAction8){
    buttonAction8 = false;
    if(_select != 0){
      showString(4*_select+3,1,"    ");
      _select--;
      showString(4*_select+3,1,_arrow);
    }
  }
  
  else if(buttonAction7 && _action == "up  " && _handing == ' ' && _total[_select][FFN(_total[_select])] != '_'){
    buttonAction7 = false;
    _handing = _total[_select][FFN(_total[_select])];
    _total[_select][FFN(_total[_select])] = ' ';
    _action = "down";
    _lastPos = _select;
  }

  else if(buttonAction7 && _action == "down" && _handing != ' '){
    buttonAction7 = false;
    if((int)_handing < (int)(_total[_select][FFN(_total[_select])])){
      _total[_select][FFN(_total[_select])-1] = _handing;
      _handing = ' ';
      _action = "up  ";
      if(_lastPos != _select){
        _click++;
      }
    }
  }  

  /*---reset index---*/
  buttonAction7 = false;
  buttonAction8 = false;
  buttonAction4 = false;

}

/*---function---*/

void showInt(int w,int h,int num){
   lcd_I2C_27.setCursor(w,h);
   lcd_I2C_27.print(num);
}

void showString(int w,int h,String text){
   lcd_I2C_27.setCursor(w,h);
   lcd_I2C_27.print(text);
}

void showChar(int w,int h,char text){
   lcd_I2C_27.setCursor(w,h);
   lcd_I2C_27.print(text);
}

int FFN(char _pos[]){
  for(int i = 0;i < plateAmount;i++){
    if(_pos[i] != ' '){
      return i;
      break;
    }
  }  
  return 0;
}

bool button(int number){
  return digitalRead(number);
}