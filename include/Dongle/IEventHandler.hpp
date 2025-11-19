#include <cstdint>
#include <vector>

namespace dongle {

using EventID = unsigned int;

class EventData {
 public:
  template<typename T> EventData(T &data) : m_data(reinterpret_cast<void *>(&data)) {}
  template<typename T> T &as() { return *reinterpret_cast<T *>(m_data); }

 private:
  void *m_data;
};

class IEventHandler;

class EventChannel {
 public:
  void registerHandler(IEventHandler *handler) { m_handlers.push_back(handler); }
  void sendEvent(EventID eventID, EventData eventData) {
    for (auto x : m_handlers)
      x->onEvent(eventID, eventData);
  }

 protected:
  std::vector<IEventHandler *> m_handlers{};
};

class IEventHandler {
 public:
  void setEventChannel(EventChannel *channel) { m_channel = channel; }
  virtual void onEvent(EventID eventID, EventData eventData) = 0;

 protected:
  void m_sendEvent(EventID eventID, EventData eventData) { m_channel->sendEvent(eventID, eventData); }
  EventChannel *m_channel{};
};

}  // namespace dongle
