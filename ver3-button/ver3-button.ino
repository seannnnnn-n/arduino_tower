#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd_I2C_27(0x27,16,2);

/*notes
char 0 = 48
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
bool selecting = true;

char _pos1[7] = {' ',' ',' ',' ',' ',' ','_'};
char _pos2[7] = {' ',' ',' ',' ',' ',' ','_'};
char _pos3[7] = {' ',' ',' ',' ',' ',' ','_'};
char _total[3][7] = {{},{},{}};
char _handing = ' ';

int _select = 0;
int plateAmount = sizeof(_total)/sizeof(char)/3;
int _click = 0;
int _lastPos = 4;
int dif[3] = {4,5,6};
int hard = 0;

String _arrow = "<-";
String _action = "down"; 

void showInt(int w,int h,int num);

void showString(int w,int h,String text);

void showChar(int w,int h,char text);

void cleanLine(int h);

void reset();

int FFN(char _pos[]); 

bool button(int number);

void setup(){ 

  lcd_I2C_27.init (); 
  lcd_I2C_27.backlight();
   
  pinMode(4,INPUT);
  pinMode(7,INPUT);
  pinMode(8,INPUT);
  pinMode(12,INPUT);

  _select = 0;

  cleanLine(0);
  cleanLine(1);
  showString(4*_select+3,1,_arrow);

  for(int i = 0; i < plateAmount;i++){
    _total[0][i] = _pos1[i];
    _total[1][i] = _pos2[i];
    _total[2][i] = _pos3[i];
  }

  _action = "down";
  _click = 0;
  _lastPos = 4;
  _handing = ' ';
  
}

void difficulty(){
  showString(4*_select+3,1,_arrow);
  showInt(2,1,dif[0]);
  showInt(6,1,dif[1]);
  showInt(10,1,dif[2]);
  showString(0,0,"   difficulty   ");

  if(button(4) == false && buttonPress4 == true) buttonAction4 = true;

  if(button(7) == false && buttonPress7 == true) buttonAction7 = true;

  if(button(8) == false && buttonPress8 == true) buttonAction8 = true;

  buttonPress4 = button(4);
  buttonPress7 = button(7);
  buttonPress8 = button(8);

  if(buttonAction8){
    buttonAction8 = false;
    if(_select != 0){
      showString(4*_select+3,1,"  ");
      _select--;
      showString(4*_select+3,1,_arrow);
    }
  }

  else if(buttonAction4){
    buttonAction4 = false;
    if(_select != 2){
      showString(4*_select+3,1,"    ");
      _select++;
      showString(4*_select+3,1,_arrow);
    }
  }

  else if(buttonAction7){
    buttonAction7 = false;
    selecting = false;
    hard = dif[_select];

    for(int i = 0;i < 6;i++){
      _pos1[i] = ' ';
    }

    for(int i = 5,j = hard;i >= 6 - hard;i--,j--){
      _pos1[i] = char(j + 48);
    }

    showString(0,0,"   choosed  ");
    showInt(12,0,hard);
    cleanLine(1);
    setup();
  }
}

void loop(){
  if(selecting){
    difficulty();
  }

  else{
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

    /*---reset---*/
    if(digitalRead(12)){
      reset();
    }

    /*---finish---*/
    if(_total[2][6 - hard] == '1'){
      _action = "win ";
      showString(0,0,"you win!! hard ");
      showInt(15,0,hard);
      showString(0,1,"    click:");
      showInt(10,1,_click);
      showString(12,1,"    ");
      tone(5,500,1000);
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
    if(buttonAction8){
      buttonAction8 = false;
      if(_select != 0){
        showString(4*_select+3,1,"  ");
        _select--;
        showString(4*_select+3,1,_arrow);
      }
    }

    else if(buttonAction4){
      buttonAction4 = false;
      if(_select != 2){
        showString(4*_select+3,1,"    ");
        _select++;
        showString(4*_select+3,1,_arrow);
      }
    }
    
    else if(buttonAction7 && _action == "up  "){
      if(_handing == ' '){
        if(_total[_select][FFN(_total[_select])] != '_'){
          buttonAction7 = false;
          _handing = _total[_select][FFN(_total[_select])];
          _total[_select][FFN(_total[_select])] = ' ';
          _action = "down";
          _lastPos = _select;
        }
      }
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

    delay(1);
  }
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

void cleanLine(int h){
  showString(0,h,"                ");
}

void reset(){
  selecting = true;
  showString(0,0,"  resetting...  ");
  cleanLine(1);
  setup();
  showString(7,1,"  ");
  showString(11,1,"  ");
  showString(4*_select+3,1,_arrow);
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