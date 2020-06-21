//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <iostream>
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
using namespace std;
char n[10];
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner){
}
// ���������� ����� ����������
void compile_operator (int t);
void eval_exp1 (void);
void eval_exp2 (void);
void eval_exp3 (void);
void eval_exp4 (void);
void eval_exp5 (void);
void eval_exp6 (void);
void eval_exp7 (void);
void eval_exp8 (void);
// ������������ ����� ��������������
#define MAX_LEN_CONST    32
// ������������ ����� ��������������
#define MAX_LEN_ID       32
// ������������ ������ ���������� ���� ������
#define MAX_LEN_STR     256
// ������������ ����� ����� �������
#define MAX_LEN_LEX      512
// ������������ ������ ������� ���������������
#define MAX_TABLE_ID 100
// ������������ ������ ������� ��������
#define MAX_TABLE_CONST 100
// ���� ������ ������������ �����������
enum{
  OK,
  ERR_NOT_PRG,          // ��������� �� ������
  ERR_OVER_ID,          // ������� ����� ���������������
  ERR_OVER_CONST,       // ������� ����� ��������
  ERR_LEN_STR,          // ������� ������� ������
  ERR_LEN_CONST,        // ������� ������� ���������
  ERR_LEN_ID,           // ������� ������� �������������
  ERR_FLT,              // �������� ������������ ���������
  ERR_STR,              // �������� ��������� ���������
  ERR_STR_CLOSE,        // ��������� ��������� �� ��������� �� ������� ������
  ERR_SINT,             // �������������� ������
  ERR_UNDEF,            // �������������� �������������
  ERR_DOUBLE_ID,        // ����������� �������������
  ERR_DOUBLE_CONST,     // ����������� ���������
  ERR_PROG_CLOSE,       // ��������� �� ���������
  ERR_COMMENT_CLOSE,    // ����������� �� ��������
  ERR_ELSE,             // ������������ �������� ��������� �����
  ERR_CHAR,             // ����������� ������
  ERR_W_PLUS,           // �������� ������ '+'
  ERR_W_SET,            // �������� ������ '='
  ERR_W_COMMA,          // �������� ������ ','
  ERR_W_LEFT,           // �������� ������ '('
  ERR_W_RIGHT,          // �������� ������ ')'
  ERR_W_LEFT1,          // �������� ������ '['
  ERR_W_RIGHT1,         // �������� ������ ']'
  ERR_W_LEFT2,          // �������� ������ '{'
  ERR_W_RIGHT2,         // �������� ������ '}'
  ERR_W_LONG,           // ��������� ����� ���������
  ERR_W_FLT,            // ��������� ������������ ���������
  ERR_W_STR,            // ��������� ��������� ���������
  ERR_W_STR_ID,         // ��������� ��������� ���������� ��� ������
  ERR_W_STR_LONG,       // ��������� ��������� ��� �������� ���������
  ERR_W_STR_VAR,        // ��������� ��������� ����������
  ERR_SIZE_INT,         // �������� ������ �������
  ERR_INIT_STR,         // ������������� ��������� ������ ������
  ERR_W_ID,             // �������� �������������
  ERR_W_PROG,           // ��������� '���������'
  ERR_W_THEN,           // ��������� '��'
  ERR_W_DO,             // ��������� '���������'
  ERR_END_PROG,         // ��������� �������� ������ ���������
};
// ���� ������
enum{
  TYPE_LONG,             // ���������  ���� long
  TYPE_FLT,              // ���������  ���� double
  TYPE_STR,              // ��������� ���������
  TYPE_ID,               // �������������
  TYPE_EOF,              // ����� ����� � ����������
  TYPE_PROG,             // '���������'
  TYPE_IF,               // '����'
  TYPE_THEN,             // '��'
  TYPE_ELSE,             // '�����'
  TYPE_WHILE,            // '����'
  TYPE_DO,               // '���������'
  TYPE_END,              // '�����'
  TYPE_INPUT,            // '����'
  TYPE_PRINT,            // '������'
  TYPE_VAR_LONG,         // '�����'
  TYPE_VAR_FLT,          // '������������'
  TYPE_VAR_INT,          // '�������'
  TYPE_SORT,             // '�����������'
  TYPE_VAR_CONST,        // '���������'
  TYPE_PLUS,             //  +
  TYPE_MINUS,            //  -
  TYPE_MUL,              //  *
  TYPE_DIV,              //  /
  TYPE_MOD,              //  ^
  TYPE_GE,               //  >=
  TYPE_LE,               //  <=
  TYPE_GT,               //  >
  TYPE_LT,               //  <
  TYPE_EQ,               //  ==
  TYPE_NE,               //  !=
  TYPE_NOT,              //  !
  TYPE_LEFT,             //  (
  TYPE_RIGHT,            //  )
  TYPE_LEFT1,            //  [
  TYPE_RIGHT1,           //  ]
  TYPE_LEFT2,            //  {
  TYPE_RIGHT2,           //  }
  TYPE_SET,              //  =
  TYPE_OR,               //  ||
  TYPE_AND,              //  &&
  TYPE_PC,               //  ;
  TYPE_COMMA,            //  ,
};
// ��������� ��������� ���������
struct STR_WORD{
  char *sym;              // ���������� ������������� ���������� �������� ������ �����
  int  id;                // ���������� ���������� �����
};
// ��������� define
struct STR_CONST{
  char *sym;              // ���������� ������������� ����� ���������
  char *def;              // ����������  �����
  int  type;              // ��� ���������: TYPE_LONG, TYPE_FLT, TYPE_STR
  int  info;              // �������� ����� ������ ��� �������� ��� ������ �����
};
// ������ ��������� ����
STR_WORD word[] =
{
  {"���������",    TYPE_PROG     },
  {"����",         TYPE_IF       },
  {"��",           TYPE_THEN     },
  {"�����",        TYPE_ELSE     },
  {"����",         TYPE_WHILE    },
  {"���������",    TYPE_DO       },
  {"�����",        TYPE_END      },
  {"����������",   TYPE_END      },
  {"��������",     TYPE_END      },
  {"����",         TYPE_INPUT    },
  {"������",       TYPE_PRINT    },
  {"�����������",  TYPE_SORT     },
  {"�����",        TYPE_VAR_LONG },
  {"������������", TYPE_VAR_FLT  },
  {"�������",      TYPE_VAR_INT  },
  {"���������",    TYPE_VAR_CONST},
  {NULL,           0             }
};
// ������ ������������ � ��������
STR_WORD op[] =
{
  {"+",            TYPE_PLUS    },
  {"-",            TYPE_MINUS   },
  {"*",            TYPE_MUL     },
  {"/",            TYPE_DIV     },
  {"^",            TYPE_MOD     },
  {">=",           TYPE_GE      },
  {"<=",           TYPE_LE      },
  {">",            TYPE_GT      },
  {"<",            TYPE_LT      },
  {"==",           TYPE_EQ      },
  {"!=",           TYPE_NE      },
  {"!",            TYPE_NOT     },
  {"(",            TYPE_LEFT    },
  {")",            TYPE_RIGHT   },
  {"[",            TYPE_LEFT1   },
  {"]",            TYPE_RIGHT1  },
  {"{",            TYPE_LEFT2   },
  {"}",            TYPE_RIGHT2  },
  {"=",            TYPE_SET     },
  {"||",           TYPE_OR      },
  {"&&",           TYPE_AND     },
  {";",            TYPE_PC      },
  {",",            TYPE_COMMA   },
  {NULL,           0            }
};
// ������� ���������� ������� �� ����������� ����� ���������
bool flag_end;
// ������� ������� ������ ����� ��������� ����������
bool yes_error = false;
// ������� ����� ������� �� ������� ��������
// ���� ����� ���, ��� -1, ����� ������ � �������
int ind_table;
// ������� ��� ���������� ��� �� �����������
int cur_type;
// ������ ��������� � ������� ��������
int ind_const;
// ����� ����� � �������� ����
int num;
// ����� ���������� ���������
int len_str;
// ������� ���������������
STR_WORD table_id[MAX_TABLE_ID];
// ������� ���������� ���������������
int cnt_id;
// ������� ��������
STR_CONST table_const[MAX_TABLE_CONST];
// ������� ���������� ��������
int cnt_const;
unsigned char c;
// ����� �������������� ���������
int n_record;
// ����� �������� ������� ������������� ������
char *src;
char *prog;
// ������ ������� � �������� ������
char *beg;
// ��� ����������� �������
int type;
// ������� �����
char russ[] = "����������������������������������������������������������������";
// �������������, ������ ��� �����
char id[MAX_LEN_LEX + 1];
// ����� ������
char txt_error[200];
// ������� ������ ������ � ������
int beg_error;
// ����� ��������� �������
int len_error;
// ����� ����������
char *result;
char *out;
int len_result;
void out_char (char c){
  int n;
  if ((n = out - result) > len_result - 10){
    result = (char *)realloc (result, len_result += 10000);
    out    = result + n;
  }
  *out++ = c;
}
void out_str (char *s){
  unsigned n;
  if ((n = out - result) > len_result - strlen (s) - 10){
    result = (char *)realloc (result, len_result += 10000);
    out    = result + n;
  }
  strcpy (out, s);
  out += strlen (s);
}
void out_delim (void){
  int i;
  for (i = 0; op[i].id != type; ++i);
  out_str (op[i].sym);
}
// ������ ��������� �� ������ � ������������ ����������
void printf1 (char *s){
  len_error = src - beg;
  for (beg_error = 0; beg != prog && *beg != '\n'; --beg,++beg_error);
  sprintf (txt_error, "%d) %s", n_record, s);
  throw 0;
}
void print_error (int err){
  switch (err){
  case ERR_NOT_PRG:
    n_record = 1;
    printf1 ("��������� �� ������");
  case ERR_OVER_ID:
    printf1 ("������� ����� ���������������");
  case ERR_OVER_CONST:
    printf1 ("������� ����� ��������");
  case ERR_LEN_STR:
    printf1 ("������� ������� ������");
  case ERR_LEN_CONST:
    printf1 ("������� ������� ���������");
  case ERR_LEN_ID:
    printf1 ("������� ������� �������������");
  case ERR_FLT:
    printf1 ("�������� ������������ ���������");
  case ERR_STR:
    printf1 ("�������� ��������� ���������");
  case ERR_STR_CLOSE:
    printf1 ("��������� ��������� �� ��������� �� ������� ������");
  case ERR_SINT:
    printf1 ("�������������� ������");
  case ERR_UNDEF:
    printf1 ("�������������� �������������");
  case ERR_DOUBLE_ID:
    printf1 ("����������� �������������");
  case ERR_DOUBLE_CONST:
    printf1 ("����������� ���������");
  case ERR_PROG_CLOSE:
    printf1 ("��������� �� ���������");
  case ERR_COMMENT_CLOSE:
    printf1 ("����������� �� ��������");
  case ERR_ELSE:
    printf1 ("������������ �������� ��������� �����");
  case ERR_CHAR:
    printf1 ("����������� ������");
  case ERR_W_PLUS:
    printf1 ("�������� ������ '+'");
  case ERR_W_SET:
    printf1 ("�������� ������ '='");
  case ERR_W_COMMA:
    printf1 ("�������� ������ ','");
  case ERR_W_LEFT:
    printf1 ("�������� ������ '('");
  case ERR_W_RIGHT:
    printf1 ("�������� ������ ')'");
  case ERR_W_LEFT1:
    printf1 ("�������� ������ '['");
  case ERR_W_RIGHT1:
    printf1 ("�������� ������ ']'");
   case ERR_W_LEFT2:
    printf1 ("�������� ������ '{'");
  case ERR_W_RIGHT2:
    printf1 ("�������� ������ '}'");
  case ERR_W_LONG:
    printf1 ("��������� ����� ���������");
  case ERR_W_FLT:
    printf1 ("��������� ������������ ���������");
  case ERR_W_STR_ID:
    printf1 ("��������� ��������� ���������� ��� ������");
  case ERR_W_STR:
    printf1 ("��������� ��������� ���������");
  case ERR_W_STR_LONG:
    printf1 ("��������� ��������� ��� �������� ���������");
  case ERR_W_STR_VAR:
    printf1 ("��������� ��������� ����������");
  case ERR_SIZE_INT:
    printf1 ("�������� ������ ������");
  case ERR_INIT_STR:
    printf1 ("������������� ��������� ������ ������");
  case ERR_W_ID:
    printf1 ("�������� �������������");
  case ERR_W_PROG:
    printf1 ("��������� '���������'");
  case ERR_W_THEN:
    printf1 ("��������� '��'");
  case ERR_W_DO:
    printf1 ("��������� '���������'");
  case ERR_END_PROG:
    printf1 ("��������� �������� ������ ���������");
  default:
    printf1 ("����������� ������, ���������� � ������������ �����������");
  }
}
// ���� ������������� �������� ������� �����, �������� ��� �� _rusNNN,
// ��� NNN - ����� �������������� � ������� ���������������
char *latinica (char *s){
  char *q;
  int  i;
  static char uniq[20];
  for (q = s; (c = *q) != 0; ++q){
    if (strchr (russ, c)){
      for (i = 0; strcmp (table_id[i].sym, s); ++i);
      sprintf (uniq, "_rus%d", i);
      return uniq;
    }
  }
  return s;
}
// ��������� ������������� � ������� ���������������
void set_id (void){
  int i;
  // �������� �� ��������
  for (i = 0; i < cnt_id; ++i){
    if (strcmp (table_id[i].sym, id) == 0)
      print_error (ERR_DOUBLE_ID); 
  }
  // ���� �� ����� � �������?
  if (i == MAX_TABLE_ID)
    print_error (ERR_OVER_ID);
  // ������� ������ ��� ��������������
  table_id[i].sym = (char *)malloc (strlen (id) + 1);
  // �������� ������������� � ��� ���
  strcpy (table_id[i].sym, id);
  table_id[i].id = cur_type;
  // �������� ���������� ���������������
  ++cnt_id;
}
// ��������� ��������� � ������� ��������
void set_const (void){
  int i;
  // �������� �� ��������
  for (i = 0; i < cnt_const; ++i){
    if (strcmp (table_const[i].sym, id) == 0)
      print_error (ERR_DOUBLE_CONST);
  }
  // ���� �� ����� � �������?
  if (i == MAX_TABLE_CONST)
    print_error (ERR_OVER_CONST);
  // ������� ������ ��� ��������������
  table_const[i].sym = (char *)malloc (strlen (id) + 1);
  // �������� ��� ���������
  strcpy (table_const[i].sym, id);
  // �������� ���������� ��������
  ++cnt_const;
}
// ����� � ������� ���������������
// ��� ������� ������ num �������� ������ � ������� ���������������
void get_id (void){
  for (num = 0; num < cnt_id; ++num){
    if (strcmp (table_id[num].sym, id) == 0)
      return;
  }
  print_error (ERR_UNDEF);
}
// ����� � ������� ��������
// return:  -1, ���� ��������� �� �������
//         >=0, ������ � ������� ��������
int get_const (void){
  int i;
  for (i = 0; i < cnt_const; ++i){
    if (strcmp (table_const[i].sym, beg) == 0)
      return i;
  }
  return -1;
}
// ������ ����� ��� ������������ ���������
//
// src - ��������� �� ������ ����� �����
//
// return: src ��������� �� ������ �� ����������
//         type �������� TYPE_LONG ��� TYPE_FLT
void get_num (void){
  // ���������� ��� ������ ����� �����
  while (isdigit (*++src));
  // ����� ����� ���������� ���������� ����� ��� ���������� �������
  switch (*src){
  case '.':
    // �� ���������� ������ ��������� ���� �� ���� �����
    if (!isdigit (*++src))
      print_error (ERR_FLT);
    // ���������� ��� ����� ����� ����� ���������� �����
    while (isdigit (*++src));
    // ���� ����� �� ������� ���������� ������� - ���
    // ����� ������������� �����
    if ((c = *src) != 'e' && c != 'E' && c != (unsigned char)'�' && c != (unsigned char)'�'){
      type = TYPE_FLT;
      return;
    }
  case 'e': case 'E': case '�': case '�':
    // ��� �������������� ������� ���������� ������� �� ��������� ������
    *src = 'e';
    // �� ����������� ������� ����� ��������� �������������� ���� '+' ��� '-'
    if ((c = *++src) == '+' || c == '-')
      c = *++src;
    // ����� ������ ���� �������� ������������������ ����
    if (!isdigit (c))
      print_error (ERR_FLT);
    // ���������� ��� ����� ����� ����� ���������� �������
    while (isdigit (*++src))
      ;
    type = TYPE_FLT;
    return;
  }
  // ������� ����� � �������� ����
  c    = *src;
  *src = 0;
  num  = atoi (beg);
  *src = c;
  type = TYPE_LONG;
}
// ������ ��������� ���������
//
// src - ��������� �� ������ ������ (������ �������)
//
// return: src ��������� �� ������ �� �������
//         type �������� TYPE_STR
void get_str (void){
  int len;
  len_str = 1;
  // �������� ����� ����� ������
  for (len_str = 1; (c = *++src) != '"'; ++len_str){
    // ����� �� ����� ���������?
    if (c == '\n')
      print_error (ERR_STR_CLOSE);
    // ������������� �������?
    if (c == '\\'){
      switch (*++src){
      case '\\' : case 'n': case 't': case 'v': case 'b': case 'r': case '"':
        continue;
      }
      print_error (ERR_STR);
    }
  }
  // �� ������� �� ������� ������?
  ++src;
  len = src - beg;
  if (len > MAX_LEN_STR)
    print_error (ERR_LEN_STR);
  c = *src;
  *src = 0;
  strcpy (id, beg);
  *src = c;
  type = TYPE_STR;
}
// ��������� ��������� �������
// ���� ������ ���, ��� ������� ���������� � type
void lex (void){
  int  i, len;
  char *s;
  // ������� �������� �� ������� ��������?
  if (ind_table >= 0){
    // ������������� ���
    type = table_const[ind_table].type;
    // �������� ���������� ����� (��� ������� �����)
    strcpy (id, table_const[ind_table].def + 1);
    // �������� ���������� ������ �����
    num = table_const[ind_table].info;
    // ������ ������� ����� ������� �� ���������
    ind_table = -1;
    return;
  }
loop:
  for (;;){
    // ���������� ������ �������
    while ((c = *src) == ' ' || c == '\t' || c == '\r'){
      // ������� ��������� � �������������� ���������
      out_char (c);
      ++src;
    }
    // ��������� ����� ���������?
    if (c == 0){
      if (flag_end)
        print_error (ERR_PROG_CLOSE);
      type = TYPE_EOF;
      return;
    }
    // ��� �� ����� ���������?
    if (c != '\n')
      break;
    ++src;
    // ������ ��������� � �������������� ���������
    out_char (c);
    // ����������� ����� ��������� ���������
    ++n_record;
  }
  // �������� ������ �������
  beg = src;
  // ��� ������ �������� ���������?
  if (isdigit (c)){
    // ������� �����
    get_num ();
    c    = *src;
    *src = 0;
    strcpy (id, beg);
    *src = c;
    return;
  }
  // ��� ������ ���������� ������?
  if (c == '"'){
    // ������� ������
    get_str ();
    return;
  }
  // ��� ������ ����������� (������������� ��� ��������������)?
  if (c == '/'){
    switch (src[1]){
    case '/':
      // ���������� ������ �� �����
      src  = strchr (src, '\n');
      *src = 0;
      // �������� ���������� � �������������� ���������
      out_str (beg);
      *src = '\n';
      // ��������� ������ � ��������� �������
      goto loop;
    case '*':
      // ��������� ����� ����� �������������� �����������
      src += 2;
      out_str ("/*");
      for (;;){
        if ((c = src[0]) == '*' && src[1] == '/'){
          out_str ("*/");
          src += 2;
          // ��������� ������ � ��������� �������
          goto loop;
        }
        // �������� �������� �� ��������� ������
        if (c == '\n')
          ++n_record;
        // ����� �� ����� ���������?
        if (c == 0){
          // ������ - ������������� �����������
          print_error (ERR_COMMENT_CLOSE);
        }
        // ������ ����������� ��������� � �������������� ���������
        out_char (c);
        ++src;
      }
    }
  }
  // ��� ������ ��������������?
  if (c == '_' || isalpha (c) || strchr (russ, c)){
    // ���������� ����� ��������������
    while ((c = *++src) == '_' || isdigit (c) || isalpha (c) || strchr (russ, c));
    // �������� ����� ����� ��������������
    *src = 0;
    // ����� ���� ������������� ��������� � ������� ��������?
    if ((ind_table = get_const ()) >= 0){
      // ����������� �������� ������
      *src = c;
      // ����� ������ ����������� ������
      s = table_const[ind_table].def;
      // ���� ������ ������ - ���� �����, �� � �������� �������
      // ������ ���� ���� � ����� �� ����� ����� ������� ���������� ���� �����
      if (*s == '+' || *s == '-'){
        type = *s == '+' ? TYPE_PLUS : TYPE_MINUS;
        out_delim ();
        return;
      }
      // ��������� �������� ������� ���������� �������
      strcpy (id, s);
      // ��������� ��� ���������
      switch (type = table_const[ind_table].type){
      case TYPE_STR:
        len_str = table_const[ind_table].info;
        break;
      case TYPE_LONG:
        num = table_const[ind_table].info;
      }
      // �� ������� �������� ����� ���, ��� ����
      ind_table = -1;
      return;
    }
    // ����� ��� ��������� �����?
    for (i = 0; (s = word[i].sym) != NULL; ++i){
      if (strcmp (s, beg) == 0){
        // ����������� ������
        *src = c;
        // ������ ���������� ���������� �����
        type = word[i].id;
        return;
      }
    }
    // �� ������� �� ������� �������������?
    if (strlen (beg) > MAX_LEN_ID)
      print_error (ERR_LEN_ID);
    // �������� �������������
    strcpy (id, beg);
    // ����������� ������
    *src = c;
    // ������ ��� ��������������
    type = TYPE_ID;
    return;
  }
  // ������������� ������������
  for (i = 0; (s = op[i].sym) != NULL; ++i){
    len = strlen (s);
    if (memcmp (s, beg, len) == 0){
      // ��������� �� ������ �����������
      src += len;
      // ������ ���������� �����������
      type = op[i].id;
      return;
    }
  }
  // ����������� ������
  print_error (ERR_CHAR);
}
// �������� �� ������ n2, ���� ��� ������� �� n1
void test (int n1, int n2){
  if (type != n1)
    print_error (n2);
}
// ��������� ��������� ������� p1 � ������ ������ p2, ���� ��� �� ��� �������
void token (int p1, int p2){
  lex ();
  if (type != p1)
    print_error (p2);
}
// ���������� ����������� ����������
void declare_id (void){
  char tmp[20];
  // �������� ��� ����������
  switch (cur_type = type){
  case TYPE_VAR_INT:
    out_str ("int ");
    break;
  case TYPE_VAR_LONG:
    out_str ("unsigned long ");
    break;
  default:
    out_str ("double ");
  }
  for (;;){
    // ������� �������������
    token (TYPE_ID, ERR_W_ID);
    // �������� ������������� � ������� ���������������
    set_id ();
    out_str (latinica (id));
    // ���� ��� ��������� �������, �� �� ���������������
    // ������ ��������� ������ ������ � ���������� �������
    if (cur_type == TYPE_VAR_INT){
      // ������� '['
      token (TYPE_LEFT1, ERR_W_LEFT1);
      out_char ('[');
      // ������� ����� �����
      token (TYPE_LONG, ERR_W_LONG);
      // �������� ������� ������
      if (num < 2 || num > MAX_LEN_STR)
        print_error (ERR_SIZE_INT);
      sprintf (tmp, "%d", num);
      out_str (tmp);
      strcpy(n,tmp);
      //strcat (n, tmp);
      // ������� ']'
      token (TYPE_RIGHT1, ERR_W_RIGHT1);
      out_char (']');
      //=============
      out_char ('[');
      sprintf (tmp, "%d", num);
      out_str (tmp);
      out_char (']');
      //=============
          }
    lex ();
    if (type == TYPE_SET){
      lex ();
      // � ����������� �� ���� ���������� ������������ �������������
      switch (cur_type){
////////////////////////////////////////////////////////////////////////////////
      /* case TYPE_VAR_INT:
        if(type==TYPE_STR){
        print_error (ERR_W_STR);
        }
        //out_char ('{');
        if (type ==TYPE_LEFT2){
        out_delim ();
        lex ();
        }
        else
        print_error (ERR_W_LEFT2);
        repeat:
                test (TYPE_LONG, ERR_W_LONG);
                out_str (id);
                lex ();
                if ( type == TYPE_COMMA ){
                        lex();
                        out_char(',');
                        goto repeat;
                }
        if (type ==TYPE_RIGHT2){
        out_delim ();
        lex ();
        }
        else
        print_error (ERR_W_RIGHT2);
        //out_char ('}');
        if (type != TYPE_PC)
        print_error (ERR_SINT);
        out_char (';');
        return;   */
////////////////////////////////////////////////////////////////////////////////
      case TYPE_VAR_LONG:
        // ����� ���������� ������������� ����� ���������� �������������� ���� '+' ��� '-'
        if (type == TYPE_PLUS || type == TYPE_MINUS){
          out_delim ();
          lex ();
        }
        // ��������� ����� ���������
        test (TYPE_LONG, ERR_W_LONG);
        out_str (id);
        break;
      case TYPE_VAR_FLT:
        // ����� ���������� ������������� ����� ���������� �������������� ���� '+' ��� '-'
        if (type == TYPE_PLUS || type == TYPE_MINUS){
          out_delim ();
          lex ();
        }
        // ��������� ����� ��� ������������ ���������
        if (type != TYPE_FLT && type != TYPE_LONG)
          print_error (ERR_W_FLT);
        out_str (id);
        break;
///////////////////////////////////////////////////////////////////////
       default:
        test (TYPE_STR, ERR_W_STR);
        // �� ������� �� ������� �������������?
        if (len_str > num)
          print_error (ERR_INIT_STR);
        out_str (id);
      }
      lex ();
    }
    else{
      //out_str (cur_type == TYPE_VAR_INT ? " \{ \} " : "0");
    }
    out_delim ();
    // ����������� ���������� ����������� �������� ';'?
    if (type == TYPE_PC)
      return;
    // ��������� �������
    test (TYPE_COMMA, ERR_W_COMMA);
  }
}
// ���������� ����������� ��������
void declare_const (void){
  char sign;
  int  len;
  for (;;){
    // ������� �������������
    token (TYPE_ID, ERR_W_ID);
    // �������� ������������� � ������� ��������
    set_const ();
    // ������� ������ '='
    token (TYPE_SET, ERR_W_SET);
    // ������ ������� �� �������� '='
    lex ();
    // ������� ���������� �����
    sign = 0;
    // ������������ �������������� ���� '+' ��� '-',
    // ������� �� ����� ���� � ��������� ���������
    if (type == TYPE_PLUS || type == TYPE_MINUS){
      sign = type == TYPE_PLUS ? '+' : '-';
      lex ();
    }
    // ������� ������ ���������
    switch (type){
    default:
      print_error (ERR_W_STR_LONG);
    case TYPE_STR:
      // ����� ������� �� ����� ���������� �������������� ���� '+' ��� '-'
      if (sign)
        print_error (ERR_SINT);
      num = len_str;
    case TYPE_LONG:
    case TYPE_VAR_FLT:
      ;
    }
    // ����� ������ ������� � �������, ����� ���� ����������
    len = strlen (id) + 2;
    // ������� ������ ��� ������
    table_const[cnt_const - 1].def = (char *)malloc (len);
    // �������� ����� �� ������
    if (sign)
        sprintf (table_const[cnt_const - 1].def, "%c%s", sign, id);
    else
        strcpy (table_const[cnt_const - 1].def, id);
    // ��������� �������������� ���������� � ����� ������ ��� �������� �������� ������ �����
    table_const[cnt_const - 1].info = num;
    lex ();
    // ����������� �������� �����������?
    if (type == TYPE_PC)
      return;
    // ��������� �������
    if (type != TYPE_COMMA)
      print_error (ERR_W_COMMA);
  }
}
// ���������� ���������
void expr (int t){
  lex ();
  eval_exp1 ();
  if (type != t)
    print_error (ERR_SINT);
}
// �������� ���������� ���
void eval_exp1 (void){
  eval_exp2 ();
  while (type  == TYPE_OR){
    out_delim ();
    lex ();
    eval_exp2 ();
  }
}
// �������� ���������� �
void eval_exp2 (void){
  eval_exp3 ();
  while (type  == TYPE_AND){
    out_delim ();
    lex ();
    eval_exp3 ();
  }
}
// �������� == � !=
void eval_exp3 (void){
  eval_exp4 ();
  while (type  == TYPE_EQ || type == TYPE_NE){
    out_delim ();
    lex ();
    eval_exp4 ();
  }
}
// �������� <, >, <=, >=
void eval_exp4 (void){
  eval_exp5 ();
  while (type  == TYPE_LT || type == TYPE_GT || type  == TYPE_LE || type == TYPE_GE){
    out_delim ();
    lex ();
    eval_exp5 ();
  }
}

