#include "Dongle/Dongle.hpp"
#include "Dongle/ITransparentDataHandler.hpp"
#include "Dongle/Transport/ITransport.hpp"

namespace dongle {

void Dongle::task() { m_handler->onDongleIdle(this); }

}  // namespace dongle
