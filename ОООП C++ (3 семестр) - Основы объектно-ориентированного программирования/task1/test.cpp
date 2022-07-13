#include "pch.h"
#include "../task1/Trit.cpp"
#include "../task1/TritSet.cpp"

TEST(Trit_test, enum_test)
{
    Trit a;
    a = (Trit)-1;
    ASSERT_EQ((Trit)-1, a);
    a = (Trit)0;
    ASSERT_EQ((Trit)0, a);
    a = (Trit)1;
    ASSERT_EQ((Trit)1, a);
}

TEST(Trit_test, operator_negative1)
{
    Trit a;
    a = (Trit)-1;
    a = !a;
    ASSERT_EQ((Trit)1, a);
    a = (Trit)0;
    a = !a;
    ASSERT_EQ((Trit)0, a);
    a = (Trit)1;
    a = !a;
    ASSERT_EQ((Trit)-1, a);
}

TEST(Trit_test, operator_negative2)
{
    Trit a;
    a = (Trit)-1;
    a = ~a;
    ASSERT_EQ((Trit)1, a);
    a = (Trit)0;
    a = ~a;
    ASSERT_EQ((Trit)0, a);
    a = (Trit)1;
    a = ~a;
    ASSERT_EQ((Trit)-1, a);
}

TEST(Trit_test, operator_and)
{
    Trit a;
    Trit b;
    Trit c;
    a = (Trit)-1;
    b = (Trit)-1;
    c = a & b;
    ASSERT_EQ((Trit)-1, c);
    b = (Trit)0;
    c = a & b;
    ASSERT_EQ((Trit)-1, c);
    b = (Trit)1;
    c = a & b;
    ASSERT_EQ((Trit)-1, c);
    a = (Trit)0;
    b = (Trit)-1;
    c = a & b;
    ASSERT_EQ((Trit)-1, c); 
    b = (Trit)0;
    c = a & b;
    ASSERT_EQ((Trit)0, c);
    b = (Trit)1;
    c = a & b;
    ASSERT_EQ((Trit)0, c);
    a = (Trit)1;
    b = (Trit)-1;
    c = a & b;
    ASSERT_EQ((Trit)-1, c); 
    b = (Trit)0;
    c = a & b;
    ASSERT_EQ((Trit)0, c);
    b = (Trit)1;
    c = a & b;
    ASSERT_EQ((Trit)1, c);
}

TEST(Trit_test, operator_or)
{
    Trit a;
    Trit b;
    Trit c;
    a = (Trit)-1;
    b = (Trit)-1;
    c = a | b;
    ASSERT_EQ((Trit)-1, c);
    b = (Trit)0;
    c = a | b;
    ASSERT_EQ((Trit)0, c);
    b = (Trit)1;
    c = a | b;
    ASSERT_EQ((Trit)1, c);
    a = (Trit)0;
    b = (Trit)-1;
    c = a | b;
    ASSERT_EQ((Trit)0, c);
    b = (Trit)0;
    c = a | b;
    ASSERT_EQ((Trit)0, c);
    b = (Trit)1;
    c = a | b;
    ASSERT_EQ((Trit)1, c);
    a = (Trit)1;
    b = (Trit)-1;
    c = a | b;
    ASSERT_EQ((Trit)1, c);
    b = (Trit)0;
    c = a | b;
    ASSERT_EQ((Trit)1, c);
    b = (Trit)1;
    c = a | b;
    ASSERT_EQ((Trit)1, c);
}

