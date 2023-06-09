#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "sqlite3/sqlite3.h"


#ifndef UTILS_H_
#define UTILS_H_



struct SUsuario;
typedef struct SUsuario Usuario;


struct SGestao;
typedef struct SGestao Gestao;
struct SCoordenacao;
typedef struct SCoordenacao Coordenacao;
struct SPreceptor;
typedef struct SPreceptor Preceptor;
struct SResidente;
typedef struct SResidente Residente;


struct SProgramaResidencia;
typedef struct SProgramaResidencia ProgramaResidencia;
struct STurma;
typedef struct STurma Turma;
struct SAtividade;
typedef struct SAtividade Atividade;
struct SSubmissao;
typedef struct SSubmissao Submissao;
struct SFeedbackGeral;
typedef struct SFeedbackGeral FeedbackGeral;



// Usuario base
struct SUsuario{
  char *nome;
  int id;
  char *email;
  char *senha;
  char *categoriaUsuario;
  void *tipoUsuario;
  int status;

};



// Tipos de Usuarios
struct SResidente{
  Usuario* usuario;
  int matricula;
  Turma* turma;
  Atividade *listaAtividades;
  Submissao *listaSubmissoes;
  Preceptor* preceptorResponsavel;
  float notasTrimestrais[4];
  FeedbackGeral *listaFeedbacks;
};
 
struct SPreceptor{
  Usuario* usuario;
  Turma* turma;
  Residente *listaResidentes;
  Atividade *listaAtividades;
  FeedbackGeral *listaFeedbacks;
};

struct SCoordenacao{
  Usuario* usuario;
  char* cargo;
  ProgramaResidencia* residencia;
};

struct SGestao{
  Usuario* usuario;
  char* cargo;
  ProgramaResidencia *listaResidencias;
};



// estruturas

struct SProgramaResidencia{
  char nomePrograma[30];
  Coordenacao* listaCoordenacao;
  Turma* listaTurmas;
};


struct STurma{
  char nomeTurma[40];
  char anoDaTurma[10];
  Residente *residentes;
  Preceptor *listaPreceptores;
  Atividade *listasAtividades;
  ProgramaResidencia* residencia;
  char *criteriosAvaliativos;
};


// objetos

struct SAtividade{
  char nomeDaAtividade[40];
  char* descricaoDaAtividade;
  Turma* turma;
  Submissao *listaSubmissao;
  char dataDaPostagem[16];
  char dataDaEntrega[16];
  int ativa;
};

struct SSubmissao{
  Residente* residente;
  Preceptor* preceptor;
  Atividade* atividade;
  float nota;
  char *resposta;
  char *feedback;
  char *status;
};

struct SFeedbackGeral{
  Preceptor* preceptor;
  Residente* residente;
  float nota;
  char *criterios;
  char *feedback;
  char *contestacao;
  char *respostaConstentacao;
  char data[16];
  char* status;
};



// Funções:


// -Gerais:
char* strFOverwrite(char** output_str, char* base_str, ...);

int sysStatus(sqlite3** db_ptr, int ret);
int getStmt(sqlite3** db_ptr, sqlite3_stmt** sql_stmt_ptr, char* sql_cmd_p);

int addUsuarioTB();
int addGestaoTB(sqlite3** db_ptr, int usuario_fk, char *cargo);
int addCoordenacaoTB(sqlite3** db_ptr, int usuario_fk, char *cargo, int residencia_fk);
int addPreceptorTB(sqlite3** db_ptr, int usuario_fk, char *cargo, int turma_fk);
int addResidenteTB(sqlite3** db_ptr, int usuario_fk, char *matricula, int turma_fk, int preceptor_fk, char* notas);


Usuario *getUsuarioTB(sqlite3** db_ptr, char *email, char *senha);

/*
void criarCriteriosDeFeedback(Turma *turma);

void criarAtividades(Turma *turma);

void visualizarTurmas(Turma *listaTurmas);

void visualizarFeedbacks(char *nome);

void visualizarAtividades(Atividade *listaAtividades);

void submeterAtividade(Residente *residente, Submissao *submissao);

void visualizarSubmissao(Submissao *submissao);

void contestarFeedback(Usuario *usuario, feedbackGeral feedbackGeral);

void visualizarUsuario(Usuario *usuario);

void avaliarAtividade(Preceptor *preceptor, Submissao *submissao);

void fazerAvaliacaoTrimestral(Usuario *usuario, float notaTrimestral);

void fazerFeedback(Preceptor *preceptor, Residente *residente, Usuario usuario);

void cadastrarTurma(ProgramaResidencia* residencia);
*/

#endif // UTILS_H_