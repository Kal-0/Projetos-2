#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sqlite3/sqlite3.h"


#ifndef UTILS_H_
#define UTILS_H_

typedef struct {
  char nome[60];
  int id;
  char *email;
  char *senha;
  char *tipoDeUsuario;
  void *categoriaUsuario;
  int status;

} Usuario;

// Tipos de Usuarios



struct Gestao;
struct Coordenacao;
struct Preceptor;
struct Residente;

struct ProgramaResidencia;
struct Turma;

struct Atividade;
struct Submissao;
struct FeedbackGeral;

/*
typedef struct Gestao Gestao;
typedef struct Coordenacao Coordenacao;
typedef struct Preceptor Preceptor;
typedef struct Residente Residente;

typedef struct ProgramaResidencia ProgramaResidencia;
typedef struct Turma Turma;

typedef struct Atividade Atividade;
typedef struct Submissao Submissao;
typedef struct FeedbackGeral FeedbackGeral;
*/

typedef struct {
  Usuario usuario;
  int matricula;
  struct Turma* turma;
  struct Atividade *listaAtividades;
  struct Submissao *listaSubmissoes;
  struct Preceptor* preceptorResponsavel;
  float notasTrimestrais[4];
  struct FeedbackGeral *listaFeedbacks;
}Residente;
 
typedef struct {
  Usuario usuario;
  struct Residente *listaResidentesAtuais;
  struct Atividade *listaAtividades;
  struct ProgramaResidencia *listaResidencias;
  struct FeedbackGeral *listaFeedbacks;
}Preceptor;

typedef struct {
  Usuario usuario;
  char cargo[30];
  struct ProgramaResidencia* residencia;
}Coordenacao;

typedef struct {
  Usuario usuario;
  char cargo[30];
  struct ProgramaResidencia *listaResidencias;
}Gestao;



// estruturas

typedef struct {
  char nomePrograma[30];
  struct Coordenacao* listaCoordenacao;
  struct Turma* listaTurmas;
}ProgramaResidencia;


typedef struct {
  char nomeTurma[40];
  char anoDaTurma[10];
  struct Residente *residentes;
  struct Preceptor *listaPreceptores;
  struct Atividade *listasAtividades;
  struct ProgramaResidencia* residencia;
  char *criteriosAvaliativos;
}Turma;


// objetos

typedef struct {
  char nomeDaAtividade[40];
  char* descricaoDaAtividade;
  struct Turma* turma;
  struct Submissao *listaSubmissao;
  char dataDaPostagem[16];
  char dataDaEntrega[16];
  int ativa;
}Atividade;

typedef struct {
  struct Residente* residente;
  struct Preceptor* preceptor;
  struct Atividade* atividade;
  float nota;
  char *resposta;
  char *feedback;
  char *status;
}Submissao;

typedef struct {
  struct Preceptor* preceptor;
  char *criterios;
  float nota;
  char *feedback;
  char *contestacao;
  char *respostaConstentacao;
  char remetente;
  char destinatario;
  char data[16];
  char status;
}FeedbackGeral;



// Funções:


// -Gerais:
char* strFOverwrite(char** output_str, char* base_str, char* variable_str);
void strOverwrite(char** oldString, char* newString);



int sysStatus(sqlite3** db_ptr, int ret);

void fazerCadastro();

Usuario *fazerLogin(sqlite3** db_ptr, char *email, char *senha);

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