TEST(Trit_test, operator_and_eq)
{
    Trit a;
    Trit b;
    a = (Trit)-1;
    b = (Trit)-1;
    a &= b;
    ASSERT_EQ((Trit)-1, a);
    a = (Trit)-1;
    b = (Trit)0;
    a &= b;
    ASSERT_EQ((Trit)-1, a);
    a = (Trit)-1;
    b = (Trit)1;
    a &= b;
    ASSERT_EQ((Trit)-1, a);
    a = (Trit)0;
    b = (Trit)-1;
    a &= b;
    ASSERT_EQ((Trit)-1, a);
    a = (Trit)0;
    b = (Trit)0;
    a &= b;
    ASSERT_EQ((Trit)0, a);
    a = (Trit)0;
    b = (Trit)1;
    a &= b;
    ASSERT_EQ((Trit)0, a);
    a = (Trit)1;
    b = (Trit)-1;
    a &= b;
    ASSERT_EQ((Trit)-1, a);
    a = (Trit)1;
    b = (Trit)0;
    a &= b;
    ASSERT_EQ((Trit)0, a);
    a = (Trit)1;
    b = (Trit)1;
    a &= b;
    ASSERT_EQ((Trit)1, a);
}

TEST(Trit_test, operator_or_eq)
{
    Trit a;
    Trit b;
    a = (Trit)-1;
    b = (Trit)-1;
    a |= b;
    ASSERT_EQ((Trit)-1, a);
    a = (Trit)-1;
    b = (Trit)0;
    a |= b;
    ASSERT_EQ((Trit)0, a);
    a = (Trit)-1;
    b = (Trit)1;
    a |= b;
    ASSERT_EQ((Trit)1, a);
    a = (Trit)0;
    b = (Trit)-1;
    a |= b;
    ASSERT_EQ((Trit)0, a);
    a = (Trit)0;
    b = (Trit)0;
    a |= b;
    ASSERT_EQ((Trit)0, a);
    a = (Trit)0;
    b = (Trit)1;
    a |= b;
    ASSERT_EQ((Trit)1, a);
    a = (Trit)1;
    b = (Trit)-1;
    a |= b;
    ASSERT_EQ((Trit)1, a);
    a = (Trit)1;
    b = (Trit)0;
    a |= b;
    ASSERT_EQ((Trit)1, a);
    a = (Trit)1;
    b = (Trit)1;
    a |= b;
    ASSERT_EQ((Trit)1, a);
}

TEST(TritSet_test, construction_create)
{
    TritSet a;
    ASSERT_EQ(0, a.capacity());
    TritSet b(1);
    ASSERT_EQ(1, b.capacity());
    TritSet c(16);
    ASSERT_EQ(1, c.capacity());
    TritSet d(17);
    ASSERT_EQ(2, d.capacity());
    TritSet e(1000);
    ASSERT_EQ(63, e.capacity());
}

TEST(TritSet_test, construction_create_memory)
{
    for (int i = 1; i <= 1000; ++i)
    {
        TritSet set(i);
        unsigned int allocLength = set.capacity();
        ASSERT_GE(allocLength, i * 2 / 8 / sizeof(unsigned int));
    }
}

TEST(TritSet_test, operator_index)
{
    TritSet set(8);
    set[0] = (Trit)-1;
    set[1] = (Trit)1;
    set[2] = (Trit)-1;
    set[3] = (Trit)1;
    set[4] = (Trit)0;
    set[5] = (Trit)0;
    set[6] = (Trit)1;
    ASSERT_EQ((Trit)-1, set[0]);
    ASSERT_EQ((Trit)1, set[1]);
    ASSERT_EQ((Trit)-1, set[2]);
    ASSERT_EQ((Trit)1, set[3]);
    ASSERT_EQ((Trit)0, set[4]);
    ASSERT_EQ((Trit)0, set[5]);
    ASSERT_EQ((Trit)1, set[6]);
    ASSERT_EQ((Trit)0, set[7]);
}

