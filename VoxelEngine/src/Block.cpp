#include "Block.hpp"

Block::Block() : m_id(0), m_light(0), m_padding(0) {
}

Block::~Block() {
}

void Block::setActive(bool active) {
    m_id = active;
}

void Block::setBlockType(BlockType blockType) {
    m_id = static_cast<short>(blockType);
}