// �������� + � -
void eval_exp5 (void){
  eval_exp6 ();
  while (type  == TYPE_PLUS || type  == TYPE_MINUS){
    out_delim ();
    lex ();
    eval_exp6 ();
  }
}

// �������� /, *, %
void eval_exp6 (void){
  eval_exp7 ();
  while (type == TYPE_MUL || type == TYPE_DIV || type == TYPE_MOD){
    out_delim ();
    lex ();
    eval_exp7 ();
  }
}
// �������� �������� +, !, -
void eval_exp7 (){
  while (type == TYPE_PLUS || type == TYPE_MINUS || type == TYPE_NOT){
    out_delim ();
    lex ();
  }
  eval_exp8 ();
}
// �������� �������� ������ ��������� �����
void func_strcmp (void){
  lex();
  switch (type){
  default:
    print_error (ERR_W_STR_ID);
  case TYPE_ID:
    // ��������� �� �������������?
    get_id ();
    if (table_id[num].id != TYPE_VAR_INT)
      print_error (ERR_W_STR_ID);
    out_str (latinica (id));
    return;
  case TYPE_STR:
    out_str (id);
  }
}
// ��������� ��������� � �������
void eval_exp8 (void){
  if(type == TYPE_LEFT){
    out_char ('(');
    lex ();
    eval_exp1 ();
    if (type != TYPE_RIGHT)
      print_error (ERR_SINT);
    out_char (')');
    lex ();
    return;
  }
  switch (type){
  default:
    print_error (ERR_SINT);
  // ������� ��������� �����
  case TYPE_VAR_INT:
    out_str ("strcmp (");
    // ��������� '('
    token (TYPE_LEFT, ERR_W_LEFT);
    // �������� 1-�� �������� ������ ��������� �����
    func_strcmp ();
    // ��������� ','
    token (TYPE_COMMA, ERR_W_COMMA);
    out_char (',');
    // �������� 2-�� �������� ������ ��������� �����
    func_strcmp ();
    // ��������� ')'
    token (TYPE_RIGHT, ERR_W_RIGHT);
    out_char (')');
    break;
  case TYPE_ID:
    // ��������� �� �������������?
    get_id ();
    out_str (latinica (id));
    break;
  case TYPE_LONG:
  case TYPE_FLT:
    out_str (id);
  }
  lex ();
}
// ���������� ��������� ������������
void asg (void){
  char save_id[MAX_LEN_LEX + 1];
  // ��������� �� ������������� ����������?
  get_id ();
  strcpy (save_id, latinica (id));
  // ��������� ������ '='
  token (TYPE_SET, ERR_W_SET);
  // ������������ ������ ����� ���� ������ �����:
  // str = a + b + c + "123" + 12 + 3.14;
  if (table_id[num].id == TYPE_VAR_INT){
    out_str (save_id);
    out_str ("[0] = 0;\n");
    for (;;){
      lex ();
      switch (type){
      default:
        print_error (ERR_W_STR_ID);
      case TYPE_ID:
        // ��������� �� �������������?
        get_id ();
        switch (table_id[num].id){
        case TYPE_VAR_LONG:
          out_str ("sprintf (");
          out_str (save_id);
          out_str (" + strlen (");
          out_str (save_id);
          out_str ("), \"%d\", ");
          out_str (latinica (id));
          out_str (");\n");
          break;
        case TYPE_VAR_FLT:
          out_str ("sprintf (");
          out_str (save_id);
          out_str (" + strlen (");
          out_str (save_id);
          out_str ("), \"%lf\", ");
          out_str (latinica (id));
          out_str (");\n");
          break;
        default:
          out_str ("strcat (");
          out_str (save_id);
          out_str (", ");
          out_str (latinica (id));
          out_str (");\n");
        }
        break;
      case TYPE_STR:
        out_str ("strcat (");
        out_str (save_id);
        out_str (", ");
        out_str (id);
        out_str (");\n");
        break;
      case TYPE_LONG:
        out_str ("sprintf (");
        out_str (save_id);
        out_str (" + strlen (");
        out_str (save_id);
        out_str ("), \"%d\", ");
        out_str (id);
        out_str (");\n");
        break;
      case TYPE_FLT:
        out_str ("sprintf (");
        out_str (save_id);
        out_str (" + strlen (");
        out_str (save_id);
        out_str ("), \"%lf\", ");
        out_str (id);
        out_str (");\n");
      }
      lex ();
      // ��������� ����������� ';'?
      if (type == TYPE_PC)
        return;
      // ��������� '+'
      test (TYPE_PLUS, ERR_W_PLUS);
    }
  }
  out_str (save_id);
  out_char ('=');
  // ���������� ���������
  expr (TYPE_PC);
  out_char (';');
}
// ���������� ��������� ���������
void compile_if (void){
  out_str ("if (");
  // ���������� ���������
  expr (TYPE_THEN);
  out_str (") {");
  lex ();
  // ���������� ����������, ����������� ��� ���������� ���������
  compile_operator (TYPE_ELSE);
  // ���� ���� '�����'?
  if (type == TYPE_ELSE){
    out_str ("} else {");
    lex ();
    // ���������� ����������, ����������� ��� �������� ���������
    compile_operator (TYPE_END);
    out_str ("}");
  }
}
// ���������� ��������� �����
void compile_while (void){
  out_str ("while (");
  // ���������� ���������
  expr (TYPE_DO);
  out_str (") {");
  lex ();
  // ���������� ����������, ������������ ���� �����
  compile_operator (TYPE_END);
  out_str ("}");
}
// ���������� ��������� �����  ////////////////////////////////////////==================
void compile_input (void){
  for (;;){
    // ������� �������������
    token (TYPE_ID, ERR_W_ID);
    // ��������� �� �������������?
    get_id ();
    switch (table_id[num].id){
    case TYPE_VAR_LONG:
      out_str ("printf (\"Vvedite '");
      out_str (id);
      out_str ("'\");");
      out_str ("scanf (\"%");
      out_str ("d\", &");
      out_str (latinica (id));
      out_str (");\n");
      break;
    case TYPE_VAR_FLT:
      out_str ("printf (\"Vvedite '");
      out_str (id);
      out_str ("'\");");
      out_str ("scanf (\"%");
      out_str ("lf\", &");
      out_str (latinica (id));
      out_str (");\n");
      break;
    case TYPE_VAR_INT:
      out_str ("printf (\"Input '");
      out_str (id);
      out_str ("'\\n\");\n");
      out_str ("for(int i=0; i<");
      out_str (n);
      out_str (";i++){\n");
      out_str ("for(int j=0; j<");
      out_str (n);
      out_str (";j++){\n");
      out_str ("printf(\"");
      out_str (id);
      out_str ("[%d][%d]=\", i, j);\n");
      out_str ("scanf(\"%d\", &");
      out_str (id);
      out_str ("[i][j]);\n");
      //out_str ("printf(\"\n\");");
      out_str ("}\n");
      out_str ("}");

      break;
    default:
      out_str ("s\", ");
      out_str (latinica (id));
      out_str (");\n");
    }
    //out_str (latinica (id));
    //out_str (");\n");
    lex ();
    // �������� ����� ��������?
    if (type == TYPE_PC)
      return;
    // ��������� ','
    test (TYPE_COMMA, ERR_W_COMMA);
  }
}
// ���������� ��������� ������
void compile_print (void){
  for (;;){
    lex ();
   // out_str ("printf (\"%");
    switch (type){
    default:
      print_error (ERR_SINT);
    case TYPE_ID:
      // ��������� �� �������������?
      get_id ();
      switch (table_id[num].id){
//////////////////////////////////////////////////////////////////
      case TYPE_VAR_INT:
      out_str ("printf (\"Output '");
      out_str (id);
      out_str ("'\\n\");\n");
      out_str ("for(int i=0; i<");
      out_str (n);
      out_str (";i++){\n");
      out_str ("for(int j=0; j<");
      out_str (n);
      out_str (";j++){\n");
      out_str ("printf(\"");
      out_str (id);
      out_str ("[%d][%d]=\", i, j);\n");
      out_str ("printf(\"%d\",");
      out_str (id);
      out_str ("[i][j]);\n");
      out_str ("printf(\"\\n\");\n");
      out_str ("}\n");
      out_str ("}\n");
      break;
/////////////////////////////////////////////////////////////////
      case TYPE_VAR_LONG:
        out_str ("printf(\"");
        out_str (id);
        out_str (" = \");");
        out_str ("printf (\"%");
        out_str ("d\", ");
        out_str (latinica (id));
        out_str (");");
        out_str ("printf(\"\\n\");");
        break;
      case TYPE_VAR_FLT:
        out_str ("printf(\"");
        out_str (id);
        out_str (" = \");");
        out_str ("printf (\"%");
        out_str ("lf\", ");
        out_str (latinica (id));
        out_str (");");
        out_str ("printf(\"\\n\");");
        break;
      default:
        out_str ("printf(\"");
        out_str (id);
        out_str (" = \");");
        out_str ("printf (\"%");
        out_str ("s\", ");
        out_str (latinica (id));
        out_str (");");
        out_str ("printf(\"\\n\");");
      }
     // out_str (latinica (id));
     //out_str (");");
      break;
    case TYPE_STR:
        out_str ("printf(\"");
        out_str (id);
        out_str (" = \");");
      out_str ("printf (\"%");
      out_str ("s\", ");
      out_str (id);
      out_str (");");
      out_str ("printf(\"\\n\");");
      break;
    case TYPE_LONG:
    out_str ("printf(\"");
        out_str (id);
        out_str (" = \");");
    out_str ("printf (\"%");
      out_str ("d\", ");
      out_str (id);
      out_str ("printf(\"\\n\");");
      break;
    case TYPE_FLT:
    out_str ("printf(\"");
        out_str (id);
        out_str (" = \");");
    out_str ("printf (\"%");
      out_str ("lf\", ");
      out_str (id);
      out_str (");");
      out_str ("printf(\"\\n\");");
    }
    out_char ('\n');
    lex ();
    // �������� ������ ��������?
    if (type == TYPE_PC)
      return;
    // ��������� ','
    test (TYPE_COMMA, ERR_W_COMMA);
  }
}
//////////////=============================================================
// ���������� ��������� ����������
void compile_sort (void){
  for (;;){
    lex ();
   // out_str ("printf (\"%");
    switch (type){
    default:
      print_error (ERR_SINT);
    case TYPE_ID:
      // ��������� �� �������������?
      get_id ();
      switch (table_id[num].id){
//////////////////////////////////////////////////////////////////
      case TYPE_VAR_INT:
      out_str ("printf (\"Sort '");
      out_str (id);
      out_str ("'\\n\");\n");
      out_str ("for(int k=0; k<");
      out_str (n);
      out_str ("*");
      out_str (n);
      out_str (";++k){\n");
      out_str ("  for(int i=0; i<");
      out_str (n);
      out_str (";++i){\n");
      out_str ("    for(int j=0; j<");
      out_str (n);
      out_str (";++j){\n");
      out_str ("    if(j!=");
      out_str (n);
      out_str ("-1){\n");
      out_str ("      if(");
      out_str (id);
      out_str ("[i][j+1]<");
      out_str (id);
      out_str ("[i][j]){\n");

      out_str ("        int t = ");
      out_str (id);
      out_str ("[i][j+1];\n        ");
      out_str (id);
      out_str ("[i][j+1] = ");
      out_str (id);
      out_str ("[i][j];\n        ");
      out_str (id);
      out_str ("[i][j] = t;\n      }\n    }\n    else{\n");

      out_str ("      if((");
      out_str (id);
      out_str ("[i+1][0]<");
      out_str (id);
      out_str ("[i][j])&&(i != ");
      out_str (n);
      out_str ("-1)){\n");

      out_str ("        int t = ");
      out_str (id);
      out_str ("[i+1][0];\n        ");
      out_str (id);
      out_str ("[i+1][0] = ");
      out_str (id);
      out_str ("[i][j];\n         ");
      out_str (id);
      out_str ("[i][j] = t;\n");

      out_str ("        }\n");
      out_str ("      }\n");
      out_str ("    }\n");
      out_str ("  }\n");
      out_str ("}\n");
      break;
      out_str ("}");
/////////////////////////////////////////////////////////////////
      default:
      out_str ("printf (\"%");
        out_str ("s\", ");
        out_str (latinica (id));
        out_str (");");
      }
    }
    out_char ('\n');
    lex ();
    if (type == TYPE_PC)
      return;
    // ��������� ','
    test (TYPE_COMMA, ERR_W_COMMA);
  }
}
 ////=============================================================

