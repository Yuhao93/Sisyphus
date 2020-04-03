#ifndef SOCKET_H
#define SOCKET_H

class Socket {
public:
  Init();
  const string& Read();
  void Write(const string& data);
};

#endif
