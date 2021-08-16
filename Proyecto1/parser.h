/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_PARSER_H_INCLUDED
# define YY_YY_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    pmkdisk = 258,
    psize = 259,
    ppath = 260,
    pf = 261,
    pu = 262,
    prmdisk = 263,
    pfdisk = 264,
    ptype = 265,
    pp = 266,
    pe = 267,
    pl = 268,
    pdelete = 269,
    pname = 270,
    padd = 271,
    pbf = 272,
    pff = 273,
    pwf = 274,
    pb = 275,
    pk = 276,
    pm = 277,
    pmount = 278,
    pumount = 279,
    pid = 280,
    pmkfs = 281,
    pfs = 282,
    pfast = 283,
    pfull = 284,
    p2fs = 285,
    p3fs = 286,
    plogin = 287,
    pusr = 288,
    ppwd = 289,
    plogout = 290,
    pmkgrp = 291,
    prmgrp = 292,
    pmkusr = 293,
    pgrp = 294,
    prmusr = 295,
    pchmod = 296,
    pugo = 297,
    pr = 298,
    ptouch = 299,
    pcount = 300,
    pstdin = 301,
    pcat = 302,
    pfile = 303,
    prm = 304,
    pedit = 305,
    pren = 306,
    pmkdir = 307,
    pcp = 308,
    pdest = 309,
    pmv = 310,
    pfind = 311,
    qmark = 312,
    pchown = 313,
    pchgrp = 314,
    pause = 315,
    exec = 316,
    rep = 317,
    pmbr = 318,
    pdisk = 319,
    pinode = 320,
    pjournaling = 321,
    pblock = 322,
    pbminode = 323,
    pbmblock = 324,
    ptree = 325,
    psb = 326,
    pls = 327,
    pruta = 328,
    proot = 329,
    punto = 330,
    bracketabre = 331,
    bracketcierra = 332,
    corcheteabre = 333,
    corchetecierra = 334,
    puntocoma = 335,
    potencia = 336,
    coma = 337,
    parentesisabre = 338,
    parentesiscierra = 339,
    llaveabre = 340,
    llavecierra = 341,
    mas = 342,
    menos = 343,
    multiplicacion = 344,
    igual = 345,
    dolar = 346,
    dospuntos = 347,
    entero = 348,
    numnegativo = 349,
    cadena = 350,
    identificador = 351,
    caracter = 352,
    ruta = 353,
    rutacualquiera = 354,
    suma = 355,
    multi = 356,
    division = 357
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 29 "Parser.y"

char TEXT[256];
class imkdisk *mdisk;


#line 166 "parser.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_PARSER_H_INCLUDED  */
