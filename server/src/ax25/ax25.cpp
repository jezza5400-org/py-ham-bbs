#include "ax25.h"
#include <vector>

std::vector<uint8_t> AX25::m_buildCallSign(const std::string &callsign,
                                           int ssid, bool last) const {
  std::string callsignStr(callsign);
  std::transform(
      callsignStr.begin(), callsignStr.end(), callsignStr.begin(),
      [](unsigned char c) { return static_cast<char>(std::toupper(c)); });

  if (callsignStr.size() > 6) {
    callsignStr.resize(6);
  } else if (callsignStr.size() < 6) {
    callsignStr.append(6 - callsignStr.size(), ' ');
  }

  std::vector<uint8_t> out;
  out.reserve(7);
  for (size_t i = 0; i < 6; ++i) {
    out.push_back(static_cast<uint8_t>(
        (static_cast<uint8_t>(callsignStr[i]) << 1) & 0xFE));
  }

  uint8_t ssid_byte = static_cast<uint8_t>(0x60 | ((ssid & 0x0F) << 1));
  if (last)
    ssid_byte |= 0x01;
  out.push_back(ssid_byte);

  return out;
}

std::vector<uint8_t> AX25::encode(const std::vector<uint8_t> &payload) {
  std::vector<uint8_t> frame;

  auto fromCallSign =
      m_buildCallSign(m_config.callSignFrom, m_config.ssidFrom, false);
  auto toCallSign = this->m_buildCallSign(m_config.callSignTo, m_config.ssidTo, true);

  std::vector<uint8_t> header;
  header.insert(header.end(), fromCallSign.begin(), fromCallSign.end());
  header.insert(header.end(), toCallSign.begin(), toCallSign.end());
  header.push_back(0x03); // Control field for UI frame
  header.push_back(0xF0); // Protocol ID for no layer 3 protocol

  frame.insert(frame.end(), header.begin(), header.end());
  frame.insert(frame.end(), payload.begin(), payload.end());

  return frame;
}