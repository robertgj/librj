/**
 * \file interp_ex.c 
 *
 * Implementation of a modified version of a simple interpreter
 * shown in "A Compact Guide to Lex & Yacc" by Thomas Niemann
 * (see http://epaperpress.com/lexandyacc).
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#include "interp_utility.h"
#include "interp_callbacks.h"
#include "interp_show.h"
#include "interp_wrapper.h"
#include "interp_ex.h"
#include "interp_yacc.h"
#include "compare.h"

data_t ex(nodeType *p) 
{
  struct nodeTypeTag **op;

  if (!p) return 0;

  switch(p->type) 
    {
    case typeCon:       
      {
        return p->con.value;
      }

    case typeId:        
      {
        return sym[p->id.i];
      }

    case typeStr:
      {
        interpMessage("%s", p->str.str);
        return 0;
      }

    case typeDef:
      switch(p->def.value)
        {
        case NIL:       
          return (data_t)NULL;

        case SHOW:      
          return (data_t)interpShow;  

        default:
          interpError(__func__, __LINE__, "Invalid define!\n"); 
          return 1; 
        }
    
    case typeOpr:
      op = p->opr.op;
      switch(p->opr.oper) 
        {
        case WHILE:
          {     
            while(ex(op[0])) ex(op[1]); 
            return 0;
          }

        case IF: 
          {
            data_t res1 = ex(op[0]);
            if (res1)
              {
                data_t res2;
                res2 = ex(op[1]);
                (void)res2;
              }
            else if (p->opr.nops > 2)
              {
                ex(op[2]);
              }
            return 0;
          }
        case FOR:
          {
            for(ex(op[0]); ex(op[1]); ex(op[2])) ex(op[3]); 
            return 0;
          }

        case PRINT:
          {
            interpMessage("%ld", ex(op[0])); 
            return 0;
          }

        case UPRINT:
          {
            interpMessage("%lu", ex(op[0])); 
            return 0;
          }

        case RAND:
          {
            return interpRand(ex(op[0]));
          }

        case TIME:
          {
            struct timespec tv;
            if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tv))
              {
                interpError(__func__, __LINE__, "clock_gettime() failed!\n");
              }
            data_t t = (tv.tv_sec*1000000000)+tv.tv_nsec;
            data_t delta_t = t-ex(op[0]);
            return delta_t;
          }

        case ';':
          {
            ex(op[0]); 
            return ex(op[1]);
          }

        case '=':
          {
            return (sym[op[0]->id.i] = ex(op[1]));
          }

        case UMINUS:    
          {
            return (-ex(op[0]));
          }

        case '+':       
          {
            return (ex(op[0]) + ex(op[1]));
          }

        case '-':       
          {
            return (ex(op[0]) - ex(op[1]));
          }

        case '*':       
          {
            return (ex(op[0]) * ex(op[1]));
          }

        case '/':       
          {
            data_t numer = ex(op[0]);
            data_t denom = ex(op[1]);
            if (denom != 0) 
              { 
                return (numer / denom); 
              }
            else 
              { 
                interpError(__func__, __LINE__, "Divide by zero!");
                return 1; 
              }
          }

        case '%':
          {
            data_t numer = ex(op[0]);
            data_t denom = ex(op[1]);
            if (denom != 0) 
              { 
                return (numer % denom); 
              }
            else 
              { 
                interpError(__func__, __LINE__, "Divide by zero!");
                return 1; 
              }
          }

        case '<':       
          {
            return (ex(op[0]) < ex(op[1]));
          }

        case '>':       
          {
            return (ex(op[0]) > ex(op[1]));
          }

        case GE:        
          {
            return (ex(op[0]) >= ex(op[1]));
          }

        case LE:        
          {
            return (ex(op[0]) <= ex(op[1]));
          }

        case NE:        
          {
            return (ex(op[0]) != ex(op[1]));
          }

        case EQ:        
          {
            data_t a, b;
            a = ex(op[0]);
            b = ex(op[1]);
            return (a == b);
          }

        case DEREFERENCE:      
          {
            struct nodeTypeTag *o = op[0];
            data_t *e;

            if (o->type != typeId)
              {
                interpMessage("Not a variable!");
                return 0;	
              }
            e = (data_t *)sym[o->id.i];
            if(e == NULL)
              {
                interpMessage("NULL pointer!");
                return 0;	
              }
            return *e;
          }

        case ADDRESSOF:  
          {
            struct nodeTypeTag *o = op[0];

            if (o->type != typeId)
              {
                interpMessage("Not a variable!");
                return 0;	
              }
            return (data_t)&(sym[o->id.i]);
          }

        case FREE:      
          {
            struct nodeTypeTag *o = op[0];
            data_t *e;

            if (o->type != typeId)
              {
                interpMessage("Not a variable!");
                return 0;	
              }
            e = (data_t *)sym[o->id.i];
            if(e == NULL)
              {
                interpError(__func__, __LINE__, "NULL pointer!");
                return 0;	 
              }
            free(e);
            sym[o->id.i] = 0;
            return 0;
          }

        case CREATE:
          {
            void *tmp = interpCreate();
            return (data_t)tmp;
          }

        case FIND:
          {
            data_t key = ex(op[0]);
            data_t val = ex(op[1]);
            void *tmp = interpFind((void *)key, &val);
            return (data_t)tmp;
          }

        case INSERT:
          {
            data_t key = ex(op[0]);
            data_t val = ex(op[1]);
            void *tmp = interpInsert((void *)key, &val);
            return (data_t)tmp;
          } 

        case REMOVE:    
          {
            data_t key = ex(op[0]);
            data_t value = ex(op[1]);
            void *tmp = interpRemove((void *)key,(void *)value);
            return (data_t)tmp;
          }

        case CLEAR:   
          {
            data_t key = ex(op[0]);
            interpClear((void *)key);
            return 0;
          }

        case DESTROY:   
          {
            data_t key = ex(op[0]);
            interpDestroy((void *)key);
            return 0;
          }

        case DEPTH:    
          {
            data_t key = ex(op[0]);
            data_t tmp = (data_t)interpGetDepth((void *)key);
            return (data_t)tmp;
          }

        case SIZE:    
          {
            data_t key = ex(op[0]);
            data_t tmp = (data_t)interpGetSize((void *)key);
            return (data_t)tmp;
          }

        case FIRST:    
          {
            data_t key = ex(op[0]);
            void *tmp = interpGetFirst((void *)key);
            return (data_t)tmp;
          }

        case LAST:    
          {
            data_t key = ex(op[0]);
            void *tmp = interpGetLast((void *)key);
            return (data_t)tmp;
          }

        case MIN:    
          {
            data_t key = ex(op[0]);
            void *tmp = interpGetMin((void *)key);
            return (data_t)tmp;
          }

        case MAX:    
          {
            data_t key = ex(op[0]);
            void *tmp = interpGetMax((void *)key);
            return (data_t)tmp;
          }

        case NEXT:      
          {
            data_t key = ex(op[0]);
            data_t val = ex(op[1]);
            void *tmp = interpGetNext((void *)key, (void *)val);
            return (data_t)tmp;
          }

        case PREVIOUS:      
          {
            data_t key = ex(op[0]);
            data_t val = ex(op[1]);
            void *tmp = interpGetPrevious((void *)key, (void *)val);
            return (data_t)tmp;
          }

        case UPPER:      
          {
            data_t key = ex(op[0]);
            data_t val = ex(op[1]);
            void *tmp = interpGetUpper((void *)key, (void *)val);
            return (data_t)tmp;
          }

        case LOWER:      
          {
            data_t key = ex(op[0]);
            data_t val = ex(op[1]);
            void *tmp = interpGetLower((void *)key, (void *)val);
            return (data_t)tmp;
          }

        case CHECK:      
          {
            data_t key = ex(op[0]);
            bool tmp = interpCheck((void *)key);
            return (data_t)tmp;
          }

        case WALK:      
          {
            data_t key = ex(op[0]);
            data_t val = ex(op[1]);
            interpWalkFunc_t walk = (interpWalkFunc_t)val;
            if (walk != (interpWalkFunc_t)interpShow)
              {
                interpMessage("Bad walk function"); 
                return 0;
              }
            return interpWalk((void *)key, walk);
          }

        case SORT:      
          {
            data_t key = ex(op[0]);
            bool tmp = interpSort((void *)key);
            return (data_t)tmp;
          }

        case BALANCE:      
          {
            data_t key = ex(op[0]);
            bool tmp = interpBalance((void *)key);
            return (data_t)tmp;
          }

        case POP:      
          { 
            data_t key = ex(op[0]);
            interpPop((void *)key);
            return 0;
          }

        case PEEK:
          {
            data_t key = ex(op[0]);
            data_t *arg = (data_t *)interpPeek((void *)key);
            if (arg == NULL)
              {
                interpMessage("NULL pointer!");
                return 0;
              }
            data_t tmp = *arg;
            return tmp;
          }

        case PUSH:      
          {
            data_t key = ex(op[0]);
            data_t val = ex(op[1]);
            void *tmp = interpPush((void *)key, &val);
            return (data_t)tmp;
          }

        case COPY:      
          {
            data_t key = ex(op[0]);
            data_t val = ex(op[1]);
            void *tmp = interpCopy((void *)key, (void *)val);
            return (data_t)tmp;
          }

        default:
          {
            interpError(__func__, __LINE__, "Invalid operator!"); 
            return 1; 
          }
        }

    default:
      {
        interpError(__func__, __LINE__, "Invalid token type!"); 
        return 1; 
      }
    }

  return 0;
}
