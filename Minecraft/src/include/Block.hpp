#pragma once
#include <cstdint>

enum class BlockType : uint16_t {
    Air = 0,
    Grass,
    Dirt,
    Stone,
    Bedrock
};

class Block {
public:
    Block();
    ~Block();

    // Getter
    bool isActive() const { return m_id != 0; }
    inline BlockType getBlockType() const { return static_cast<BlockType>(m_id); }

    // Setters
    void setActive(bool active);
    void setBlockType(BlockType blockType);

private:
    uint16_t m_id;
    int16_t m_light;
    int32_t m_padding;
};