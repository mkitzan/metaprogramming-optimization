/*


   B. Bird - 01/08/2019
*/

#include <iostream>

template<typename T1, typename T2>
class Add{
public:
   constexpr static int eval(int x, int y){
      return T1::eval(x,y) + T2::eval(x,y);
   }
};


template<typename T1, typename T2>
class Multiply{
public:
   constexpr static int eval(int x, int y){
      return T1::eval(x,y) * T2::eval(x,y);
   }
};

template<char s>
class Symbol; //No non-specialized implementation

template<>
class Symbol<'x'>{
public:
   constexpr static int eval(int x, int y){
      return x;
   }
};

template<>
class Symbol<'y'>{
public:
   constexpr static int eval(int x, int y){
      return y;
   }
};

template<int c>
class Constant{
public:
   constexpr static int eval(int x, int y){
      return c;
   }
};

int main(){
   using S = Multiply< Constant<50>, Constant<2> >;
   constexpr int f = S::eval(6,10);
   using T = Multiply< Add< Symbol<'x'>, Constant<f> >, Multiply<Symbol<'y'>,Constant<10>>>;
   int g = T::eval(6,10);
   std::cout << g << std::endl;
   return 0;
}

