#include <iostream>
#include <string>
#include <stack>
#include <deque>
#include <math.h>
#include <stdexcept>

using namespace std;

class token{
private:
  bool isop;
  double num;
  char op;
public:
  token(int a){
    this->isop = false;
    this->num = (double)a;
  }
  token(double a){
    this->isop = false;
    this->num = a;
  }
  token(char b){
    this->isop = true;
    this->op = b;
  }
  bool isOperation(){
    if(isop) return 1;
    else return 0;
  }
  double getNum() {return this->num;}
  char getOp() {return this->op;}
};

int preced(char in){
	if(in=='+'||in=='-')return 2;
	if(in=='*'||in=='/')return 3;
	if(in=='^')return 4;

	return 0;
}

void print_deque(deque<token*> * syexp){
  stack<token*> stk;
  while(!syexp->empty()){

    if(syexp->front()->isOperation()){
      cout<<syexp->front()->getOp()<<" ";
      stk.push(syexp->front());
      syexp->pop_front();
    }
    else{
      cout<<syexp->front()->getNum()<<" ";
      stk.push(syexp->front());
      syexp->pop_front();
    }

  }

  while(!stk.empty()){
    syexp->push_front(stk.top());
    stk.pop();
  }
  cout<<endl;
}

void simplify (deque<token*> *syexp){
  stack <token*> stk;
  while(syexp->size() != 1){
    while(!(syexp->front()->isOperation())){
      stk.push(syexp->front());
      syexp->pop_front();
    }

    double num2 = (double)stk.top()->getNum();
    stk.pop();
    double num1 = (double)stk.top()->getNum();
    stk.pop();
    double num3;
    if(syexp->front()->getOp() == '+'){
      num3 = num1 + num2;
    }
    else if(syexp->front()->getOp() == '-'){
      num3 = num1 - num2;
    }
    else if(syexp->front()->getOp() == '*'){
      num3 = num1 * num2;
    }
    else if(syexp->front()->getOp() == '/'){
      if (num2 == 0) throw std::overflow_error("Division by zero error.");
      num3 = (double)num1 / (double)num2;
    }
    else if(syexp->front()->getOp() == '^'){
      num3 = pow(num1, num2);
    }

    syexp->pop_front();
    token* tok = new token(num3);
    syexp->push_front(tok);
    print_deque(syexp);
  }

  cout<<endl;

}

void evaluate (deque <token*> *syexp){


    simplify(syexp);

}



int main(){

  string exprsn;

  getline(cin,exprsn);

  stack <token*>expstack;
  deque<token*> syexp;

  bool prevnum = false;
  bool isneg = false;
  bool reset = false;
  bool ldnum = false;
  bool lddec = false;
  int num = 0;
  double decimal = 0;

  for(int i = 0; i < exprsn.size(); i++){

    if(!ldnum && !lddec &&
      (exprsn[i] == '0' ||
       exprsn[i] == '1' ||
       exprsn[i] == '2' ||
       exprsn[i] == '3' ||
       exprsn[i] == '4' ||
       exprsn[i] == '5' ||
       exprsn[i] == '6' ||
       exprsn[i] == '7' ||
       exprsn[i] == '8' ||
       exprsn[i] == '9' ||
       exprsn[i] == '0')){
         num = exprsn[i] - '0';
         ldnum = true;
       }

      else if(ldnum &&
        (exprsn[i] == '0' ||
         exprsn[i] == '1' ||
         exprsn[i] == '2' ||
         exprsn[i] == '3' ||
         exprsn[i] == '4' ||
         exprsn[i] == '5' ||
         exprsn[i] == '6' ||
         exprsn[i] == '7' ||
         exprsn[i] == '8' ||
         exprsn[i] == '9' ||
         exprsn[i] == '0')){
           num = num*10;
           num += exprsn[i] - '0';
           ldnum = true;
         }

      else if(ldnum && exprsn[i] != ' ' && exprsn[i] != '.') {
        token* tok;
        if (isneg) tok = new token(-1*num);
        else tok = new token(num);
        syexp.push_back(tok);
        ldnum = false;
        isneg = false;
        prevnum = true;
        num = 0;
      }

      else if(exprsn[i] == '.' ) {
        ldnum = false;
        lddec = true;
      }

      else if(lddec &&
        (exprsn[i] == '0' ||
         exprsn[i] == '1' ||
         exprsn[i] == '2' ||
         exprsn[i] == '3' ||
         exprsn[i] == '4' ||
         exprsn[i] == '5' ||
         exprsn[i] == '6' ||
         exprsn[i] == '7' ||
         exprsn[i] == '8' ||
         exprsn[i] == '9' ||
         exprsn[i] == '0')){
           while((int)decimal != decimal) decimal*=10;
           decimal*=10;
           decimal += exprsn[i] - '0';

           while(decimal>1) decimal/=10;

         }

         else if(lddec && exprsn[i] != ' ' ) {
           token* tok;
           if (isneg) tok = new token(-1*((double)num+decimal));
           else tok = new token((double)num + decimal);
           syexp.push_back(tok);
           ldnum = false;
           isneg = false;
           lddec = false;
           prevnum = true;
           num = 0;
           decimal = 0;
         }


      if(exprsn[i] == '-' && !prevnum) {
        isneg = true;

      }
      else if(exprsn[i] == '+' || exprsn[i] == '-' || exprsn[i] == '*' || exprsn[i] == '/' || exprsn[i] == '^' ){
        
        if(!expstack.empty() && preced(exprsn[i]) > preced(expstack.top()->getOp()) && exprsn[i] != '^'){
          token* tok = new token(exprsn[i]);
          expstack.push(tok);
          prevnum = false;

        }

        else if(!expstack.empty() && preced(exprsn[i]) >= preced(expstack.top()->getOp()) && exprsn[i] == '^'){
          token* tok = new token(exprsn[i]);
          expstack.push(tok);
          prevnum = false;

        }

        else if(!expstack.empty()){
          syexp.push_back(expstack.top());
          expstack.pop();
          token* tok = new token(exprsn[i]);
          expstack.push(tok);
          prevnum = false;

        }

        else{
          token* tok = new token(exprsn[i]);
          expstack.push(tok);
          prevnum = false;

        }
      }

      else if(exprsn[i] == '('){
        token* tok = new token(exprsn[i]);
        expstack.push(tok);
        prevnum = false;

      }

      else if(exprsn[i] == ')'){
        while(expstack.top()->getOp() != '('){
          syexp.push_back(expstack.top());
          expstack.pop();
        }
        expstack.pop();
        prevnum = true;

      }


  }

  if(ldnum){
    token* tok;
    if (isneg) tok = new token(-1*num);
    else tok = new token(num);
    syexp.push_back(tok);
    ldnum = true;
    prevnum = false;
    isneg = false;
    num = 0;
  }

  else if (lddec){
    token* tok;
    if (isneg) tok = new token(-1*((double)num+decimal));
    else tok = new token((double)num + decimal);
    syexp.push_back(tok);
    ldnum = false;
    isneg = false;
    lddec = false;
    prevnum = true;
    num = 0;
  }

  while(!expstack.empty()){
    syexp.push_back(expstack.top());
    expstack.pop();
  }

  print_deque(&syexp);
  cout<<endl;

  try{
  evaluate(&syexp);
  }
  catch(std::overflow_error e){
    cout<<e.what()<<endl;
  }



  return 0;
}
