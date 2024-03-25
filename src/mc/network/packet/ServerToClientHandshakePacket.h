#pragma once

#include "mc/_HeaderOutputPredefine.h"

// auto generated inclusion list
#include "mc/deps/core/common/bedrock/Result.h"
#include "mc/enums/MinecraftPacketIds.h"
#include "mc/network/packet/Packet.h"

class ServerToClientHandshakePacket : public ::Packet {
public:
    std::string mData; // this+0x30

    // prevent constructor by default
    ServerToClientHandshakePacket& operator=(ServerToClientHandshakePacket const&);
    ServerToClientHandshakePacket(ServerToClientHandshakePacket const&);

public:
    // NOLINTBEGIN
    // vIndex: 0, symbol: ??1ServerToClientHandshakePacket@@UEAA@XZ
    virtual ~ServerToClientHandshakePacket();

    // vIndex: 1, symbol: ?getId@ServerToClientHandshakePacket@@UEBA?AW4MinecraftPacketIds@@XZ
    virtual ::MinecraftPacketIds getId() const;

    // vIndex: 2, symbol:
    // ?getName@ServerToClientHandshakePacket@@UEBA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ
    virtual std::string getName() const;

    // vIndex: 4, symbol: ?write@ServerToClientHandshakePacket@@UEBAXAEAVBinaryStream@@@Z
    virtual void write(class BinaryStream& stream) const;

    // vIndex: 8, symbol:
    // ?_read@ServerToClientHandshakePacket@@EEAA?AV?$Result@XVerror_code@std@@@Bedrock@@AEAVReadOnlyBinaryStream@@@Z
    virtual class Bedrock::Result<void> _read(class ReadOnlyBinaryStream& stream);

    // symbol: ??0ServerToClientHandshakePacket@@QEAA@XZ
    MCAPI ServerToClientHandshakePacket();

    // symbol:
    // ??0ServerToClientHandshakePacket@@QEAA@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z
    MCAPI explicit ServerToClientHandshakePacket(std::string const& webTokenData);

    // NOLINTEND
};
