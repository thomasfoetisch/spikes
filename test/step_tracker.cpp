
#include <string>
#include <chrono>
#include <vector>
#include <iostream>
#include <thread>
#include <set>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include <sstream>

#include <unistd.h>


class logbook_view {
public:
  virtual ~logbook_view() {}
  virtual void on_open_section(const std::string& title) = 0;
  virtual void on_close_section() = 0;
  virtual void on_log_line(const std::string& line) = 0;
};


struct logbook {
public:
  static logbook& instance() {
    if (not inst)
      inst = new logbook();

    return *inst;
  }

  static void release() {
    delete inst;
    inst = nullptr;
  }
private:
  static logbook* inst;

  
public:

  void open(const std::string& title) {
    for (auto& view: views)
      view->on_open_section(title);
  }
  
  void close() {
    for (auto& view: views)
      view->on_close_section();
  }
  
  void log(const std::string& line) {
    for (auto& view: views)
      view->on_log_line(line);
  }


  void subscribe(logbook_view* view) {
    views.insert(view);
  }

  void unsubscribe(logbook_view* view) {
    views.erase(view);
  }
  
private:
  logbook() {}
  ~logbook() {}

  std::set<logbook_view*> views;
};

logbook* logbook::inst(nullptr);


class section {
public:
  section(const std::string& title) {
    logbook::instance().open(title);
  }
  
  ~section() {
    logbook::instance().close();
  }

  void log(const std::string& lines) {
    std::istringstream iss(lines);
    std::string line;
    while (std::getline(iss, line, '\n')) {
      logbook::instance().log(line);
    }
  }
};


class logbook_stdout_view: public logbook_view {
public:
  logbook_stdout_view(logbook& b)
    : book(b), level(0) {
    book.subscribe(this);
  }

  virtual ~logbook_stdout_view() {
    book.unsubscribe(this);
  }

  virtual void on_open_section(const std::string& title) {
    level += 1;
    
    print_header();
    print_indentation(level);
    std::cout << title << std::endl;
  }
  
  virtual void on_close_section() {
    level -= 1;
  }
  
  virtual void on_log_line(const std::string& line) {
    print_header();
    print_indentation(level + 1);
    std::cout << "[INFO] " << line << std::endl;
  }
  
private:
  logbook& book;

  std::size_t level;

  void print_header() {
    std::cout << "[ " << std::setw(6) << get_process_id() << "] ";
  }

  void print_indentation(unsigned int d) {
    std::cout << std::string((d - 1) * 2, ' ');
  }

  static unsigned int get_process_id() {
    return getpid();
  }
};


class logbook_file_view: public logbook_view {
public:
  logbook_file_view(logbook& b, std::ofstream& f)
    : book(b), file(f) {
    book.subscribe(this);
  }

  virtual ~logbook_file_view() {
    book.unsubscribe(this);
  }

  virtual void on_open_section(const std::string& title) {}
  virtual void on_close_section() {}
  virtual void on_log_line(const std::string& line) {}
  
private:
  logbook& book;
  std::ofstream& file;
};


class logbook_term_view: public logbook_view {
public:
  logbook_term_view(logbook& b)
    : book(b), root(nullptr) {
    book.subscribe(this);
    root = new node(new section("root section"));
    insertion_stack.push_back(root);
    cursor = begin();
  }

  virtual ~logbook_term_view() {
    book.unsubscribe(this);

    std::cout << get_length() << "entries" << std::endl;
    render(begin(), end());
    test_iterators();
  }

  virtual void on_open_section(const std::string& title) {
    node* n(new node(new section(title)));
    if (cursor + 1 == end()) {
      insertion_stack.back()->children.push_back(n);
      ++cursor;
    } else {
      insertion_stack.back()->children.push_back(n);
    }
    insertion_stack.push_back(n);
  }
  virtual void on_close_section() {
    insertion_stack.back()->collapse();
    insertion_stack.pop_back();
    cursor.escape_collapse();
  }
  virtual void on_log_line(const std::string& line) {
    node* n(new node(new paragraph(line)));
    if (cursor + 1 == end()) {
      insertion_stack.back()->children.push_back(n);
      ++cursor;
    } else {
      insertion_stack.back()->children.push_back(n);
    }
  }
  
private:
  logbook& book;
  
  
  struct entry {
    virtual ~entry() {}
    virtual void print() = 0;
  };
  
  struct paragraph: public entry {
    paragraph(const std::string& l): lines(1, l) {}
    std::vector<std::string> lines;
    virtual void print() { std::cout << lines.front() << std::endl; }
  };
  
  struct section: public entry {
    section(const std::string& t): title(t) {}
    std::string title;
    virtual void print() { std::cout << title << std::endl; }
  };

