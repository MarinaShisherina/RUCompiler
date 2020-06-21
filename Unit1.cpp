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
// Компиляция блока операторов
void compile_operator (int t);
void eval_exp1 (void);
void eval_exp2 (void);
void eval_exp3 (void);
void eval_exp4 (void);
void eval_exp5 (void);
void eval_exp6 (void);
void eval_exp7 (void);
void eval_exp8 (void);
// Максимальная длина идентификатора
#define MAX_LEN_CONST    32
// Максимальная длина идентификатора
#define MAX_LEN_ID       32
// Максимальный размер переменной типа строка
#define MAX_LEN_STR     256
// Максимальная длина любой лексемы
#define MAX_LEN_LEX      512
// Максимальный размер таблицы идентификаторов
#define MAX_TABLE_ID 100
// Максимальный размер таблицы констант
#define MAX_TABLE_CONST 100
// Коды ошибок лексического анализатора
enum{
  OK,
  ERR_NOT_PRG,          // Программа не задана
  ERR_OVER_ID,          // Слишком много идентификаторов
  ERR_OVER_CONST,       // Слишком много констант
  ERR_LEN_STR,          // Слишком длинная строка
  ERR_LEN_CONST,        // Слишком длинная константа
  ERR_LEN_ID,           // Слишком длинный идентификатор
  ERR_FLT,              // Ошибочна вещественная константа
  ERR_STR,              // Ошибочна строковая константа
  ERR_STR_CLOSE,        // Строковая константа не завершена на текущей строке
  ERR_SINT,             // Синтаксическая ошибка
  ERR_UNDEF,            // Неопределенный идентификатор
  ERR_DOUBLE_ID,        // Дубликатный идентификатор
  ERR_DOUBLE_CONST,     // Дубликатная константа
  ERR_PROG_CLOSE,       // Программа не завершена
  ERR_COMMENT_CLOSE,    // Комментарий не завершен
  ERR_ELSE,             // Недопустимый контекст оператора ИНАЧЕ
  ERR_CHAR,             // Неизвестный символ
  ERR_W_PLUS,           // Ожидался символ '+'
  ERR_W_SET,            // Ожидался символ '='
  ERR_W_COMMA,          // Ожидался символ ','
  ERR_W_LEFT,           // Ожидался символ '('
  ERR_W_RIGHT,          // Ожидался символ ')'
  ERR_W_LEFT1,          // Ожидался символ '['
  ERR_W_RIGHT1,         // Ожидался символ ']'
  ERR_W_LEFT2,          // Ожидался символ '{'
  ERR_W_RIGHT2,         // Ожидался символ '}'
  ERR_W_LONG,           // Ожидалась целая константа
  ERR_W_FLT,            // Ожидалась вещественная константа
  ERR_W_STR,            // Ожидалась строковая константа
  ERR_W_STR_ID,         // Ожидалась строковая переменная или строка
  ERR_W_STR_LONG,       // Ожидалась строковая или числовая константа
  ERR_W_STR_VAR,        // Ожидалась строковая переменная
  ERR_SIZE_INT,         // Ошибочен размер массива
  ERR_INIT_STR,         // Инициализация превышает размер строки
  ERR_W_ID,             // Ожидался идентификатор
  ERR_W_PROG,           // Ожидалось 'Программа'
  ERR_W_THEN,           // Ожидалось 'То'
  ERR_W_DO,             // Ожидалось 'Выполнить'
  ERR_END_PROG,         // Программа содержит лишние операторы
};
// Типы лексем
enum{
  TYPE_LONG,             // Константа  типа long
  TYPE_FLT,              // Константа  типа double
  TYPE_STR,              // Строковая константа
  TYPE_ID,               // Идентификатор
  TYPE_EOF,              // Конец файла с программой
  TYPE_PROG,             // 'Программа'
  TYPE_IF,               // 'Если'
  TYPE_THEN,             // 'То'
  TYPE_ELSE,             // 'Иначе'
  TYPE_WHILE,            // 'Пока'
  TYPE_DO,               // 'Выполнить'
  TYPE_END,              // 'Конец'
  TYPE_INPUT,            // 'Ввод'
  TYPE_PRINT,            // 'Печать'
  TYPE_VAR_LONG,         // 'Целое'
  TYPE_VAR_FLT,          // 'Вещественное'
  TYPE_VAR_INT,          // 'Матрица'
  TYPE_SORT,             // 'Сортировать'
  TYPE_VAR_CONST,        // 'Константа'
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
// Структура служебных элементов
struct STR_WORD{
  char *sym;              // Символьное представление служебного элемента нашего языка
  int  id;                // Заменитель служебного слова
};
// Структура define
struct STR_CONST{
  char *sym;              // Символьное представление имени константы
  char *def;              // Заменяемый  текст
  int  type;              // Тип константы: TYPE_LONG, TYPE_FLT, TYPE_STR
  int  info;              // Реальная длина строки или двоичный вид целого числа
};
// Список служебных слов
STR_WORD word[] =
{
  {"Программа",    TYPE_PROG     },
  {"Если",         TYPE_IF       },
  {"То",           TYPE_THEN     },
  {"Иначе",        TYPE_ELSE     },
  {"Пока",         TYPE_WHILE    },
  {"Выполнить",    TYPE_DO       },
  {"Конец",        TYPE_END      },
  {"Продолжить",   TYPE_END      },
  {"Прервать",     TYPE_END      },
  {"Ввод",         TYPE_INPUT    },
  {"Печать",       TYPE_PRINT    },
  {"Сортировать",  TYPE_SORT     },
  {"Целое",        TYPE_VAR_LONG },
  {"Вещественное", TYPE_VAR_FLT  },
  {"Матрица",      TYPE_VAR_INT  },
  {"Константа",    TYPE_VAR_CONST},
  {NULL,           0             }
};
// Список разделителей и операций
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
// Признак разрешения реакции на неожиданный конец программы
bool flag_end;
// Признак наличия ошибки после последней компиляции
bool yes_error = false;
// Признак ввода лексемы из таблицы констант
// Если ввода нет, это -1, иначе индекс в таблице
int ind_table;
// Текущий тип переменной при ее определении
int cur_type;
// Индекс константы в таблице констант
int ind_const;
// Целое число в двоичном виде
int num;
// Длина символьной константы
int len_str;
// Таблица идентификаторов
STR_WORD table_id[MAX_TABLE_ID];
// Текущее количество идентификаторов
int cnt_id;
// Таблица констант
STR_CONST table_const[MAX_TABLE_CONST];
// Текущее количество констант
int cnt_const;
unsigned char c;
// Номер компилируемого оператора
int n_record;
// Адрес текущего символа компилируемой строки
char *src;
char *prog;
// Начало лексемы в исходной строке
char *beg;
// Тип разобранной лексемы
int type;
// Русские буквы
char russ[] = "йцукенгшщзхъфывапролджэячсмитьбюЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ";
// Идентификатор, строка или число
char id[MAX_LEN_LEX + 1];
// Текст ошибки
char txt_error[200];
// Позиция начала ошибки в строке
int beg_error;
// Длина ошибочной лексемы
int len_error;
// Буфер результата
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
// Печать сообщения об ошибке с возбуждением исключения
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
    printf1 ("Программа не задана");
  case ERR_OVER_ID:
    printf1 ("Слишком много идентификаторов");
  case ERR_OVER_CONST:
    printf1 ("Слишком много констант");
  case ERR_LEN_STR:
    printf1 ("Слишком длинная строка");
  case ERR_LEN_CONST:
    printf1 ("Слишком длинная константа");
  case ERR_LEN_ID:
    printf1 ("Слишком длинный идентификатор");
  case ERR_FLT:
    printf1 ("Ошибочна вещественная константа");
  case ERR_STR:
    printf1 ("Ошибочна строковая константа");
  case ERR_STR_CLOSE:
    printf1 ("Строковая константа не завершена на текущей строке");
  case ERR_SINT:
    printf1 ("Синтаксическая ошибка");
  case ERR_UNDEF:
    printf1 ("Неопределенный идентификатор");
  case ERR_DOUBLE_ID:
    printf1 ("Дубликатный идентификатор");
  case ERR_DOUBLE_CONST:
    printf1 ("Дубликатная константа");
  case ERR_PROG_CLOSE:
    printf1 ("Программа не завершена");
  case ERR_COMMENT_CLOSE:
    printf1 ("Комментарий не завершен");
  case ERR_ELSE:
    printf1 ("Недопустимый контекст оператора ИНАЧЕ");
  case ERR_CHAR:
    printf1 ("Неизвестный символ");
  case ERR_W_PLUS:
    printf1 ("Ожидался символ '+'");
  case ERR_W_SET:
    printf1 ("Ожидался символ '='");
  case ERR_W_COMMA:
    printf1 ("Ожидался символ ','");
  case ERR_W_LEFT:
    printf1 ("Ожидался символ '('");
  case ERR_W_RIGHT:
    printf1 ("Ожидался символ ')'");
  case ERR_W_LEFT1:
    printf1 ("Ожидался символ '['");
  case ERR_W_RIGHT1:
    printf1 ("Ожидался символ ']'");
   case ERR_W_LEFT2:
    printf1 ("Ожидался символ '{'");
  case ERR_W_RIGHT2:
    printf1 ("Ожидался символ '}'");
  case ERR_W_LONG:
    printf1 ("Ожидалась целая константа");
  case ERR_W_FLT:
    printf1 ("Ожидалась вещественная константа");
  case ERR_W_STR_ID:
    printf1 ("Ожидалась строковая переменная или строка");
  case ERR_W_STR:
    printf1 ("Ожидалась строковая константа");
  case ERR_W_STR_LONG:
    printf1 ("Ожидалась строковая или числовая константа");
  case ERR_W_STR_VAR:
    printf1 ("Ожидалась строковая переменная");
  case ERR_SIZE_INT:
    printf1 ("Ошибочен размер строки");
  case ERR_INIT_STR:
    printf1 ("Инициализация превышает размер строки");
  case ERR_W_ID:
    printf1 ("Ожидался идентификатор");
  case ERR_W_PROG:
    printf1 ("Ожидалось 'Программа'");
  case ERR_W_THEN:
    printf1 ("Ожидалось 'То'");
  case ERR_W_DO:
    printf1 ("Ожидалось 'Выполнить'");
  case ERR_END_PROG:
    printf1 ("Программа содержит лишние операторы");
  default:
    printf1 ("Неизвестная ошибка, обратитесь к разработчику компилятора");
  }
}
// Если идентификатор содержит русские буквы, заменить его на _rusNNN,
// где NNN - номер идентификатора в таблице идентификаторов
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
// Поместить идентификатор в таблицу идентификаторов
void set_id (void){
  int i;
  // Проверка на дубликат
  for (i = 0; i < cnt_id; ++i){
    if (strcmp (table_id[i].sym, id) == 0)
      print_error (ERR_DOUBLE_ID); 
  }
  // Есть ли место в таблице?
  if (i == MAX_TABLE_ID)
    print_error (ERR_OVER_ID);
  // Выделим память для идентификатора
  table_id[i].sym = (char *)malloc (strlen (id) + 1);
  // Сохраним идентификатор и его тип
  strcpy (table_id[i].sym, id);
  table_id[i].id = cur_type;
  // Увеличим количество идентификаторов
  ++cnt_id;
}
// Поместить константу в таблицу констант
void set_const (void){
  int i;
  // Проверка на дубликат
  for (i = 0; i < cnt_const; ++i){
    if (strcmp (table_const[i].sym, id) == 0)
      print_error (ERR_DOUBLE_CONST);
  }
  // Есть ли место в таблице?
  if (i == MAX_TABLE_CONST)
    print_error (ERR_OVER_CONST);
  // Выделим память для идентификатора
  table_const[i].sym = (char *)malloc (strlen (id) + 1);
  // Сохраним имя константы
  strcpy (table_const[i].sym, id);
  // Увеличим количество констант
  ++cnt_const;
}
// Поиск в таблице идентификаторов
// При удачном поиске num содержит индекс в таблице идентификаторов
void get_id (void){
  for (num = 0; num < cnt_id; ++num){
    if (strcmp (table_id[num].sym, id) == 0)
      return;
  }
  print_error (ERR_UNDEF);
}
// Поиск в таблице констант
// return:  -1, если константа не найдена
//         >=0, индекс в таблице констант
int get_const (void){
  int i;
  for (i = 0; i < cnt_const; ++i){
    if (strcmp (table_const[i].sym, beg) == 0)
      return i;
  }
  return -1;
}
// Разбор целой или вещественной константы
//
// src - указывает на первую цифру числа
//
// return: src указывает на символ за константой
//         type содержит TYPE_LONG или TYPE_FLT
void get_num (void){
  // Пропускаем все первые цифры числа
  while (isdigit (*++src));
  // Далее может находиться десятичная точка или показатель степени
  switch (*src){
  case '.':
    // За десятичной точкой ожидается хотя бы одна цифра
    if (!isdigit (*++src))
      print_error (ERR_FLT);
    // Пропускаем все цифры числа после десятичной точки
    while (isdigit (*++src));
    // Если далее не следует показатель степени - это
    // конец вещественного числа
    if ((c = *src) != 'e' && c != 'E' && c != (unsigned char)'е' && c != (unsigned char)'Е'){
      type = TYPE_FLT;
      return;
    }
  case 'e': case 'E': case 'е': case 'Е':
    // Для определенности заменим показатель степент на латинский символ
    *src = 'e';
    // За показателем степени может следовать необязательный знак '+' или '-'
    if ((c = *++src) == '+' || c == '-')
      c = *++src;
    // Далее должна идти непустая последовательность цифр
    if (!isdigit (c))
      print_error (ERR_FLT);
    // Пропускаем все цифры числа после показателя степени
    while (isdigit (*++src))
      ;
    type = TYPE_FLT;
    return;
  }
  // Получим число в двоичном виде
  c    = *src;
  *src = 0;
  num  = atoi (beg);
  *src = c;
  type = TYPE_LONG;
}
// Разбор строковой константы
//
// src - указывает на начало строки (символ кавычки)
//
// return: src указывает на символ за строкой
//         type содержит TYPE_STR
void get_str (void){
  int len;
  len_str = 1;
  // Пытаемся найти конец строки
  for (len_str = 1; (c = *++src) != '"'; ++len_str){
    // Дошли до конца оператора?
    if (c == '\n')
      print_error (ERR_STR_CLOSE);
    // Экранирование символа?
    if (c == '\\'){
      switch (*++src){
      case '\\' : case 'n': case 't': case 'v': case 'b': case 'r': case '"':
        continue;
      }
      print_error (ERR_STR);
    }
  }
  // Не слишком ли длинная строка?
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
// Прочитать очередную лексему
// Если ошибок нет, тип лексемы содержится в type
void lex (void){
  int  i, len;
  char *s;
  // Лексему забираем из таблицы констант?
  if (ind_table >= 0){
    // Устанавливаем тип
    type = table_const[ind_table].type;
    // Копируем заменяющий текст (без первого знака)
    strcpy (id, table_const[ind_table].def + 1);
    // Двоичный эквивалент целого числа
    num = table_const[ind_table].info;
    // Теперь лексема будет браться из программы
    ind_table = -1;
    return;
  }
loop:
  for (;;){
    // Игнорируем пустые символы
    while ((c = *src) == ' ' || c == '\t' || c == '\r'){
      // Символы дублируем в результирующую программу
      out_char (c);
      ++src;
    }
    // Встретили конец программы?
    if (c == 0){
      if (flag_end)
        print_error (ERR_PROG_CLOSE);
      type = TYPE_EOF;
      return;
    }
    // Это не конец оператора?
    if (c != '\n')
      break;
    ++src;
    // Символ дублируем в результирующую программу
    out_char (c);
    // Увеличиваем номер исходного оператора
    ++n_record;
  }
  // Запомним начало лексемы
  beg = src;
  // Это начало числовой константы?
  if (isdigit (c)){
    // Соберем число
    get_num ();
    c    = *src;
    *src = 0;
    strcpy (id, beg);
    *src = c;
    return;
  }
  // Это начало символьной строки?
  if (c == '"'){
    // Соберем строку
    get_str ();
    return;
  }
  // Это начало комментария (однострочного или многострочного)?
  if (c == '/'){
    switch (src[1]){
    case '/':
      // Игнорируем строку до конца
      src  = strchr (src, '\n');
      *src = 0;
      // Отправим коментарий в результирующую программу
      out_str (beg);
      *src = '\n';
      // Выполняем подход к следующей лексеме
      goto loop;
    case '*':
      // Выполняем поиск конца многострочного комментария
      src += 2;
      out_str ("/*");
      for (;;){
        if ((c = src[0]) == '*' && src[1] == '/'){
          out_str ("*/");
          src += 2;
          // Выполняем подход к следующей лексеме
          goto loop;
        }
        // Обращаем внимание на окончание строки
        if (c == '\n')
          ++n_record;
        // Дошли до конца программы?
        if (c == 0){
          // Ошибка - незаконченный комментарий
          print_error (ERR_COMMENT_CLOSE);
        }
        // Символ комментария дублируем в результирующую программу
        out_char (c);
        ++src;
      }
    }
  }
  // Это начало идентификатора?
  if (c == '_' || isalpha (c) || strchr (russ, c)){
    // Определяем конец идентификатора
    while ((c = *++src) == '_' || isdigit (c) || isalpha (c) || strchr (russ, c));
    // Завершим нулем конец идентификатора
    *src = 0;
    // Может этот идентификатор определен в таблице констант?
    if ((ind_table = get_const ()) >= 0){
      // Восстановим затертый символ
      *src = c;
      // Адрес начала замещающего текста
      s = table_const[ind_table].def;
      // Если первый символ - знак числа, то в качестве лексемы
      // вернем этот знак и далее за втрой заход заберем собственно само число
      if (*s == '+' || *s == '-'){
        type = *s == '+' ? TYPE_PLUS : TYPE_MINUS;
        out_delim ();
        return;
      }
      // Установим значение лексемы замещающим текстом
      strcpy (id, s);
      // Установим тип константы
      switch (type = table_const[ind_table].type){
      case TYPE_STR:
        len_str = table_const[ind_table].info;
        break;
      case TYPE_LONG:
        num = table_const[ind_table].info;
      }
      // Из таблицы констант взяли все, что надо
      ind_table = -1;
      return;
    }
    // Может это служебное слово?
    for (i = 0; (s = word[i].sym) != NULL; ++i){
      if (strcmp (s, beg) == 0){
        // Восстановим символ
        *src = c;
        // Вернем заменитель служебного слова
        type = word[i].id;
        return;
      }
    }
    // Не слишком ли длинный идентификатор?
    if (strlen (beg) > MAX_LEN_ID)
      print_error (ERR_LEN_ID);
    // Сохраним идентификатор
    strcpy (id, beg);
    // Восстановим символ
    *src = c;
    // Вернем тип идентификатора
    type = TYPE_ID;
    return;
  }
  // Распознавание разделителей
  for (i = 0; (s = op[i].sym) != NULL; ++i){
    len = strlen (s);
    if (memcmp (s, beg, len) == 0){
      // Сдвинемся на размер разделителя
      src += len;
      // Вернем заменитель разделителя
      type = op[i].id;
      return;
    }
  }
  // Неизвестный символ
  print_error (ERR_CHAR);
}
// Сообщить об ошибке n2, если тип лексемы не n1
void test (int n1, int n2){
  if (type != n1)
    print_error (n2);
}
// Прочитать ожидаемую лексему p1 и выдать ошибку p2, если это не эта лексема
void token (int p1, int p2){
  lex ();
  if (type != p1)
    print_error (p2);
}
// Компиляция определения переменных
void declare_id (void){
  char tmp[20];
  // Запомним тип переменной
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
    // Ожидаем идентификатор
    token (TYPE_ID, ERR_W_ID);
    // Поместим идентификатор в таблицу идентификаторов
    set_id ();
    out_str (latinica (id));
    // Если тип переменой матрица, то за идентификатором
    // должен следовать размер строки в квадратных скобках
    if (cur_type == TYPE_VAR_INT){
      // Ожидаем '['
      token (TYPE_LEFT1, ERR_W_LEFT1);
      out_char ('[');
      // Ожидаем целое число
      token (TYPE_LONG, ERR_W_LONG);
      // Контроль размера строки
      if (num < 2 || num > MAX_LEN_STR)
        print_error (ERR_SIZE_INT);
      sprintf (tmp, "%d", num);
      out_str (tmp);
      strcpy(n,tmp);
      //strcat (n, tmp);
      // Ожидаем ']'
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
      // В зависимости от типа переменной контролируем инициализацию
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
        // Перед константой инициализации может находиться необязательный знак '+' или '-'
        if (type == TYPE_PLUS || type == TYPE_MINUS){
          out_delim ();
          lex ();
        }
        // Ожидается целая константа
        test (TYPE_LONG, ERR_W_LONG);
        out_str (id);
        break;
      case TYPE_VAR_FLT:
        // Перед константой инициализации может находиться необязательный знак '+' или '-'
        if (type == TYPE_PLUS || type == TYPE_MINUS){
          out_delim ();
          lex ();
        }
        // Ожидается целая или вещественная константа
        if (type != TYPE_FLT && type != TYPE_LONG)
          print_error (ERR_W_FLT);
        out_str (id);
        break;
///////////////////////////////////////////////////////////////////////
       default:
        test (TYPE_STR, ERR_W_STR);
        // Не слишком ли длинная инициализация?
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
    // Определение переменных закончилось символом ';'?
    if (type == TYPE_PC)
      return;
    // Ожидается запятая
    test (TYPE_COMMA, ERR_W_COMMA);
  }
}
// Компиляция определения констант
void declare_const (void){
  char sign;
  int  len;
  for (;;){
    // Ожидаем идентификатор
    token (TYPE_ID, ERR_W_ID);
    // Поместим идентификатор в таблицу констант
    set_const ();
    // Ожидаем символ '='
    token (TYPE_SET, ERR_W_SET);
    // Читаем лексему за символом '='
    lex ();
    // Признак отсутствия знака
    sign = 0;
    // Обрабатываем необязательный знак '+' или '-',
    // который не может быть у строковой константы
    if (type == TYPE_PLUS || type == TYPE_MINUS){
      sign = type == TYPE_PLUS ? '+' : '-';
      lex ();
    }
    // Ожидаем только константу
    switch (type){
    default:
      print_error (ERR_W_STR_LONG);
    case TYPE_STR:
      // Перед строкой не может находиться необязательный знак '+' или '-'
      if (sign)
        print_error (ERR_SINT);
      num = len_str;
    case TYPE_LONG:
    case TYPE_VAR_FLT:
      ;
    }
    // Длину текста возьмем с запасом, чтобы знак поместился
    len = strlen (id) + 2;
    // Выделим память для текста
    table_const[cnt_const - 1].def = (char *)malloc (len);
    // Сохраним текст со знаком
    if (sign)
        sprintf (table_const[cnt_const - 1].def, "%c%s", sign, id);
    else
        strcpy (table_const[cnt_const - 1].def, id);
    // Сохраняем дополнительную информацию о длине строки или двоичное значение целого числа
    table_const[cnt_const - 1].info = num;
    lex ();
    // Определение констант закончилось?
    if (type == TYPE_PC)
      return;
    // Ожидается запятая
    if (type != TYPE_COMMA)
      print_error (ERR_W_COMMA);
  }
}
// Компиляция выражения
void expr (int t){
  lex ();
  eval_exp1 ();
  if (type != t)
    print_error (ERR_SINT);
}
// Операция логическое ИЛИ
void eval_exp1 (void){
  eval_exp2 ();
  while (type  == TYPE_OR){
    out_delim ();
    lex ();
    eval_exp2 ();
  }
}
// Операция логическое И
void eval_exp2 (void){
  eval_exp3 ();
  while (type  == TYPE_AND){
    out_delim ();
    lex ();
    eval_exp3 ();
  }
}
// Операции == и !=
void eval_exp3 (void){
  eval_exp4 ();
  while (type  == TYPE_EQ || type == TYPE_NE){
    out_delim ();
    lex ();
    eval_exp4 ();
  }
}
// Операции <, >, <=, >=
void eval_exp4 (void){
  eval_exp5 ();
  while (type  == TYPE_LT || type == TYPE_GT || type  == TYPE_LE || type == TYPE_GE){
    out_delim ();
    lex ();
    eval_exp5 ();
  }
}

// Операции + и -
void eval_exp5 (void){
  eval_exp6 ();
  while (type  == TYPE_PLUS || type  == TYPE_MINUS){
    out_delim ();
    lex ();
    eval_exp6 ();
  }
}

// Операции /, *, %
void eval_exp6 (void){
  eval_exp7 ();
  while (type == TYPE_MUL || type == TYPE_DIV || type == TYPE_MOD){
    out_delim ();
    lex ();
    eval_exp7 ();
  }
}
// Операции унарного +, !, -
void eval_exp7 (){
  while (type == TYPE_PLUS || type == TYPE_MINUS || type == TYPE_NOT){
    out_delim ();
    lex ();
  }
  eval_exp8 ();
}
// Контроль операнда функии сравнения строк
void func_strcmp (void){
  lex();
  switch (type){
  default:
    print_error (ERR_W_STR_ID);
  case TYPE_ID:
    // Определен ли идентификатор?
    get_id ();
    if (table_id[num].id != TYPE_VAR_INT)
      print_error (ERR_W_STR_ID);
    out_str (latinica (id));
    return;
  case TYPE_STR:
    out_str (id);
  }
}
// Обработка выражения в скобках
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
  // Функция спавнения строк
  case TYPE_VAR_INT:
    out_str ("strcmp (");
    // Ожидается '('
    token (TYPE_LEFT, ERR_W_LEFT);
    // Контроль 1-го операнда функии сравнения строк
    func_strcmp ();
    // Ожидается ','
    token (TYPE_COMMA, ERR_W_COMMA);
    out_char (',');
    // Контроль 2-го операнда функии сравнения строк
    func_strcmp ();
    // Ожидается ')'
    token (TYPE_RIGHT, ERR_W_RIGHT);
    out_char (')');
    break;
  case TYPE_ID:
    // Определен ли идентификатор?
    get_id ();
    out_str (latinica (id));
    break;
  case TYPE_LONG:
  case TYPE_FLT:
    out_str (id);
  }
  lex ();
}
// Компиляция оператора присваивания
void asg (void){
  char save_id[MAX_LEN_LEX + 1];
  // Определен ли идентификатор результата?
  get_id ();
  strcpy (save_id, latinica (id));
  // Ожидается символ '='
  token (TYPE_SET, ERR_W_SET);
  // Присваивание строке может быть только таким:
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
        // Определен ли идентификатор?
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
      // Катенация закончилась ';'?
      if (type == TYPE_PC)
        return;
      // Ожидается '+'
      test (TYPE_PLUS, ERR_W_PLUS);
    }
  }
  out_str (save_id);
  out_char ('=');
  // Компиляция выражения
  expr (TYPE_PC);
  out_char (';');
}
// Компиляция условного оператора
void compile_if (void){
  out_str ("if (");
  // Компиляция выражения
  expr (TYPE_THEN);
  out_str (") {");
  lex ();
  // Компиляция операторов, выполняемых при истинности выражения
  compile_operator (TYPE_ELSE);
  // Есть блок 'Иначе'?
  if (type == TYPE_ELSE){
    out_str ("} else {");
    lex ();
    // Компиляция операторов, выполняемых при ложности выражения
    compile_operator (TYPE_END);
    out_str ("}");
  }
}
// Компиляция оператора цикла
void compile_while (void){
  out_str ("while (");
  // Компиляция выражения
  expr (TYPE_DO);
  out_str (") {");
  lex ();
  // Компиляция операторов, составляющих тело цикла
  compile_operator (TYPE_END);
  out_str ("}");
}
// Компиляция оператора ввода  ////////////////////////////////////////==================
void compile_input (void){
  for (;;){
    // Ожидаем идентификатор
    token (TYPE_ID, ERR_W_ID);
    // Определен ли идентификатор?
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
    // Оператор ввода завершен?
    if (type == TYPE_PC)
      return;
    // Ожидается ','
    test (TYPE_COMMA, ERR_W_COMMA);
  }
}
// Компиляция оператора печати
void compile_print (void){
  for (;;){
    lex ();
   // out_str ("printf (\"%");
    switch (type){
    default:
      print_error (ERR_SINT);
    case TYPE_ID:
      // Определен ли идентификатор?
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
    // Оператор печати завершен?
    if (type == TYPE_PC)
      return;
    // Ожидается ','
    test (TYPE_COMMA, ERR_W_COMMA);
  }
}
//////////////=============================================================
// Компиляция оператора сортировки
void compile_sort (void){
  for (;;){
    lex ();
   // out_str ("printf (\"%");
    switch (type){
    default:
      print_error (ERR_SINT);
    case TYPE_ID:
      // Определен ли идентификатор?
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
    // Ожидается ','
    test (TYPE_COMMA, ERR_W_COMMA);
  }
}
 ////=============================================================

// Компиляция блока операторов
void compile_operator (int t){
  while (!(t == type || t == TYPE_ELSE && type == TYPE_END)){
    // Распознаем очередной оператор программы
    switch (type){
    default:
      print_error (ERR_SINT);
    // Оператор 'Если'
    case TYPE_IF:
      // Компилируем условный оператор
      compile_if ();
      break;
    // Оператор 'Пока'
    case TYPE_WHILE:
      // Компилируем оператор цикла
      compile_while ();
      break;
    // Оператор 'Ввод'
    case TYPE_INPUT:
      // Компилируем оператор ввода
      compile_input ();
      break;
    // Оператор 'Печать'
    case TYPE_PRINT:
      // Компилируем оператор печати
      compile_print ();
      break;
    case TYPE_SORT:
      // Компилируем оператор сортировки
      compile_sort ();
      break;
    // Оператор присваивания
    case TYPE_ID:
      // Компилируем оператор присваивания
      asg ();
      break;
    // Определение переменных
    case TYPE_VAR_LONG:
    case TYPE_VAR_FLT:
    case TYPE_VAR_INT:
      // Компилируем определение переменных
      declare_id ();
      break;
    // Определение констант
    case TYPE_VAR_CONST:
      // Компилируем определение констант
      declare_const ();
    }
    lex ();
  }
}
// Компиляция программы
void compile (void)
{
  // На неожиданный конец программы пока не реагируем
  flag_end = false;
  // Установить начальный номер оператора
  n_record = 1;
  // Из таблицы констант ничего пока не берем
  ind_table = -1;
  // Посмотрим, с чего начинается программа
  lex ();
  switch (type)
  {
  default:
    // Программа должна начинаться со слова 'Программа'
    print_error (ERR_W_PROG);
  case TYPE_EOF:
    // Программы вообще нет
    print_error (ERR_NOT_PRG);
  case TYPE_PROG:
    ;
  }
  out_str ("#include <conio.h>\n\#include <iostream>\n\#include <stdio.h>\n\#include <string.h>\n\nusing namespace std;\n\nint main ()\n{\n");
  // На неожиданный конец программы реагируем
  flag_end = true;
  // Далее должна следовать строковая константа - название программы
  token (TYPE_STR, ERR_W_STR);
  out_str ("printf (\"Programma %s\\n\", ");
  out_str (id);
  out_str (");\n");
  lex ();
  // Компилируем программу до 'End'
  compile_operator (TYPE_END);
  out_str ("\getch();\n");
  out_str ("\treturn 0;\n }\n");
  // На неожиданный конец программы реагируем
  flag_end = false;
  // Программа завершена, проверим, нет ли мусора
  // до конца файла
  token (TYPE_EOF, ERR_END_PROG);
}
// Освободить все ресурсы после компиляции
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
// Загрузить программу
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
        ShowMessage ("Ошибка загрузки программы");
    }
}
// Сохранить программу
void __fastcall TForm1::N3Click(TObject *Sender){
    FILE      *f;
  AnsiString str;
  int        i;
  if (!SaveDialog1->Execute ())
    return;
  str = SaveDialog1->FileName;
  if ((f = fopen (str.c_str(), "w")) == NULL){
    ShowMessage ("Нельзя создать файл для сохранения программы");
    return;
  }
  for (i = 0; i < RichEdit1->Lines->Count; ++i){
    str = RichEdit1->Lines->Strings[i];
    fprintf (f, "%s\n", str.c_str());
  }
  fclose (f);
}
// Компиляция программы
bool __fastcall TForm1::CompileModel(bool mess){
    char       *s, *q;
    int        i, len, cnt;
    AnsiString str;
    // Вычислим размер исходного текста
    len = (cnt = RichEdit1->Lines->Count) + 4;
    for (i = 0; i < cnt; ++i){
        len += strlen (RichEdit1->Lines->Strings[i].c_str());
    }
    // Разместим всю программу в свободной памяти
    beg = src = s = prog = (char *)malloc (len);
    *s = 0;
    for (i = 0; i < cnt; ++i){
        sprintf (s, "%s\n", RichEdit1->Lines->Strings[i].c_str());
        s += strlen (s);
    }
    strcpy (s, "\n");
    // Память для результата компиляции
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
        MessageBox(0,"Программа ошибочна","Ошибка!",MB_OK);
        del_res ();
        return false;
    }
    // Обнародовать результат компиляции
    yes_error = false;
    Edit1->Text = "";
    *out++ = '\n';
    *out   = 0;
    RichEdit2->Lines->Add (result);
    del_res ();
    if (mess)
        MessageBox(0,"Трансляция прошла успешно","Трансляция",MB_OK);
    return true;
}
void __fastcall TForm1::N4Click(TObject *Sender){
    Close ();
}
// Компиляция C++
void __fastcall TForm1::FormCreate(TObject *Sender){
    RichEdit1->Lines->Clear();
    RichEdit2->Lines->Clear();
    RichEdit1->Lines->Add("Программа \"Numbers\"");
    RichEdit1->Lines->Add("Целое a, b;");
    RichEdit1->Lines->Add("b = 2+3;");
    RichEdit1->Lines->Add("Печать b;");
    RichEdit1->Lines->Add("b = 9-5;");
    RichEdit1->Lines->Add("Печать b;");
    RichEdit1->Lines->Add("Ввод a;");
    RichEdit1->Lines->Add("Печать a;");
    RichEdit1->Lines->Add("Конец");
}
//---------------------------------------------------------------------------