// ���������� ����� ����������
void compile_operator (int t){
  while (!(t == type || t == TYPE_ELSE && type == TYPE_END)){
    // ���������� ��������� �������� ���������
    switch (type){
    default:
      print_error (ERR_SINT);
    // �������� '����'
    case TYPE_IF:
      // ����������� �������� ��������
      compile_if ();
      break;
    // �������� '����'
    case TYPE_WHILE:
      // ����������� �������� �����
      compile_while ();
      break;
    // �������� '����'
    case TYPE_INPUT:
      // ����������� �������� �����
      compile_input ();
      break;
    // �������� '������'
    case TYPE_PRINT:
      // ����������� �������� ������
      compile_print ();
      break;
    case TYPE_SORT:
      // ����������� �������� ����������
      compile_sort ();
      break;
    // �������� ������������
    case TYPE_ID:
      // ����������� �������� ������������
      asg ();
      break;
    // ����������� ����������
    case TYPE_VAR_LONG:
    case TYPE_VAR_FLT:
    case TYPE_VAR_INT:
      // ����������� ����������� ����������
      declare_id ();
      break;
    // ����������� ��������
    case TYPE_VAR_CONST:
      // ����������� ����������� ��������
      declare_const ();
    }
    lex ();
  }
}
// ���������� ���������
void compile (void)
{
  // �� ����������� ����� ��������� ���� �� ���������
  flag_end = false;
  // ���������� ��������� ����� ���������
  n_record = 1;
  // �� ������� �������� ������ ���� �� �����
  ind_table = -1;
  // ���������, � ���� ���������� ���������
  lex ();
  switch (type)
  {
  default:
    // ��������� ������ ���������� �� ����� '���������'
    print_error (ERR_W_PROG);
  case TYPE_EOF:
    // ��������� ������ ���
    print_error (ERR_NOT_PRG);
  case TYPE_PROG:
    ;
  }
  out_str ("#include <conio.h>\n\#include <iostream>\n\#include <stdio.h>\n\#include <string.h>\n\nusing namespace std;\n\nint main ()\n{\n");
  // �� ����������� ����� ��������� ���������
  flag_end = true;
  // ����� ������ ��������� ��������� ��������� - �������� ���������
  token (TYPE_STR, ERR_W_STR);
  out_str ("printf (\"Programma %s\\n\", ");
  out_str (id);
  out_str (");\n");
  lex ();
  // ����������� ��������� �� 'End'
  compile_operator (TYPE_END);
  out_str ("\getch();\n");
  out_str ("\treturn 0;\n }\n");
  // �� ����������� ����� ��������� ���������
  flag_end = false;
  // ��������� ���������, ��������, ��� �� ������
  // �� ����� �����
  token (TYPE_EOF, ERR_END_PROG);
}
// ���������� ��� ������� ����� ����������
void del_res (void){
    int i;
    free (prog);
    free (result);
    prog   = NULL;
    result = NULL;
    for (i = 0; i < cnt_id; ++i){
        free (table_id[i].sym);
        table_id[i].sym = NULL;
    }
    cnt_id = 0;
    for (i = 0; i < cnt_const; ++i){
        free (table_const[i].sym);
        free (table_const[i].def);
        table_const[i].sym = NULL;
        table_const[i].def = NULL;
    }
    cnt_const = 0;
}
// ��������� ���������
void __fastcall TForm1::N2Click(TObject *Sender){
    if (!OpenDialog1->Execute ())
        return;
    yes_error = false;
    RichEdit2->Clear();
    Edit1->Text = "";
    try{
        RichEdit1->Lines->LoadFromFile (OpenDialog1->FileName);
    }
    catch (...){
        ShowMessage ("������ �������� ���������");
    }
}
// ��������� ���������
void __fastcall TForm1::N3Click(TObject *Sender){
    FILE      *f;
  AnsiString str;
  int        i;
  if (!SaveDialog1->Execute ())
    return;
  str = SaveDialog1->FileName;
  if ((f = fopen (str.c_str(), "w")) == NULL){
    ShowMessage ("������ ������� ���� ��� ���������� ���������");
    return;
  }
  for (i = 0; i < RichEdit1->Lines->Count; ++i){
    str = RichEdit1->Lines->Strings[i];
    fprintf (f, "%s\n", str.c_str());
  }
  fclose (f);
}
// ���������� ���������
bool __fastcall TForm1::CompileModel(bool mess){
    char       *s, *q;
    int        i, len, cnt;
    AnsiString str;
    // �������� ������ ��������� ������
    len = (cnt = RichEdit1->Lines->Count) + 4;
    for (i = 0; i < cnt; ++i){
        len += strlen (RichEdit1->Lines->Strings[i].c_str());
    }
    // ��������� ��� ��������� � ��������� ������
    beg = src = s = prog = (char *)malloc (len);
    *s = 0;
    for (i = 0; i < cnt; ++i){
        sprintf (s, "%s\n", RichEdit1->Lines->Strings[i].c_str());
        s += strlen (s);
    }
    strcpy (s, "\n");
    // ������ ��� ���������� ����������
    result = (char *)malloc (len_result = 10000);
    out = result;
    RichEdit2->Clear();
    try{
        compile ();
    }
    catch (int){
        Edit1->Text = txt_error;
        if (n_record && beg_error){
            yes_error = true;
            RichEdit1->SelStart = RichEdit1->Perform (EM_LINEINDEX, n_record - 1, 0) + beg_error - 1;
            RichEdit1->SelLength = len_error;
            RichEdit1->SelAttributes->Color = clRed;
            RichEdit1->SetFocus();
        }
        MessageBox(0,"��������� ��������","������!",MB_OK);
        del_res ();
        return false;
    }
    // ������������ ��������� ����������
    yes_error = false;
    Edit1->Text = "";
    *out++ = '\n';
    *out   = 0;
    RichEdit2->Lines->Add (result);
    del_res ();
    if (mess)
        MessageBox(0,"���������� ������ �������","����������",MB_OK);
    return true;
}
void __fastcall TForm1::N4Click(TObject *Sender){
    Close ();
}
// ���������� C++
void __fastcall TForm1::FormCreate(TObject *Sender){
    RichEdit1->Lines->Clear();
    RichEdit2->Lines->Clear();
    RichEdit1->Lines->Add("��������� \"Numbers\"");
    RichEdit1->Lines->Add("����� a, b;");
    RichEdit1->Lines->Add("b = 2+3;");
    RichEdit1->Lines->Add("������ b;");
    RichEdit1->Lines->Add("b = 9-5;");
    RichEdit1->Lines->Add("������ b;");
    RichEdit1->Lines->Add("���� a;");
    RichEdit1->Lines->Add("������ a;");
    RichEdit1->Lines->Add("�����");
}
//---------------------------------------------------------------------------


