/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-11 by Raw Material Software Ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the GNU General
   Public License (Version 2), as published by the Free Software Foundation.
   A copy of the license is included in the JUCE distribution, or can be found
   online at www.gnu.org/licenses.

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.rawmaterialsoftware.com/juce for more information.

  ==============================================================================
*/

#include "../core/juce_StandardHeader.h"

BEGIN_JUCE_NAMESPACE

#include "juce_BigInteger.h"
#include "../memory/juce_MemoryBlock.h"
#include "juce_Random.h"


//==============================================================================
BigInteger::BigInteger()
    : numValues (4),
      highestBit (-1),
      negative (false)
{
    values.calloc (numValues + 1);
}

BigInteger::BigInteger (const int32 value)
    : numValues (4),
      highestBit (31),
      negative (value < 0)
{
    values.calloc (numValues + 1);
    values[0] = abs (value);
    highestBit = getHighestBit();
}

BigInteger::BigInteger (const uint32 value)
    : numValues (4),
      highestBit (31),
      negative (false)
{
    values.calloc (numValues + 1);
    values[0] = value;
    highestBit = getHighestBit();
}

BigInteger::BigInteger (int64 value)
    : numValues (4),
      highestBit (63),
      negative (value < 0)
{
    values.calloc (numValues + 1);

    if (value < 0)
        value = -value;

    values[0] = (uint32) value;
    values[1] = (uint32) (value >> 32);
    highestBit = getHighestBit();
}

BigInteger::BigInteger (const BigInteger& other)
    : numValues (jmax (4, bitToIndex (other.highestBit) + 1)),
      highestBit (other.getHighestBit()),
      negative (other.negative)
{
    values.malloc (numValues + 1);
    memcpy (values, other.values, sizeof (uint32) * (numValues + 1));
}

BigInteger::~BigInteger()
{
}

void BigInteger::swapWith (BigInteger& other) noexcept
{
    values.swapWith (other.values);
    std::swap (numValues, other.numValues);
    std::swap (highestBit, other.highestBit);
    std::swap (negative, other.negative);
}

BigInteger& BigInteger::operator= (const BigInteger& other)
{
    if (this != &other)
    {
        highestBit = other.getHighestBit();
        numValues = jmax (4, bitToIndex (highestBit) + 1);
        negative = other.negative;
        values.malloc (numValues + 1);
        memcpy (values, other.values, sizeof (uint32) * (numValues + 1));
    }

    return *this;
}

void BigInteger::ensureSize (const int numVals)
{
    if (numVals + 2 >= numValues)
    {
        int oldSize = numValues;
        numValues = ((numVals + 2) * 3) / 2;
        values.realloc (numValues + 1);

        while (oldSize < numValues)
            values [oldSize++] = 0;
    }
}

//==============================================================================
bool BigInteger::operator[] (const int bit) const noexcept
{
    return bit <= highestBit && bit >= 0
             && ((values [bitToIndex (bit)] & bitToMask (bit)) != 0);
}

int BigInteger::toInteger() const noexcept
{
    const int n = (int) (values[0] & 0x7fffffff);
    return negative ? -n : n;
}

BigInteger BigInteger::getBitRange (int startBit, int numBits) const
{
    BigInteger r;
    numBits = jmin (numBits, getHighestBit() + 1 - startBit);
    r.ensureSize (bitToIndex (numBits));
    r.highestBit = numBits;

    int i = 0;
    while (numBits > 0)
    {
        r.values[i++] = getBitRangeAsInt (startBit, jmin (32, numBits));
        numBits -= 32;
        startBit += 32;
    }

    r.highestBit = r.getHighestBit();
    return r;
}

int BigInteger::getBitRangeAsInt (const int startBit, int numBits) const noexcept
{
    if (numBits > 32)
    {
        jassertfalse;  // use getBitRange() if you need more than 32 bits..
        numBits = 32;
    }

    numBits = jmin (numBits, highestBit + 1 - startBit);

    if (numBits <= 0)
        return 0;

    const int pos = bitToIndex (startBit);
    const int offset = startBit & 31;
    const int endSpace = 32 - numBits;

    uint32 n = ((uint32) values [pos]) >> offset;

    if (offset > endSpace)
        n |= ((uint32) values [pos + 1]) << (32 - offset);

    return (int) (n & (((uint32) 0xffffffff) >> endSpace));
}

