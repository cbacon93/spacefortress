#include "client.hpp"

Client::Client() : socket() {}

Client::Client(const std::string& address, const unsigned int& port,
               const std::string& username, const std::string& crewcode)
    : socket() {
  this->connect(address, port, username, crewcode);
}

Client::~Client() {
#ifdef WIN32
  // windows cleanup
  WSACleanup();
#endif
}

/**
 * Connects client to server
 * @param address Server IP
 * @param port    Server Post
 */
void Client::connect(const std::string& address, const unsigned int& port,
                     const std::string& username, const std::string& crewcode) {
  this->socket.connect(address, port);
  if (socket.isConnected()) {
    socket.authenticate(username, crewcode);
    socket.startListener();
  }
}

/**
 * Properly disconnects and closes connection
 * Needs to be called before reconnection - to close socket and stop threads
 */
void Client::disconnect() {
  this->socket.disconnect();
  this->socket.close();
}

/**
 * Waits for authentication process to finish or timeout, returns result of auth
 * Should only be called once per connect
 * @return bool true if auth was successful
 */
bool Client::waitForHandshake() {
  auto fut = socket.getReadyForGameFuture();
  if (fut.wait_for(std::chrono::milliseconds(1000)) !=
      std::future_status::timeout) {
    return fut.get();
  }
  return false;
}

/**
 * Function to sync ship
 * @param ship shared_ptr<Ship>
 */
void Client::syncMyShip(std::shared_ptr<GameObject> ship) {
  if (!ship) return;
  NetMsg msg(ship.get());
  this->socket.sendMsg(msg);
}

/**
 * [Client::updateMyShip description]
 * @param ship [description]
 */
void Client::updateMyShip(std::shared_ptr<GameObject> ship) {
  if (!ship) return;

  while (this->socket.isMsgAvailable()) {
    auto msg = this->socket.popMessage();
    switch ((NetMsgType)msg->type) {
      case (NetMsgType::OBJECT): {
        NetMsgObject* o = (NetMsgObject*)msg->data;
        ship->setPos(o->pos);
        ship->setVel(o->vel);
      }
      default:
        // nothing
        break;
    }
  }
}
