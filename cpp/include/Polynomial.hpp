#ifndef POLY_H
#define POLY_H

#include <stdint.h>
#include <string.h>

namespace RS
{

#define poly_max(a, b) ((a > b) ? (a) : (b))

struct Poly
{
    Poly();
    Poly(uint8_t id, uint16_t offset, uint8_t size);

    bool Append(uint8_t num);
    void Init(uint8_t id, uint16_t offset, uint8_t size, uint8_t **memory_ptr);
    void Reset();
    void Set(const uint8_t *src, uint8_t len, uint8_t offset = 0);
    void Copy(const Poly *src);

    uint8_t &at(uint8_t i) const;
    inline uint8_t id() const { return _id; }
    inline uint8_t size() const { return _size; }
    uint8_t *ptr() const;

    uint8_t length;

protected:
    uint8_t _id;
    uint8_t _size;
    uint16_t _offset;
    uint8_t **_memory;
};

} // namespace RS

#endif // POLY_H