#ifndef RS_HPP
#define RS_HPP

#include "GaloisField.hpp"
#include "Polynomial.hpp"
#include <stdint.h>
#include <string.h>

namespace RS
{

#define MSG_CNT 3
#define POLY_CNT 14

class ReedSolomon
{
public:
    ReedSolomon(uint8_t msg_length, uint8_t ecc_length);
    ~ReedSolomon();

    void EncodeBlock(const void *src, void *dst);
    void Encode(const void *src, void *dst);
    int DecodeBlock(const void *src, const void *ecc, void *dst, uint8_t *erase_pos = NULL, size_t erase_count = 0);
    int Decode(const void *src, void *dst, uint8_t *erase_pos = NULL, size_t erase_count = 0);

private:
    uint8_t msg_length;
    uint8_t ecc_length;

    enum POLY_ID
    {
        ID_MSG_IN = 0,
        ID_MSG_OUT,
        ID_GENERATOR,
        ID_TPOLY1,
        ID_TPOLY2,
        ID_MSG_E,
        ID_TPOLY3,
        ID_TPOLY4,
        ID_SYNDROMES,
        ID_FORNEY,
        ID_ERASURES_LOC,
        ID_ERRORS_LOC,
        ID_ERASURES,
        ID_ERRORS,
        ID_COEF_POS,
        ID_ERR_EVAL
    };

    uint8_t *memory;
    Poly polynoms[MSG_CNT + POLY_CNT];

    void GeneratorPoly();
    void CalcSyndromes(const Poly *msg);
    void FindErrataLocator(const Poly *epos);
    void FindErrorEvaluator(const Poly *synd, const Poly *errata_loc, Poly *dst, uint8_t ecclen);
    void CorrectErrata(const Poly *synd, const Poly *err_pos, const Poly *msg_in);
    bool FindErrorLocator(const Poly *synd, Poly *erase_loc = NULL, size_t erase_count = 0);
    bool FindErrors(const Poly *error_loc, size_t msg_in_size);
    void CalcForneySyndromes(const Poly *synd, const Poly *erasures_pos, size_t msg_in_size);
};

} // namespace RS

#endif // RS_HPP