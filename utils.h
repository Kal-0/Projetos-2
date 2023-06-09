#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdarg.h>

#include "sqlite3/sqlite3.h"


#ifndef UTILS_H_
#define UTILS_H_


//usuarios
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


//estruturas
struct SProgramaResidencia;
typedef struct SProgramaResidencia ProgramaResidencia;
struct STurma;
typedef struct STurma Turma;


//objetos
struct SAtividade;
typedef struct SAtividade Atividade;
struct SSubmissao;
typedef struct SSubmissao Submissao;
struct SFeedbackGeral;
typedef struct SFeedbackGeral FeedbackGeral;


typedef int ID;
struct SlsID;
typedef struct SlsID lsID;


// Usuario base
struct SUsuario{
  ID id;
  char *nome;
  char *email;
  char *senha;
  char *categoriaUsuario;
  void *tipoUsuario;
  int status;

};



// Tipos de Usuarios
struct SResidente{
  ID id;
  ID usuarioFk;
  char* matricula;
  ID turmaFk;
  ID preceptorFk;
  float notasTrimestrais[4];
  lsID *listaAtividades;
  lsID *listaSubmissoes;
  lsID *listaFeedbacks;
};
 
struct SPreceptor{
  ID id;
  ID usuarioFk;
  Turma* turma;
  lsID *listaResidentes;
  lsID *listaAtividades;
  lsID *listaFeedbacks;
};

struct SCoordenacao{
  ID id;
  ID usuarioFk;
  char* cargo;
  ProgramaResidencia* residencia;
};

struct SGestao{
  ID id;
  ID usuarioFk;
  char* cargo;
  lsID *listaResidencias;
};



// estruturas

struct SProgramaResidencia{
  ID id;
  char nomePrograma[30];
  lsID* listaCoordenacao;
  lsID* listaTurmas;
};


struct STurma{
  ID id;
  char* nome;
  char* ano;
  lsID *residentes;
  lsID *listaPreceptores;
  lsID *listasAtividades;
  ID residenciaFK;
  char *criteriosAvaliativos;
};


// objetos

struct SAtividade{
  ID id;
  char* nomeDaAtividade;
  char* descricaoDaAtividade;
  Turma* turma;
  lsID *listaSubmissao;
  char dataDaPostagem[16];
  char dataDaEntrega[16];
  int ativa;
};

struct SSubmissao{
  ID id;
  ID residenteFK;
  ID preceptorFK;
  ID atividadeFK;
  float nota;
  char *resposta;
  char *feedback;
  char *status;
};

struct SFeedbackGeral{
  ID id;
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

struct SlsID{
  ID id;

  lsID* next;
  lsID* last;
};

// Funções:
void getNota(int** arryNota,char* notaDB);

// -Gerais:
char* strFOverwrite(char** output_str, char* base_str, ...);


//listas
void append(lsID **head, int item);
int getItemLs(lsID** head, int index);
int lenLs(lsID **head);
void printLs(lsID **head);
void freeLs(lsID **head);


//banco de dados
int sysStatus(sqlite3* db_ptr, int ret);
int getStmt(sqlite3* db_ptr, sqlite3_stmt** sql_stmt_ptr, char* sql_cmd_p);
//void* getCellVoid(sqlite3* db_ptr, int* cell_size, char* tableName, char* field, char* condition);
int printTableColumn(sqlite3* db_ptr, char* tableName, char* field, char* condition);
lsID* getTableIDLs(sqlite3* db_ptr, char* tableName, char* condition);

//cadastro
int addUsuarioTB();
int addGestaoTB(sqlite3* db_ptr, int usuario_fk, char *cargo);
int addCoordenacaoTB(sqlite3* db_ptr, int usuario_fk, int residencia_fk, char *cargo);
int addPreceptorTB(sqlite3* db_ptr, int usuario_fk, int turma_fk);
int addResidenteTB(sqlite3* db_ptr, int usuario_fk, char *matricula, int turma_fk, int preceptor_fk, char* notas);


//pegando dados
Usuario *fazerLogin(sqlite3* db_ptr, char *email, char *senha);
int getUsuarioTB(sqlite3* db_ptr,Usuario* usuario, int usuario_id);
void getResidente(sqlite3* db_ptr, Residente* residente, int residente_id);
void getTurmaTB(sqlite3* db_ptr, Turma* turma, int turma_id);


//estruturas
int addResidenciaTB(sqlite3* db_ptr, char* nome);
int addTurmaTB(sqlite3* db_ptr, int residencia_fk, char* nome, char* ano);

//objetos
int addAtividadeTB(sqlite3* db_ptr, int turma_fk, char* nome, char* descricao, char* data_p, char* data_e, char* status);


//recursao
int forRecursivo(int i, int range, int ac);



//arquivos
int arquivarUsuarios(sqlite3* db_ptr, char* filePath);

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