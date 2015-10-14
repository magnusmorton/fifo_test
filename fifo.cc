#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <boost/program_options.hpp>
using namespace std;

namespace po = boost::program_options;
int ip(char*  buf);
int fifo(char*  buf);
const int size = 1024*1024*8;
int main(int argc, const char** argv) {
  po::options_description desc("Allowed options");

  desc.add_options()
    ("help", "produce help message")
    ("ip,i", "tcp connection")
    ;

  
  po::variables_map vm;
  try {
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
  } catch (po::error& e) {
    cout << e.what() << endl;
    return 1;
  }

  if (vm.count("help")) {
    cout << desc << "\n";
    return 1;
  }
  // big array
  
  char* buffer = new char[size];

  if (vm.count("ip")) {
    return ip(buffer);
  } else {
    return fifo(buffer);
  }
  delete[] buffer;
  return 0;
}

int ip(char* buf) {
  struct sockaddr_in sa;
  int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock < 0) {
    perror("Can't create socket");
    return -1;
  }

  memset(&sa, 0,sizeof(sa));
  sa.sin_family = AF_INET;
  sa.sin_port = htons(4444);
  if (!inet_pton(AF_INET, "127.0.0.1", &sa.sin_addr)) {
    perror("Can't parse address");
    return -1;
  }
  
  
  if (connect(sock, (struct sockaddr *)&sa, sizeof sa) == 0) {
    cout << "HELLO\n" << endl;
    int sent = send(sock, buf,size, 0);
    cout << "sent: " << sent << endl;
    if (sent < 0){
      perror("Something bad sending");
      return -1;
    }
    int rem_size = size - sent;
    while (rem_size > 0) {
      cout << "BAD STUFF LIKELY TO HAPPEN!!!!1111" << endl;
      sent = send(sock, buf + sent, size - sent,0);
      rem_size -= sent;
      
    }
    shutdown(sock, SHUT_RDWR);
    close(sock);
  } else {
    perror("connect error");
  }
  return 0;
}

int fifo(char* buf) {
  auto fd = fopen("test_pipe", "w");
  fwrite(buf,sizeof(char), size, fd);
  return 0;
}
