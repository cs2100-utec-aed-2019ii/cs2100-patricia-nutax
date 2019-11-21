#include <iostream>
#include "trie.h"

int main() {
  trie<compressable> a;
  
  a.insert("romane");
  a.insert("romanus");
  a.insert("romulus");
  a.insert("rubens");
  a.insert("ruber");
  a.insert("rubicon");
  a.insert("rubicundus");
  
  a.compress();
  a.print();
}