void BigInteger::setBitRangeAsInt (const int startBit, int numBits, uint32 valueToSet)
{
    if (numBits > 32)
    {
        jassertfalse;
        numBits = 32;
    }

    for (int i = 0; i < numBits; ++i)
    {
        setBit (startBit + i, (valueToSet & 1) != 0);
        valueToSet >>= 1;
    }
}

//==============================================================================
void BigInteger::clear()
{
    if (numValues > 16)
    {
        numValues = 4;
        values.calloc (numValues + 1);
    }
    else
    {
        values.clear (numValues + 1);
    }

    highestBit = -1;
    negative = false;
}

void BigInteger::setBit (const int bit)
{
    if (bit >= 0)
    {
        if (bit > highestBit)
        {
            ensureSize (bitToIndex (bit));
            highestBit = bit;
        }

        values [bitToIndex (bit)] |= bitToMask (bit);
    }
}

void BigInteger::setBit (const int bit, const bool shouldBeSet)
{
    if (shouldBeSet)
        setBit (bit);
    else
        clearBit (bit);
}

void BigInteger::clearBit (const int bit) noexcept
{
    if (bit >= 0 && bit <= highestBit)
        values [bitToIndex (bit)] &= ~bitToMask (bit);
}

void BigInteger::setRange (int startBit, int numBits, const bool shouldBeSet)
{
    while (--numBits >= 0)
        setBit (startBit++, shouldBeSet);
}

void BigInteger::insertBit (const int bit, const bool shouldBeSet)
{
    if (bit >= 0)
        shiftBits (1, bit);

    setBit (bit, shouldBeSet);
}

//==============================================================================
bool BigInteger::isZero() const noexcept
{
    return getHighestBit() < 0;
}

bool BigInteger::isOne() const noexcept
{
    return getHighestBit() == 0 && ! negative;
}

bool BigInteger::isNegative() const noexcept
{
    return negative && ! isZero();
}

void BigInteger::setNegative (const bool neg) noexcept
{
    negative = neg;
}

void BigInteger::negate() noexcept
{
    negative = (! negative) && ! isZero();
}

#if JUCE_USE_INTRINSICS && ! defined (__INTEL_COMPILER)
 #pragma intrinsic (_BitScanReverse)
#endif

namespace BitFunctions
{
    inline int countBitsInInt32 (uint32 n) noexcept
    {
        n -= ((n >> 1) & 0x55555555);
        n =  (((n >> 2) & 0x33333333) + (n & 0x33333333));
        n =  (((n >> 4) + n) & 0x0f0f0f0f);
        n += (n >> 8);
        n += (n >> 16);
        return n & 0x3f;
    }

    inline int highestBitInInt (uint32 n) noexcept
    {
        jassert (n != 0); // (the built-in functions may not work for n = 0)

      #if JUCE_GCC
        return 31 - __builtin_clz (n);
      #elif JUCE_USE_INTRINSICS
        unsigned long highest;
        _BitScanReverse (&highest, n);
        return (int) highest;
      #else
        n |= (n >> 1);
        n |= (n >> 2);
        n |= (n >> 4);
        n |= (n >> 8);
        n |= (n >> 16);
        return countBitsInInt32 (n >> 1);
      #endif
    }
}

int BigInteger::countNumberOfSetBits() const noexcept
{
    int total = 0;

    for (int i = bitToIndex (highestBit) + 1; --i >= 0;)
        total += BitFunctions::countBitsInInt32 (values[i]);

    return total;
}

int BigInteger::getHighestBit() const noexcept
{
    for (int i = bitToIndex (highestBit + 1); i >= 0; --i)
    {
        const uint32 n = values[i];

        if (n != 0)
            return BitFunctions::highestBitInInt (n) + (i << 5);
    }

    return -1;
}

int BigInteger::findNextSetBit (int i) const noexcept
{
    for (; i <= highestBit; ++i)
        if ((values [bitToIndex (i)] & bitToMask (i)) != 0)
            return i;

    return -1;
}

