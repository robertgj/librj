/**
 * \file interp_ex.h
 *
 * A modified version of a simple interpreter shown in "A Compact Guide
 * to Lex & Yacc" by Thomas Niemann (see http://epaperpress.com/lexandyacc).
 */

#if !defined(INTERP_EX_H)
#define INTERP_EX_H

#include "interp_data.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \e nodEnum defines the tokens recognised by the interpreter parser. */
typedef enum { 
  typeCon, 
  /**< Integer constant value (eg: 123) */

  typeDef, 
  /**< Predefined constant value (eg: show , a function pointer) */

  typeId, 
  /**< Variable name (eg: a to z) */

  typeStr, 
  /**< Constant string (eg: "This is a string") */

  typeOpr,
  /**< Operator (eg: = or create) */
  
} nodeEnum;

/** \e typeCon constant node */
typedef struct {
  data_t value;
  /**< Constant value */
} conNodeType;

/** \e typeDef predefined value node */
typedef struct {
  data_t value;
  /**< Predefined value */
} defNodeType;

/** \e typeId variable name subscript(0 to 25 for a to z) */
typedef struct {
  data_t i;
  /**< Subscript into sym variable array */
} idNodeType;

/** \e typeStr string identifier */
typedef struct {
  char str[1];
  /**< Pointer to the string */
} strNodeType;

/** \e typeOpr operator identifier */
typedef struct {
  data_t oper;
  /**< Operator identifier */
  data_t nops;
  /**< Number of operands for \e oper */
  struct nodeTypeTag *op[1];
  /**< Pointer to an array of operands */
} oprNodeType;

/** \e nodeType contains a union of the node types.
 *
 * The union must be last entry in nodeType
 * because oprNodeType or strNodeType may dynamically increase.
 */
typedef struct nodeTypeTag {
  nodeEnum type;
  /**< Type of node */

  /** Union of node types. */
  union {
    conNodeType con;
    /**< Integer constant */
    defNodeType def;
    /**< Predefined constant */
    idNodeType id;
    /**< Variable identifier */
    strNodeType str;
    /**< String identifier */
    oprNodeType opr;
    /**< Operator identifier */
  };
} nodeType;

/** A fixed array of integer variables named \e a to \e z. */
extern data_t sym[26];

/** Recursive evaluation of the parser nodes found by the interpreter. 
 * \param p \e nodeType pointer identifying the type of node.
 * \return Value found (integer, pointer to string, pointer to function, etc) 
 */
data_t ex(nodeType *p);

#ifdef __cplusplus
}
#endif

#endif
