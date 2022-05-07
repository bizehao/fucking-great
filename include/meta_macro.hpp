#pragma once

// clang-format off

#define _ARG_OP_1(op, sep, arg     )  op(arg)
#define _ARG_OP_2(op, sep, arg, ...)  op(arg) sep  _EXPAND_(_ARG_OP_1(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_3(op, sep, arg, ...)  op(arg) sep  _EXPAND_(_ARG_OP_2(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_4(op, sep, arg, ...)  op(arg) sep  _EXPAND_(_ARG_OP_3(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_5(op, sep, arg, ...)  op(arg) sep  _EXPAND_(_ARG_OP_4(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_6(op, sep, arg, ...)  op(arg) sep  _EXPAND_(_ARG_OP_5(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_7(op, sep, arg, ...)  op(arg) sep  _EXPAND_(_ARG_OP_6(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_8(op, sep, arg, ...)  op(arg) sep  _EXPAND_(_ARG_OP_7(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_9(op, sep, arg, ...)  op(arg) sep  _EXPAND_(_ARG_OP_8(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_10(op, sep, arg, ...) op(arg) sep  _EXPAND_(_ARG_OP_9(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_11(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_10(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_12(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_11(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_13(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_12(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_14(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_13(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_15(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_14(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_16(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_15(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_16(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_15(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_17(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_16(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_18(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_17(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_19(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_18(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_20(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_19(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_21(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_20(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_22(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_21(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_23(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_22(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_24(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_23(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_25(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_24(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_26(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_25(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_27(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_26(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_28(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_27(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_29(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_28(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_30(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_29(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_31(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_30(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_32(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_31(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_33(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_32(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_34(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_33(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_35(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_34(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_36(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_35(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_37(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_36(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_38(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_37(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_39(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_38(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_40(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_39(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_41(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_40(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_42(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_41(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_43(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_42(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_44(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_43(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_45(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_44(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_46(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_45(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_47(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_46(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_48(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_47(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_49(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_48(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_50(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_49(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_51(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_50(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_52(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_51(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_53(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_52(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_54(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_53(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_55(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_54(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_56(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_55(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_57(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_56(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_58(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_57(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_59(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_58(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_60(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_59(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_61(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_60(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_62(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_61(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_63(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_62(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_64(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_63(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_65(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_64(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_66(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_65(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_67(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_66(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_68(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_67(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_69(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_68(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_70(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_69(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_71(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_70(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_72(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_71(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_73(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_72(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_74(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_73(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_75(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_74(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_76(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_75(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_77(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_76(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_78(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_77(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_79(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_78(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_80(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_79(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_81(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_80(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_82(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_81(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_83(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_82(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_84(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_83(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_85(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_84(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_86(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_85(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_87(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_86(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_88(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_87(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_89(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_88(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_90(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_89(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_91(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_90(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_92(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_91(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_93(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_92(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_94(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_93(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_95(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_94(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_96(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_95(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_97(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_96(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_98(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_97(op, _EXPAND_(sep), ##__VA_ARGS__))
#define _ARG_OP_99(op, sep, arg, ...) op(arg) sep _EXPAND_(_ARG_OP_98(op, _EXPAND_(sep), ##__VA_ARGS__))

#define _ARG_SEQ() \
         99,98,97,96,95,94,93,92,91,90, \
         89,88,87,86,85,84,83,82,81,80, \
         79,78,77,76,75,74,73,72,71,70, \
         69,68,67,66,65,64,63,62,61,60, \
         59,58,57,56,55,54,53,52,51,50, \
         49,48,47,46,45,44,43,42,41,40, \
         39,38,37,36,35,34,33,32,31,30, \
         29,28,27,26,25,24,23,22,21,20, \
         19,18,17,16,15,14,13,12,11,10, \
          9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#define _ARG_N( \
        _0, _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
        _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
        _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
        _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
        _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
        _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
        _61,_62,_63,_64,_65,_66,_67,_68,_69,_70, \
        _71,_72,_73,_74,_75,_76,_77,_78,_79,_80, \
        _81,_82,_83,_84,_85,_86,_87,_88,_89,_90, \
        _91,_92,_93,_94,_95,_96,_97,_98,_99, N, ...) N

#define _MACRO_CONCAT_(A, B) A##B
#define _MACRO_CONCAT(A, B) _MACRO_CONCAT_(A, B)
#define _EXPAND_(...) __VA_ARGS__

#define _ARG_COUNT_(...) _EXPAND_(_ARG_N(__VA_ARGS__))
#define _ARG_COUNT(...) _ARG_COUNT_(unused __VA_OPT__(, ) __VA_ARGS__, _ARG_SEQ())
#define _ARG_OP(...) _MACRO_CONCAT(_ARG_OP_, _ARG_COUNT(__VA_ARGS__))

#define FOR_EACH(op, sep, arg, ...) _ARG_OP(arg, ##__VA_ARGS__)(op, sep, arg, ##__VA_ARGS__)

#define _ARG_COMMA ,
#define FOR_EACH_COMMA(op, arg, ...) _ARG_OP(arg, ##__VA_ARGS__)(op, _ARG_COMMA, arg, ##__VA_ARGS__)

// FOR_EACH_COMMA(MACRO, 1, 2, 3, 4)
// -->
// MICRO(1),MICRO(2),MICRO(3),MICRO(4)

#define WRAP_CALL(call, op, arg, ...) call(FOR_EACH_COMMA(op, arg, ##__VA_ARGS__))
// WRAP_CALL(call, MACRO, 1, 2, 3, 4)
// -->
// call(MACRO(1), MACRO(2), MACRO(3), MACRO(4))

// cannot use FOR_EACH on gcc and clang, why?
#define WRAP_CALL1(call, op, arg, ...) ARG_OP(arg, ##__VA_ARGS__)(op, ARG_COMMA, arg, ##__VA_ARGS__)

#define SEPERATOR_ARGS(sep, arg, ...) FOR_EACH(EXPAND_, sep, arg, ##__VA_ARGS__)
// std::cout <<  SEPERATOR_ARGS(<<, 1, 2, 3, std::endl)
//           -->
// std::cout <<  1 << 2 << 3 << std::endl

#define FOR_EACH_CALL(call, arg, ...) FOR_EACH(call, ;, arg, ##__VA_ARGS__)
// ARG_OP_CALL(MACRO, 1, 2, 3, 4)
// -->
// MACRO(1); MACRO(2); MACRO(3); MACRO(4)

#define CHAIN_CALL(call, arg, ...) FOR_EACH(call, ., arg, ##__VA_ARGS__)
// CHAIN_CALL(MACRO, 1, 2, 3, 4)
// -->
// MACRO(1).MACRO(2).MACRO(3).MACRO(4)
// clang-format on