int BigInteger::findNextClearBit (int i) const noexcept
{
    for (; i <= highestBit; ++i)
        if ((values [bitToIndex (i)] & bitToMask (i)) == 0)
            break;

    return i;
}

//==============================================================================
BigInteger& BigInteger::operator+= (const BigInteger& other)
{
    if (other.isNegative())
        return operator-= (-other);

    if (isNegative())
    {
        if (compareAbsolute (other) < 0)
        {
            BigInteger temp (*this);
            temp.negate();
            *this = other;
            operator-= (temp);
        }
        else
        {
            negate();
            operator-= (other);
            negate();
        }
    }
    else
    {
        if (other.highestBit > highestBit)
            highestBit = other.highestBit;

        ++highestBit;

        const int numInts = bitToIndex (highestBit) + 1;
        ensureSize (numInts);

        int64 remainder = 0;

        for (int i = 0; i <= numInts; ++i)
        {
            if (i < numValues)
                remainder += values[i];

            if (i < other.numValues)
                remainder += other.values[i];

            values[i] = (uint32) remainder;
            remainder >>= 32;
        }

        jassert (remainder == 0);
        highestBit = getHighestBit();
    }

    return *this;
}

BigInteger& BigInteger::operator-= (const BigInteger& other)
{
    if (other.isNegative())
        return operator+= (-other);

    if (! isNegative())
    {
        if (compareAbsolute (other) < 0)
        {
            BigInteger temp (other);
            swapWith (temp);
            operator-= (temp);
            negate();
            return *this;
        }
    }
    else
    {
        negate();
        operator+= (other);
        negate();
        return *this;
    }

    const int numInts = bitToIndex (highestBit) + 1;
    const int maxOtherInts = bitToIndex (other.highestBit) + 1;
    int64 amountToSubtract = 0;

    for (int i = 0; i <= numInts; ++i)
    {
        if (i <= maxOtherInts)
            amountToSubtract += (int64) other.values[i];

        if (values[i] >= amountToSubtract)
        {
            values[i] = (uint32) (values[i] - amountToSubtract);
            amountToSubtract = 0;
        }
        else
        {
            const int64 n = ((int64) values[i] + (((int64) 1) << 32)) - amountToSubtract;
            values[i] = (uint32) n;
            amountToSubtract = 1;
        }
    }

    return *this;
}

BigInteger& BigInteger::operator*= (const BigInteger& other)
{
    BigInteger total;
    highestBit = getHighestBit();
    const bool wasNegative = isNegative();
    setNegative (false);

    for (int i = 0; i <= highestBit; ++i)
    {
        if (operator[](i))
        {
            BigInteger n (other);
            n.setNegative (false);
            n <<= i;
            total += n;
        }
    }

    total.setNegative (wasNegative ^ other.isNegative());
    swapWith (total);
    return *this;
}

void BigInteger::divideBy (const BigInteger& divisor, BigInteger& remainder)
{
    jassert (this != &remainder); // (can't handle passing itself in to get the remainder)

    const int divHB = divisor.getHighestBit();
    const int ourHB = getHighestBit();

    if (divHB < 0 || ourHB < 0)
    {
        // division by zero
        remainder.clear();
        clear();
    }
    else
    {
        const bool wasNegative = isNegative();

        swapWith (remainder);
        remainder.setNegative (false);
        clear();

        BigInteger temp (divisor);
        temp.setNegative (false);

        int leftShift = ourHB - divHB;
        temp <<= leftShift;

        while (leftShift >= 0)
        {
            if (remainder.compareAbsolute (temp) >= 0)
            {
                remainder -= temp;
                setBit (leftShift);
            }

            if (--leftShift >= 0)
                temp >>= 1;
        }

        negative = wasNegative ^ divisor.isNegative();
        remainder.setNegative (wasNegative);
    }
}

BigInteger& BigInteger::operator/= (const BigInteger& other)
{
    BigInteger remainder;
    divideBy (other, remainder);
    return *this;
}