TEST(TritSet_test, operator_index_and)
{
    TritSet set(12);
    set[0] = (Trit)-1;
    set[1] = (Trit)0;
    set[2] = (Trit)1;
    set[3] = set[0] & set[0];
    set[4] = set[0] & set[1];
    set[5] = set[0] & set[2];
    set[6] = set[1] & set[0];
    set[7] = set[1] & set[1];
    set[8] = set[1] & set[2];
    set[9] = set[2] & set[0];
    set[10] = set[2] & set[1];
    set[11] = set[2] & set[2];
    ASSERT_EQ((Trit)-1, set[3]);
    ASSERT_EQ((Trit)-1, set[4]);
    ASSERT_EQ((Trit)-1, set[5]);
    ASSERT_EQ((Trit)-1, set[6]);
    ASSERT_EQ((Trit)0, set[7]);
    ASSERT_EQ((Trit)0, set[8]);
    ASSERT_EQ((Trit)-1, set[9]);
    ASSERT_EQ((Trit)0, set[10]);
    ASSERT_EQ((Trit)1, set[11]);
}

TEST(TritSet_test, operator_index_or)
{
    TritSet set(12);
    set[0] = (Trit)-1;
    set[1] = (Trit)0;
    set[2] = (Trit)1;
    set[3] = set[0] | set[0];
    set[4] = set[0] | set[1];
    set[5] = set[0] | set[2];
    set[6] = set[1] | set[0];
    set[7] = set[1] | set[1];
    set[8] = set[1] | set[2];
    set[9] = set[2] | set[0];
    set[10] = set[2] | set[1];
    set[11] = set[2] | set[2];
    ASSERT_EQ((Trit)-1, set[3]);
    ASSERT_EQ((Trit)0, set[4]);
    ASSERT_EQ((Trit)1, set[5]);
    ASSERT_EQ((Trit)0, set[6]);
    ASSERT_EQ((Trit)0, set[7]);
    ASSERT_EQ((Trit)1, set[8]);
    ASSERT_EQ((Trit)1, set[9]);
    ASSERT_EQ((Trit)1, set[10]);
    ASSERT_EQ((Trit)1, set[11]);
}

TEST(TritSet_test, operator_index_and_eq)
{
    TritSet set(12);
    set[0] = (Trit)-1;
    set[1] = (Trit)0;
    set[2] = (Trit)1;
    set[3] = set[0];
    set[3] &= set[0];
    set[4] = set[0];
    set[4] &= set[1];
    set[5] = set[0];
    set[5] &= set[2];
    set[6] = set[1];
    set[6] &= set[0];
    set[7] = set[1];
    set[7] &= set[1];
    set[8] = set[1];
    set[8] &= set[2];
    set[9] = set[2];
    set[9] &= set[0];
    set[10] = set[2];
    set[10] &= set[1];
    set[11] = set[2];
    set[11] &= set[2];
    ASSERT_EQ((Trit)-1, set[3]);
    ASSERT_EQ((Trit)-1, set[4]);
    ASSERT_EQ((Trit)-1, set[5]);
    ASSERT_EQ((Trit)-1, set[6]);
    ASSERT_EQ((Trit)0, set[7]);
    ASSERT_EQ((Trit)0, set[8]);
    ASSERT_EQ((Trit)-1, set[9]);
    ASSERT_EQ((Trit)0, set[10]);
    ASSERT_EQ((Trit)1, set[11]);
}

TEST(TritSet_test, operator_index_or_eq)
{
    TritSet set(12);
    set[0] = (Trit)-1;
    set[1] = (Trit)0;
    set[2] = (Trit)1;
    set[3] = set[0];
    set[3] |= set[0];
    set[4] = set[0];
    set[4] |= set[1];
    set[5] = set[0];
    set[5] |= set[2];
    set[6] = set[1];
    set[6] |= set[0];
    set[7] = set[1];
    set[7] |= set[1];
    set[8] = set[1];
    set[8] |= set[2];
    set[9] = set[2];
    set[9] |= set[0];
    set[10] = set[2];
    set[10] |= set[1];
    set[11] = set[2];
    set[11] |= set[2];
    ASSERT_EQ((Trit)-1, set[3]);
    ASSERT_EQ((Trit)0, set[4]);
    ASSERT_EQ((Trit)1, set[5]);
    ASSERT_EQ((Trit)0, set[6]);
    ASSERT_EQ((Trit)0, set[7]);
    ASSERT_EQ((Trit)1, set[8]);
    ASSERT_EQ((Trit)1, set[9]);
    ASSERT_EQ((Trit)1, set[10]);
    ASSERT_EQ((Trit)1, set[11]);
}

