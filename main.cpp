#include <iostream>
#include <string>
#include <stack>
#include <deque>
#include <math.h>
#include <stdexcept>
#include <unordered_map>

using namespace std;

bool ex = false;
unordered_map <string,double> variables;
unordered_map <string,bool> isHashed;
bool let = true;


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
}

void simplify (deque<token*> *syexp){
  stack <token*> stk;
  while(syexp->size() != 1){
    while(!(syexp->front()->isOperation())){
      stk.push(syexp->front());
      syexp->pop_front();
    }
    double num2;
    if(syexp->front()->getOp() == '+'||
       syexp->front()->getOp() == '-'||
       syexp->front()->getOp() == '*'||
       syexp->front()->getOp() == '/'||
       syexp->front()->getOp() == '^'){
         num2 = (double)stk.top()->getNum();
         stk.pop();
       }
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
      if (num2 == 0) throw std::overflow_error("Division-By-Zero");
      num3 = (double)num1 / (double)num2;
    }
    else if(syexp->front()->getOp() == '^'){
      if (num2 < 0 && num1 == 0) throw std::overflow_error("Division-By-Zero");
      num3 = pow(num1, num2);
    }

    else if (syexp->front()->getOp() == 's'){
      num3 = sin(num1);
    }

    else if (syexp->front()->getOp() == 'c'){
      num3 = cos(num1);
    }
    else if (syexp->front()->getOp() == 'l'){
      num3 = log(num1);
    }

    syexp->pop_front();
    token* tok = new token(num3);
    syexp->push_front(tok);
    //print_deque(syexp);
  }


}

deque<token*> *evaluate (deque <token*> *syexp){


    simplify(syexp);
    return syexp;

    //print_deque(syexp);
}


double solve(string exprsn){

  stack <token*>expstack;
  deque<token*> syexp;

  bool prevnum = false;
  bool isneg = false;
  bool reset = false;
  bool ldnum = false;
  bool lddec = false;
  bool ldstr = false;
  let = true;
  int num = 0;
  double decimal = 0;
  string str = "";


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
         let = false;
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
        let = false;
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

         if((exprsn[i]>=65 && exprsn[i] <= 90) || (exprsn[i]>=97 && exprsn[i] <= 122)){
           if(!ldstr) ldstr = true;
           str += exprsn[i];

         }

         if(!((exprsn[i]>=65 && exprsn[i] <= 90) || (exprsn[i]>=97 && exprsn[i] <= 122)) && ldstr){
           ldstr = false;
           if(str == "e"){
             token* tok;
             if (isneg) tok = new token(-1*2.718);
             else tok = new token(2.718);
             syexp.push_back(tok);
             ldnum = false;
             isneg = false;
             prevnum = true;
             num = 0;
             let = false;
           }

           else if(str == "Pi"){
             token* tok;
             if (isneg) tok = new token(-1*3.14169);
             else tok = new token(3.14169);
             syexp.push_back(tok);
             ldnum = false;
             isneg = false;
             prevnum = true;
             num = 0;
             let = false;
           }

           else if(str == "sin"){
             token* tok = new token('s');
             expstack.push(tok);
             prevnum = false;
             let = false;
           }

           else if(str == "cos"){
             token* tok = new token('c');
             expstack.push(tok);
             prevnum = false;
             let = false;
           }

           else if(str == "log"){
             token* tok = new token('l');
             expstack.push(tok);
             prevnum = false;
             let = false;
           }

           else if(str == "quit"){
             ex = true;
             let = false;
             return 0;
           }

           else if(str == "let" && let){
             int begin, end, start;
             for(begin = i; begin < exprsn.size() && exprsn[begin] == ' '; begin++);
             for(start = begin; start < exprsn.size() && exprsn[start] != '='; start++);
             for(end = start-1; end < exprsn.size() && exprsn[end] == ' '; end--);
             string name = exprsn.substr(begin, end-begin+1);
             variables[name] = solve(exprsn.substr(start+1));
             isHashed[name] = true;
             let = true;

             return variables[name];


           }
           else{
             let = false;
             //std::unordered_map<std::string,double>::const_iterator got = variables.find (str);
             //if(got == variables.end()){
             if(!isHashed[str]){
               throw std::overflow_error("Undeclared-Variable");
             }
             else{
               token *tok;
               if (isneg) tok = new token(-1*variables[str]);
               else tok = new token(variables[str]);
               syexp.push_back(tok);
               ldnum = false;
               isneg = false;
               prevnum = true;
               num = 0;
             }
           }

           str = "";
         }

      if(exprsn[i] == '-' && !prevnum) {
        isneg = true;
        let = false;
      }
      else if(exprsn[i] == '+' || exprsn[i] == '-' || exprsn[i] == '*' || exprsn[i] == '/' || exprsn[i] == '^' ){
        let = false;
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
        let = false;

      }

      else if(exprsn[i] == ')'){
        while(expstack.top()->getOp() != '('){
          syexp.push_back(expstack.top());
          expstack.pop();
        }

        expstack.pop();
        if(!expstack.empty()){
          if(expstack.top()->isOperation()){
            if(expstack.top()->getOp() == 's' ||
               expstack.top()->getOp() == 'c' ||
               expstack.top()->getOp() == 'l'){
                 syexp.push_back(expstack.top());
                 expstack.pop();
               }
          }
        }
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

  else if(ldstr){
    ldstr = false;
    let = false;
    if(str == "e"){
      token* tok;
      if (isneg) tok = new token(-1*2.718);
      else tok = new token(2.718);
      syexp.push_back(tok);
      ldnum = false;
      isneg = false;
      prevnum = true;
      num = 0;
    }

    else if(str == "Pi"){
      token* tok;
      if (isneg) tok = new token(-1*3.14169);
      else tok = new token(3.14169);
      syexp.push_back(tok);
      ldnum = false;
      isneg = false;
      prevnum = true;
      num = 0;
    }
    else if(str == "sin"){
      token* tok = new token('s');
      expstack.push(tok);
      prevnum = false;
    }

    else if(str == "cos"){
      token* tok = new token('c');
      expstack.push(tok);
      prevnum = false;
    }

    else if(str == "log"){
      token* tok = new token('l');
      expstack.push(tok);
      prevnum = false;
    }
    else if(str == "quit"){
      ex = true;
      return 0;
    }
    else{
      //std::unordered_map<std::string,double>::const_iterator got = variables.find (str);
      //if(got == variables.end()){
      if(!isHashed[str]){
        throw std::overflow_error("Undeclared-Variable");
      }

      else{
        token *tok;
        if (isneg) tok = new token(-1*variables[str]);
        else tok = new token(variables[str]);
        syexp.push_back(tok);
        ldnum = false;
        isneg = false;
        prevnum = true;
        num = 0;
      }
    }


    str = "";
  }

  while(!expstack.empty()){
    syexp.push_back(expstack.top());
    expstack.pop();
  }

  print_deque(&syexp);
  return evaluate(&syexp)->front()->getNum();
  //print_deque(evaluate(&syexp));

}

int main(){

  while(1){
    try{
      string exprsn;
      getline(cin,exprsn);
      double contain;
      contain = solve(exprsn);
      if(ex){
        break;
      }
      else{
        if(!let)cout<<contain<<endl;
      }
    }
    catch(std::overflow_error e){
      cout<<e.what()<<endl;
    }
  }
  return 0;
}
