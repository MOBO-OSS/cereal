#include <iostream>
#include <cstddef>
#include <chrono>
#include <thread>
#include <cassert>

#include "messaging.h"
#include "impl_zmq.h"

#define MSGS 1e5

typedef struct DataFormat
{
  /* data */
  uint64_t x;
  uint64_t y;
  uint64_t z;
  std::vector<float> array;
};


int main() {
  Context * c = Context::create();
  SubSocket * sub_sock = SubSocket::create(c, "controlsState");
  PubSocket * pub_sock = PubSocket::create(c, "controlsState");

  char data[8];

  Poller * poller = Poller::create({sub_sock});

  auto start = std::chrono::steady_clock::now();

  DataFormat sData;

  for (uint64_t i = 0; i < MSGS; i++){
    *(uint64_t*)data = i;
    sData.x = i;
    sData.array.push_back(i/10000);

    pub_sock->send((char*)&sData, sizeof(sData));

    printf("send data: %lld\n", sData.x);
    auto r = poller->poll(100);

    for (auto p : r){
      Message * m = p->receive();
      DataFormat * ii = (DataFormat *)m->getData();
      printf("receive data: %lld\n", ii->x);
      assert(i == ii->x);
      delete m;
    }
  }


  auto end = std::chrono::steady_clock::now();
  double elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / 1e9;
  double throughput = ((double) MSGS / (double) elapsed);
  std::cout << throughput << " msg/s" << std::endl;

  delete poller;
  delete sub_sock;
  delete pub_sock;
  delete c;


  return 0;
}