void __fastcall TForm1::N6Click(TObject *Sender)
{
    RichEdit1->Lines->Clear();
    RichEdit2->Lines->Clear();
    RichEdit1->Lines->Add("Программа \"Numbers\"");
    RichEdit1->Lines->Add("Целое a, b;");
    RichEdit1->Lines->Add("b = 2+3;");
    RichEdit1->Lines->Add("Печать b;");
    RichEdit1->Lines->Add("b = 9-5;");
    RichEdit1->Lines->Add("Печать b;");
    RichEdit1->Lines->Add("Ввод a;");
    RichEdit1->Lines->Add("Печать a;");
    RichEdit1->Lines->Add("Конец");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N7Click(TObject *Sender)
{
    RichEdit1->Lines->Clear();
    RichEdit2->Lines->Clear();
    RichEdit1->Lines->Add("Программа \"Matrix\"");
    RichEdit1->Lines->Add("Матрица m[3];");
    RichEdit1->Lines->Add("Ввод m;");
    RichEdit1->Lines->Add("Сортировать m;");
    RichEdit1->Lines->Add("Печать m;");
    RichEdit1->Lines->Add("Конец");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::while1Click(TObject *Sender)
{
    RichEdit1->Lines->Clear();
    RichEdit2->Lines->Clear();
    RichEdit1->Lines->Add("Программа \"While_Prog\"");
    RichEdit1->Lines->Add("Целое a;");
    RichEdit1->Lines->Add("a = 10;");
    RichEdit1->Lines->Add("Пока a >= 5 Выполнить");
    RichEdit1->Lines->Add("a=a-1;");
    RichEdit1->Lines->Add("Печать a;");
    RichEdit1->Lines->Add("Конец");
    RichEdit1->Lines->Add("Конец");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N8Click(TObject *Sender)
{
    RichEdit1->Lines->Clear();
    RichEdit2->Lines->Clear();
    RichEdit1->Lines->Add("Программа \"If_Prog\"");
    RichEdit1->Lines->Add("Целое a, b, c;");
    RichEdit1->Lines->Add("Ввод a;");
    RichEdit1->Lines->Add("Ввод b;");
    RichEdit1->Lines->Add("Если a>b То");
    RichEdit1->Lines->Add("Печать a;");
    RichEdit1->Lines->Add("Иначе");
    RichEdit1->Lines->Add("Печать b;");
    RichEdit1->Lines->Add("Продолжить");
    RichEdit1->Lines->Add("Конец");
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
    // Компилируем модельный язык
    if (!CompileModel(false))
        return;
    // Сохраняем результат компиляции во временный файл
    f = fopen ("temp.cpp", "w");
    cnt = RichEdit2->Lines->Count;
    for (i = 0; i < cnt; ++i){
        fprintf (f, "%s", RichEdit2->Lines->Strings[i].c_str());
    }
    fclose (f);
    // Готовим структуры данных для запуска процесса
    STARTUPINFO cif;
  	ZeroMemory (&cif,sizeof(STARTUPINFO));
    PROCESS_INFORMATION pi;
    // Запускаем компилятор с++
  	if (!CreateProcess(NULL,"bcc32 temp.cpp", NULL, NULL, FALSE, NULL, NULL, NULL, &cif, &pi)){
        // Если не запустился, то выводим соответствующее сообщение
        MessageBox(0,"Ошибка старта компилятора","Ошибка!",MB_OK);
        return;
  	}
    // Ждем пока не закончится компиляция, но не более 10 секунд
    WaitForSingleObject (pi.hProcess,10000);
    unsigned long r;
    GetExitCodeProcess (pi.hProcess, &r);
    // Если код возврата компилятора не равен 0, значит возникла непредвиденная ситуация
    if (r){
        MessageBox(0,"Невозможно выполнить компиляцию","Ошибка!",MB_OK);
        return;
    }
   	ZeroMemory(&cif,sizeof(STARTUPINFO));
    // Запускаем скомпилированную программу на выполнение
    if (!CreateProcess(NULL, "temp.exe", NULL, NULL, FALSE, NULL, NULL, NULL, &cif,&pi)){
        MessageBox(0,"Невозможно выполнить temp.exe","Ошибка!",MB_OK);
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