BigInteger& BigInteger::operator|= (const BigInteger& other)
{
    // this operation doesn't take into account negative values..
    jassert (isNegative() == other.isNegative());

    if (other.highestBit >= 0)
    {
        ensureSize (bitToIndex (other.highestBit));

        int n = bitToIndex (other.highestBit) + 1;

        while (--n >= 0)
            values[n] |= other.values[n];

        if (other.highestBit > highestBit)
            highestBit = other.highestBit;

        highestBit = getHighestBit();
    }

    return *this;
}

BigInteger& BigInteger::operator&= (const BigInteger& other)
{
    // this operation doesn't take into account negative values..
    jassert (isNegative() == other.isNegative());

    int n = numValues;

    while (n > other.numValues)
        values[--n] = 0;

    while (--n >= 0)
        values[n] &= other.values[n];

    if (other.highestBit < highestBit)
        highestBit = other.highestBit;

    highestBit = getHighestBit();
    return *this;
}

BigInteger& BigInteger::operator^= (const BigInteger& other)
{
    // this operation will only work with the absolute values
    jassert (isNegative() == other.isNegative());

    if (other.highestBit >= 0)
    {
        ensureSize (bitToIndex (other.highestBit));

        int n = bitToIndex (other.highestBit) + 1;

        while (--n >= 0)
            values[n] ^= other.values[n];

        if (other.highestBit > highestBit)
            highestBit = other.highestBit;

        highestBit = getHighestBit();
    }

    return *this;
}

BigInteger& BigInteger::operator%= (const BigInteger& divisor)
{
    BigInteger remainder;
    divideBy (divisor, remainder);
    swapWith (remainder);
    return *this;
}

BigInteger& BigInteger::operator++()      { return operator+= (1); }
BigInteger& BigInteger::operator--()      { return operator-= (1); }
BigInteger  BigInteger::operator++ (int)  { const BigInteger old (*this); operator+= (1); return old; }
BigInteger  BigInteger::operator-- (int)  { const BigInteger old (*this); operator-= (1); return old; }

BigInteger  BigInteger::operator-() const                            { BigInteger b (*this); b.negate(); return b; }
BigInteger  BigInteger::operator+   (const BigInteger& other) const  { BigInteger b (*this); return b += other; }
BigInteger  BigInteger::operator-   (const BigInteger& other) const  { BigInteger b (*this); return b -= other; }
BigInteger  BigInteger::operator*   (const BigInteger& other) const  { BigInteger b (*this); return b *= other; }
BigInteger  BigInteger::operator/   (const BigInteger& other) const  { BigInteger b (*this); return b /= other; }
BigInteger  BigInteger::operator|   (const BigInteger& other) const  { BigInteger b (*this); return b |= other; }
BigInteger  BigInteger::operator&   (const BigInteger& other) const  { BigInteger b (*this); return b &= other; }
BigInteger  BigInteger::operator^   (const BigInteger& other) const  { BigInteger b (*this); return b ^= other; }
BigInteger  BigInteger::operator%   (const BigInteger& other) const  { BigInteger b (*this); return b %= other; }
BigInteger  BigInteger::operator<<  (const int numBits) const        { BigInteger b (*this); return b <<= numBits; }
BigInteger  BigInteger::operator>>  (const int numBits) const        { BigInteger b (*this); return b >>= numBits; }
BigInteger& BigInteger::operator<<= (const int numBits)              { shiftBits (numBits, 0);  return *this; }
BigInteger& BigInteger::operator>>= (const int numBits)              { shiftBits (-numBits, 0); return *this; }

//==============================================================================
int BigInteger::compare (const BigInteger& other) const noexcept
{
    if (isNegative() == other.isNegative())
    {
        const int absComp = compareAbsolute (other);
        return isNegative() ? -absComp : absComp;
    }
    else
    {
        return isNegative() ? -1 : 1;
    }
}

int BigInteger::compareAbsolute (const BigInteger& other) const noexcept
{
    const int h1 = getHighestBit();
    const int h2 = other.getHighestBit();

    if (h1 > h2)
        return 1;
    else if (h1 < h2)
        return -1;

    for (int i = bitToIndex (h1) + 1; --i >= 0;)
        if (values[i] != other.values[i])
            return (values[i] > other.values[i]) ? 1 : -1;

    return 0;
}

