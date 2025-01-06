#include "Polynomial.hpp"

namespace RS
{

Poly::Poly() : length(0), _memory(NULL) {}

Poly::Poly(uint8_t id, uint16_t offset, uint8_t size) : length(0), _id(id), _size(size), _offset(offset), _memory(NULL)
{
}

bool Poly::Append(uint8_t num)
{
    ptr()[length++] = num;
    return true;
}

void Poly::Init(uint8_t id, uint16_t offset, uint8_t size, uint8_t **memory_ptr)
{
    _id = id;
    _offset = offset;
    _size = size;
    length = 0;
    _memory = memory_ptr;
}

void Poly::Reset() { memset((void *)ptr(), 0, _size); }

void Poly::Set(const uint8_t *src, uint8_t len, uint8_t offset)
{
    memcpy(ptr() + offset, src, len * sizeof(uint8_t));
    length = len + offset;
}

void Poly::Copy(const Poly *src)
{
    length = poly_max(length, src->length);
    Set(src->ptr(), length);
}

uint8_t &Poly::at(uint8_t i) const { return ptr()[i]; }

uint8_t *Poly::ptr() const { return (*_memory) + _offset; }

}