TEST(TritSet_test, base_test_no_new_memory)
{
    TritSet set(1000);
    unsigned int allocLenth = set.capacity();
    set[1000000000] = (Trit)0;
    ASSERT_EQ(allocLenth, set.capacity());
}

TEST(TritSet_test, base_test_new_memory)
{
    TritSet set(1000);
    unsigned int allocLenth = set.capacity();
    set[1000000000] = (Trit)1;
    ASSERT_LE(allocLenth, set.capacity());
}

TEST(TritSet_test, base_test_no_memory_operations)
{
    TritSet set(1000);
    set[1000000000] = (Trit)1;
    unsigned int allocLenth = set.capacity();
    set[1000000000] = (Trit)0;
    set[1000000] = (Trit)-1;
    ASSERT_EQ(allocLenth, set.capacity());
}

TEST(TritSet_test, operator_assignment_1)
{
    TritSet a(10);
    unsigned int countTrits_a;
    unsigned int allocLenth_a;
    TritSet b(20);
    unsigned int countTrits_b = b.capacityTrit();
    unsigned int allocLenth_b = b.capacity();
    a = b;
    countTrits_a = a.capacityTrit();
    allocLenth_a = a.capacity();
    ASSERT_EQ(countTrits_a, countTrits_b);
    ASSERT_EQ(allocLenth_a, allocLenth_b);
}

TEST(TritSet_test, operator_assignment_2)
{
    TritSet a(10);
    unsigned int countTrits_a;
    unsigned int allocLenth_a;
    TritSet b(20);
    unsigned int countTrits_b = b.capacityTrit();
    unsigned int allocLenth_b = b.capacity();
    b = a;
    countTrits_a = a.capacityTrit();
    allocLenth_a = a.capacity();
    ASSERT_LE(countTrits_a, countTrits_b);
    ASSERT_LE(allocLenth_a, allocLenth_b);
}

TEST(TritSet_test, operator_assignment_3)
{
    TritSet b(10);
    b[2] = (Trit)-1;
    b[8] = (Trit)1;
    TritSet a = b;
    ASSERT_EQ(a[2], (Trit)-1);
    ASSERT_EQ(a[8], (Trit)1);
}

TEST(TritSet_test, operator_assignment_4)
{
    TritSet a(10);
    a[0] = (Trit)-1;
    a[9] = (Trit)1;
    TritSet b(10);
    b[2] = (Trit)-1;
    b[8] = (Trit)1;
    a = b;
    ASSERT_EQ(a[0], (Trit)0);
    ASSERT_EQ(a[2], (Trit)-1);
    ASSERT_EQ(a[8], (Trit)1);
    ASSERT_EQ(a[9], (Trit)0);
}

TEST(TritSet_test, operator_negative)
{
    TritSet a(10);
    a[0] = (Trit)-1;
    a[1] = (Trit)-1;
    a[2] = (Trit)-1;
    a[3] = (Trit)0;
    a[4] = (Trit)0;
    a[5] = (Trit)0;
    a[6] = (Trit)1;
    a[7] = (Trit)1;
    a[8] = (Trit)1;
    a = !a;
    ASSERT_EQ(a[0], (Trit)1);
    ASSERT_EQ(a[1], (Trit)1);
    ASSERT_EQ(a[2], (Trit)1);
    ASSERT_EQ(a[3], (Trit)0);
    ASSERT_EQ(a[4], (Trit)0);
    ASSERT_EQ(a[5], (Trit)0);
    ASSERT_EQ(a[6], (Trit)-1);
    ASSERT_EQ(a[7], (Trit)-1);
    ASSERT_EQ(a[8], (Trit)-1);
    ASSERT_EQ(a[9], (Trit)0);
}

