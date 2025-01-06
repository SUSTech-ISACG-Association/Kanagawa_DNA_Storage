#ifndef GF_H
#define GF_H

#include "Polynomial.hpp"
#include <stdint.h>
#include <string.h>

namespace RS
{
namespace gf
{

extern const uint8_t exp[255];
extern const uint8_t log[256];

uint8_t add(uint8_t x, uint8_t y);
uint8_t sub(uint8_t x, uint8_t y);
uint8_t mul(uint16_t x, uint16_t y);
uint8_t div(uint8_t x, uint8_t y);
uint8_t pow(uint8_t x, intmax_t power);
uint8_t inverse(uint8_t x);

void poly_scale(const Poly *p, Poly *newp, uint16_t x);
void poly_add(const Poly *p, const Poly *q, Poly *newp);
void poly_mul(const Poly *p, const Poly *q, Poly *newp);
void poly_div(const Poly *p, const Poly *q, Poly *newp);
int8_t poly_eval(const Poly *p, uint16_t x);

} // namespace gf
} // namespace RS

#endif // GF_H