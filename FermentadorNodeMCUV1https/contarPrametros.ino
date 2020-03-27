int count(String str){
  int a = 1;
  for (int i = 0; i < str.length(); i ++){
      if (str[i] == ':') {a = a + 1;}
  }
  return a;
}