TEST(TritSet_test, operator_or_1)
{
    TritSet a(10);
    a[0] = (Trit)-1;
    a[1] = (Trit)-1;
    a[2] = (Trit)-1;
    a[3] = (Trit)0;
    a[4] = (Trit)0;
    a[5] = (Trit)0;
    a[6] = (Trit)1;
    a[7] = (Trit)1;
    a[8] = (Trit)1;
    TritSet b(20);
    b[0] = (Trit)-1;
    b[1] = (Trit)0;
    b[2] = (Trit)1;
    b[3] = (Trit)-1;
    b[4] = (Trit)0;
    b[5] = (Trit)1;
    b[6] = (Trit)-1;
    b[7] = (Trit)0;
    b[8] = (Trit)1;
    b[19] = (Trit)1;
    TritSet c = a | b;
    ASSERT_EQ(c[0], (Trit)-1);
    ASSERT_EQ(c[1], (Trit)0);
    ASSERT_EQ(c[2], (Trit)1);
    ASSERT_EQ(c[3], (Trit)0);
    ASSERT_EQ(c[4], (Trit)0);
    ASSERT_EQ(c[5], (Trit)1);
    ASSERT_EQ(c[6], (Trit)1);
    ASSERT_EQ(c[7], (Trit)1);
    ASSERT_EQ(c[8], (Trit)1);
    ASSERT_EQ(c[9], (Trit)0);
    ASSERT_EQ(c[15], (Trit)0);
    ASSERT_EQ(c[19], (Trit)1);
}

TEST(TritSet_test, operator_or_2)
{
    TritSet a(10);
    a[0] = (Trit)-1;
    a[1] = (Trit)-1;
    a[2] = (Trit)-1;
    a[3] = (Trit)0;
    a[4] = (Trit)0;
    a[5] = (Trit)0;
    a[6] = (Trit)1;
    a[7] = (Trit)1;
    a[8] = (Trit)1;
    TritSet b(20);
    b[0] = (Trit)-1;
    b[1] = (Trit)0;
    b[2] = (Trit)1;
    b[3] = (Trit)-1;
    b[4] = (Trit)0;
    b[5] = (Trit)1;
    b[6] = (Trit)-1;
    b[7] = (Trit)0;
    b[8] = (Trit)1;
    b[19] = (Trit)1;
    TritSet c;
    c = a | b;
    ASSERT_EQ(c[0], (Trit)-1);
    ASSERT_EQ(c[1], (Trit)0);
    ASSERT_EQ(c[2], (Trit)1);
    ASSERT_EQ(c[3], (Trit)0);
    ASSERT_EQ(c[4], (Trit)0);
    ASSERT_EQ(c[5], (Trit)1);
    ASSERT_EQ(c[6], (Trit)1);
    ASSERT_EQ(c[7], (Trit)1);
    ASSERT_EQ(c[8], (Trit)1);
    ASSERT_EQ(c[9], (Trit)0);
    ASSERT_EQ(c[15], (Trit)0);
    ASSERT_EQ(c[19], (Trit)1);
}

