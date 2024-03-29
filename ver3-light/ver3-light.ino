#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd_I2C_27(0x27,16,2);

/*notes
char 0 = 48
*/

/*---index declare---*/
bool lighting0 = false;
bool lighting1 = false;
bool lighting2 = false;
bool lightAction0 = false;
bool lightAction1 = false;
bool lightAction2 = false;
bool selecting = true;

char _pos1[7] = {' ',' ',' ',' ',' ',' ','_'};
char _pos2[7] = {' ',' ',' ',' ',' ',' ','_'};
char _pos3[7] = {' ',' ',' ',' ',' ',' ','_'};
char _total[3][7] = {{},{},{}};
char _handing = ' ';

int dif[3] = {4,5,6};
int _select = 0;
int omega = 0;
int plateAmount = sizeof(_total)/sizeof(char)/3;
int _click = 0;
int _lastPos = 4;
int times = 0;
int hard = 0;
int max0 = 0;
int max1 = 0;
int max2 = 0;

String _arrow = "<-";
String _action = "down"; 

int FFN(char _pos[]); 

bool lighting(int number);

void showInt(int w,int h,int num);

void showString(int w,int h,String text);

void showChar(int w,int h,char text);

void cleanLine(int h);

void setup(){ 

  lcd_I2C_27.init (); 
  lcd_I2C_27.backlight();
  
  pinMode(12,INPUT);

  _select = 0;

  cleanLine(0);
  cleanLine(1);
  showString(4*_select+3,1,_arrow);

  for(int i = 0; i < plateAmount;i++){
    _total[0][i] = _pos1[i];
    _total[1][i] = _pos2[i];
    _total[2][i] = _pos3[i];

    _action = "down";
    _click = 0;
    _lastPos = 4;
    _handing = ' ';
    times = 0;
    max0 = 0;
    max1 = 0;
    max2 = 0;
  }
}

void difficulty(){
  showString(4*_select+3,1,_arrow);
  showInt(2,1,dif[0]);
  showInt(6,1,dif[1]);
  showInt(10,1,dif[2]);
  showString(0,0,"   difficulty   ");

  if(lighting(0) == false && lighting0 == true) lightAction0 = true;

  if(lighting(1) == false && lighting1 == true) lightAction1 = true;

  if(lighting(2) == false && lighting2 == true) lightAction2 = true;

  lighting0 = lighting(0);
  lighting1 = lighting(1);
  lighting2 = lighting(2);

  if(lightAction2){
    lightAction2 = false;
    if(_select != 2){
      showString(4*_select+3,1,"  ");
      _select++;
      showString(4*_select+3,1,_arrow);
    }
  }

  else if(lightAction0){
    lightAction0 = false;
    if(_select != 0){
      showString(4*_select+3,1,"    ");
      _select--;
      showString(4*_select+3,1,_arrow);
    }
  }

  else if(lightAction1){
    lightAction1 = false;
    selecting = false;
    hard = dif[_select];

    for(int i = 0;i < 6;i++){
      _pos1[i] = ' ';
    }

    for(int i = hard - 1;i >= 0;i--){
      _pos1[i] = char(i + 49);
    }

    showString(0,0,"   choosed  ");
    showInt(12,0,hard);
    cleanLine(1);
    setup();
  }
}

void loop(){
  if(analogRead(A0) > max0) max0 = analogRead(A0);
  
  if(analogRead(A1) > max1) max1 = analogRead(A1);

  if(analogRead(A2) > max2) max2 = analogRead(A2);

  if(selecting){
    difficulty();
  }

  if(!selecting){
    /*---showing---*/
    if(_action != "win "){
      showChar(0,0,_handing);
      showChar(2,1,_total[0][FFN(_total[0])]);
      showChar(6,1,_total[1][FFN(_total[1])]);
      showChar(10,1,_total[2][FFN(_total[2])]);

      for(int i = 0;i < plateAmount - 1;i++){
        showChar(5+i,0,_total[_select][i]);
      }
    }

    /*---reset---*/
    if(digitalRead(12)){
      selecting = true;
      showString(0,0,"  reseting....  ");
      cleanLine(1);
      setup();
      showString(7,1,"  ");
      showString(11,1,"  ");
      showString(4*_select+3,1,_arrow);
    }

    /*---finish---*/
    if(_total[1][(6 - hard)] == '1' || _total[2][6 - hard] == '1'){
      _action = "win ";
      showString(0,0,"you win!! hard ");
      showInt(15,0,hard);
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

    if(lighting(0) == false && lighting0 == true) lightAction0 = true;

    if(lighting(1) == false && lighting1 == true) lightAction1 = true;

    if(lighting(2) == false && lighting2 == true) lightAction2 = true;

    lighting0 = lighting(0);
    lighting1 = lighting(1);
    lighting2 = lighting(2);

    /*---action control---*/
    if(lightAction0){
      lightAction0 = false;
      if(_select != 0){
        showString(4*_select+3,1,"  ");
        _select--;
        showString(4*_select+3,1,_arrow);
      }
    }

    else if(lightAction2){
      lightAction2 = false;
      if(_select != 2){
        showString(4*_select+3,1,"    ");
        _select++;
        showString(4*_select+3,1,_arrow);
      }
    }
    
    else if(lightAction1 && _action == "up  "){
      if(_handing == ' '){
        if(_total[_select][FFN(_total[_select])] != '_'){
          lightAction1 = false;
          _handing = _total[_select][FFN(_total[_select])];
          _total[_select][FFN(_total[_select])] = ' ';
          _action = "down";
          _lastPos = _select;
        }
      }
    }

    else if(lightAction1 && _action == "down" && _handing != ' '){
      lightAction1 = false;
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
    lightAction1 = false;
    lightAction2 = false;
    lightAction0 = false;

    times++;

    delay(10);
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

int FFN(char _pos[]){
  for(int i = 0;i < plateAmount;i++){
    if(_pos[i] != ' '){
      return i;
      break;
    }
  }  
  return 0;
}

bool lighting(int number){
  if(number == 0){
    if(analogRead(A0) < max0 - 150) return true;
    else return false;
  }
  else if(number == 1){
    if(analogRead(A1) < max1 - 150) return true;
    else return false;
  }
  else if(number == 2){
    if(analogRead(A2) < max2 - 150) return true;
    else return false;
  }
}