  struct node {
    node(entry* d, node* p = nullptr): parent(p), collapsed(false), data(d) {}

    void collapse() { collapsed = true; }
    
    node* parent;
    std::vector<node*> children;

    bool collapsed;
    entry* data;
  };

  node* root;
  std::vector<node*> insertion_stack;

  struct node_iterator: public std::iterator<std::forward_iterator_tag, entry*> {
    node_iterator(node* r = nullptr): current(r) {}
    
    node* current;
    std::vector<node*> n;
    std::vector<std::size_t> i;

    unsigned int get_current_level() { return i.size(); }

    entry* operator*() { return current->data; }
    entry& operator->() { return *current->data; }
    
    node_iterator& operator++() {
      if (current->children.size() and not current->collapsed) {
	n.push_back(current);
	i.push_back(0);
	current = current->children.front();
      } else {
	while (i.size() and i.back() == n.back()->children.size() - 1) {
	  n.pop_back();
	  i.pop_back();
	}
	if (i.size()) {
	  ++i.back();
	  current = n.back()->children[i.back()];
	} else {
	  current = nullptr;
	}
      }

      return *this;
    }

    node_iterator& operator--() {
      if (not i.size()) {
	current = nullptr;
      } else if (i.back() == 0) {
	current = n.back();
	n.pop_back();
	i.pop_back();
      } else {
	--i.back();
	while ( n.back()->children[i.back()]->children.size() and not n.back()->children[i.back()]->collapsed) {
	  const unsigned int j((n.back()->children[i.back()])->children.size() - 1);
	  n.push_back( n.back()->children[i.back()] );
	  i.push_back(j);
	}
      current = n.back()->children[i.back()];
      }

      return *this;
    }

    node_iterator& operator-=(std::size_t n) {
      while (n) {
	--(*this);
	--n;
      }
    }
    
    node_iterator& operator+=(std::size_t n) {
      while (n) {
	++(*this);
	--n;
      }
    }

    node_iterator operator+(std::size_t n) const {
      node_iterator i(*this);
      return i += n;
    }

    node_iterator operator-(std::size_t n) const {
      node_iterator i(*this);
      return i -= n;
    }
    
    bool operator==(const node_iterator& i) const {
      return i.current == current;
    }

    bool operator!=(const node_iterator& i) const {
      return i.current != current;
    }

    void escape_collapse() {}
  };

  node_iterator begin() { return node_iterator(root); }
  node_iterator end() { return node_iterator(nullptr); }
  

  std::size_t get_length() { return std::distance(begin(), end()); }
  
  void render(node_iterator start, const node_iterator& end) {
    while (start != end) {
      std::cout << "[" << start.current << "]"
		<< std::string(start.get_current_level(), ' ');
      (*start)->print();
      ++start;
    }
  }

  void test_iterators() {
    std::vector<node_iterator> fw_is;
    fw_is.push_back(begin());
    while(fw_is.back() != end()) {
      node_iterator i(fw_is.back());
      ++i;
      fw_is.push_back(i);
    }

    for (unsigned int i(0); i < fw_is.size() - 2; ++i) {
      node_iterator j(fw_is[i + 1]);
      --j;
      if (j != fw_is[i]) {
	std::cout << "--fw_is[].current points to " << j.current << " instead of "  << fw_is[i].current << std::endl;
      }
    }
  }

  node_iterator cursor;
};




void do_work(double d = 1.0) {
  //std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(d*1000)));
}

void rince_rice() {
  section s("Rincing rice");
  { section s("Fill water"); do_work(0.1); }
  { section s("Let it soak"); do_work(0.1); }
  { section s("Drain water"); do_work(0.1); }
}

void fetch_ingredient(const std::string& ingredient_name) {
  section s("Fetching " + ingredient_name);

  do_work(0.5);
}

void prepare_ingredients() {
  section s("Preparing the ingredients");
  s.log("Number of rincing cycles: 5\n"
	"Liquid used for rincing:  distilled water");
    
  fetch_ingredient("rice");
  for (unsigned int i(0); i < 5; ++i)
    rince_rice();

  fetch_ingredient("sugar");
  fetch_ingredient("banana");
  fetch_ingredient("salt");
  fetch_ingredient("vanilla powder");
  fetch_ingredient("milk");
}

void mix_ingredients() {
  section s("Mixing everything");

  do_work(2.5);
}

void bake_in_oven() {
  section s("Baking in oven");

  do_work(3.0);
}

void make_rice_cake() {
  section s("Making a rice cake");
  
  prepare_ingredients();
  mix_ingredients();
  bake_in_oven();
}

int main(int argc, char *argv[]) {
  {
    //logbook_stdout_view view(logbook::instance());
    logbook_term_view view(logbook::instance());
  
    make_rice_cake();
  }
  logbook::release();
  return 0;
}