TEST(TritSet_test, operator_and_1)
{
    TritSet a(10);
    a[0] = (Trit)-1;
    a[1] = (Trit)-1;
    a[2] = (Trit)-1;
    a[3] = (Trit)0;
    a[4] = (Trit)0;
    a[5] = (Trit)0;
    a[6] = (Trit)1;
    a[7] = (Trit)1;
    a[8] = (Trit)1;
    TritSet b(20);
    b[0] = (Trit)-1;
    b[1] = (Trit)0;
    b[2] = (Trit)1;
    b[3] = (Trit)-1;
    b[4] = (Trit)0;
    b[5] = (Trit)1;
    b[6] = (Trit)-1;
    b[7] = (Trit)0;
    b[8] = (Trit)1;
    b[19] = (Trit)1;
    TritSet c = a & b;
    ASSERT_EQ(c[0], (Trit)-1);
    ASSERT_EQ(c[1], (Trit)-1);
    ASSERT_EQ(c[2], (Trit)-1);
    ASSERT_EQ(c[3], (Trit)-1);
    ASSERT_EQ(c[4], (Trit)0);
    ASSERT_EQ(c[5], (Trit)0);
    ASSERT_EQ(c[6], (Trit)-1);
    ASSERT_EQ(c[7], (Trit)0);
    ASSERT_EQ(c[8], (Trit)1);
    ASSERT_EQ(c[9], (Trit)0);
    ASSERT_EQ(c[15], (Trit)0);
    ASSERT_EQ(c[19], (Trit)0);
}

TEST(TritSet_test, operator_and_2)
{
    TritSet a(10);
    a[0] = (Trit)-1;
    a[1] = (Trit)-1;
    a[2] = (Trit)-1;
    a[3] = (Trit)0;
    a[4] = (Trit)0;
    a[5] = (Trit)0;
    a[6] = (Trit)1;
    a[7] = (Trit)1;
    a[8] = (Trit)1;
    TritSet b(20);
    b[0] = (Trit)-1;
    b[1] = (Trit)0;
    b[2] = (Trit)1;
    b[3] = (Trit)-1;
    b[4] = (Trit)0;
    b[5] = (Trit)1;
    b[6] = (Trit)-1;
    b[7] = (Trit)0;
    b[8] = (Trit)1;
    b[19] = (Trit)1;
    TritSet c;
    c = a & b;
    ASSERT_EQ(c[0], (Trit)-1);
    ASSERT_EQ(c[1], (Trit)-1);
    ASSERT_EQ(c[2], (Trit)-1);
    ASSERT_EQ(c[3], (Trit)-1);
    ASSERT_EQ(c[4], (Trit)0);
    ASSERT_EQ(c[5], (Trit)0);
    ASSERT_EQ(c[6], (Trit)-1);
    ASSERT_EQ(c[7], (Trit)0);
    ASSERT_EQ(c[8], (Trit)1);
    ASSERT_EQ(c[9], (Trit)0);
    ASSERT_EQ(c[15], (Trit)0);
    ASSERT_EQ(c[19], (Trit)0);
}

TEST(TritSet_test, operator_or_eq_1)
{
    TritSet a(10);
    a[0] = (Trit)-1;
    a[1] = (Trit)-1;
    a[2] = (Trit)-1;
    a[3] = (Trit)0;
    a[4] = (Trit)0;
    a[5] = (Trit)0;
    a[6] = (Trit)1;
    a[7] = (Trit)1;
    a[8] = (Trit)1;
    TritSet b(20);
    b[0] = (Trit)-1;
    b[1] = (Trit)0;
    b[2] = (Trit)1;
    b[3] = (Trit)-1;
    b[4] = (Trit)0;
    b[5] = (Trit)1;
    b[6] = (Trit)-1;
    b[7] = (Trit)0;
    b[8] = (Trit)1;
    b[19] = (Trit)1;
    b |= a;
    ASSERT_EQ(b[0], (Trit)-1);
    ASSERT_EQ(b[1], (Trit)0);
    ASSERT_EQ(b[2], (Trit)1);
    ASSERT_EQ(b[3], (Trit)0);
    ASSERT_EQ(b[4], (Trit)0);
    ASSERT_EQ(b[5], (Trit)1);
    ASSERT_EQ(b[6], (Trit)1);
    ASSERT_EQ(b[7], (Trit)1);
    ASSERT_EQ(b[8], (Trit)1);
    ASSERT_EQ(b[9], (Trit)0);
    ASSERT_EQ(b[15], (Trit)0);
    ASSERT_EQ(b[19], (Trit)1);
}