void __fastcall TForm1::N6Click(TObject *Sender)
{
    RichEdit1->Lines->Clear();
    RichEdit2->Lines->Clear();
    RichEdit1->Lines->Add("��������� \"Numbers\"");
    RichEdit1->Lines->Add("����� a, b;");
    RichEdit1->Lines->Add("b = 2+3;");
    RichEdit1->Lines->Add("������ b;");
    RichEdit1->Lines->Add("b = 9-5;");
    RichEdit1->Lines->Add("������ b;");
    RichEdit1->Lines->Add("���� a;");
    RichEdit1->Lines->Add("������ a;");
    RichEdit1->Lines->Add("�����");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N7Click(TObject *Sender)
{
    RichEdit1->Lines->Clear();
    RichEdit2->Lines->Clear();
    RichEdit1->Lines->Add("��������� \"Matrix\"");
    RichEdit1->Lines->Add("������� m[3];");
    RichEdit1->Lines->Add("���� m;");
    RichEdit1->Lines->Add("����������� m;");
    RichEdit1->Lines->Add("������ m;");
    RichEdit1->Lines->Add("�����");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::while1Click(TObject *Sender)
{
    RichEdit1->Lines->Clear();
    RichEdit2->Lines->Clear();
    RichEdit1->Lines->Add("��������� \"While_Prog\"");
    RichEdit1->Lines->Add("����� a;");
    RichEdit1->Lines->Add("a = 10;");
    RichEdit1->Lines->Add("���� a >= 5 ���������");
    RichEdit1->Lines->Add("a=a-1;");
    RichEdit1->Lines->Add("������ a;");
    RichEdit1->Lines->Add("�����");
    RichEdit1->Lines->Add("�����");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N8Click(TObject *Sender)
{
    RichEdit1->Lines->Clear();
    RichEdit2->Lines->Clear();
    RichEdit1->Lines->Add("��������� \"If_Prog\"");
    RichEdit1->Lines->Add("����� a, b, c;");
    RichEdit1->Lines->Add("���� a;");
    RichEdit1->Lines->Add("���� b;");
    RichEdit1->Lines->Add("���� a>b ��");
    RichEdit1->Lines->Add("������ a;");
    RichEdit1->Lines->Add("�����");
    RichEdit1->Lines->Add("������ b;");
    RichEdit1->Lines->Add("����������");
    RichEdit1->Lines->Add("�����");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N9Click(TObject *Sender)
{
  CompileModel(true);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N10Click(TObject *Sender)
{
       int  i, cnt;
    FILE *f;
    // ����������� ��������� ����
    if (!CompileModel(false))
        return;
    // ��������� ��������� ���������� �� ��������� ����
    f = fopen ("temp.cpp", "w");
    cnt = RichEdit2->Lines->Count;
    for (i = 0; i < cnt; ++i){
        fprintf (f, "%s", RichEdit2->Lines->Strings[i].c_str());
    }
    fclose (f);
    // ������� ��������� ������ ��� ������� ��������
    STARTUPINFO cif;
  	ZeroMemory (&cif,sizeof(STARTUPINFO));
    PROCESS_INFORMATION pi;
    // ��������� ���������� �++
  	if (!CreateProcess(NULL,"bcc32 temp.cpp", NULL, NULL, FALSE, NULL, NULL, NULL, &cif, &pi)){
        // ���� �� ����������, �� ������� ��������������� ���������
        MessageBox(0,"������ ������ �����������","������!",MB_OK);
        return;
  	}
    // ���� ���� �� ���������� ����������, �� �� ����� 10 ������
    WaitForSingleObject (pi.hProcess,10000);
    unsigned long r;
    GetExitCodeProcess (pi.hProcess, &r);
    // ���� ��� �������� ����������� �� ����� 0, ������ �������� �������������� ��������
    if (r){
        MessageBox(0,"���������� ��������� ����������","������!",MB_OK);
        return;
    }
   	ZeroMemory(&cif,sizeof(STARTUPINFO));
    // ��������� ���������������� ��������� �� ����������
    if (!CreateProcess(NULL, "temp.exe", NULL, NULL, FALSE, NULL, NULL, NULL, &cif,&pi)){
        MessageBox(0,"���������� ��������� temp.exe","������!",MB_OK);
    }     
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N11Click(TObject *Sender)
{
  Edit1->Text = "";
  if (yes_error)
  {
    yes_error = false;
    if (n_record && beg_error)
    {
      RichEdit1->SelStart = RichEdit1->Perform (EM_LINEINDEX, n_record - 1, 0) + beg_error - 1;
      RichEdit1->SelLength = len_error;
      RichEdit1->SelAttributes->Color = clBlack;
    }
  }        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N12Click(TObject *Sender)
{
  RichEdit1->Lines->Clear();
  RichEdit2->Lines->Clear();
}
//---------------------------------------------------------------------------

