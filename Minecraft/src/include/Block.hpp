#pragma once

enum class BlockType {
    Default = 0,
    Grass,
    Dirt,
    Stone,
};

class Block {
public:
    Block();
    ~Block();

    // Getter
    bool isActive() const { return m_active; }
    BlockType getBlockType() const { return m_blockType; }

    // Setters
    void setActive(bool active);
    void setBlockType(BlockType blockType);

private:
    bool m_active;
    BlockType m_blockType;
};