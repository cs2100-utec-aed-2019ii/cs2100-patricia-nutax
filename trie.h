#include <memory>
#include <string>
#include <map>

enum{
  simple,
  compressable
};

template<int degree>
class trie{};


template<>
class trie<simple>{

  private:
  class node;

  private:
  using letter_t = char;
  using word_t = std::string;
  using ptr_t = std::shared_ptr<node>;
  using connections = std::map<letter_t, ptr_t>;

  private:
  class node{
    public:
    bool is_word;
    connections childrens;

    public:
    node(void) = default;

  };

  private:
  ptr_t root;

  private:
  void insert(ptr_t& actual, word_t word, const bool& is_word){
    if(!actual) actual = std::make_shared<node>();
    if(is_word) {actual->is_word = true; return;}

    letter_t letter = word[0];
    word.erase(begin(word));
    
    insert(actual->childrens[letter], word, word.length() == 0);
  }

  void erase(ptr_t& actual, word_t word, const bool& is_word){
    if(!actual) return;
    if(is_word) {actual->is_word = false; return;}

    letter_t letter = word[0];
    word.erase(begin(word));
    
    if(actual->childrens.find(letter) == actual->childrens.end()) return;
    erase(actual->childrens[letter], word, word.length() == 0);
    if(actual->childrens[letter]->childrens.empty()) actual->childrens.erase(letter);
    
  }

  void print(const ptr_t& actual, const unsigned int& tabs){
    if(actual)
     for(const auto& letter: actual->childrens){
      for(int i = 0; i<tabs; i++) std::cout<<'\t';
      std::cout<<letter.first<<std::endl;
      print(letter.second, tabs + 1);
      std::cout<<std::endl;
    }
  }

  public:
  void insert(const word_t& word){
    insert(root, word, false);
  }

  void erase(const word_t& word){
    erase(root, word, false);
    
  }

  void print(){
   print(root, 0);
  }

};


template<>
class trie<compressable>{
  private:
  class node;

  private:
  using letter_t = char;
  using word_t = std::string;
  using ptr_t = std::shared_ptr<node>;
  using connections = std::map<word_t, ptr_t>;

  private:
  class node{
    public:
    word_t word;
    connections childrens;
  };

  private:
  ptr_t root;
  bool compressed = false;

  private:
  void insert(ptr_t& actual, const word_t& full_word, word_t word, const bool& is_word){
    if(!actual) actual = std::make_shared<node>();
    if(is_word) {actual->word = full_word; return;}

    word_t letter(1, word[0] );
    word.erase(begin(word));
    
    insert(actual->childrens[letter], full_word, word, word.length() == 0);
  }

  void erase(ptr_t& actual, word_t word, const bool& is_word){
    if(!actual) return;
    if(is_word) {actual->word = ""; return;}

    word_t letter(1, word[0] );
    word.erase(begin(word));
    
    if(actual->childrens.find(letter) == actual->childrens.end()) return;
    erase(actual->childrens[letter], word, word.length() == 0);
    if(actual->childrens[letter]->childrens.empty()) actual->childrens.erase(letter);
    
  }

  void print(const ptr_t& actual, const unsigned int& tabs){
    if(actual)
     for(const auto& letter: actual->childrens){
      for(int i = 0; i<tabs; i++) std::cout<<'\t';
      std::cout<<letter.first<<std::endl;
      print(letter.second, tabs + 1);
      std::cout<<std::endl;
    }
  }

  bool compress(ptr_t& actual){
    if(actual){
      bool alone;
      auto it = begin(actual->childrens);
      while(it != end(actual->childrens)){
        alone = compress(it->second);
        if(alone) {
          auto* a = &(it->first);
          auto* b = &((begin(it->second->childrens))->first);
          auto* c = &((begin(it->second->childrens))->second);
          actual->childrens.insert(std::pair<word_t, ptr_t>(*a + *b, *c));
          actual->childrens.erase(it);
          }
        it++;
      };
      if(actual->childrens.size() == 1 && actual->word.empty()) return true;
    }
    return false;
     
  }

  public:
  void insert(const word_t& word){
    if(compressed) return;
    insert(root, word, word, false);
  }

  void erase(const word_t& word){
    if(compressed) return;
    erase(root, word, false);
  }

  void print(){
   print(root, 0);
  }

  void compress(){
    if(compressed) return;
    compressed = true;

    compress(root);
  }

};
