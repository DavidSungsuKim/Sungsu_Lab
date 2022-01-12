// 메인 파일
#include <stdio.h>
#include <iostream>
#include "function.h"
#include "../common/libtest.h"
 
using namespace std;
 
int main()
{
  cout << "hello world" << endl;
  // 다른 cpp의 함수를 호출
  function();
  // 라이브러리의 함수 호출
  libtest();
}