TEST(TritSet_test, operator_or_eq_2)
{
    TritSet a(10);
    a[0] = (Trit)-1;
    a[1] = (Trit)-1;
    a[2] = (Trit)-1;
    a[3] = (Trit)0;
    a[4] = (Trit)0;
    a[5] = (Trit)0;
    a[6] = (Trit)1;
    a[7] = (Trit)1;
    a[8] = (Trit)1;
    TritSet b(20);
    b[0] = (Trit)-1;
    b[1] = (Trit)0;
    b[2] = (Trit)1;
    b[3] = (Trit)-1;
    b[4] = (Trit)0;
    b[5] = (Trit)1;
    b[6] = (Trit)-1;
    b[7] = (Trit)0;
    b[8] = (Trit)1;
    b[19] = (Trit)1;
    a |= b;
    ASSERT_EQ(a[0], (Trit)-1);
    ASSERT_EQ(a[1], (Trit)0);
    ASSERT_EQ(a[2], (Trit)1);
    ASSERT_EQ(a[3], (Trit)0);
    ASSERT_EQ(a[4], (Trit)0);
    ASSERT_EQ(a[5], (Trit)1);
    ASSERT_EQ(a[6], (Trit)1);
    ASSERT_EQ(a[7], (Trit)1);
    ASSERT_EQ(a[8], (Trit)1);
    ASSERT_EQ(a[9], (Trit)0);
}

TEST(TritSet_test, operator_and_eq_1)
{
    TritSet a(10);
    a[0] = (Trit)-1;
    a[1] = (Trit)-1;
    a[2] = (Trit)-1;
    a[3] = (Trit)0;
    a[4] = (Trit)0;
    a[5] = (Trit)0;
    a[6] = (Trit)1;
    a[7] = (Trit)1;
    a[8] = (Trit)1;
    TritSet b(20);
    b[0] = (Trit)-1;
    b[1] = (Trit)0;
    b[2] = (Trit)1;
    b[3] = (Trit)-1;
    b[4] = (Trit)0;
    b[5] = (Trit)1;
    b[6] = (Trit)-1;
    b[7] = (Trit)0;
    b[8] = (Trit)1;
    b[19] = (Trit)1;
    b &= a;
    ASSERT_EQ(b[0], (Trit)-1);
    ASSERT_EQ(b[1], (Trit)-1);
    ASSERT_EQ(b[2], (Trit)-1);
    ASSERT_EQ(b[3], (Trit)-1);
    ASSERT_EQ(b[4], (Trit)0);
    ASSERT_EQ(b[5], (Trit)0);
    ASSERT_EQ(b[6], (Trit)-1);
    ASSERT_EQ(b[7], (Trit)0);
    ASSERT_EQ(b[8], (Trit)1);
    ASSERT_EQ(b[9], (Trit)0);
    ASSERT_EQ(b[15], (Trit)0);
    ASSERT_EQ(b[19], (Trit)0);
}

TEST(TritSet_test, operator_and_eq_2)
{
    TritSet a(10);
    a[0] = (Trit)-1;
    a[1] = (Trit)-1;
    a[2] = (Trit)-1;
    a[3] = (Trit)0;
    a[4] = (Trit)0;
    a[5] = (Trit)0;
    a[6] = (Trit)1;
    a[7] = (Trit)1;
    a[8] = (Trit)1;
    TritSet b(20);
    b[0] = (Trit)-1;
    b[1] = (Trit)0;
    b[2] = (Trit)1;
    b[3] = (Trit)-1;
    b[4] = (Trit)0;
    b[5] = (Trit)1;
    b[6] = (Trit)-1;
    b[7] = (Trit)0;
    b[8] = (Trit)1;
    b[19] = (Trit)1;
    a &= b;
    ASSERT_EQ(a[0], (Trit)-1);
    ASSERT_EQ(a[1], (Trit)-1);
    ASSERT_EQ(a[2], (Trit)-1);
    ASSERT_EQ(a[3], (Trit)-1);
    ASSERT_EQ(a[4], (Trit)0);
    ASSERT_EQ(a[5], (Trit)0);
    ASSERT_EQ(a[6], (Trit)-1);
    ASSERT_EQ(a[7], (Trit)0);
    ASSERT_EQ(a[8], (Trit)1);
    ASSERT_EQ(a[9], (Trit)0);
}

