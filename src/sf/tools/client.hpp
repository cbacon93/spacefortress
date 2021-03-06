#pragma once

#include <memory>
#include <string>

#include "../gameobject.hpp"
#include "clientsocket.hpp"

class Client {
 public:
  Client();
  Client(const std::string& address, const unsigned int& port,
         const std::string& username, const std::string& crewcode = "");
  ~Client();

  // management functions
  void connect(const std::string& address, const unsigned int& port,
               const std::string& username, const std::string& crewcode = "");
  void disconnect();
  void syncMyShip(std::shared_ptr<GameObject> ship);
  void updateMyShip(std::shared_ptr<GameObject> ship);

  // info functions
  bool isConnected() { return socket.isConnected(); }
  bool isAuthenticated() { return socket.isAuthenticated(); }
  bool waitForHandshake();
  std::string getCrewCode() { return socket.getCrewCode(); }

 private:
  ClientSocket socket;
};