bool BigInteger::operator== (const BigInteger& other) const noexcept    { return compare (other) == 0; }
bool BigInteger::operator!= (const BigInteger& other) const noexcept    { return compare (other) != 0; }
bool BigInteger::operator<  (const BigInteger& other) const noexcept    { return compare (other) < 0; }
bool BigInteger::operator<= (const BigInteger& other) const noexcept    { return compare (other) <= 0; }
bool BigInteger::operator>  (const BigInteger& other) const noexcept    { return compare (other) > 0; }
bool BigInteger::operator>= (const BigInteger& other) const noexcept    { return compare (other) >= 0; }

//==============================================================================
void BigInteger::shiftLeft (int bits, const int startBit)
{
    if (startBit > 0)
    {
        for (int i = highestBit + 1; --i >= startBit;)
            setBit (i + bits, operator[] (i));

        while (--bits >= 0)
            clearBit (bits + startBit);
    }
    else
    {
        ensureSize (bitToIndex (highestBit + bits) + 1);

        const int wordsToMove = bitToIndex (bits);
        int top = 1 + bitToIndex (highestBit);
        highestBit += bits;

        if (wordsToMove > 0)
        {
            int i;
            for (i = top; --i >= 0;)
                values [i + wordsToMove] = values [i];

            for (i = 0; i < wordsToMove; ++i)
                values [i] = 0;

            bits &= 31;
        }

        if (bits != 0)
        {
            const int invBits = 32 - bits;

            for (int i = top + 1 + wordsToMove; --i > wordsToMove;)
                values[i] = (values[i] << bits) | (values [i - 1] >> invBits);

            values [wordsToMove] = values [wordsToMove] << bits;
        }

        highestBit = getHighestBit();
    }
}

void BigInteger::shiftRight (int bits, const int startBit)
{
    if (startBit > 0)
    {
        for (int i = startBit; i <= highestBit; ++i)
            setBit (i, operator[] (i + bits));

        highestBit = getHighestBit();
    }
    else
    {
        if (bits > highestBit)
        {
            clear();
        }
        else
        {
            const int wordsToMove = bitToIndex (bits);
            int top = 1 + bitToIndex (highestBit) - wordsToMove;
            highestBit -= bits;

            if (wordsToMove > 0)
            {
                int i;
                for (i = 0; i < top; ++i)
                    values [i] = values [i + wordsToMove];

                for (i = 0; i < wordsToMove; ++i)
                    values [top + i] = 0;

                bits &= 31;
            }

            if (bits != 0)
            {
                const int invBits = 32 - bits;

                --top;
                for (int i = 0; i < top; ++i)
                    values[i] = (values[i] >> bits) | (values [i + 1] << invBits);

                values[top] = (values[top] >> bits);
            }

            highestBit = getHighestBit();
        }
    }
}

void BigInteger::shiftBits (int bits, const int startBit)
{
    if (highestBit >= 0)
    {
        if (bits < 0)
            shiftRight (-bits, startBit);
        else if (bits > 0)
            shiftLeft (bits, startBit);
    }
}

//==============================================================================
BigInteger BigInteger::simpleGCD (BigInteger* m, BigInteger* n)
{
    while (! m->isZero())
    {
        if (n->compareAbsolute (*m) > 0)
            std::swap (m, n);

        *m -= *n;
    }

    return *n;
}

BigInteger BigInteger::findGreatestCommonDivisor (BigInteger n) const
{
    BigInteger m (*this);

    while (! n.isZero())
    {
        if (abs (m.getHighestBit() - n.getHighestBit()) <= 16)
            return simpleGCD (&m, &n);

        BigInteger temp1 (m), temp2;
        temp1.divideBy (n, temp2);

        m = n;
        n = temp2;
    }

    return m;
}

void BigInteger::exponentModulo (const BigInteger& exponent, const BigInteger& modulus)
{
    BigInteger exp (exponent);
    exp %= modulus;

    BigInteger value (1);
    swapWith (value);
    value %= modulus;

    while (! exp.isZero())
    {
        if (exp [0])
        {
            operator*= (value);
            operator%= (modulus);
        }

        value *= value;
        value %= modulus;
        exp >>= 1;
    }
}