TEST(TritSet_test, func_length)
{
    TritSet set(1000);
    set[0] = (Trit)0;
    ASSERT_EQ(0, set.length());
    set[1] = (Trit)1;
    ASSERT_EQ(2, set.length());
    set[10] = (Trit)-1;
    ASSERT_EQ(11, set.length());
    set[20] = (Trit)0;
    ASSERT_EQ(11, set.length());
    set[1000] = (Trit)1;
    ASSERT_EQ(1001, set.length());
    set[1000] = (Trit)0;
    ASSERT_EQ(11, set.length());
}

TEST(TritSet_test, func_shrink)
{
    TritSet set(1000);
    set[1000] = (Trit)0;
    set[10000] = (Trit)-1;
    set[100000] = (Trit)0;
    set[1000000] = (Trit)1;
    unsigned int allocLength = set.capacity();
    set.shrink();
    ASSERT_EQ(allocLength, set.capacity());
    set[1000000] = (Trit)0;
    set.shrink();
    ASSERT_GE(allocLength, set.capacity());
    allocLength = set.capacity();
    set[10000] = (Trit)0;
    set.shrink();
    ASSERT_GE(allocLength, set.capacity());
    ASSERT_EQ(1, set.capacity());
}

TEST(TritSet_test, func_trim)
{
    TritSet set(1000);
    set.trim(990);
    ASSERT_EQ(991, set.capacityTrit());
    set.trim(100);
    ASSERT_EQ(101, set.capacityTrit());
    set.trim(1);
    ASSERT_EQ(2, set.capacityTrit());
    set.trim(100);
    ASSERT_EQ(2, set.capacityTrit());
    set.trim(0);
    ASSERT_EQ(1, set.capacityTrit());
}

TEST(TritSet_test, func_cardinality)
{
    TritSet set(10);
    set[0] = (Trit)-1;
    set[1] = (Trit)0;
    set[2] = (Trit)1;
    set[3] = (Trit)-1;
    set[4] = (Trit)0;
    set[5] = (Trit)1;
    set[6] = (Trit)-1;
    set[7] = (Trit)0;
    set[8] = (Trit)1;
    ASSERT_EQ(3, set.cardinality((Trit)-1));
    ASSERT_EQ(3, set.cardinality((Trit)0));
    ASSERT_EQ(3, set.cardinality((Trit)1));
    set[9] = (Trit)0;
    ASSERT_EQ(3, set.cardinality((Trit)-1));
    ASSERT_EQ(3, set.cardinality((Trit)0));
    ASSERT_EQ(3, set.cardinality((Trit)1));
    set[10] = (Trit)1;
    set[11] = (Trit)-1;
    ASSERT_EQ(4, set.cardinality((Trit)-1));
    ASSERT_EQ(4, set.cardinality((Trit)0));
    ASSERT_EQ(4, set.cardinality((Trit)1));
    set[100] = (Trit)0;
    ASSERT_EQ(4, set.cardinality((Trit)-1));
    ASSERT_EQ(4, set.cardinality((Trit)0));
    ASSERT_EQ(4, set.cardinality((Trit)1));
    set[100] = (Trit)1;
    ASSERT_EQ(4, set.cardinality((Trit)-1));
    ASSERT_EQ(92, set.cardinality((Trit)0));
    ASSERT_EQ(5, set.cardinality((Trit)1));
}