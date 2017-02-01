//
//  ThreaddedNetwork.h
//  
//
//  Created by Zach Williamson on 11/5/16.
//
//  Implement networking in a thread
//  A seperate thread will constantly be checking for new messages
//  and when they arrive they will be added to a queue that can be checked
//  in the main thread
//

#ifndef ThreaddedNetwork_h
#define ThreaddedNetwork_h

#include "lib/PracticalSocket.h"  // For Socket, ServerSocket, and SocketException
#include <iostream>           // For cerr and cout
#include <string>
#include <cstring>

#include <thread>
#include "util/ThreadSafeQueue.h"

#include <vector> 
#define RCVBUFSIZE  50        // Size of receive buffer

class ThreaddedNetwork
{
public:
  /* delete default constructor*/
  ThreaddedNetwork() = delete;

  /* Constructor */
  ThreaddedNetwork(int servPort);

  /* getNewMessages */
  std::vector<std::string> getNewMessages();

  /* sendMessage */
  void sendMessage(std::string message);

  /* clearToSend */
  void clearToSend();
  
private:
  TCPServerSocket m_servSock;
  ThreadSafeQueue<std::string> q_new_messages;
  ThreadSafeQueue<std::string> q_outgoing_messages;
  std::thread my_thread;
  bool cts;

  /* function to be run in thread */
  void inThreadUpdate();
};

#endif // ThreaddedNetwork_h