void BigInteger::inverseModulo (const BigInteger& modulus)
{
    if (modulus.isOne() || modulus.isNegative())
    {
        clear();
        return;
    }

    if (isNegative() || compareAbsolute (modulus) >= 0)
        operator%= (modulus);

    if (isOne())
        return;

    if (! (*this)[0])
    {
        // not invertible
        clear();
        return;
    }

    BigInteger a1 (modulus);
    BigInteger a2 (*this);
    BigInteger b1 (modulus);
    BigInteger b2 (1);

    while (! a2.isOne())
    {
        BigInteger temp1, temp2, multiplier (a1);
        multiplier.divideBy (a2, temp1);

        temp1 = a2;
        temp1 *= multiplier;
        temp2 = a1;
        temp2 -= temp1;
        a1 = a2;
        a2 = temp2;

        temp1 = b2;
        temp1 *= multiplier;
        temp2 = b1;
        temp2 -= temp1;
        b1 = b2;
        b2 = temp2;
    }

    while (b2.isNegative())
        b2 += modulus;

    b2 %= modulus;
    swapWith (b2);
}

//==============================================================================
OutputStream& JUCE_CALLTYPE operator<< (OutputStream& stream, const BigInteger& value)
{
    return stream << value.toString (10);
}

String BigInteger::toString (const int base, const int minimumNumCharacters) const
{
    String s;
    BigInteger v (*this);

    if (base == 2 || base == 8 || base == 16)
    {
        const int bits = (base == 2) ? 1 : (base == 8 ? 3 : 4);
        static const char* const hexDigits = "0123456789abcdef";

        for (;;)
        {
            const int remainder = v.getBitRangeAsInt (0, bits);

            v >>= bits;

            if (remainder == 0 && v.isZero())
                break;

            s = String::charToString (hexDigits [remainder]) + s;
        }
    }
    else if (base == 10)
    {
        const BigInteger ten (10);
        BigInteger remainder;

        for (;;)
        {
            v.divideBy (ten, remainder);

            if (remainder.isZero() && v.isZero())
                break;

            s = String (remainder.getBitRangeAsInt (0, 8)) + s;
        }
    }
    else
    {
        jassertfalse; // can't do the specified base!
        return String::empty;
    }

    s = s.paddedLeft ('0', minimumNumCharacters);

    return isNegative() ? "-" + s : s;
}

void BigInteger::parseString (const String& text, const int base)
{
    clear();
    String::CharPointerType t (text.getCharPointer());

    if (base == 2 || base == 8 || base == 16)
    {
        const int bits = (base == 2) ? 1 : (base == 8 ? 3 : 4);

        for (;;)
        {
            const juce_wchar c = t.getAndAdvance();
            const int digit = CharacterFunctions::getHexDigitValue (c);

            if (((uint32) digit) < (uint32) base)
            {
                operator<<= (bits);
                operator+= (digit);
            }
            else if (c == 0)
            {
                break;
            }
        }
    }
    else if (base == 10)
    {
        const BigInteger ten ((uint32) 10);

        for (;;)
        {
            const juce_wchar c = t.getAndAdvance();

            if (c >= '0' && c <= '9')
            {
                operator*= (ten);
                operator+= ((int) (c - '0'));
            }
            else if (c == 0)
            {
                break;
            }
        }
    }

    setNegative (text.trimStart().startsWithChar ('-'));
}

MemoryBlock BigInteger::toMemoryBlock() const
{
    const int numBytes = (getHighestBit() + 8) >> 3;
    MemoryBlock mb ((size_t) numBytes);

    for (int i = 0; i < numBytes; ++i)
        mb[i] = (uint8) getBitRangeAsInt (i << 3, 8);

    return mb;
}

void BigInteger::loadFromMemoryBlock (const MemoryBlock& data)
{
    clear();

    for (int i = (int) data.getSize(); --i >= 0;)
        this->setBitRangeAsInt ((int) (i << 3), 8, data [i]);
}

END_JUCE_NAMESPACE
