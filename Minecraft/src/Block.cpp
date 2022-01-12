#include "Block.hpp"

Block::Block() : m_active(false), m_blockType(BlockType::Default) {
}

Block::~Block() {
}

void Block::setActive(bool active) {
    m_active = active;
}

void Block::setBlockType(BlockType blockType) {
    m_blockType = blockType;
}
