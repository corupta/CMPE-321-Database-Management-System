#include <iostream>
#include <fstream>
using namespace std;
int main() {
  fstream mystream;
  mystream.open("hello.txt", ios::in | ios::binary | ios::out);
  // mystream.open("hello.txt", ios::binary | ios::in | ios::out | ios::trunc);
  if (!mystream) {
      mystream.open("hello.txt", ios::out | ios::binary);
      mystream.close();
      cout << "Aa" << endl;
      mystream.open("hello.txt", ios::in | ios::binary | ios::out);
      if (!mystream) {
          cout << "BB" << strerror(errno) << endl;
      }

  }
  char x[10] = "halit";
    mystream.write(x, sizeof(x));
    mystream.write(x, sizeof(x));
    mystream.write(x, sizeof(x));

    mystream.seekp((unsigned long long)40);
    mystream.write(x, sizeof(x));

}