#include "ax25.h"
#include <vector>

std::vector<uint8_t> AX25::m_buildCallSign(const std::string &callsign, int ssid, bool last) const {
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

  frame.insert(frame.end(), header.begin(), header.end());
  frame.insert(frame.end(), payload.begin(), payload.end());

  return frame;
}

DecodedAX25Frame AX25::decode(const std::vector<uint8_t> &frame) {
  if (frame.size() < 15) {
    throw std::runtime_error("Frame too short to decode");
  }

  DecodedAX25Frame decoded;

  auto decodeCallSign = [](const std::vector<uint8_t> &data, size_t start) {
    std::string callsign;
    for (size_t i = start; i < start + 6; ++i) {
      callsign += static_cast<char>(data[i] >> 1);
    }
    callsign.erase(callsign.find_last_not_of(' ') + 1);
    return callsign;
  };

  decoded.fromCallSign = decodeCallSign(frame, 0);
  decoded.fromSSID = (frame[6] >> 1) & 0x0F;
  decoded.toCallSign = decodeCallSign(frame, 7);
  decoded.toSSID = (frame[13] >> 1) & 0x0F;

  decoded.payload.assign(frame.begin() + 15, frame.end());

  return decoded;
}