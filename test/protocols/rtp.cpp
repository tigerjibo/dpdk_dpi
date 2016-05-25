#include <gtest/gtest.h>
#include "../utils.h"
#include "common.h"

extern protocol_type SearchRtp(rte_mbuf *);

using namespace packet_modifier;

TEST(RTP, BadLength) {
  uint8_t data[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x08, 0x00,

    0x05, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x40, 0x11, // (ttl, proto)
    0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,

    0x80, 0x08, 0x00, 0x03,
    0x00, 0x00, 0x00, 0x00, // timestamp
    0x00, 0x00, 0x00, // ssrc (without one byte)
  };
  auto m = InitPacket(data, sizeof(data));
  ASSERT_EQ(PreparePacket(m), true);
  ASSERT_EQ(SearchRtp(m), UNKNOWN);
  rte_pktmbuf_free(m);
}

TEST(RTP, BadVersion) {
  uint8_t data[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x08, 0x00,

    0x05, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x40, 0x11, // (ttl, proto)
    0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,

    0x40, 0x08, 0x00, 0x03,
    0x00, 0x00, 0x00, 0x00, // timestamp
    0x00, 0x00, 0x00, 0x01,
  };
  auto m = InitPacket(data, sizeof(data));
  ASSERT_EQ(PreparePacket(m), true);
  ASSERT_EQ(SearchRtp(m), UNKNOWN);
  rte_pktmbuf_free(m);
}

TEST(RTP, BadSsrc) {
  uint8_t data[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x08, 0x00,

    0x05, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x40, 0x11, // (ttl, proto)
    0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,

    0x80, 0x08, 0x00, 0x03,
    0x00, 0x00, 0x00, 0x00, // timestamp
    0x00, 0x00, 0x00, 0x00,
  };
  auto m = InitPacket(data, sizeof(data));
  ASSERT_EQ(PreparePacket(m), true);
  ASSERT_EQ(SearchRtp(m), UNKNOWN);
  rte_pktmbuf_free(m);
}

TEST(RTP, BadPayloadType) {
  uint8_t data[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x08, 0x00,

    0x05, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x40, 0x11, // (ttl, proto)
    0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,

    0x80, 0x40, 0x00, 0x03,
    0x00, 0x00, 0x00, 0x00, // timestamp
    0x00, 0x00, 0x00, 0x01,
  };
  auto m = InitPacket(data, sizeof(data));
  ASSERT_EQ(PreparePacket(m), true);
  ASSERT_EQ(SearchRtp(m), UNKNOWN);
  rte_pktmbuf_free(m);
}

TEST(RTP, BadCsrc) {
  uint8_t data[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x08, 0x00,

    0x05, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x40, 0x11, // (ttl, proto)
    0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,

    0x82, 0x08, 0x00, 0x03,
    0x00, 0x00, 0x00, 0x00, // timestamp
    0x00, 0x00, 0x00, 0x01,
    // csrc (second is absent)
    0x00, 0x00, 0x00, 0x00,
  };
  auto m = InitPacket(data, sizeof(data));
  ASSERT_EQ(PreparePacket(m), true);
  ASSERT_EQ(SearchRtp(m), UNKNOWN);
  rte_pktmbuf_free(m);
}

TEST(RTP, BadExt) {
  uint8_t data[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x08, 0x00,

    0x05, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x40, 0x11, // (ttl, proto)
    0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,

    0x90, 0x08, 0x00, 0x03,
    0x00, 0x00, 0x00, 0x00, // timestamp
    0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x02,
    // ext data (second is absent)
    0x00, 0x00, 0x00, 0x00,
  };
  auto m = InitPacket(data, sizeof(data));
  ASSERT_EQ(PreparePacket(m), true);
  ASSERT_EQ(SearchRtp(m), UNKNOWN);
  rte_pktmbuf_free(m);
}

TEST(RTP, GoodWithoutCsrcAndWithoutExt) {
  uint8_t data[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x08, 0x00,

    0x05, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x40, 0x11, // (ttl, proto)
    0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,

    0x80, 0x08, 0x00, 0x03,
    0x00, 0x00, 0x00, 0x00, // timestamp
    0x00, 0x00, 0x00, 0x01,
  };
  auto m = InitPacket(data, sizeof(data));
  ASSERT_EQ(PreparePacket(m), true);
  ASSERT_EQ(SearchRtp(m), RTP);
  rte_pktmbuf_free(m);
}

TEST(RTP, GoodWithCsrcAndWithoutExt) {
  uint8_t data[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x08, 0x00,

    0x05, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x40, 0x11, // (ttl, proto)
    0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,

    0x81, 0x08, 0x00, 0x03,
    0x00, 0x00, 0x00, 0x00, // timestamp
    0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00,
  };
  auto m = InitPacket(data, sizeof(data));
  ASSERT_EQ(PreparePacket(m), true);
  ASSERT_EQ(SearchRtp(m), RTP);
  rte_pktmbuf_free(m);
}

TEST(RTP, GoodWithoutCsrcAndWithExt) {
  uint8_t data[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x08, 0x00,

    0x05, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x40, 0x11, // (ttl, proto)
    0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,

    0x90, 0x08, 0x00, 0x03,
    0x00, 0x00, 0x00, 0x00, // timestamp
    0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00,
  };
  auto m = InitPacket(data, sizeof(data));
  ASSERT_EQ(PreparePacket(m), true);
  ASSERT_EQ(SearchRtp(m), RTP);
  rte_pktmbuf_free(m);
}

TEST(RTP, GoodWithCsrcAndWithExt) {
  uint8_t data[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x08, 0x00,

    0x05, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x40, 0x11, // (ttl, proto)
    0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,

    0x91, 0x08, 0x00, 0x03,
    0x00, 0x00, 0x00, 0x00, // timestamp
    0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00,
  };
  auto m = InitPacket(data, sizeof(data));
  ASSERT_EQ(PreparePacket(m), true);
  ASSERT_EQ(SearchRtp(m), RTP);
  rte_pktmbuf_free(m);
}
