# Not including main.cpp
set(SRC_FILES 
  ${CMAKE_CURRENT_LIST_DIR}/Server.cpp
  ${CMAKE_CURRENT_LIST_DIR}/dns/DnsHeader.cpp
  ${CMAKE_CURRENT_LIST_DIR}/dns/DnsPacket.cpp
  ${CMAKE_CURRENT_LIST_DIR}/dns/DnsRecord.cpp
  ${CMAKE_CURRENT_LIST_DIR}/dns/DnsQuestion.cpp
  ${CMAKE_CURRENT_LIST_DIR}/dns/BytePacketBuffer.cpp
  ${CMAKE_CURRENT_LIST_DIR}/dns/RecordType.cpp
  ${CMAKE_CURRENT_LIST_DIR}/dns/RecordClass.cpp
)

set(HPP_FILES 
  ${CMAKE_CURRENT_LIST_DIR}/Server.hpp
  ${CMAKE_CURRENT_LIST_DIR}/dns/DnsHeader.hpp
  ${CMAKE_CURRENT_LIST_DIR}/dns/DnsPacket.hpp
  ${CMAKE_CURRENT_LIST_DIR}/dns/DnsQuestion.hpp
  ${CMAKE_CURRENT_LIST_DIR}/dns/DnsRecord.hpp
  ${CMAKE_CURRENT_LIST_DIR}/dns/BytePacketBuffer.hpp
  ${CMAKE_CURRENT_LIST_DIR}/dns/RecordType.hpp
  ${CMAKE_CURRENT_LIST_DIR}/dns/RecordClass.hpp

)

set(TEST_FILES 
  ${CMAKE_CURRENT_LIST_DIR}/BytePacketBuffer_test.cpp
  ${CMAKE_CURRENT_LIST_DIR}/dns/DnsPacket_test.cpp
)
